#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_interfaces/action/move_robot.hpp"
#include <string>

#include "std_msgs/msg/string.hpp"

using MoveRobot = robot_interfaces::action::MoveRobot;
using MoveRobotGoalHandle = rclcpp_action::ServerGoalHandle<MoveRobot>;
using namespace std::placeholders;

class MoveRobotServerNode : public rclcpp::Node
{
public:
    MoveRobotServerNode() : Node("move_robot_server")
    {   
        original_position_ = 50;
        cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
        move_robot_server_ = rclcpp_action::create_server<MoveRobot>(
            this,
            "count_until",
            std::bind(&MoveRobotServerNode::goal_callback, this, _1, _2 ),
            std::bind(&MoveRobotServerNode::cancel_callback, this, _1),
            std::bind(&MoveRobotServerNode::handle_accepted_callback, this, _1),
            rcl_action_server_get_default_options(),
            cb_group_
        );
        RCLCPP_INFO(this->get_logger(), "Action server has been started");
        RCLCPP_INFO(this->get_logger(), "Robot position: %d", original_position_);
    }


private:
    rclcpp_action::GoalResponse goal_callback(
        const rclcpp_action::GoalUUID &uuid, std::shared_ptr <const MoveRobot::Goal> goal)
    {
        (void)uuid;
        auto message1 = std_msgs::msg::String();
        message1.data = std::to_string(goal->goal_num);
        RCLCPP_INFO(this->get_logger(), "Received goal number %s", message1.data.c_str());

        if ((goal->position < 0.0) || (goal->position > 100) || (goal->velocity <= 0))
        {
            RCLCPP_INFO(this->get_logger(), "Invalid Request.  Rejecting goal %s", message1.data.c_str());
            return rclcpp_action::GoalResponse::REJECT;
        }
        // Policy:  Preempt existing goal when receiving new valid goal
        {           
            std::lock_guard<std::mutex> lock(mutex_);
            if (goal_handle_)
            {
                if (goal_handle_->is_active())
                {
                    RCLCPP_INFO(this->get_logger(), "Abort goal number: %s and accept new goal", message1.data.c_str());
                    preempted_goal_id_ = goal_handle_->get_goal_id();
                }
            }
        }
        
        RCLCPP_INFO(this->get_logger(), "Accepted goal %s", message1.data.c_str());
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    rclcpp_action::CancelResponse cancel_callback(
        const std::shared_ptr<MoveRobotGoalHandle> goal_handle)
    {
        RCLCPP_INFO(this->get_logger(), "Received cancel request");
        (void)goal_handle;
        return rclcpp_action::CancelResponse::ACCEPT;
    }

    void handle_accepted_callback(
        const std::shared_ptr<MoveRobotGoalHandle> goal_handle)
    {
            int goal_number = goal_handle->get_goal()->goal_num;
            auto message1 = std_msgs::msg::String(); 
            message1.data = std::to_string(goal_number);
            int position = goal_handle->get_goal()->position;
            auto message2 = std_msgs::msg::String();
            message2.data = std::to_string(position); 
            int velocity = goal_handle->get_goal()->velocity; 
            auto message3 = std_msgs::msg::String();
            message3.data = std::to_string(velocity);               
            RCLCPP_INFO(this->get_logger(), "Executing goal %s to move to %sm with velocity %sm/s", \
                message1.data.c_str(),message2.data.c_str(), message3.data.c_str());
            execute_goal(goal_handle);
    }

    void execute_goal(const std::shared_ptr<MoveRobotGoalHandle> goal_handle)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            this->goal_handle_ = goal_handle;
        }        

        //Get request from goal
        int position_ = goal_handle->get_goal()->position;
        int velocity_ = goal_handle->get_goal()->velocity;
        int goal_number_ = goal_handle->get_goal()->goal_num;

        //Execute the action
        auto result = std::make_shared<MoveRobot::Result>();        
        auto feedback = std::make_shared<MoveRobot::Feedback>();
        auto message1 = std_msgs::msg::String(); 
        auto message2 = std_msgs::msg::String();
        auto message3 = std_msgs::msg::String();
        message1.data = std::to_string(goal_number_);
        message2.data = std::to_string(position_);
        message3.data = std::to_string(velocity_);
        rclcpp::Rate loop_rate(1.0);

        int current_position_ = original_position_;
        int final_position_ = position_;               
        
        RCLCPP_INFO(this->get_logger(), "Execute goal %s moving to %s with velocity %s", \
            message1.data.c_str(), message2.data.c_str(), message3.data.c_str());
        while (rclcpp::ok())
        {
            {
                //Check to see if new goal will preempt existing goal
                std::lock_guard<std::mutex> lock(mutex_);
                if (goal_handle->get_goal_id() == preempted_goal_id_)
                {
                    result->position = current_position_;
                    result->goal_num = goal_number_;
                    result->message = "Preempted by another goal";
                    goal_handle->abort(result);
                    return;
                }
            }
        

            //Check if cancel goal request has been received
            if (goal_handle->is_canceling()) 
            {
                result->position = current_position_;
                result->goal_num = goal_number_;
                if (position_ == current_position_) 
                {
                    result->message = "Robot at requested position";
                    goal_handle->succeed(result);
                }
            }
            else 
            {
                    result->message = "Move request canceled";
                    goal_handle->canceled(result);
            return;
            }
        
            int difference_ = final_position_ - current_position_;
            
            if (difference_ == 0) 
            {
                result->position = current_position_;
                result->message = "Robot at new position";
                result->goal_num = goal_number_;
                goal_handle->succeed(result);
                return;
            }
            else if (difference_ > 0) 
            {
                if (difference_ > velocity_)
                    current_position_ += velocity_;
                else
                    current_position_ += difference_;
            }
            else if (difference_ < 0) 
            {
                if (abs(difference_) >= velocity_)
                {
                    current_position_ -= velocity_;
                }
                else
                {
                    current_position_ -= abs(difference_);
                }
            }

            message2.data = std::to_string(current_position_);    
            RCLCPP_INFO(this->get_logger(), "Goal number %s current %s", message1.data.c_str(), message2.data.c_str());
            feedback->current_position = current_position_;
            feedback->goal_num = goal_number_;
            goal_handle->publish_feedback(feedback);
            loop_rate.sleep();
        }


        //Set final state and return result
        result->position = current_position_;
        result->goal_num = goal_number_;
        goal_handle->succeed(result);
    }

    rclcpp_action::Server<MoveRobot>::SharedPtr move_robot_server_;
    rclcpp::CallbackGroup::SharedPtr cb_group_;
    int original_position_;
    std::mutex mutex_;
    std::shared_ptr<MoveRobotGoalHandle> goal_handle_;
    rclcpp_action::GoalUUID preempted_goal_id_;

};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MoveRobotServerNode>();
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}