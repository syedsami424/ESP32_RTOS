//Make esp use only one core:-
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const int led_pin = 2;
/*doubt: why is there "void *parameter"? => in the setup function, you can see
the "NULL" right under the 1024. this is a parameter to be passed here.*/
void toggleLed(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    //Delay(500) would also work, but using this is better as it tells the scheduler to run other processes (tasks) until the delay is over and then come back to run this task
    vTaskDelay(500 / portTICK_PERIOD_MS); 
    /*doubt: what is a TICK? => The hardware timer in the esp that. when it is called like above, it causes a delay.
    By default, the value of portTICK_PERIOD_MS is 1*/
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  pinMode(led_pin,OUTPUT);
  /* we need to tell RTOS scheduler that there is a 
  function we created, so that it can prioritize and time slice it appropriately*/
  xTaskCreatePinnedToCore(
    toggleLed,     //function or task or process to be called 
    "Toggle LED", //name of task
    1024, //allocate a stack size for this process. (min stack size is 768)
    NULL, //parameter to pass to this process
    1, //priority
    NULL, //task handle, allows user to see memory usage of this process
    app_cpu //which core this process should run on.
  );
}

void loop() {

}
