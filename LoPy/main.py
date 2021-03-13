from network import Bluetooth
import machine
import ubinascii
import time

bluetooth = Bluetooth()
# Mac Address Array
blescanmac = []


#create mac address array
blescanmac.clear()
print("Scanning BLE devices")
bluetooth.start_scan(5)
t_end = time.time() + 6
while time.time() < t_end:
    #collect adv packet from BLE scan
    adv=bluetooth.get_adv()
    #if there are ADV packets
    if adv:
        print(".")
        #obluetoothain Mac Address
        mac=ubinascii.hexlify(adv.mac)
        #chekc if I already have it in Mac array
        blescanmac.append(mac)

#Print Scan result
blescanmac = list(dict.fromkeys(blescanmac))
print("Number of detected BLE devices: "+ str(len(blescanmac)))
print("BLE Mac Addresses: ")
print(blescanmac)
