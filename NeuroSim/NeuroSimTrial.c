#include "NeuroSimTrial.h"



void neurosim_set_num_of_trials(int num_of_trials)
{
	num_of_neurosim_trials = num_of_trials;
	printf("NeuroSimTrial: INFO: NeuroSim number of trails is %d\n", num_of_neurosim_trials);
}


int  neurosim_get_num_of_trials(void)
{
	return num_of_neurosim_trials;
}
