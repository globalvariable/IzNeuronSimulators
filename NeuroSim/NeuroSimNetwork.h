#ifndef NEUROSIM_NETWORK_H
#define NEUROSIM_NETWORK_H

#include "../Library/ExtNetwork/ExtNetwork.h"


ExtNetwork * neurosim_get_ext_network(void);
void neurosim_set_ext_network(ExtNetwork *network);

Network *spike_pattern_generator_get_network(void);			///	dummy definitions to enable compilation with datahandlers os SpikePAtternGenerator 
void spike_pattern_generator_set_network(Network *network);


#endif
