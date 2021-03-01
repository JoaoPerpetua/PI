
#include <Arduino.h>




void gotosleep() {
  Serial.println("zzzz");
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_34, 0);
  esp_deep_sleep_start();
  
}

void setup() {
  Serial.begin(115200);
  delay(5000);
  gotosleep();
}

void loop() {
}
