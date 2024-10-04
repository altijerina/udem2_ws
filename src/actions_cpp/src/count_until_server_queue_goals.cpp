#include <queue>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_interfaces/action/count_until.hpp"
#include <string>

#include "std_msgs/msg/string.hpp"

using CountUntil = robot_interfaces::action::CountUntil;
using CountUntilGoalHandle = rclcpp_action::ServerGoalHandle<CountUntil>;
using namespace std::placeholders;

class CountUntilServerNode : public rclcpp::Node // MODIFY NAME
{
public:
    CountUntilServerNode() : Node("count_until_server") // MODIFY NAME
    {
        goal_queue_thread_ = std::thread(&CountUntilServerNode::run_goal_queue_thread, this);
        cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
        count_until_server_ = rclcpp_action::create_server<CountUntil>(
            this,
            "count_until",
            std::bind(&CountUntilServerNode::goal_callback, this, _1, _2 ),
            std::bind(&CountUntilServerNode::cancel_callback, this, _1),
            std::bind(&CountUntilServerNode::handle_accepted_callback, this, _1),
            rcl_action_server_get_default_options(),
            cb_group_
        );
        RCLCPP_INFO(this->get_logger(), "Action server has been started");
    }

    ~CountUntilServerNode()
    {
        goal_queue_thread_.join();
    }

private:
    rclcpp_action::GoalResponse goal_callback(
        const rclcpp_action::GoalUUID &uuid, std::shared_ptr <const CountUntil::Goal> goal)
    {
        (void)uuid;
        auto message1 = std_msgs::msg::String();
        message1.data = std::to_string(goal->goal_number);
        RCLCPP_INFO(this->get_logger(), "Received goal number %s", message1.data.c_str());

        if (goal->target_number <= 0.0)
        {
            RCLCPP_INFO(this->get_logger(), "Invalid Request.  Rejecting goal %s", message1.data.c_str());
            return rclcpp_action::GoalResponse::REJECT;
        }
        
        RCLCPP_INFO(this->get_logger(), "Accepted goal %s", message1.data.c_str());
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    rclcpp_action::CancelResponse cancel_callback(
        const std::shared_ptr<CountUntilGoalHandle> goal_handle)
    {
        RCLCPP_INFO(this->get_logger(), "Received cancel request");
        (void)goal_handle;
        return rclcpp_action::CancelResponse::ACCEPT;
    }

    void handle_accepted_callback(
        const std::shared_ptr<CountUntilGoalHandle> goal_handle)
    {
        int goal_number = goal_handle->get_goal()->goal_number;
        auto message1 = std_msgs::msg::String(); 
        message1.data = std::to_string(goal_number);
            // RCLCPP_INFO(this->get_logger(), "Executing goal %s", message1.data.c_str());
            // execute_goal(goal_handle);
        std::lock_guard<std::mutex> lock(mutex_);
        goal_queue_.push(goal_handle);        
        RCLCPP_INFO(this->get_logger(), "Add goal %s to queue", message1.data.c_str());
        RCLCPP_INFO(this->get_logger(), "Queue size: %d", (int)goal_queue_.size());
    }

    void run_goal_queue_thread()
    {
        rclcpp::Rate loop_rate(1000.0);
        while (rclcpp::ok())
        {
            std::shared_ptr<CountUntilGoalHandle> next_goal;
            {
                std::lock_guard<std::mutex> lock(mutex_);
                if (goal_queue_.size() > 0)
                {
                    next_goal = goal_queue_.front();
                    goal_queue_.pop();
                }
            }

            if (next_goal)
            {
                RCLCPP_INFO(this->get_logger(), "Execute next goal in queue");
                execute_goal(next_goal);
            }
            
            
            loop_rate.sleep();
        }
    }

    void execute_goal(const std::shared_ptr<CountUntilGoalHandle> goal_handle)
    {
        //Get request from goal
        int target_number = goal_handle->get_goal()->target_number;
        double period = goal_handle->get_goal()->period;
        int goal_number = goal_handle->get_goal()->goal_number;

        //Execute the action
        int counter = 0;
        auto result = std::make_shared<CountUntil::Result>();        
        auto feedback = std::make_shared<CountUntil::Feedback>();
        auto message1 = std_msgs::msg::String(); 
        auto message2 = std_msgs::msg::String();
        message1.data = std::to_string(goal_number);
        rclcpp::Rate loop_rate(1.0/period);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        for (int i = 0; i < target_number; i++) 
        {
            if (goal_handle->is_canceling())
            {
                result->reached_number = counter;
                result->goal_number = goal_number;
                goal_handle->canceled(result);
                return;
            }
            counter++;     
            message2.data = std::to_string(counter);    
            RCLCPP_INFO(this->get_logger(), "Goal number %s current %s", message1.data.c_str(), message2.data.c_str());
            feedback->current_number = counter;
            feedback->goal_number = goal_number;
            goal_handle->publish_feedback(feedback);
            loop_rate.sleep();
        }


        //Set final state and return result
        result->reached_number = counter;
        result->goal_number = goal_number;
        goal_handle->succeed(result);
    }

    rclcpp_action::Server<CountUntil>::SharedPtr count_until_server_;
    rclcpp::CallbackGroup::SharedPtr cb_group_;
    std::mutex mutex_;
    std::queue<std::shared_ptr<CountUntilGoalHandle>> goal_queue_;
    std::thread goal_queue_thread_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CountUntilServerNode>();
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();
    // rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}