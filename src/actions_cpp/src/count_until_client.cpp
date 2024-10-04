#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_interfaces/action/count_until.hpp"
#include <string>

#include "std_msgs/msg/string.hpp"

using CountUntil = robot_interfaces::action::CountUntil;
using CountUntilGoalHandle = rclcpp_action::ClientGoalHandle<CountUntil>;
using namespace std::placeholders;

class CountUntilClientNode : public rclcpp::Node
{
public:
    CountUntilClientNode() : Node("count_until_client")
    {
            count_until_client_ = 
                rclcpp_action::create_client<CountUntil>(this, "count_until");
    }

    void send_goal(int goal_number, int target_number, double period)
    {
        // Wait for the Actino Server
        count_until_client_->wait_for_action_server();

        //Create a goal
        auto goal = CountUntil::Goal();
        goal.goal_number = goal_number;
        goal.target_number = target_number;
        goal.period = period;
        auto message = std_msgs::msg::String();
        message.data = std::to_string(goal.goal_number);        

        // Add Callbacks
        auto options = rclcpp_action::Client<CountUntil>::SendGoalOptions();
        options.result_callback = 
            std::bind(&CountUntilClientNode::goal_result_callback, this, _1);
        options.goal_response_callback =
            std::bind(&CountUntilClientNode::goal_response_callback, this, _1);
        options.feedback_callback = 
            std::bind(&CountUntilClientNode::goal_feedback_callback, this, _1, _2);

        //Send the goal
        RCLCPP_INFO(this->get_logger(), "Sending goal num: %s", message.data.c_str());
        count_until_client_->async_send_goal(goal, options);

        //Cancel the goal (test)
        // timer_ = create_wall_timer(
        //         std::chrono::seconds(2),
        //         std::bind(&CountUntilClientNode::timer_callback, this));
    }

private:

    void timer_callback()
    {
        RCLCPP_INFO(this->get_logger(), "Cancel the goal");
        count_until_client_->async_cancel_goal(goal_handle_);
        timer_ -> cancel();
    }

    //Callback to know if the goal was accepted or rejected
    void goal_response_callback(const CountUntilGoalHandle::SharedPtr &goal_handle)
    {        
        if (!goal_handle)
        {
            RCLCPP_INFO(this->get_logger(), "Goal rejected");
        }
        else
        {
            this->goal_handle_ = goal_handle;
            RCLCPP_INFO(this->get_logger(), "Goal accepted");
        }
    }

    //Callback to receive the result once the goal is reached
    void goal_result_callback(const CountUntilGoalHandle::WrappedResult &result)
    {
        auto message = std_msgs::msg::String();
        message.data = std::to_string(result.result->goal_number); 

        auto status = result.code;
        if (status == rclcpp_action::ResultCode::SUCCEEDED)
        {
            RCLCPP_INFO(this->get_logger(), "Goal %s Succeeded", message.data.c_str());
        }
        else if (status == rclcpp_action::ResultCode::ABORTED)
        {
            RCLCPP_ERROR(this->get_logger(), "Goal %s Aborted", message.data.c_str());
        }
        else if (status == rclcpp_action::ResultCode::CANCELED)
        {
            RCLCPP_WARN(this->get_logger(), "Goal %s Canceled", message.data.c_str());
        }

        int reached_number = result.result->reached_number;
        auto message1 = std_msgs::msg::String();
        message1.data = std::to_string(reached_number);
        RCLCPP_INFO(this->get_logger(), "Goal %s Result: %s", message.data.c_str(), message1.data.c_str());
    }

    //Callback to receive feedback during goal execution
    void goal_feedback_callback(const CountUntilGoalHandle::SharedPtr &goal_handle, 
        const std::shared_ptr<const CountUntil::Feedback> feedback)
    {
        (void)goal_handle;
        int number = feedback->current_number;
        int goal_num = feedback->goal_number;
        RCLCPP_INFO(this->get_logger(), "Goal %d feedback: %d", goal_num, number);
    }

    rclcpp_action::Client<CountUntil>::SharedPtr count_until_client_;
    rclcpp::TimerBase::SharedPtr timer_;
    CountUntilGoalHandle::SharedPtr goal_handle_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CountUntilClientNode>();
    node->send_goal(1, 6, 0.8);
    node->send_goal(2, -8, 0.6);   
    node->send_goal(3, 8, 0.6);
    //std::this_thread::sleep_for(std::chrono::seconds(8));     
    node->send_goal(4, 7, 0.5);    
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}