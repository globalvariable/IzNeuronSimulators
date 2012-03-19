#ifndef BMI_SIMULATION_SPIKE_GENERATOR_H
#define BMI_SIMULATION_SPIKE_GENERATOR_H

#include "Gui.h"
#include "BMISimulationSpikeGeneratorRtTask.h"


void set_bmi_simulation_spike_generator_trials_data(TrialsData *data);
TrialsData * get_bmi_simulation_spike_generator_trials_data(void);
void set_bmi_simulation_spike_generator_spike_gen_data(SpikeGenData *data);
SpikeGenData* get_bmi_simulation_spike_generator_spike_gen_data(void);



#endif


