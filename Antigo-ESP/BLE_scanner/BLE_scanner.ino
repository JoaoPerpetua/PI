
#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEEddystoneURL.h>
#include <BLEEddystoneTLM.h>
#include <BLEBeacon.h>
#include <ArduinoJson.h>

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

int scanTime = 5; //In seconds
BLEScan *pBLEScan;
//variable that holds the number of beacons using EddyStone and iBeacon 
volatile int n_beacons = 0; 
//array that contains the scanned MACs address  
volatile char mac_array [1000][12];

//function that gets the beacon's MAC address and fills the array holding them
void get_mac_addr(BLEAdvertisedDevice advertisedDevice)
{
   char mac_addr[12];
   int b = 0; 
   for(int i = 0; i < 18; i++)
   {
     char half_byte = advertisedDevice.getAddress().toString()[i]; 
     if(half_byte != ':')
     {
        mac_addr[b] = half_byte; 
        mac_array[n_beacons][b] = half_byte;  
        b ++ ;  
     }
   } 
   
   //Serial.printf("\n Mac Address: %s \n",mac_addr);       
   /*
   String s = "";  
   for(int i = 0; i<12; i++)
   {
      s = s + mac_addr[i]; 
   }
   Serial.println(s);
   */
   n_beacons++;
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{ 
    
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
      
      if (advertisedDevice.haveName())
      {
        /*
        Serial.print("Device name: ");
        Serial.println(advertisedDevice.getName().c_str());
        Serial.println("");
        */
      }

      if (advertisedDevice.haveServiceUUID())
      {
       
        BLEUUID devUUID = advertisedDevice.getServiceUUID();
        /*
        Serial.print("Found ServiceUUID: ");
        Serial.println(devUUID.toString().c_str());
        Serial.println("");
        */
        
      }
      else
      {
        if (advertisedDevice.haveManufacturerData() == true)
        {
          std::string strManufacturerData = advertisedDevice.getManufacturerData();

          uint8_t cManufacturerData[100];
          strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

          if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00)
          {
            Serial.println("Found an iBeacon!");
            BLEBeacon oBeacon = BLEBeacon();
            oBeacon.setData(strManufacturerData);
            Serial.printf("iBeacon Frame\n");
            Serial.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d\n", oBeacon.getManufacturerId(), ENDIAN_CHANGE_U16(oBeacon.getMajor()), ENDIAN_CHANGE_U16(oBeacon.getMinor()), oBeacon.getProximityUUID().toString().c_str(), oBeacon.getSignalPower());
            get_mac_addr(advertisedDevice); 
          }
          else
          {
            /*
            Serial.println("Found another manufacturers beacon!");
            Serial.printf("strManufacturerData: %d ", strManufacturerData.length());
            for (int i = 0; i < strManufacturerData.length(); i++)
            {
              Serial.printf("[%X]", cManufacturerData[i]);
            }
            Serial.printf("\n");
            */
          }
        }
        return;
      }
      
      uint8_t *payLoad = advertisedDevice.getPayload();

      BLEUUID checkUrlUUID = (uint16_t)0xfeaa;

      if (advertisedDevice.getServiceUUID().equals(checkUrlUUID))
      {
        if (payLoad[11] == 0x10)
        {
          Serial.println("Found an EddystoneURL beacon!");
          BLEEddystoneURL foundEddyURL = BLEEddystoneURL();
          std::string eddyContent((char *)&payLoad[11]); // incomplete EddystoneURL struct!

          foundEddyURL.setData(eddyContent);
          std::string bareURL = foundEddyURL.getURL();
          if (bareURL[0] == 0x00)
          {
            size_t payLoadLen = advertisedDevice.getPayloadLength();
            Serial.println("DATA-->");
            for (int idx = 0; idx < payLoadLen; idx++)
            {
              Serial.printf("0x%08X ", payLoad[idx]);
            }
            Serial.println("\nInvalid Data");
            return;
          }

          Serial.printf("Found URL: %s\n", foundEddyURL.getURL().c_str());
          Serial.printf("Decoded URL: %s\n", foundEddyURL.getDecodedURL().c_str());
          Serial.printf("TX power %d\n", foundEddyURL.getPower());
          Serial.println("\n");
        }
        else if (payLoad[11] == 0x20)
        {
          Serial.println("Found an EddystoneTLM beacon!");
          BLEEddystoneTLM foundEddyURL = BLEEddystoneTLM();
          std::string eddyContent((char *)&payLoad[11]); // incomplete EddystoneURL struct!

          eddyContent = "01234567890123";

          for (int idx = 0; idx < 14; idx++)
          {
            eddyContent[idx] = payLoad[idx + 11];
          }

          foundEddyURL.setData(eddyContent);
          Serial.printf("Reported battery voltage: %dmV\n", foundEddyURL.getVolt());
          Serial.printf("Reported temperature from TLM class: %.2fC\n", (double)foundEddyURL.getTemp());
          int temp = (int)payLoad[16] + (int)(payLoad[15] << 8);
          float calcTemp = temp / 256.0f;
          Serial.printf("Reported temperature from data: %.2fC\n", calcTemp);
          Serial.printf("Reported advertise count: %d\n", foundEddyURL.getCount());
          Serial.printf("Reported time since last reboot: %ds\n", foundEddyURL.getTime());
          Serial.println("\n");
          Serial.print(foundEddyURL.toString().c_str());
          Serial.println("\n");
        }
        else if (payLoad[11] == 0x00)
        {
          Serial.print("\nFound UID EddyStone Beacon\n");          
          signed int f = payLoad[12]; 
          const int negative = (f & (1 << 7)) != 0;
          int nativeInt;

          if (negative)
            nativeInt = f | ~((1 << 8) - 1);
          else
            nativeInt = f;
            
          Serial.printf("RSSI @ 0m: %d dBm", nativeInt);
          Serial.print("\n");
          Serial.print("NameSpace ID: ");
          for(int k = 13; k < 23; k++)
          {
            byte j = payLoad[k];
            Serial.printf("%x", j);
          }
          Serial.print("\nInstance ID: ");
          for(int k = 23; k < 29; k++)
          {
            byte j = payLoad[k];
            Serial.printf("%x", j);
          }
        }
        get_mac_addr(advertisedDevice);
      }
    }
};

void setup()
{
  Serial.begin(115200);
  Serial.println("Scanning...");
  
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value
}

void loop()
{
  // put your main code here, to run repeatedly:
  n_beacons = 0; 
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
   Serial.println("\nScan done!");

   
  char mac_array_final [n_beacons][12]; 
   
  for(int i = 0; i < n_beacons; i++)
  {
    //Print da info dos beacons
    Serial.printf("Beacon #%d    Address %s\n", i, mac_array[i]); 
    
    //Eliminação dos dados guardados na variável dos MACs
    for(int k = 0; k < 12; k++)
    {
      mac_array_final[i][k] = mac_array[i][k];  
      mac_array[i][k] = NULL;   
    }
  }
  //Fazer Parse aqui, depois do Scan ter terminado.
  StaticJsonDocument<256> root;
  root["mac"] = serialized(mac_array_final);
  serializeJson(root, Serial);
 
  pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
  delay(2000);  
}
