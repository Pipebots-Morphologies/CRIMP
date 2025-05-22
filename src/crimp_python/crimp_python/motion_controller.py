import rclpy
import time
from rclpy.node import Node
from sensor_msgs.msg import JointState
from custom_msgs.srv import SuctionControl

class SequenceExecutor(Node):
    def __init__(self):
        super().__init__('sequence_executor')
        self.joint_pub = self.create_publisher(JointState, 'joint_targets', 10)
        self.suction_cli = self.create_client(SuctionControl, 'suction_control')
        while not self.suction_cli.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('Waiting for suction_control service...')
        self.walk_upwards()

    def move_joints(self, e1, e2, e3, v=600, a=15):
        msg = JointState()
        msg.name = ['elbow_1', 'elbow_2', 'elbow_3']
        msg.position = [e1, e2, e3]
        msg.velocity = [v]
        msg.effort = [a]
        self.joint_pub.publish(msg)
        self.get_logger().info(f'Moved joints to: {e1}, {e2}, {e3}')

    def move_wrists(self, w1, w2, v=600):
        msg = JointState()
        msg.name = ['wrist_1', 'wrist_2']
        msg.position = [w1, w2]
        msg.velocity = [v]
        msg.effort = [0]  # Can be 0 or omitted depending on controller setup
        self.joint_pub.publish(msg)
        self.get_logger().info(f'Moved wrists to: {w1}, {w2} with velocity {v}')

    def set_suction(self, cup1, cup2):
        req = SuctionControl.Request()
        req.cup_1 = cup1
        req.cup_2 = cup2
        future = self.suction_cli.call_async(req)
        rclpy.spin_until_future_complete(self, future)
        self.get_logger().info(f'Suction response: {future.result().success}')
        

    def wait_for_user(self, prompt):
        input(f'{prompt} Press Enter to continue...')

    def walk_upwards(self):
        x = 0 
        self.move_joints(150, 60, 150)
        self.move_wrists(88, 92)

        self.wait_for_user("attatch")
        self.set_suction(1, 1)

        self.wait_for_user("walk")

        while(x < 5):

            self.set_suction(1, -1)

            #self.wait_for_user("Step 2: lift")
            self.move_joints(134.12, 62.27, 163.61)

            #self.wait_for_user("Step 2: Extend front leg")
            self.move_joints(119.2, 112.52, 128.28)

            #self.wait_for_user("Step 3: Press front leg")
            self.move_joints(125, 112, 123)
            time.sleep(1)

            #self.wait_for_user("Step 4: Attach front cup")
            self.set_suction(1, 0)
            self.set_suction(1, 1)
            self.set_suction(-1, 1)

            #self.wait_for_user("Step 5: lift")
            self.move_joints(131.95, 114.14, 113.91)

            #self.wait_for_user("Step 5: retract")
            self.move_joints(157.21, 60.57, 142.22)

            #self.wait_for_user("Step 5: press")
            self.move_joints(148, 60, 150)

            time.sleep(1)
            #self.wait_for_user("Step 5: atatch")
            self.set_suction(0, 1)
            self.set_suction(1, 1)

            self.get_logger().info("Sequence completed")
            x += 1


    def walk_inverted(self):
        x = 0 
        self.move_joints(150, 60, 150)
        self.move_wrists(88, 92)

        self.wait_for_user("walk")
        self.set_suction(1, 1)

        while(x < 5):

            self.set_suction(1, -1)

            #self.wait_for_user("Step 2: lift")
            self.move_joints(142.22, 60.57, 157.21)

            #self.wait_for_user("Step 2: Extend front leg")
            self.move_joints(119.2, 112.52, 128.28)

            #self.wait_for_user("Step 3: Press front leg")
            self.move_joints(125, 109.5, 117.5, 300)
            time.sleep(1)

            #self.wait_for_user("Step 4: Attach front cup")
            self.set_suction(1, 0)
            self.set_suction(1, 1)
            self.set_suction(-1, 1)

            #self.wait_for_user("Step 5: lift")
            #self.move_joints(128.28, 112.52, 114.2)

            #self.wait_for_user("Step 5: retract")
            self.move_joints(157.21, 60.57, 138.22)

            #self.wait_for_user("Step 5: press")
            self.move_joints(145, 58, 148.3, 300)

            time.sleep(1)
            #self.wait_for_user("Step 5: atatch")
            self.set_suction(0, 1)
            self.set_suction(1, 1)

            self.get_logger().info("Sequence completed")
            x += 1

def main(args=None):
    rclpy.init(args=args)
    node = SequenceExecutor()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
