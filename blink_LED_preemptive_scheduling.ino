#include<stdlib.h>

#if CONFIG_FREERTOS_UNICORE
  static BaseType_t app_cpu = 1;
#else 
  static BaseType_t app_cpu = 0; 
#endif

static int led_delay = 500; //default rate
static const uint8_t buffer_len = 20;
static const int led_pin = 2;

void blink_led(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
  }
}

void user_input(void *parameters) {
  char buf[buffer_len];
  char input;
  uint8_t idx = 0;
  memset(buf, 0, buffer_len);

  while(1) {
    if (Serial.available() > 0){
      input = Serial.read();
      if (input == '\n') {
        led_delay = input;
        Serial.print("Updated LED delay to: ");
        Serial.println(led_delay);
        memset(buf, 0, buffer_len);
        idx = 0;
      }
      else {
        if (idx < buffer_len - 1) {
          buf[idx] = input;
          idx  ;
        }
      }
    }
  }
}

void setup() {
  pinMode(led_pin, OUTPUT);
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(
    blink_led,
    "Blink LED",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );

  xTaskCreatePinnedToCore(
    user_input,
    "User Input",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
