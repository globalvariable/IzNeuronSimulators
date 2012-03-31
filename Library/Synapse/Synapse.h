#ifndef SYNAPSE_H
#define SYNAPSE_H

typedef struct 	__NeuronSynapseList		NeuronSynapseList;

typedef unsigned int 	SynapticDelay;		// No problem would appear when adding to TimeStamp type (long long unsigned int)
typedef double 		SynapticWeight;

/// determine values below according to the latency due to PARKER_SOCHACKI_INTEGRATION_STEP_SIZE. 
#define MINIMUM_INTERNAL_TO_INTERNAL_NEURON_EPSP_DELAY 1000000
#define MAXIMUM_INTERNAL_TO_INTERNAL_NEURON_EPSP_DELAY 3000000
#define MINIMUM_INTERNAL_TO_INTERNAL_NEURON_IPSP_DELAY 1000000
#define MAXIMUM_INTERNAL_TO_INTERNAL_NEURON_IPSP_DELAY 3000000

/// determine values below according to the latency due to BlueSpike spike detection - Ext to Internal network event scheduler and internal network simulation period. 
#define MINIMUM_EXTERNAL_TO_INTERNAL_NEURON_EPSP_DELAY 3000000		
#define MAXIMUM_EXTERNAL_TO_INTERNAL_NEURON_EPSP_DELAY 5000000
#define MINIMUM_EXTERNAL_TO_INTERNAL_NEURON_IPSP_DELAY 3000000
#define MAXIMUM_EXTERNAL_TO_INTERNAL_NEURON_IPSP_DELAY 5000000

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
