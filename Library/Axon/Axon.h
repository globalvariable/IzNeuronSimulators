#ifndef AXON_H
#define AXON_H

typedef struct 	__NeuronAxonList		NeuronAxonList;

typedef unsigned int 	AxonalDelay;		// No problem would appear when adding to TimeStamp type (long long unsigned int)

#include <stdlib.h>

#include "../../../BlueSpike/Library/Misc/Randn.h"
#include "../Neuron/Neuron.h"
#include "../Synapse/Synapse.h"




struct __NeuronAxonList
{
	Neuron 			**to;
	AxonalDelay		*delay;
	SynapseIndex		*syn_idx;
	unsigned int		num_of_axons;	// Determine event_buff_size by incrementing with each connection. Finally allocate the event buffers (time, *from, weight)
	unsigned int		*target_layer;		// it is added here to save the axonal connections and load them later on during file save/load
	unsigned int		*target_neuron_group;	// it is added here to save the axonal connections and load them later on during file save/load
	unsigned int		*target_neuron_num;	// it is added here to save the axonal connections and load them later on during file save/load
};

///////       Fuctions

bool create_axon(Neuron *this_neuron, Neuron *target_neuron, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, AxonalDelay EPSP_delay_min, AxonalDelay EPSP_delay_max, AxonalDelay IPSP_delay_min, AxonalDelay IPSP_delay_max, AxonalDelay delay_hard_min, AxonalDelay delay_hard_max, double excitatory_connection_probability, double inhibitory_connection_probability, bool *did_connection, bool excitatory_plastic, bool inhibitory_plastic);
bool create_axon_with_values(Neuron *this_neuron, Neuron *target_neuron, SynapticWeight weight, AxonalDelay arg_delay, AxonalDelay delay_hard_min, AxonalDelay delay_hard_max, bool plastic);
void destroy_neuron_axon_list(Neuron *neuron);
bool search_identical_axon (Neuron *this_neuron, Neuron *target_neuron);
#endif
