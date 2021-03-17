import time
from machine import Timer
from machine import Pin
from time import sleep
import machine, time
from machine import Timer
from network import Bluetooth
import ubinascii

#Pin Instance
#button = Pin('P10', mode = Pin.IN)
#Create BLE Instance
bluetooth = Bluetooth()
#Duration of the Scan
TIME_SCANNING = 3
#Variable to hold the mac address
macs = []
#Variable that defines the pir active state
active_state_pir = 0
#Variable that defines the time needed to have no movement for the scan
buffer_timer = 10
def scan():
    #BLE's Mac Array
    blescanmac = []
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
    time.sleep(3)
    return blescanmac

def sleepmode():
    switch=Pin('P10', Pin.IN, Pin.PULL_UP)

    print ('switch', switch(), 'deepsleep')                               #read switch eg 0=on
    machine.pin_sleep_wakeup(pins=['P10'],mode=not switch(),enable_pull=1)#wakeup when switch changes eg 1=off
    print('5 seconds before sleep')
    time.sleep(1)
    print('Going to sleep now')

    machine.deepsleep()


def time_interruption():
    chrono = Timer.Chrono()


    p_in = Pin('P10', mode=Pin.IN, pull=Pin.PULL_UP)
    p_in() # get value, 0 or 1
    chrono.start()
    while chrono.read() < buffer_timer: #10s até scan se houver movimento volta a 0s
        total = chrono.read()
        print(p_in())
        if p_in() ==  active_state_pir:
            chrono.reset()
            print('New person - reset')

        print("\nthe racer took %f seconds to finish the race" % total)

led = Pin('P19', mode=Pin.OUT)
led.value(1)
time_interruption()
scan()
led.value(0)
sleepmode()
