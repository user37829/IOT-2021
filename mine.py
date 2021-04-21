#coding: utf-8
import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
from gpiozero import Buzzer, LED

ID = "mqtt-iprofi_730109725-3soglt"

vent = LED(26) # нет отдельного класса для реле/транзистора
buzzer = Buzzer(22)

def on_connect(client, userdata, flags, rc):
    print ("Connected")
    client.subscribe("buzzer")
    client.subscribe("ventilation")

def on_message(client, userdata, msg):
    msg_p = str(msg.payload).strip()
    if msg.topic == "buzzer":
        buzzer.value = msg_p == "1"
    elif msg.topic == "ventilation":
        vent.value = msg_p == "1"

client = mqtt.Client(ID)
client.on_connect = on_connect
client.on_message = on_message
client.connect("sandbox.rightech.io", 1883, 60)
client.loop_forever()
