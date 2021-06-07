import paho.mqtt.client as mqtt

import json
import time, argparse
import qwiic

parser = argparse.ArgumentParser()
parser.add_argument('--change_address', dest="change_address", action="store_true")
parser.set_defaults(change_address=False)
args = parser.parse_args()

if args.change_address:
    import VL53L1X_change_address
    VL53L1X_change_address.change_address(old_address=41, new_address=85)

# Connect to Mux
mux = qwiic.QwiicTCA9548A()
# Initially Disable All Channels
mux.disable_all()

pca = qwiic.QwiicPCA9685()
pca.set_addr_bit(0, 0)

mux.enable_channels(3)
mux.enable_channels(4)

ToF_front = qwiic.QwiicVL53L1X(41)
ToF_front.SensorInit()
print("Forward sensor initialized.")
ToF_rear = qwiic.QwiicVL53L1X(85)
ToF_rear.SensorInit()
print("Rear sensor initialized.")


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("10.24.95.233", 8883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.

try:
    client.loop_start()

    while True:
        # Start Measurements
        ToF_front.StartRanging()
        time.sleep(.005)
        ToF_rear.StartRanging()
        time.sleep(.005)

        # Take Measurements
        fwd_distance = ToF_front.GetDistance()
        time.sleep(.005)
        rear_distance = ToF_rear.GetDistance()
        time.sleep(.005)

        # Stop Measurements
        ToF_front.StopRanging()
        time.sleep(.005)
        ToF_rear.StopRanging()

        reading = {
            'fwd_distance': fwd_distance,
            'rear_distance': rear_distance
        }
        data = json.dumps(reading)
        client.publish("qwiic/tof", data)

except KeyboardInterrupt:
    print("Keyboard interrupt..")
finally:
    client.disconnect()
    client.loop_stop()
