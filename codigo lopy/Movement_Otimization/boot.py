# boot.py -- run on boot-up
import pycom
from network import WLAN

pycom.heartbeat(False)
pycom.wifi_on_boot(False)
wlan=WLAN()
wlan.deinit()
