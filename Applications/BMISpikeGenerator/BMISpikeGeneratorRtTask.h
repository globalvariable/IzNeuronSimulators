#ifndef BMI_SPIKE_GENERATOR_RT_TASK_H
#define BMI_SPIKE_GENERATOR_RT_TASK_H

#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "../../../BlueSpike/Library/RtTaskStats/RtTaskStats.h"
#include "../../../BlueSpike/Library/SpikeGen/SpikeGen.h"
#include "BMISpikeGenerator.h"



void bmi_simulation_spike_generator_create_rt_thread(void);
void bmi_simulation_spike_generator_kill_rt_task(void);









#endif

