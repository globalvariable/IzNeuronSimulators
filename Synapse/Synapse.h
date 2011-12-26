#ifndef SYNAPSE_H
#define SYNAPSE_H

typedef struct 	__MainSynapseList			MainSynapseList;
typedef struct 	__LayerSynapseList 		LayerSynapseList;
typedef struct 	__NeuronGroupSynapseList 	NeuronGroupSynapseList;
typedef struct 	__NeuronSynapseList		NeuronSynapseList;

typedef unsigned int 	SynapticDelay;		// No problem would appear when adding to TimeStamp type (long long unsigned int)
typedef double 		SynapticWeight;

#define MINIMUM_EPSP_DELAY 1000000
#define MINIMUM_IPSP_DELAY 1000000

#include "../IzNeuron/Neuron.h"
#include "../Event/Event.h"
#include <stdlib.h>

struct __MainSynapseList
{
	LayerSynapseList	**layer_syn_list;
	int 				layer_syn_list_count;
};

struct __LayerSynapseList
{
	NeuronGroupSynapseList	**neuron_group_syn_list;
	int 					neuron_group_syn_list_count;
};

struct __NeuronGroupSynapseList
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

MainSynapseList	*main_synapse_list;		 // Use dynamic allocation for each neuron.    Layer ----> Neuron Group ---> Neuron   3D array. 

struct __LayerConnectionsMatrix
{
	int 		**connections;
	int		matrix_size;
} layer_connections_matrix;

///////       Fuctions
bool initialize_main_synapse_list(void);
bool add_layer_synapse_list_to_main_synapse_list(int layer);
bool add_neuron_group_synapse_list_to_layer_synapse_list(int layer, int num_of_neuron);
void initialize_layer_connections_matrix(void);
bool add_layer_to_layer_connections_matrix(int new_num_of_layer);				
int create_synapse(Neuron *this_neuron, Neuron *target_neuron, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, SynapticDelay EPSP_delay_min, SynapticDelay EPSP_delay_max, SynapticDelay IPSP_delay_min, SynapticDelay IPSP_delay_max, float connection_probability);

#endif
