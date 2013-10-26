#ifndef SYNAPSE_H
#define SYNAPSE_H

#define	EXCITATORY_SYNAPSE	1
#define	INHIBITORY_SYNAPSE	0

#include <stdbool.h>

#define SYNAPTIC_WEIGHT_HISTORY_BUFFER_SIZE	50

typedef unsigned int SynapseIndex;   

typedef struct 	__Synapse		Synapse;
typedef struct 	__SynapseList		SynapseList;
typedef double SynapticWeight;
typedef bool SynapseType;   // excitatory = 1, inhibitory = 0;

#include "../Network/Network.h"
#include "../STDP/STDP.h"
#include "../Eligibility/Eligibility.h"

struct __Synapse
{
	SynapticWeight	weight;
	SynapseType		type;    // save the list of the synapse type. Do not let excitatory synapses be negative or inhibitory synapses be positive during learning.
	bool			plastic;
	NeuronSynapticEventBuffer	*event_buffer;
	Network			*axon_from_network;
	unsigned int		axon_from_layer;
	unsigned int		axon_from_neuron_group;
	unsigned int		axon_from_neuron;
	SynapticWeight	weight_history[SYNAPTIC_WEIGHT_HISTORY_BUFFER_SIZE];
	unsigned int		weight_history_write_idx;
	PsStdpPrePost	*ps_stdp_pre_post; 	// parker_sochacki_stdp polynomial vals.  it is allocated when synapse is plastic
	PsStdpPostPre	*ps_stdp_post_pre; 	// parker_sochacki_stdp polynomial vals.  it is allocated when synapse is plastic
	PsEligibility		*ps_eligibility;
};

struct __SynapseList
{
	Synapse			*synapses;
	SynapseIndex		num_of_synapses; 
};


bool get_num_of_synapses_in_neuron(Network *network, unsigned int layer, unsigned int nrn_grp, unsigned int nrn_num, unsigned int *num_of_synapses);
bool update_neuron_synaptic_weights(Neuron *neuron, double reward);
bool update_neuron_synaptic_weights_with_history(Neuron *neuron, double reward, double learning_rate);
bool set_neuron_synaptic_weights(Neuron *neuron, double lower_limit, double upper_limit);
bool set_neuron_excitatory_synaptic_weights(Neuron *neuron, double lower_limit, double upper_limit);
bool set_neuron_synapse_synaptic_weight(Neuron *neuron, double lower_limit, double upper_limit, SynapseIndex syn_idx);
void write_to_synapse_history_buffer(Synapse *synapse);
bool write_neuron_synaptic_weights_to_history(Neuron *neuron);
#endif
