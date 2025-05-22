#include <Arduino.h>
#include "gpio_driver.h"
#include "uart_driver.h"
#include "timer_driver.h"
#include "scheduler.h"
#include "indicator_logic.h"

void App_Task() {
    unsigned long currentMillis = millis();
    IndicatorLogic_Update(currentMillis);
    IndicatorLogic_Toggle();
}

void setup() {
    GPIO_Init();
    UART_Init();
    IndicatorLogic_Init();
    Scheduler_Init(App_Task);
}

void loop() {
    Scheduler_Run();  // handles 100 ms task calling
}

