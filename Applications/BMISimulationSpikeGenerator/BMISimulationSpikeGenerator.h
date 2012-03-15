#ifndef BMI_SIMULATION_SPIKE_GENERATOR_H
#define BMI_SIMULATION_SPIKE_GENERATOR_H

#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include "../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialsData.h"
#include "../../../BlueSpike/Library/RtTaskStats/RtTaskStats.h"
#include "../../Library/SpikeGenData/SpikeGenData.h"
#include "Gui.h"


void set_bmi_simulation_spike_generator_trials_data(TrialsData *data);
TrialsData * get_bmi_simulation_spike_generator_trials_data(void);
void set_bmi_simulation_spike_generator_spike_gen_data(SpikeGenData *data);
SpikeGenData* get_bmi_simulation_spike_generator_spike_gen_data(void);



#endif


