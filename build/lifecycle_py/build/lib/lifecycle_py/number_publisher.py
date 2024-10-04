#!/usr/bin/env python3
import rclpy
from rclpy.lifecycle import LifecycleNode
from rclpy.lifecycle.node import LifecycleState, TransitionCallbackReturn
from example_interfaces.msg import Int64

class NumberPublisherNode(LifecycleNode):
    def __init__(self):
        super().__init__("number_publisher")
        self.get_logger().info("IN Constructor")
        self.number_ = 1
        self.publish_frequency_ = 1.0
        self.number_publisher_ = None
        self.number_timer_ = None
          
    #Create ROS2 Communications to connect to hardware                
    def on_configure(self, previous_state: LifecycleState):
        self.get_logger().info("IN on_configure")        
        self.number_publisher_ = self.create_lifecycle_publisher(Int64, "number", 10)
        self.number_timer_ = self.create_timer(
            1.0 / self.publish_frequency_, self.publish_number)
        self.number_timer_.cancel
        #raise Exception()
        return TransitionCallbackReturn.SUCCESS
    
    # Activate/Enable hardware
    def on_activate(self, previous_state: LifecycleState):
        self.get_logger().info("IN on_activate") 
        self.number_timer_.reset()
        return super().on_activate(previous_state)
    
    # Deactivate/Disable hardware
    def on_deactivate(self, previous_state: LifecycleState):
        self.get_logger().info("IN on_deactivate")
        self.number_timer_.cancel
        return super().on_deactivate(previous_state)

    #Destroy ROS2 Communications from hardware
    def on_cleanup(self, previous_state: LifecycleState):
        self.get_logger().info("IN on_cleanup") 
        self.destroy_lifecycle_publisher(self.number_publisher_)
        self.destroy_timer(self.number_timer_)
        return TransitionCallbackReturn.SUCCESS  
    
    def on_shutdown(self, previous_state: LifecycleState):
        self.get_logger().info("IN on_shutdown")
        self.destroy_lifecycle_publisher(self.number_publisher_)
        self.destroy_timer(self.number_timer_)
        return TransitionCallbackReturn.SUCCESS     
    
    # Process errors, deactivate and cleanup
    def on_error(self, previous_state: LifecycleState):
        self.get_logger().info("IN on_error")
        self.destroy_lifecycle_publisher(self.number_publisher_)
        self.destroy_timer(self.number_timer_)   
        # Do some checks, if okay, then return SUCCESS, if not FAILURE
        return TransitionCallbackReturn.SUCCESS    
   
        

    def publish_number(self):
        msg = Int64()
        msg.data = self.number_
        self.number_publisher_.publish(msg)
        self.number_ += 1

def main(args=None):
    rclpy.init(args=args)
    node = NumberPublisherNode()
    rclpy.spin(node)
    rclpy.shutdown()