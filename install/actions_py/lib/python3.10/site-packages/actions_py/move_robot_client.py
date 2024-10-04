#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
import time 
from rclpy.action import ActionClient
from rclpy.action.client import ClientGoalHandle, GoalStatus
from robot_interfaces.action import MoveRobot
from example_interfaces.msg import Empty


class MoveRobotClientNode(Node):
    def __init__(self):
        super().__init__("move_robot_client") 
        self.goal_handle_ = None
        self.move_robot_client_ = ActionClient(self, MoveRobot, "move_robot")
        self.cancel_subscriber = self.create_subscription(
            Empty, "cancel_move", self.callback_cancel_move, 10)
        
    def send_goal(self, goal_num, position, velocity):
        # Wait for the server
        self.move_robot_client_.wait_for_server()
        
        #Create a goal
        goal = MoveRobot.Goal()
        goal.goal_num = goal_num
        goal.position = position
        goal.velocity = velocity
        
        #Send the goal
        self.get_logger().info("Moving robot to: " + str(position) + "m")
        self.move_robot_client_. \
            send_goal_async(goal, feedback_callback=self.goal_feedback_callback). \
                add_done_callback(self.goal_response_callback)       
        
    def goal_response_callback(self, future):
        self.goal_handle_: ClientGoalHandle = future.result()
        if self.goal_handle_.accepted:
           self.get_logger().info("Goal accepted")
           self.goal_handle_.get_result_async().add_done_callback(self.goal_result_callback)
        else:
            self.get_logger().warn("Goal rejected")
            
    def goal_result_callback(self, future):
        status = future.result().status
        result = future.result().result
        if status == GoalStatus.STATUS_SUCCEEDED:
            self.get_logger().info("Robot successfully moved to " + str(result.position))
        elif   status == GoalStatus.STATUS_ABORTED:
            self.get_logger().info("Robot move request Aborted")
        elif status == GoalStatus.STATUS_CANCELED:
            self.get_logger().warn("Robot move request Canceled")
        self.get_logger().info("Robot moved to " + str(result.position))
        self.get_logger().info("Message: " + str(result.message))
        
    def goal_feedback_callback(self, feedback_msg):
        current_location_ = feedback_msg.feedback.current_position
        self.get_logger().info("Robot currently located at " + str(current_location_))
        
    def callback_cancel_move(self, msg):
        self.cancel_goal()
        
    def cancel_goal(self):
        if self.goal_handle_ is not None:
            self.get_logger().info("Send a cancel request")
            self.goal_handle_.cancel_goal_async()
        

def main(args=None):
    rclpy.init(args=args)
    node = MoveRobotClientNode()
    node.send_goal(1, 75, 5)
    time.sleep(8.0)
    node.send_goal(2, 25, 7)   
    time.sleep(8.0)
    node.send_goal(3, 15, 2) 
    time.sleep(8.0)
    node.send_goal(4, 85, 12)         
           
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()