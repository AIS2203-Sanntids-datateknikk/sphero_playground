import paho.mqtt.client as mqtt
import json


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("qwiic/+")
    client.subscribe("sphero/+")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    data = str(msg.payload.decode("utf-8", "ignore"))
    print(f"topic: {msg.topic}, data: {json.loads(data)}")


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("10.24.95.233", 8883, 60)

client.loop_forever()
