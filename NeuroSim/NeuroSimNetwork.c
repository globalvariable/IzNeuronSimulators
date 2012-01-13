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



