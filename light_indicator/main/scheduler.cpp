#include <Arduino.h>
#include "timer_driver.h"

static void (*scheduledTask)(void);
static unsigned long lastRunTime = 0;
const unsigned long interval = 100; // 100 ms

void Scheduler_Init(void (*task)(void)) {
    scheduledTask = task;
    lastRunTime = Timer_GetMillis();
}

void Scheduler_Run() {
    unsigned long now = Timer_GetMillis();
    if (now - lastRunTime >= interval) {
        lastRunTime = now;
        if (scheduledTask) scheduledTask();
    }
}

