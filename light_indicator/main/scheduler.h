#ifndef SCHEDULER_H
#define SCHEDULER_H

void Scheduler_Init(void (*task)(void));
void Scheduler_Run();

#endif

