
import argparse

from sphero_service import SpheroService, ttypes

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer

import os
import sys
import time
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../sphero-sdk-raspberrypi-python')))

from sphero_sdk import SpheroRvrObserver
from sphero_sdk import RvrStreamingServices


class SpheroHandler:

    def __init__(self):

        self.imu = ttypes.IMU(False, 0, 0, 0)
        self.accelerometer = ttypes.Accelerometer(False, 0, 0, 0)
        self.color = ttypes.Color(False, 0, 0, 0, 0, 0)
        self.ambient = ttypes.AmbientLight(False, 0)

        self.rvr = SpheroRvrObserver()
        self.rvr.wake()
        time.sleep(2)

        self. rvr.reset_yaw()

        self.rvr.sensor_control.add_sensor_data_handler(
            service=RvrStreamingServices.imu,
            handler=self.imu_handler
        )
        self.rvr.sensor_control.add_sensor_data_handler(
            service=RvrStreamingServices.color_detection,
            handler=self.color_detected_handler
        )
        self.rvr.sensor_control.add_sensor_data_handler(
            service=RvrStreamingServices.accelerometer,
            handler=self.accelerometer_handler
        )
        self.rvr.sensor_control.add_sensor_data_handler(
            service=RvrStreamingServices.ambient_light,
            handler=self.ambient_light_handler
        )

        self.rvr.sensor_control.start(interval=250)

    def sense(self):
        return ttypes.SensorData(self.imu, self.accelerometer, self.ambient, self.color)

    def drive_with_heading(self, speed, heading, flags):
        #print(f"speed={speed}, heading={heading}, flags={flags}")
        self.rvr.drive_with_heading(
            speed=speed,  # Valid speed values are 0-255
            heading=heading,  # Valid heading values are 0-359
            flags=flags
        )
        time.sleep(0.1)

    def imu_handler(self, imu_data):
        data = imu_data["IMU"]
        self.imu.is_valid = data["is_valid"]
        self.imu.roll = data["Roll"]
        self.imu.pitch = data["Pitch"]
        self.imu.yaw = data["Yaw"]

    def color_detected_handler(self, color_detected_data):
        data = color_detected_data["ColorDetection"]
        self.color.is_valid = data["is_valid"]
        self.color.r = data["R"]
        self.color.g = data["G"]
        self.color.b = data["B"]
        self.color.index = data["Index"]
        self.color.confidence = data["Confidence"]

    def accelerometer_handler(self, accelerometer_data):
        data = accelerometer_data["Accelerometer"]
        self.accelerometer.is_valid = data["is_valid"]
        self.accelerometer.x = data["X"]
        self.accelerometer.Y = data["Y"]
        self.accelerometer.Z = data["Z"]

    def ambient_light_handler(self, ambient_light_data):
        data = ambient_light_data["AmbientLight"]
        self.ambient.is_valid = data["is_valid"]
        self.ambient.value = data["Light"]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', type=int,
                        help='Remote port')

    args = parser.parse_args()

    try:

        handler = SpheroHandler()
        processor = SpheroService.Processor(handler)
        transport = TSocket.TServerSocket(port=args.port)
        tfactory = TTransport.TFramedTransportFactory()
        pfactory = TBinaryProtocol.TBinaryProtocolFactory()
        server = TServer.TSimpleServer(processor, transport, tfactory, pfactory)

        server.serve()

    except Exception as e:
        print(f"something went wrong: {e}")


if __name__ == "__main__":
    main()
