#include <Arduino.h>
#include "gpio_driver.h"

void GPIO_Init() {
    pinMode(3, INPUT);  // RIGHT_BUTTON
    pinMode(4, INPUT);  // LEFT_BUTTON
    pinMode(10, OUTPUT); // RIGHT_LED
    pinMode(11, OUTPUT); // LEFT_LED
}

int GPIO_Read(int pin) {
    return digitalRead(pin);
}

void GPIO_Write(int pin, int value) {
    digitalWrite(pin, value);
}

