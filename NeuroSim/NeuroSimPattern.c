#include "NeuroSimPattern.h"


static int num_of_spike_generator_data_patterns = 0;



void neurosim_set_num_of_spike_generator_data_patterns(int pattern_num)
{
	num_of_spike_generator_data_patterns = pattern_num;
	printf("NeuroSimPattern: INFO: SpikeGeneratorData number of patterns is %d\n", num_of_spike_generator_data_patterns);
}


int neurosim_get_num_of_spike_generator_data_patterns(void)
{
	return num_of_spike_generator_data_patterns;
}
