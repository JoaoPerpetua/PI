
#include <Arduino.h>


#define LED_PIN 5

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// Code with critica section
void IRAM_ATTR onTime() {
  portENTER_CRITICAL_ISR(&timerMux);
  digitalWrite(LED_PIN, HIGH);
  portEXIT_CRITICAL_ISR(&timerMux);
}
void setup() {
  Serial.begin(115200);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_34, 0);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Configure the Prescaler at 80 the quarter of the ESP32 is cadence at 80Mhz
  // 80000000 / 800 = 100000 tics / seconde
  timer = timerBegin(0, 800, true);
  timerAttachInterrupt(timer, &onTime, true);

  // Sets an alarm to sound every 10 seconds
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
  Serial.print("ZZZZ");
  esp_deep_sleep_start();
}

void loop() {
}
