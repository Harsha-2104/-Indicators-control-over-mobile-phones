#include <Arduino.h>
#include "uart_driver.h"

void UART_Init() {
    Serial.begin(9600);
    Serial.println("System Initialized");
}

void UART_Print(const char* msg) {
    Serial.println(msg);
}

