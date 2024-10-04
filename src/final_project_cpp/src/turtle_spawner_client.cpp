#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/kill.hpp"
#include "turtlesim/srv/spawn.hpp"

class TurtleController : public rclcpp::Node
{
public:
    TurtleController() : Node("turtle_controller")
    {
        this->declare_parameter("turtle_name", rclcpp::PARAMETER_STRING);
        turtle_name_ = this->get_parameter("turtle_name").as_string();
        cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
        spawn_turtle_client_ = this->create_client<turtlesim::srv::Spawn>(
            "/spawn", rmw_qos_profile_services_default, cb_group_);
        kill_turtle_client_ = this->create_client<turtlesim::srv::Kill>(
            "/kill", rmw_qos_profile_services_default, cb_group_);
        spawn_turtle_thread_ = std::thread(std::bind(&TurtleController::spawn_turtle, this));
    }

private:
    void spawn_turtle()
    {
        spawn_turtle_client_->wait_for_service();
        auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
        int random_x = 1 + (rand() % 9);
        int random_y = 1 + (rand() % 9);

        request->name = turtle_name_;
        request->x = random_x;
        request->y = random_y;
        request->theta = 0.0;
        RCLCPP_INFO(this->get_logger(), "Attempting to spawn turtle");
        auto result = spawn_turtle_client_->async_send_request(request);
        RCLCPP_INFO(this->get_logger(), "New spawned turtle: %s", result.get()->name.c_str());

    }

    void kill_turtle()
    {
        kill_turtle_client_->wait_for_service();
        auto request = std::make_shared<turtlesim::srv::Kill::Request>();
        request->name = turtle_name_;
        RCLCPP_INFO(this->get_logger(), "Attempting to remove turtle");
        kill_turtle_client_->async_send_request(request);
        RCLCPP_INFO(this->get_logger(), "Turtle has been removed");
    }

    std::string turtle_name_;
    rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr spawn_turtle_client_;
    rclcpp::Client<turtlesim::srv::Kill>::SharedPtr kill_turtle_client_;
    rclcpp::CallbackGroup::SharedPtr cb_group_;

    std::thread spawn_turtle_thread_;

};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TurtleController>();
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}