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

bool add_neurons_to_layer(int num_of_neuron, int layer, double v, double a, double b, double c,double d, double k, double C, double v_resting, double v_threshold, double v_peak, double I_inject, bool inhibitory, 
							double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory, int randomize_params);
bool add_layer_to_network(int layer);		
bool increment_number_of_layers(void);
bool increment_number_of_neuron_group_in_layer(int layer);
bool add_neuron_group_to_layer(int layer, int num_of_neuron, double v, double a, double b, double c, double d, double k, double C, double v_resting, double v_threshold, double v_peak, double I_inject, bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory);				
bool connect_layer_to_layer(int ThisLayer, int TargetLayer, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, 
						SynapticDelay EPSP_delay_min, SynapticDelay EPSP_delay_max, SynapticDelay IPSP_delay_min, SynapticDelay IPSP_delay_max, float connection_probability);
bool increment_layer_connections_matrix_size(void);		
bool decrement_layer_connections_matrix_size(void);
bool is_network_allocated(void);
bool is_layer_free (int layer);
bool is_neuron_group_free (int layer, int neuron_group_num);
bool is_neuron(int layer, int nrn_grp, int nrn_num);
Neuron* get_neuron_address(int layer, int nrn_grp, int nrn_num);					
void reset_all_neuron_dynamics (void);
void interrogate_network(void);
bool destroy_network(void);

#endif
