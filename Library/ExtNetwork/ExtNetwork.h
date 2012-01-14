#ifndef EXTNETWORK_H
#define EXTNETWORK_H


typedef struct 	__ExtNeuronGroup 	ExtNeuronGroup;
typedef struct 	__ExtLayer 		ExtLayer;
typedef struct 	__ExtNetwork 	ExtNetwork;

#include "../ExtNeuron/ExtNeuron.h"
#include "../Network/Network.h"

struct __ExtNetwork
{
	ExtLayer	**layers;
	int 		layer_count;
};

struct __ExtLayer
{
	ExtNeuronGroup	**neuron_groups;
	int 				neuron_group_count;
	ExtLayer			*connected_to_ext_network_layer;
	Layer			*connected_to_network_layer;
};

struct __ExtNeuronGroup
{
	ExtNeuron	*neurons;
	int 			neuron_count;
};



ExtNetwork* allocate_external_network(ExtNetwork *network);
ExtNetwork* deallocate_external_network(ExtNetwork *network);
bool add_neurons_to_external_network_layer(ExtNetwork *ext_network, int num_of_neuron, int layer, bool inhibitory);


#endif
