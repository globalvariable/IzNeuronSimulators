#ifndef SYNAPSE_H
#define SYNAPSE_H

typedef struct 	__MainSynapseList			MainSynapseList;
typedef struct 	__LayerSynapseList 		LayerSynapseList;
typedef struct 	__NeuroGroupSynapseList 	NeuroGroupSynapseList;
typedef struct 	__NeuronSynapseList		NeuronSynapseList;

typedef unsigned int 	SynapticDelay;		// No problem would appear when adding to TimeStamp type (long long unsigned int)
typedef double 		SynapticWeight;

#include "../IzNeuron/Neuron.h"

struct __MainSynapseList
{
	LayerSynapseList	**layer_syn_list;
	int 				layer_syn_list_count;
};

struct __LayerSynapseList
{
	NeuroGroupSynapseList	**neuron_group_syn_list;
	int 					neuron_group_syn_list_count;
};

struct __NeuroGroupSynapseList
{
	NeuronSynapseList	*neuron_syn_list;
	int 				neuron_syn_list_count;
};

struct __NeuronSynapseList
{
	Neuron 			**to;
	SynapticDelay		*delay;
	SynapticWeight		*weight;
	int 				num_of_synapses;	// Determine event_buff_size by incrementing with each connection. Finally allocate the event buffers (time, *from, weight)
};



///////       Fuctions
bool initialize_main_synapse_list(void);
bool add_layer_synapse_list_to_main_synapse_list(int layer);
bool add_neuron_group_synapse_list_to_layer_synapse_list(int layer, int num_of _neuron);
void initialize_layer_connections_matrix(void);
bool add_layer_to_layer_connections_matrix(int new_num_of_layer);				


#endif
