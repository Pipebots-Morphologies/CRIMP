# Copyright 2016 Open Source Robotics Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import rclpy
from rclpy.node import Node

from std_msgs.msg import String


class MinimalPubSub(Node):

    def __init__(self):
        super().__init__('minimal_pubsub')
        # publisher setup
        self.publisher_ = self.create_publisher(String, 'channel_1', 10)
        timer_period = 0.5  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback) 
        self.i = 0

        # subscriber setup
        self.subscription = self.create_subscription(
            String,
            'channel_2',
            self.listener_callback,
            10)
        self.subscription  # prevent unused variable warning

    def timer_callback(self): # called when the timer triggers
        msg = String()
        msg.data = 'Hello World: %d' % self.i
        self.publisher_.publish(msg)
        self.get_logger().info('Publishing: "%s"' % msg.data)
        self.i += 1
    
    def listener_callback(self, msg): # called when a message is detected
        self.get_logger().info('I heard: "%s"' % msg.data)



def main(args=None):
    rclpy.init(args=args)

    minimal_pubsub = MinimalPubSub()

    rclpy.spin(minimal_pubsub)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_pubsub.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
