#ifndef EXTNETWORK_H
#define EXTNETWORK_H


typedef struct 	__ExtNeuronGroup 	ExtNeuronGroup;
typedef struct 	__ExtLayer 		ExtLayer;
typedef struct 	__ExtNetwork 	ExtNetwork;

#include "../ExtNeuron/ExtNeuron.h"

struct __ExtNetwork
{
	ExtLayer	**layers;
	int 		layer_count;
};

struct __Layer
{
	ExtNeuronGroup	**neuron_groups;
	int 				neuron_group_count;
};

struct __NeuronGroup
{
	ExtNeuron	*neurons;
	int 			neuron_count;
};




bool add_neurons_to_external_network_layer(ExtNetwork *ext_network, int num_of_neuron, int layer);


#endif
