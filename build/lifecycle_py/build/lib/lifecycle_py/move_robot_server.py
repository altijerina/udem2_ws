#!/usr/bin/env python3
import rclpy
import time
import threading
from rclpy.lifecycle import LifecycleNode
from rclpy.lifecycle.node import LifecycleNode, LifecycleState, TransitionCallbackReturn
from rclpy.action import ActionServer
from rclpy.action.server import ServerGoalHandle, GoalResponse,  CancelResponse
from robot_interfaces.action import MoveRobot
from rclpy.executors import MultiThreadedExecutor
from rclpy.callback_groups import ReentrantCallbackGroup


class MoveRobotServerNode(LifecycleNode):
    def __init__(self):
        super().__init__("move_robot_server")
        self.goal_lock_ = threading.Lock()
        self.goal_handle_: ServerGoalHandle = None
        self.robot_position_ = 50
        self.server_activated = False
        self.get_logger().info("Initial robot position: " + str(self.robot_position_) + "m")
        
    def on_configure(self, state: LifecycleState):
        self.declare_parameter("robot_name", rclpy.Parameter.Type.STRING)
        self.robot_name_ = self.get_parameter("robot_name").value
        self.move_robot_server_ = ActionServer(
            self,
            MoveRobot,
            "move_robot_" + self.robot_name_,
            goal_callback=self.goal_callback,
            cancel_callback=self.cancel_callback,
            execute_callback=self.execute_callback,
            callback_group=ReentrantCallbackGroup())
        self.get_logger().info("Action server has been started")
        return TransitionCallbackReturn.SUCCESS
    
    def on_cleanup(self, state: LifecycleState):
        self.undeclare_parameter("robot_name")
        self.robot_name_ = ""
        self.move_robot_server_.destroy()
        return TransitionCallbackReturn.SUCCESS
    
    def on_activate(self, state: LifecycleState):
        self.get_logger().info("Activate node")
        self.server_activated_ = True
        return super().on_activate(state)
    
    def on_deactivate(self, state: LifecycleState):
        self.get_logger().info("Deactivate node")
        self.server_activated_ = False
        with self.goal_lock_:
            if self.goal_handle_ is not None and self.goal_handle_.is_active:
                self.goal_handle_.abort()
        return super().on_deactivate(state)
    
    def on_shutdown(self, state: LifecycleState):
        self.undeclare_parameter("robot_name")
        self.robot_name_ = ""
        self.move_robot_server_.destroy()
        return TransitionCallbackReturn.SUCCESS
        
    def goal_callback(self, goal_request: MoveRobot.Goal):
        new_position_ = goal_request.position
        robot_velocity_ = goal_request.velocity
        self.get_logger().info("Received new position: " + str(new_position_) + "m")
        
        if not self.server_activated:
            self.get_logger().warn("Node Not Activated Yet")
            return GoalResponse.REJECT
               
        #Validate the goal request        
        if new_position_ not in range(0, 100) or robot_velocity_ <= 0:
            self.get_logger().info("Invalid Request. Rejecting new position: " + str(new_position_))
            return GoalResponse.REJECT
        
        # #Policy: Preempt existing goal when receiving new goal
        with self.goal_lock_:
            if self.goal_handle_ is not None and self.goal_handle_.is_active:
                self.get_logger().info("Move robot request Aborted.")
                self.goal_handle_.abort()
        
        self.get_logger().info("Accepting move robot request: " + str(new_position_))
        return GoalResponse.ACCEPT                
                
    def cancel_callback(self, goal_handle: ServerGoalHandle):
        self.get_logger().info("Received a cancel request")
        return CancelResponse.ACCEPT # Or Reject     
    
    def execute_callback(self, goal_handle: ServerGoalHandle):
        with self.goal_lock_:
            self.goal_handle_ = goal_handle
        
        #Get request from the goal
        final_position_ = goal_handle.request.position
        velocity_ = goal_handle.request.velocity
        
        #Execute the action
        self.get_logger().info("Moving robot to: " + str(final_position_))
        feedback = MoveRobot.Feedback()
        result = MoveRobot.Result()
        new_position_ = self.robot_position_
        while rclpy.ok():            
            if not goal_handle.is_active:
                result.position = new_position_
                result.message = "Preempted by a new request or node deactivated"
                return result  
                         
            if goal_handle.is_cancel_requested:
                result.position = new_position_
                if final_position_ == new_position_:
                    result.message = "Success after cancel request"
                    goal_handle.succeed()
                else:
                    result.message = "Request canceled"
                    goal_handle.canceled()
                return result                  
                
            difference_ = final_position_ - new_position_
            
            if difference_ == 0:
                result.position = new_position_
                result.message = "Robot at new position"
                goal_handle.succeed()
                return result
            elif difference_ > 0:
                if difference_ > velocity_:
                    new_position_ = new_position_ + velocity_
                else:
                    new_position_ = new_position_ + difference_
            else:
                if abs(difference_) >= velocity_:
                    new_position_ = new_position_ - velocity_
                else:
                    new_position_ = new_position_ - abs(difference_)
                    
            time.sleep(1.0)
                
                
            self.get_logger().info("Robot position: " + str(new_position_))
            feedback.current_position = new_position_
            goal_handle.publish_feedback(feedback)
                
            time.sleep(1.0)         


def main(args=None):
    rclpy.init(args=args)
    node = MoveRobotServerNode()
    rclpy.spin(node, MultiThreadedExecutor())
    rclpy.shutdown()


if __name__ == "__main__":
    main()