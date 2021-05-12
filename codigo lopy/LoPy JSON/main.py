import time
from machine import Timer
from machine import Pin
from time import sleep
import machine, time
from machine import Timer
from network import Bluetooth
import ubinascii
import json
from network import LoRa
import socket




#Pin Instance
#button = Pin('P10', mode = Pin.IN)
#Create BLE Instance
bluetooth = Bluetooth()
#Duration of the Scan
TIME_SCANNING = 2
#Variable to hold the mac address
macs = []
#Variable that defines the pir active state
active_state_pir = 0
#Variable that defines the time needed to have no movement for the scan
buffer_timer = 10

time_sleep = 10000
def loracom(send):
    # Initialise LoRa in LORAWAN mode.
    # Europe = LoRa.EU868
    lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.EU868)
    # create an OTAA authentication parameters, change them to the provided credentials
    app_eui = ubinascii.unhexlify('b827ebfffeb77faa')
    app_key = ubinascii.unhexlify('f8b476241432768a624a684a91078590')
    # join a network using OTAA (Over the Air Activation)
    lora.join(activation=LoRa.OTAA, auth=(app_eui, app_key), timeout=0)
    # wait until the module has joined the network
    while not lora.has_joined():
        time.sleep(2.5)
        print('Not yet joined...')

    print('Joined')

    # create a LoRa socket
    s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)

    # set the LoRaWAN data rate
    s.setsockopt(socket.SOL_LORA, socket.SO_DR, 5)
    # make the socket blocking
    # (waits for the data to be sent and for the 2 receive windows to expire)
    s.setblocking(True)
    # send some data

    s.send(send)
    # make the socket non-blocking
    # (because if there's no data received it will block forever...)
    s.setblocking(False)



def scan():
    #BLE's Mac Array
    blescanmac = []
    #BLE's json Mac Array
    macjson = []
    #Start Scan
    bluetooth.start_scan(TIME_SCANNING)
    #Debug print
    print("Scanning")
    #obter os dados
    while bluetooth.isscanning():
        #print(".")
        adv = bluetooth.get_adv()
        if adv:
            # try to get the complete name
            mfg_data = bluetooth.resolve_adv_data(adv.data, Bluetooth.ADV_MANUFACTURER_DATA)
            info =ubinascii.hexlify(adv.data)

            info = info[0:14]
            if info == b'0201060303aafe':
                print("EddyStone")
                mac=ubinascii.hexlify(adv.mac)
                print(mac)
                blescanmac.append(mac)

            if mfg_data:
                #try to get the manufacturer data (Apple's iBeacon data is sent here)
                print("iBeacon")
                mac=ubinascii.hexlify(adv.mac)
                print(mac)
                blescanmac.append(mac)

    print("Finish Scan")
    #Print Scan result
    blescanmac = list(dict.fromkeys(blescanmac))
    print("Number of detected BLE devices: "+ str(len(blescanmac)))
    print("BLE Mac Addresses: ")
    print(blescanmac)
    macjson=jsonconvert(blescanmac)

    loracom(macjson)

    return blescanmac

def sleepmode():
    switch=Pin('P10', Pin.IN, Pin.PULL_UP)

    print ('switch', switch(), 'deepsleep')                               #read switch eg 0=on
    machine.pin_sleep_wakeup(pins=['P10'],mode=not switch(),enable_pull=1)#wakeup when switch changes eg 1=off
    print('Going to sleep now')

    machine.deepsleep()


def time_interruption():
    #chrono = Timer.Chrono()
    p_in = Pin('P10', mode=Pin.IN, pull=Pin.PULL_UP)
    p_in() # get value, 0 or 1
    #chrono.start()
    #while chrono.read() < buffer_timer: #10s até scan se houver movimento volta a 0s
        #total = chrono.read()
        #print(p_in())
    if p_in() ==  active_state_pir:
        machine.deepsleep(time_sleep)
            #print('New person - reset')

        #print("\nthe racer took %f seconds to finish the race" % total)



def jsonconvert(arr):
	arr2=[]
	l=len(arr)
	i=0


	while i < l:
		y=0
		y=arr[i].decode('utf-8')
		t='"'+y+'"'
		arr2.append(y)

		i+=1

	x= {"mac": arr2 }
	y=json.dumps(x)


	return y
#led = Pin('P19', mode=Pin.OUT)
#led.value(1)
time_interruption()

scan()
#led.value(0)
sleepmode()
