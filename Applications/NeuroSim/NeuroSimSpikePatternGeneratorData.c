#include "NeuroSimSpikePatternGeneratorData.h"


static unsigned int num_of_patterns_in_spike_pattern_generator_data = 0;
static char spike_pattern_generator_data_path[1000];
static unsigned int max_num_of_pattern_spikes_in_spike_pattern_generator_data = 0;
static int spike_pattern_generator_data_version = 0;

void neurosim_set_num_of_patterns_in_spike_pattern_generator_data(unsigned int num_of_patterns)
{
	num_of_patterns_in_spike_pattern_generator_data = num_of_patterns;
}
unsigned int neurosim_get_num_of_patterns_in_spike_pattern_generator_data(void)
{	
	return num_of_patterns_in_spike_pattern_generator_data;
}
void neurosim_set_spike_pattern_generator_data_path(char *path)
{
	strcpy(spike_pattern_generator_data_path, path);
}
char* neurosim_get_spike_pattern_generator_data_path(void)
{
	return spike_pattern_generator_data_path;
}
void neurosim_set_max_num_of_pattern_spikes_in_spike_pattern_generator_data(unsigned int num_of_spikes)
{
	max_num_of_pattern_spikes_in_spike_pattern_generator_data = num_of_spikes;
}
unsigned int neurosim_get_max_num_of_pattern_spikes_in_spike_pattern_generator_data(void)
{
	return max_num_of_pattern_spikes_in_spike_pattern_generator_data;
}
void neurosim_set_spike_pattern_generator_data_version(int version)
{
	spike_pattern_generator_data_version = version;
}
int neurosim_get_spike_pattern_generator_data_version(void)
{
	return spike_pattern_generator_data_version;
}
