#ifndef EXTNETWORK_H
#define EXTNETWORK_H


typedef struct 	__ExtNeuronGroup 	ExtNeuronGroup;
typedef struct 	__ExtLayer 		ExtLayer;
typedef struct 	__ExtNetwork 	ExtNetwork;

#include "../ExtNeuron/ExtNeuron.h"
#include "../Network/Network.h"

struct __ExtNetwork
{
	Network		*connected_to_internal_network;
	ExtLayer		**layers;
	unsigned int	layer_count;
};

struct __ExtLayer
{
	ExtNeuronGroup	**neuron_groups;
	unsigned int		neuron_group_count;
	Layer			**connected_to_internal_network_layer;
};

struct __ExtNeuronGroup
{
	ExtNeuron	*neurons;
	unsigned int	neuron_count;
};



ExtNetwork* allocate_external_network(ExtNetwork *network);
ExtNetwork* deallocate_external_network(ExtNetwork *network);
bool add_neurons_to_external_network_layer(ExtNetwork *ext_network, unsigned int num_of_neuron, unsigned int layer, bool inhibitory);
bool increment_ext_to_int_network_layer_connection_matrix(ExtNetwork *ext_network);

#endif
