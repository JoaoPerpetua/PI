
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */
#define MOVEMENT 0
#define TIME_DELAY  30 //30 segundos
void setup() {
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_34, 0);
  //esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP*uS_TO_S_FACTOR);
  esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF); 
  //esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF); 
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF); 
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF); 
  Serial.begin(115200); 
  Serial.println("Woke up");
  
  pinMode(34, INPUT); 
  boolean room_clear = false; 
  while(room_clear == false) 
  {
    if(digitalRead(34) == MOVEMENT)
    {
      room_clear = false; 
      Serial.println("há movimento");
      delay(TIME_DELAY * 1000); 
    }
    else
    {
      room_clear = true; 
      Serial.println("não há movimento - siga mandar scan");
    }
  }
  //Scan & send data
  delay(1000); 
  Serial.println("ZZZZZ");
  esp_deep_sleep_start();
}

void loop() {
}

 /*unsigned long int now = millis(); 
  Serial.println("Woke up");
  while(now < (time_seconds*1000))
  {
    now = millis(); 
  }
  */
