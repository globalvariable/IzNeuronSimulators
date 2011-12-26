#ifndef NETWORK_H
#define NETWORK_H

typedef struct 	__NeuronGroup 	NeuronGroup;
typedef struct 	__Layer 		Layer;
typedef struct 	__AllNetwork 	AllNetwork;

#include "../IzNeuron/Neuron.h"
#include "../Event/Event.h"
#include "../Synapse/Synapse.h"

struct __AllNetwork
{
	Layer	**layers;
	int 		layer_count;
};

struct __Layer
{
	NeuronGroup	**neuron_groups;
	int 			neuron_group_count;
};

struct __NeuronGroup
{
	Neuron	*neurons;
	int 		neuron_count;
};

AllNetwork	*all_network;

bool initialize_network(void);
bool add_neurons_to_layer(int num_of_neuron, int layer, double v, double a, double b, double c,double d, double k, double C, double v_resting, double v_threshold, double v_peak, double I_inject, bool inhibitory, 
							double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory, int randomize_params);
bool add_layer_to_network(int layer);							
bool add_neuron_group_to_layer(int layer, int num_of_neuron);
				
bool connect_layer_to_layer(int ThisLayer, int TargetLayer, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, 
						SynapticDelay EPSP_delay_min, SynapticDelay EPSP_delay_max, SynapticDelay IPSP_delay_min, SynapticDelay IPSP_delay_max, float connection_probability);
bool is_layer_free (GPtrArray *layer);
bool is_neuron_group_free (GPtrArray *nrn_group);
bool reset_all_neurons (void);
bool reset_all_synapses (double weight_excitatory_max, double weight_excitatory_min, double weight_inhibitory_max, double weight_inhibitory_min);
bool finalize_network_construction(void);
Neuron* is_neuron_allocated(int layer, int nrn_grp, int nrn);
bool reset_neurons_in_group(int nrn_grp, int layer, double v, double a, double b, double c,double d, double I_inject, bool inhibitory, int randomize_params, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory);
bool reset_neuron(int nrn_num, int nrn_grp, int layer, double v, double a, double b, double c,double d, double I_inject, bool inhibitory, int randomize_params, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory);

#endif
