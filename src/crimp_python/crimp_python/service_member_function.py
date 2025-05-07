from example_interfaces.srv import AddTwoInts

import rclpy
from rclpy.node import Node


class IMU(Node):

    def __init__(self):
        super().__init__('imu')
        # self.srv = self.create_service(AddTwoInts, 'get_down', self.get_down_callback)
        timer_period = 3
        self.timer = self.create_timer(timer_period, self.timer_callback)
    
    def get_down():
        pass
    
    
    """def get_down_callback(self, request, response):
        response.sum = request.a + request.b
        self.get_logger().info('Incoming request\na: %d b: %d' % (request.a, request.b))

        return response"""


def main():
    rclpy.init()

    imu = IMU()

    rclpy.spin(imu)

    rclpy.shutdown()


if __name__ == '__main__':
    main()