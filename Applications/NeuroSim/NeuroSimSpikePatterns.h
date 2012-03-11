#ifndef NEUROSIM_SPIKE_PATTERNS_H
#define NEUROSIM_SPIKE_PATTERNS_H


#include "../../BlueSpike/Library/SpikePatterns/SpikePatterns.h"



SpikePatterns* neurosim_get_ext_network_spike_patterns(void);
void neurosim_set_ext_network_spike_patterns(SpikePatterns* spike_patterns);

SingleSpikePattern* neurosim_get_ext_network_single_spike_pattern(void);
void neurosim_set_ext_network_single_spike_pattern(SingleSpikePattern* single_spike_pattern);

SingleSpikePattern* neurosim_get_ext_network_unsorted_single_spike_pattern(void);
void neurosim_set_ext_network_unsorted_single_spike_pattern(SingleSpikePattern* single_spike_pattern);



SpikePatterns* neurosim_get_network_spike_patterns(void);		
void neurosim_set_network_spike_patterns(SpikePatterns* spike_patterns);

SingleSpikePattern* neurosim_get_network_single_spike_pattern(void);
void neurosim_set_network_single_spike_pattern(SingleSpikePattern* single_spike_pattern);







#endif
