#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
import time 
from rclpy.action import ActionClient
from rclpy.action.client import ClientGoalHandle, GoalStatus
from robot_interfaces.action import CountUntil


class CountUntilClientNode(Node):
    def __init__(self):
        super().__init__("count_until_client") 
        self.count_until_client_ = ActionClient(self, CountUntil, "count_until")
        
    def send_goal(self, goal_number, target_number, period):
        # Wait for the server
        self.count_until_client_.wait_for_server()
        
        #Create a goal
        goal = CountUntil.Goal()
        goal.goal_number = goal_number
        goal.target_number = target_number
        goal.period = period
        
        #Send the goal
        self.get_logger().info("Sending goal number: " + str(goal_number))
        self.count_until_client_. \
            send_goal_async(goal, feedback_callback=self.goal_feedback_callback). \
                add_done_callback(self.goal_response_callback)
                
        #send a cancel request 2 seconds later
        #self.timer_ = self.create_timer(1.5, self.cancel_goal)
        
    def cancel_goal(self):
        self.get_logger().info("Send a cancel request")
        self.goal_handle_.cancel_goal_async()
        self.timer_.cancel()
        
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
            self.get_logger().info("Goal Number " + str(result.goal_number) + " Success")
        elif   status == GoalStatus.STATUS_ABORTED:
            self.get_logger().info("Goal Number " + str(result.goal_number) + " Aborted")
        elif status == GoalStatus.STATUS_CANCELED:
            self.get_logger().warn("Goal Number " + str(result.goal_number) + " Canceled")
        self.get_logger().info("Goal Number " + str(result.goal_number) + " Result: " + str(result.reached_number))
        
    def goal_feedback_callback(self, feedback_msg):
        number = feedback_msg.feedback.current_number
        goal_num = feedback_msg.feedback.goal_number 
        self.get_logger().info("Goal number " + str(goal_num) + " feedback: " + str(number))
        

def main(args=None):
    rclpy.init(args=args)
    node = CountUntilClientNode()
    node.send_goal(1, 5, 0.2)
    #time.sleep(8.0)
    node.send_goal(2, 6, 0.1)   
    #time.sleep(1.2) 
    node.send_goal(3, -6, 0.1)   
    #time.sleep(15.0)  
    node.send_goal(4, 7, 0.2)    
    node.send_goal(5, 8, 0.2) 
    node.send_goal(6, 3, 0.2)              
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()