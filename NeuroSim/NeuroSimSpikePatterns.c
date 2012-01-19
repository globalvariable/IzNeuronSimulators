#include "NeuroSimSpikePatterns.h"

static SpikePatterns *ext_network_spike_patterns = NULL;
static SpikePatterns *network_spike_patterns = NULL;

SpikePatterns* neurosim_get_ext_network_spike_patterns(void)
{
	return ext_network_spike_patterns;
}
void neurosim_set_ext_network_spike_patterns(SpikePatterns* spike_patterns)
{
	ext_network_spike_patterns = spike_patterns;
}


SpikePatterns* neurosim_get_network_spike_patterns(void)
{
	return network_spike_patterns;
}		

void neurosim_set_network_spike_patterns(SpikePatterns* spike_patterns)
{
	network_spike_patterns = spike_patterns;
}


