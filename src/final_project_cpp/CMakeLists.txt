cmake_minimum_required(VERSION 3.8)
project(final_project_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(turtlesim REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(robot_interfaces REQUIRED)

add_library(turtle_spawner_component SHARED src/turtle_spawner_component.cpp)
ament_target_dependencies(turtle_spawner_component rclcpp rclcpp_action rclcpp_lifecycle rclcpp_components turtlesim geometry_msgs robot_interfaces)
rclcpp_components_register_nodes(turtle_spawner_component "turtle_spawner_component::TurtleController")

add_library(turtlebot_control_component SHARED src/turtlebot3_spawner.cpp)
ament_target_dependencies(turtlebot_control_component  rclcpp rclcpp_action rclcpp_lifecycle rclcpp_components geometry_msgs robot_interfaces)
rclcpp_components_register_nodes(turtlebot_control_component  "turtlebot3_spawner::Turtlebot3Controller")

add_executable(turtle_spawner_client src/turtle_spawner_client.cpp)
ament_target_dependencies(turtle_spawner_client rclcpp rclcpp_action turtlesim geometry_msgs robot_interfaces)

add_executable(turtle_spawner_server src/turtle_spawner_server.cpp)
ament_target_dependencies(turtle_spawner_server rclcpp rclcpp_action turtlesim geometry_msgs robot_interfaces)

add_executable(turtle_spawner_lifecycle src/turtle_spawner_lifecycle.cpp)
ament_target_dependencies(turtle_spawner_lifecycle rclcpp rclcpp_action rclcpp_lifecycle turtlesim geometry_msgs robot_interfaces)

install(TARGETS 
  turtle_spawner_component
  turtlebot_control_component 
  ARCHIVE DESTINATION lib 
  LIBRARY DESTINATION lib 
  RUNTIME DESTINATION bin
)

install(TARGETS 
  turtle_spawner_client
  turtle_spawner_server
  turtle_spawner_lifecycle
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
