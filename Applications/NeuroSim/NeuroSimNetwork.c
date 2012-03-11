#include "NeuroSimNetwork.h"

static Network *internal_network = NULL;
static ExtNetwork *external_network = NULL;

Network * neurosim_get_network(void)
{
	return internal_network;
}

void neurosim_set_network(Network *network)
{
	internal_network = network;
}

ExtNetwork * neurosim_get_ext_network(void)
{
	return external_network;
}

void neurosim_set_ext_network(ExtNetwork *network)
{
	external_network = network;
}


Network *spike_pattern_generator_get_network(void)			///	dummy definitions
{
	return NULL;
}

void spike_pattern_generator_set_network(Network *network)
{
	return;
}
