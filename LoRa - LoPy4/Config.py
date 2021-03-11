""" LoPy LoRaWAN Nano Gateway configuration options """
GATEWAY_ID = '11aa334455bb7788' # specified in when registering your gateway
SERVER = 'router.eu.thethings.network' # server address & port to forward received data to
PORT = 1700
NTP = "pool.ntp.org" # NTP server for getting/setting time
NTP_PERIOD_S = 3600 # NTP server polling interval
WIFI_SSID = 'my-wifi'
WIFI_PASS = 'my-wifi-password'
LORA_FREQUENCY = 868100000 # check your specifc region for LORA_FREQUENCY and LORA_DR (datarate)
LORA_DR = "SF7BW125"   # DR_5