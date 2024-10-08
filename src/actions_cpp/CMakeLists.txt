cmake_minimum_required(VERSION 3.8)
project(actions_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(robot_interfaces REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(std_msgs REQUIRED)
find_package(example_interfaces REQUIRED)

add_executable(count_until_server src/count_until_server.cpp)
ament_target_dependencies(count_until_server rclcpp rclcpp_action std_msgs robot_interfaces)

add_executable(count_until_server_queue_goals src/count_until_server_queue_goals.cpp)
ament_target_dependencies(count_until_server_queue_goals rclcpp rclcpp_action std_msgs robot_interfaces)

add_executable(count_until_client src/count_until_client.cpp)
ament_target_dependencies(count_until_client rclcpp rclcpp_action std_msgs robot_interfaces)

add_executable(move_robot_server src/move_robot_server.cpp)
ament_target_dependencies(move_robot_server rclcpp rclcpp_action std_msgs robot_interfaces)

add_executable(move_robot_client src/move_robot_client.cpp)
ament_target_dependencies(move_robot_client rclcpp rclcpp_action std_msgs robot_interfaces example_interfaces)

add_executable(his_move_robot_client src/his_move_robot_client.cpp)
ament_target_dependencies(his_move_robot_client rclcpp rclcpp_action std_msgs robot_interfaces example_interfaces)

install(TARGETS 
  count_until_server
  count_until_server_queue_goals
  count_until_client
  move_robot_server
  move_robot_client
  his_move_robot_client
  DESTINATION lib/${PROJECT_NAME}/
)

ament_package()
