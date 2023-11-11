//Make esp use only one core:-
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const int led_pin = 2;
void toggleLed(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    //Delay(500) would also work, but using this is better as it tells the scheduler to run other processes (tasks) until the delay is over and then come back to run this task
    vTaskDelay(500 / portTICK_PERIOD_MS); 
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  pinMode(led_pin,OUTPUT);
  xTaskCreatePinnedToCore(
    toggleLed,  
    "Toggle LED",
    1024,
    NULL,
    1, 
    NULL, 
    app_cpu
  );
}

void loop() {

}
