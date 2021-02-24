import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish

Broker = "localhost"

sub_topic="esp/device"

######setup####
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe(sub_topic)