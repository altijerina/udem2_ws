#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_interfaces/action/move_robot.hpp"
#include <string>
#include "example_interfaces/msg/empty.hpp"

#include "std_msgs/msg/string.hpp"

using MoveRobot = robot_interfaces::action::MoveRobot;
using MoveRobotGoalHandle = rclcpp_action::ClientGoalHandle<MoveRobot>;
using Empty = example_interfaces::msg::Empty;
using namespace std::placeholders;

class MoveRobotClientNode : public rclcpp::Node
{
public:
    MoveRobotClientNode() : Node("move_robot_client")
    {
            move_robot_client_ = 
                rclcpp_action::create_client<MoveRobot>(this, "move_robot");

            cancel_subscriber_ = this->create_subscription<Empty>(
                "cancel_move", 10, std::bind(&MoveRobotClientNode::callback_cancel_move, this, _1)
            );
    }

    void send_goal(int goal_number, int position, int velocity)
    {
        // Wait for the Actino Server
        move_robot_client_->wait_for_action_server();

        //Create a goal
        auto goal = MoveRobot::Goal();
        goal.goal_num = goal_number;
        goal.position = position;
        goal.velocity = velocity;
        auto message1 = std_msgs::msg::String();
        message1.data = std::to_string(goal.goal_num);       
        auto message2 = std_msgs::msg::String();
        message2.data = std::to_string(goal.position);  
        auto message3 = std_msgs::msg::String();
        message3.data = std::to_string(goal.velocity);                   

        // Add Callbacks
        auto options = rclcpp_action::Client<MoveRobot>::SendGoalOptions();
        options.goal_response_callback =
            std::bind(&MoveRobotClientNode::goal_response_callback, this, _1);
        options.result_callback = 
            std::bind(&MoveRobotClientNode::goal_result_callback, this, _1);            
        options.feedback_callback = 
            std::bind(&MoveRobotClientNode::goal_feedback_callback, this, _1, _2);

        //Send the goal
        RCLCPP_INFO(this->get_logger(), "Sending goal num: %s with position %sm and velocity %s m/s", \
              message1.data.c_str(),message2.data.c_str(), message3.data.c_str());
        move_robot_client_->async_send_goal(goal, options);
    }

private:

    //Callback to know if the goal was accepted or rejected
    void goal_response_callback(const MoveRobotGoalHandle::SharedPtr &goal_handle)
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
    void goal_result_callback(const MoveRobotGoalHandle::WrappedResult &result)
    {
        auto message = std_msgs::msg::String();
        message.data = std::to_string(result.result->goal_num); 

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

        int position = result.result->position;
        auto message1 = std_msgs::msg::String();
        message1.data = std::to_string(position);
        RCLCPP_INFO(this->get_logger(), "Goal %s Robot located at: %sm", message.data.c_str(), message1.data.c_str());
    }

    //Callback to receive feedback during goal execution
    void goal_feedback_callback(const MoveRobotGoalHandle::SharedPtr &goal_handle, 
        const std::shared_ptr<const MoveRobot::Feedback> feedback)
    {
        (void)goal_handle;
        int current_position_ = feedback->current_position;
        int goal_num_ = feedback->goal_num;
        RCLCPP_INFO(this->get_logger(), "Goal %d feedback: %d", goal_num_, current_position_);
    }

    void callback_cancel_move(const Empty::SharedPtr msg)
    {
        (void)msg;
        cancel_goal();
    }

    void cancel_goal()
    {
        if (this->goal_handle_) {
            this->move_robot_client_->async_cancel_goal(goal_handle_);
            goal_handle_.reset();
        }
    }    

    rclcpp_action::Client<MoveRobot>::SharedPtr move_robot_client_;
    rclcpp::Subscription<Empty>::SharedPtr cancel_subscriber_;
    MoveRobotGoalHandle::SharedPtr goal_handle_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MoveRobotClientNode>();
    node->send_goal(1, 75, 5);
    // node->send_goal(2, 25, 7); 
    // node->send_goal(3, 15, 2); 
    // node->send_goal(4, 85, 12);   
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}