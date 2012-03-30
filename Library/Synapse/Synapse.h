#ifndef SYNAPSE_H
#define SYNAPSE_H

typedef struct 	__NeuronSynapseList		NeuronSynapseList;

typedef unsigned int 	SynapticDelay;		// No problem would appear when adding to TimeStamp type (long long unsigned int)
typedef double 		SynapticWeight;

#define MINIMUM_EPSP_DELAY 1000000
#define MINIMUM_IPSP_DELAY 1000000

#include "../IzNeuron/Neuron.h"
#include "../ExtNeuron/ExtNeuron.h"
#include <stdlib.h>

struct __NeuronSynapseList
{
	Neuron 			**to;
	SynapticDelay		*delay;
	SynapticWeight	*weight;
	int 				num_of_synapses;	// Determine event_buff_size by incrementing with each connection. Finally allocate the event buffers (time, *from, weight)
};

///////       Fuctions

bool create_synapse(Neuron *this_neuron, Neuron *target_neuron, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, SynapticDelay EPSP_delay_min, SynapticDelay EPSP_delay_max, SynapticDelay IPSP_delay_min, SynapticDelay IPSP_delay_max, double excitatory_connection_probability, double inhibitory_connection_probability, bool *did_connection);
void destroy_neuron_synapse_list(Neuron *neuron);

bool create_synapse_from_ext_neuron_to_int_neuron(ExtNeuron *this_neuron, Neuron *target_neuron, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, SynapticDelay EPSP_delay_min, SynapticDelay EPSP_delay_max, SynapticDelay IPSP_delay_min, SynapticDelay IPSP_delay_max, double excitatory_connection_probability, double inhibitory_connection_probability, bool *did_connection);
void destroy_ext_neuron_synapse_list(ExtNeuron *neuron);
#endif
