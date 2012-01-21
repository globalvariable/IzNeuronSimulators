#include "NeuroSimSpikePatterns.h"

static SpikePatterns *ext_network_spike_patterns = NULL;
static SingleSpikePattern *ext_network_single_spike_pattern = NULL;

static SpikePatterns *network_spike_patterns = NULL;
static SingleSpikePattern *network_single_spike_pattern = NULL;

SpikePatterns* neurosim_get_ext_network_spike_patterns(void)
{
	return ext_network_spike_patterns;
}
void neurosim_set_ext_network_spike_patterns(SpikePatterns* spike_patterns)
{
	ext_network_spike_patterns = spike_patterns;
}

SingleSpikePattern* neurosim_get_ext_network_single_spike_pattern(void)
{
	return ext_network_single_spike_pattern;
}

void neurosim_set_ext_network_single_spike_pattern(SingleSpikePattern* single_spike_pattern)
{
	ext_network_single_spike_pattern = single_spike_pattern;
}


SpikePatterns* neurosim_get_network_spike_patterns(void)
{
	return network_spike_patterns;
}

void neurosim_set_network_spike_patterns(SpikePatterns* spike_patterns)
{
	network_spike_patterns = spike_patterns;
}

SingleSpikePattern* neurosim_get_network_single_spike_pattern(void)
{
	return network_single_spike_pattern;
}

void neurosim_set_network_single_spike_pattern(SingleSpikePattern* single_spike_pattern)
{
	network_single_spike_pattern = single_spike_pattern;
}

