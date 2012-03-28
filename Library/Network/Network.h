#ifndef NETWORK_H
#define NETWORK_H

typedef struct 	__NeuronGroup 	NeuronGroup;
typedef struct 	__Layer 		Layer;
typedef struct 	__Network 		Network;

#include "../IzNeuron/Neuron.h"
#include "../Event/Event.h"
#include "../Synapse/Synapse.h"
#include "../ExtNetwork/ExtNetwork.h"

struct __Network
{
	ExtNetwork		*connection_from_ext_network;
	Layer			**layers;
	unsigned int 		layer_count;
};

struct __Layer
{
	NeuronGroup	**neuron_groups;
	unsigned int	neuron_group_count;
	Layer		**connected_to_network_layer;	
};

struct __NeuronGroup
{
	Neuron		*neurons;
	unsigned int	neuron_count;
};

Network* allocate_network(Network *network);
Network* deallocate_network(Network *network);
bool add_neurons_to_layer(Network *network, unsigned int num_of_neuron, unsigned int layer, double a, double b, double c,double d, double k, double C, double v_resting, double v_threshold, double v_peak, bool inhibitory, 	 	double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory, unsigned int randomize_params);
bool connect_network_layer_to_network_layer(Network *network, unsigned int this_layer, unsigned int target_layer, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, SynapticDelay EPSP_delay_min, SynapticDelay EPSP_delay_max, SynapticDelay IPSP_delay_min, SynapticDelay IPSP_delay_max, double connection_probability);

bool is_network_allocated(Network *network);
bool is_layer_free (Network *network, int layer);
bool is_neuron_group_free (Network *network, int layer, int neuron_group_num);
bool is_neuron(Network *network, int layer, int nrn_grp, int nrn_num);
Neuron* get_neuron_address(Network *network, int layer, int nrn_grp, int nrn_num);					
void reset_all_network_neuron_dynamics (Network *network);
void interrogate_network(Network *network);
bool get_num_of_layers_in_network(Network *network, unsigned int *num_of_layers);
bool get_num_of_neuron_groups_in_layer(Network *network, unsigned int layer, unsigned int *num_of_neuron_groups);
bool get_num_of_neurons_in_neuron_group(Network *network, unsigned int layer, unsigned int neuron_group, unsigned int *num_of_neurons);
bool get_num_of_neurons_in_network(Network *network, unsigned int *num_of_neurons);
bool get_num_of_neuron_groups_in_network(Network *network, unsigned int *num_of_neuron_groups);
bool is_layer_connected_to_layer(Network *network, unsigned int this_layer, unsigned int target_layer, bool *connected);
#endif
