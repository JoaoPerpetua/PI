import time
import pycom
import ubinascii
from network import Bluetooth
from machine import Pin
#Pin Instance
button = Pin('P10', mode = Pin.IN)
#Create BLE Instance
bluetooth = Bluetooth()
#Duration of the Scan
TIME_SCANNING = 3


def scan():
    #BLE's Mac Array
    blescanmac = []
    #Start Scan
    bluetooth.start_scan(TIME_SCANNING)
    #Debug print
    print("Scanning")
    #obter os dados
    while bluetooth.isscanning():
        print(".")
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
    time.sleep(2)
    return blescanmac

while True:
    macs = []
    if(button() == 0):
        print("movement, going to scan")
        macs = scan()
    else:
        print("No movement")
