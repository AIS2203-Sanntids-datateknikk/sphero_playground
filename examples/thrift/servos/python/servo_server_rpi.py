
import argparse

import pi_servo_hat
from servo_service import ServoService

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer


class ServoHandler:

    def __init__(self):
        self.servo = pi_servo_hat.PiServoHat()
        self.servo.restart()

    def get_pwm_frequency(self):
        return self.servo.get_pwm_frequency()

    def get_servo_position(self, channel, swing):
        return self.servo.get_servo_position(channel, swing)

    def move_servo_position(self, channel, pos, swing):
        return self.servo.move_servo_position(channel, pos, swing)

    def set_pwm_frequency(self, frequency):
        return self.servo.set_pwm_frequency(frequency)

    def restart(self):
        self.servo.restart()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', type=int,
                        help='Remote port')

    args = parser.parse_args()

    try:

        handler = ServoHandler()
        processor = ServoService.Processor(handler)
        transport = TSocket.TServerSocket(port=args.port)
        tfactory = TTransport.TFramedTransportFactory()
        pfactory = TBinaryProtocol.TBinaryProtocolFactory()
        server = TServer.TThreadedServer(processor, transport, tfactory, pfactory)

        server.serve()

    except Exception as e:
        print(f"something went wrong: {e}")


if __name__ == "__main__":
    main()
