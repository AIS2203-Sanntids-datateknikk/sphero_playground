
import paho.mqtt.client as mqtt

import json
import sys, time
import qwiic_titan_gps


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))


qwiicGPS = qwiic_titan_gps.QwiicTitanGps()
if qwiicGPS.connected is False:
    print("Could not connect to to the SparkFun GPS Unit. Double check that\
              it's wired correctly.", file=sys.stderr)
    sys.exit(-1)

qwiicGPS.begin()

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("10.24.95.233", 8883, 60)

try:
    client.loop_start()

    while True:
        if qwiicGPS.get_nmea_data() is True:
            data = json.dumps({
                "Latitude": qwiicGPS.gnss_messages['Latitude'],
                "Longitude": qwiicGPS.gnss_messages['Longitude']
            })
            client.publish("qwiic/titanx1", data)

            time.sleep(1)

except KeyboardInterrupt:
    print("Keyboard interrupt..")
finally:
    client.disconnect()
    client.loop_stop()
