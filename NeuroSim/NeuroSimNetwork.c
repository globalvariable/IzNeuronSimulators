#include "NeuroSimNetwork.h"


static ExtNetwork *ext_network = NULL;

ExtNetwork * neurosim_get_ext_network(void)
{
	return ext_network;
}

void neurosim_set_ext_network(ExtNetwork *network)
{
	ext_network = network;
}


Network *spike_pattern_generator_get_network(void)			///	dummy definitions
{
	return NULL;
}

void spike_pattern_generator_set_network(Network *network)
{
	return;
}
