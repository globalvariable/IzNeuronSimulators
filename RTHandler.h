#ifndef RTHANDLER_H
#define RTHANDLER_H

#include <rtai_serial.h>
#include <rtai_lxrt.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include "Simulate.h"

#define MY_CPU_ID 5 

bool SelfClock;

int EndTask;
int NeuroSimThread;

void *NeuroSimHandler(void *args);

#endif
