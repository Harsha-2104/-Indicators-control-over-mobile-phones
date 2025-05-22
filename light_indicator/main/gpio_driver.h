#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

void GPIO_Init();
int GPIO_Read(int pin);
void GPIO_Write(int pin, int value);

#define HIGH 1
#define LOW  0

#endif

