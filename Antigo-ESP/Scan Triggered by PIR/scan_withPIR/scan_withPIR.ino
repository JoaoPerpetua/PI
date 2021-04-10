#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <ArduinoJson.h>

#define timeSeconds 10

// Set GPIOs for LED and PIR Motion Sensor
const int led = 33;
const int motionSensor = 34;

// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

//Set BLE scan related variables 
uint8_t scanTime = 5; //In seconds
BLEScan *pBLEScan;
volatile uint8_t n_beacons = 0; //variable that holds the number of beacons using EddyStone and iBeacon  
volatile char mac_array [100][12]; //array that contains the scanned MACs address 
uint8_t i, k; 


//Function that gets the beacon's MAC address and fills the array holding them
void get_mac_addr(BLEAdvertisedDevice advertisedDevice)
{
   char mac_addr[12];
   uint8_t b = 0; 
   for( i = 0; i < 18; i++)
   {
     char half_byte = advertisedDevice.getAddress().toString()[i]; 
     if(half_byte != ':')
     {
        mac_addr[b] = half_byte; 
        mac_array[n_beacons][b] = half_byte;  
       // Serial.printf("char : %c \n",mac_array[n_beacons][b] ); 
        b ++ ;  
        
     }
   } 
   n_beacons++;
}


class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{ 
    
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
      
           
        if (advertisedDevice.haveManufacturerData() == true)
        {
          std::string strManufacturerData = advertisedDevice.getManufacturerData();

          uint8_t cManufacturerData[100];
          strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

          if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00)
          {
            get_mac_addr(advertisedDevice); 
          }
   
        return;
      }
      
      uint8_t *payLoad = advertisedDevice.getPayload();

      BLEUUID checkUrlUUID = (uint16_t)0xfeaa;

      if (advertisedDevice.getServiceUUID().equals(checkUrlUUID))
      {
        get_mac_addr(advertisedDevice);
      }
    }
};
void scan()
{
  Serial.print("\nStarting Scan...");
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("\nDevices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("\nScan done!");
  
  char mac_array_final [n_beacons][12]; 
  Serial.printf("Number of beacons: %d \n", n_beacons);
  
  for( i = 0; i < n_beacons; i++)
  {
    //Print da info dos beacons
    Serial.printf("Beacon #%d  | Address %s \n", i, mac_array[i]); 

    //Eliminação dos dados guardados na variável dos MACs
    for( k = 0; k < 12; k++)
    {
      mac_array_final[i][k] = mac_array[i][k];  
      mac_array[i][k] = NULL;   
    }
  }
  //Fazer Parse aqui, depois do Scan ter terminado.
 
  for( i = 0; i < n_beacons; i++)
  {
    //Eliminação dos dados guardados na variável dos MACs
    for( k = 0; k < 12; k++)
    {
      mac_array_final[i][k] = NULL;
    }
  }
  pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
  n_beacons = 0; 
}
void setup() {
  // Serial port for debugging purposes
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_32,1);
  Serial.begin(115200);
  Serial.print("Wake up");

  //BLE Configurations Setup
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value
  
  
  
  scan(); 
  //Serial.println("ZZZZ"); 
  esp_deep_sleep_start();
}

void loop() {
  /*// Current time
  now = millis();
  // Turn off the LED after the number of seconds defined in the timeSeconds variable
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    Serial.println("Motion stopped...");
    
    digitalWrite(led, LOW);
    startTimer = false;
  }
  */
}
