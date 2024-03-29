import time
from machine import Timer
from machine import Pin
from time import sleep
import machine, time
from machine import Timer
from network import Bluetooth
import ubinascii


def time_interruption():
    chrono = Timer.Chrono()

    active_state_pir = 0
    p_in = Pin('P10', mode=Pin.IN, pull=Pin.PULL_UP)
    p_in() # get value, 0 or 1
    chrono.start()
    while chrono.read() < 10: #10s até scan se houver movimento volta a 0s
        total = chrono.read()
        if p_in() == active_state_pir:
            chrono.reset()
            print('New person - reset')

        print("\nthe racer took %f seconds to finish the race" % total)

time_interruption()

def scanner():

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

scanner()


def sleepmode():
    switch=Pin('P10', Pin.IN, Pin.PULL_UP)

    print ('switch', switch(), 'deepsleep')                               #read switch eg 0=on
    machine.pin_sleep_wakeup(pins=['P10'],mode=not switch(),enable_pull=1)#wakeup when switch changes eg 1=off
    print('5 seconds before sleep')
    time.sleep(5)
    print('Going to sleep now')
    machine.deepsleep()

print('Start sleeping mode')
print('guinha')

sleepmode()
