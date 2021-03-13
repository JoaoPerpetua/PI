

#include <BLEDevice.h> //Gustavo e manu mete nojo
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include <WiFi.h>

#include <PubSubClient.h>
#include <Wire.h>
#include <ArduinoJson.h>

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

//Time between movement detection and the scan
#define timeSeconds 10

// Set GPIOs for LED and PIR Motion Sensor
const int led = 33;
const int motionSensor = 34;

// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;


//MQTT / Network settings
const char* ssid = "MEO-LEVEL1";
const char* password = "c19371c76b";
const char* mqtt_server = "192.168.1.97";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
uint8_t value = 0;



uint8_t scanTime = 5; //In seconds
BLEScan *pBLEScan;
//variable that holds the number of beacons using EddyStone and iBeacon 
volatile uint8_t n_beacons = 0; 
//array that contains the scanned MACs address  
volatile char mac_array [10][12];

//function that gets the beacon's MAC address and fills the array holding them
void get_mac_addr(BLEAdvertisedDevice advertisedDevice)
{
   char mac_addr[12];
   uint8_t b = 0; 
   for(uint8_t i = 0; i < 18; i++)
   {
     char half_byte = advertisedDevice.getAddress().toString()[i]; 
     if(half_byte != ':')
     {
        mac_addr[b] = half_byte; 
        mac_array[n_beacons][b] = half_byte;  
        b ++ ;  
     }
   } 
  
   n_beacons++;
}

// Checks if motion was detected, sets LED HIGH and starts a timer
void detectsMovement() {
  Serial.println("MOTION DETECTED!!!");
  digitalWrite(led, HIGH);
  startTimer = true;
  lastTrigger = millis();
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

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void scan()
{
  Serial.print("\nStarting Scan...");
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("\nDevices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("\nScan done!");
  
  char mac_array_final [n_beacons][12]; 
  Serial.printf("Number of beacons: %d \n", n_beacons);
  
  send_MQTT_data(); 
}

void send_MQTT_data()
{
  //mqtt
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
   
  char mac_array_final [3][15]; 

  //Ciclo para enviar dados artificiais 
  for(uint8_t i = 0; i < 3; i++)
  {
    //Print da info dos beacons
    //Serial.printf("Beacon #%d    Address %s\n", i, mac_array[i]); 
    
      //Eliminação dos dados guardados na variável dos MACs
      for(uint8_t k = 0; k < 15; k++)
      {
          if((k==0)||k==14) 
          {
            mac_array_final[i][k] = '"';
          }
          else if(k==13)
          {
            mac_array_final[i][k] = ','; 
          }
          else
          {
            if(i == 0)
            {
              mac_array_final[i][k] = 'X';   
            } 
            if(i == 1)
            {
              mac_array_final[i][k] = 'Y';   
            }
            if(i == 2)
            {
              mac_array_final[i][k] = 'Z';   
            }                           
           } 
      }
  }

   //Ciclo para enviar os dados do Scan 
  /*
  for(uint8_t i = 0; i < n_beacons; i++)
  {
    //Print da info dos beacons
    //Serial.printf("Beacon #%d    Address %s\n", i, mac_array[i]); 
    
    //Eliminação dos dados guardados na variável dos MACs
   
      for(uint8_t k = 0; k < 14; k++)
        {
          if((k==0)||k==14) {
            mac_array_final[i][k] = '"';
          }
          else{
          mac_array_final[i][k] = mac_array[i][k-1];  
          mac_array[i][k-1] = NULL;  
          } 
        }
  }
  */
  //Fazer Parse aqui, depois do Scan ter terminado.
  StaticJsonDocument<256> root;
  root["mac"] = serialized(mac_array_final);
   
  char output[55];
  serializeJson(root, output);
  if (client.publish("esp/device", output) == true) {
    Serial.println("Success sending message");
  } else {
    Serial.println("Error sending message");
  }

  // delete results fromBLEScan buffer to release memory
  pBLEScan->clearResults(); 
  n_beacons = 0; 
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp/device");
    } else {
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
//  Serial.println("Scanning...");
  
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value
  // MQTT
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);

  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // Set LED to LOW
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
}

  
void loop()
{
  // Current time
  now = millis();
  // Turn off the LED after the number of seconds defined in the timeSeconds variable
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    Serial.println("Motion stopped...");
    scan(); 
    digitalWrite(led, LOW);
    startTimer = false;
  }
}
