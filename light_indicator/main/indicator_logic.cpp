#include <Arduino.h>
#include "gpio_driver.h"
#include "uart_driver.h"

#define LEFT_BUTTON 4
#define RIGHT_BUTTON 3
#define LEFT_LED 11
#define RIGHT_LED 10

#define LONG_PRESS_TIME 1000
#define TOGGLE_INTERVAL 300

static bool leftIndicatorOn = false;
static bool rightIndicatorOn = false;
static bool hazardMode = false;
static bool toggleState = false;

static unsigned long leftPressStart = 0;
static unsigned long rightPressStart = 0;
static unsigned long lastToggle = 0;

static bool leftHandled = false;
static bool rightHandled = false;

void IndicatorLogic_Init() {
    leftIndicatorOn = false;
    rightIndicatorOn = false;
    hazardMode = false;
    toggleState = false;
}

void IndicatorLogic_Update(unsigned long now) {
    bool leftPressed = GPIO_Read(LEFT_BUTTON) == HIGH;
    bool rightPressed = GPIO_Read(RIGHT_BUTTON) == HIGH;

    // Left Button Logic
    if (leftPressed) {
        if (leftPressStart == 0) leftPressStart = now;
        if (!leftHandled && now - leftPressStart >= LONG_PRESS_TIME) {
            if (hazardMode) {
                hazardMode = false;
                UART_Print("Hazard Mode Deactivated");
            } else {
                leftIndicatorOn = !leftIndicatorOn;
                rightIndicatorOn = false;
                UART_Print(leftIndicatorOn ? "Left Indicator ON" : "Left Indicator OFF");
            }
            leftHandled = true;
        }
    } else {
        leftPressStart = 0;
        leftHandled = false;
    }

    // Right Button Logic
    if (rightPressed) {
        if (rightPressStart == 0) rightPressStart = now;
        if (!rightHandled && now - rightPressStart >= LONG_PRESS_TIME) {
            if (hazardMode) {
                hazardMode = false;
                UART_Print("Hazard Mode Deactivated");
            } else {
                rightIndicatorOn = !rightIndicatorOn;
                leftIndicatorOn = false;
                UART_Print(rightIndicatorOn ? "Right Indicator ON" : "Right Indicator OFF");
            }
            rightHandled = true;
        }
    } else {
        rightPressStart = 0;
        rightHandled = false;
    }

    // Hazard Mode
    if (leftPressed && rightPressed &&
        (now - leftPressStart >= LONG_PRESS_TIME) &&
        (now - rightPressStart >= LONG_PRESS_TIME)) {
        if (!hazardMode) {
            hazardMode = true;
            leftIndicatorOn = false;
            rightIndicatorOn = false;
            UART_Print("Hazard Mode Activated");
        }
    }
}

void IndicatorLogic_Toggle() {
    unsigned long now = millis();
    if (now - lastToggle >= TOGGLE_INTERVAL) {
        lastToggle = now;
        toggleState = !toggleState;

        if (hazardMode) {
            GPIO_Write(LEFT_LED, toggleState);
            GPIO_Write(RIGHT_LED, toggleState);
        } else {
            GPIO_Write(LEFT_LED, leftIndicatorOn ? toggleState : LOW);
            GPIO_Write(RIGHT_LED, rightIndicatorOn ? toggleState : LOW);
        }
    }
}

