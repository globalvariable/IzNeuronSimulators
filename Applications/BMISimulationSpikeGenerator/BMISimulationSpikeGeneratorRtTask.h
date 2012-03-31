#ifndef BMI_SIMULATION_SPIKE_GENERATOR_RT_TASK_H
#define BMI_SIMULATION_SPIKE_GENERATOR_RT_TASK_H

#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialsData.h"
#include "../../../BlueSpike/Library/RtTaskStats/RtTaskStats.h"
#include "../../Library/SpikeGenData/SpikeGenData.h"
#include "BMISimulationSpikeGenerator.h"



void bmi_simulation_spike_generator_create_rt_thread(void);
void bmi_simulation_spike_generator_kill_rt_task(void);









#endif

