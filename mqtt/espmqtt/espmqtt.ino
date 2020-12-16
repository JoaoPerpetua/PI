

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ArduinoJson.h>


// Replace the next variables with your SSID/Password combination
const char* ssid = "1234";
const char* password = "1234";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.1.144";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

const char* resource = "/device";

// LED Pin
const int ledPin = 4;

void setup() {
  Serial.begin(115200);
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  //status = bme.begin();  
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;


   
    
    if(sendRequest(mqtt_server, resource)) {
      Serial.print("HTTP POST request finished.");
    }
   
  }
}



bool sendRequest(const char* host, const char* resource) {
  // Reserve memory space for your JSON data
 
 StaticJsonDocument<256> root;

 //a leitura do sensores é aqui



    
    // Build your own object tree in memory to store the data you want to send in the request
    
    root["sensor"] = "BLE";
    root["mac"], "11-11-11-11-11";
    
  
  // Generate the JSON string
 serializeJson(root, Serial);
  
  Serial.print("POST ");
  Serial.println(resource);


  char output[100];
  serializeJson(root, output);
  if (client.publish("esp/device", output) == true) {
    Serial.println("Success sending message");
  } else {
    Serial.println("Error sending message");
  }
 

  return true;
}
