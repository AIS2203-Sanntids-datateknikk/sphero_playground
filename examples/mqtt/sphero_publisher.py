import paho.mqtt.client as mqtt
import json
import time
import sys
import os

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../sphero-sdk-raspberrypi-python')))

from sphero_sdk import SpheroRvrObserver
from sphero_sdk import RvrStreamingServices


def main():

    connected = False

    # The callback for when the client receives a CONNACK response from the server.
    def on_connect(client, userdata, flags, rc):
        nonlocal connected
        connected = rc == 0
        print("Connected with result code " + str(rc))

    client = mqtt.Client()
    client.on_connect = on_connect

    client.connect("10.24.95.233", 8883, 60)

    rvr = SpheroRvrObserver()
    rvr.wake()
    time.sleep(2)

    rvr.reset_yaw()

    def imu_handler(imu_data):
        data = imu_data["IMU"]
        if connected:
            client.publish("sphero/imu", json.dumps(data))

    def color_detected_handler(color_detected_data):
        data = color_detected_data["ColorDetection"]
        if connected:
            client.publish("sphero/color", json.dumps(data))

    def accelerometer_handler(accelerometer_data):
        data = accelerometer_data["Accelerometer"]
        if connected:
            client.publish("sphero/accelerometer", json.dumps(data))

    def ambient_light_handler(ambient_light_data):
        data = ambient_light_data["AmbientLight"]
        if connected:
            client.publish("sphero/ambient_light", json.dumps(data))

    rvr.sensor_control.add_sensor_data_handler(
        service=RvrStreamingServices.imu,
        handler=imu_handler
    )
    rvr.sensor_control.add_sensor_data_handler(
        service=RvrStreamingServices.color_detection,
        handler=color_detected_handler
    )
    rvr.sensor_control.add_sensor_data_handler(
        service=RvrStreamingServices.accelerometer,
        handler=accelerometer_handler
    )
    rvr.sensor_control.add_sensor_data_handler(
        service=RvrStreamingServices.ambient_light,
        handler=ambient_light_handler
    )

    rvr.sensor_control.start(interval=250)

    try:
        client.loop_forever()
    except KeyboardInterrupt:
        print("Keyboard interrupt..")
    finally:
        client.disconnect()
        client.loop_stop()


if __name__ == "__main__":
    main()

