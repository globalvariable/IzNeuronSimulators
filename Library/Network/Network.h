#ifndef NETWORK_H
#define NETWORK_H

typedef struct 	__NeuronGroup 	NeuronGroup;
typedef struct 	__Layer 		Layer;
typedef struct 	__Network 		Network;

#include "../Neuron/Neuron.h"
#include "../Event/Event.h"
#include "../Axon/Axon.h"
#include "../Synapse/Synapse.h"
struct __Network
{
	Neuron			**all_neurons;
	unsigned int 		num_of_neurons;
	Layer			**layers;
	unsigned int 		layer_count;
};

struct __Layer
{
	NeuronGroup	**neuron_groups;
	unsigned int	neuron_group_count;
	Layer		**connected_to_layers;
	unsigned int	num_of_connections;	// from this layer to other layers	
};

struct __NeuronGroup
{
	Neuron		*neurons;
	unsigned int	neuron_count;
};

Network* allocate_network(Network *network);
Network* deallocate_network(Network *network);
bool add_iz_neurons_to_layer(Network *network, unsigned int num_of_neuron, unsigned int layer, double a, double b, double c,double d, double k, double C, double v_resting, double v_threshold, double v_peak, bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory, unsigned int randomize_params);
bool add_neuron_nodes_to_layer(Network *network, unsigned int num_of_neuron, unsigned int layer, bool inhibitory);
bool add_poisson_neurons_to_layer(Network *network, unsigned int num_of_neuron, unsigned int layer, bool inhibitory, double firing_rate);
bool connect_layers(Network *this_network, unsigned int this_layer, Network *target_network, unsigned int target_layer, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, AxonalDelay EPSP_delay_max, AxonalDelay EPSP_delay_min, AxonalDelay IPSP_delay_max, AxonalDelay IPSP_delay_min, AxonalDelay delay_hard_max, AxonalDelay delay_hard_min, double excitatory_connection_probability, double inhibitory_connection_probability, bool excitatory_plastic, bool inhibitory_plastic);
bool connect_neurons(Network *this_network, unsigned int this_layer, unsigned int this_neuron_group, unsigned int this_neuron_num, Network *target_network, unsigned int target_layer, unsigned int target_neuron_group, unsigned int target_neuron_num, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, AxonalDelay EPSP_delay_max, AxonalDelay EPSP_delay_min, AxonalDelay IPSP_delay_max, AxonalDelay IPSP_delay_min, AxonalDelay delay_hard_max, AxonalDelay delay_hard_min, bool excitatory_plastic, bool inhibitory_plastic);
bool is_network_allocated(Network *network);
bool is_layer_free (Network *network, int layer);
bool is_neuron_group_free (Network *network, int layer, int neuron_group_num);
bool is_neuron(Network *network, int layer, int nrn_grp, int nrn_num);
Neuron* get_neuron_address(Network *network, int layer, int nrn_grp, int nrn_num);					
void reset_all_network_iz_neuron_dynamics (Network *network);
void reset_all_network_neuron_injection_currents (Network *network);
void interrogate_network(Network *network);
bool get_num_of_layers_in_network(Network *network, unsigned int *num_of_layers);
bool get_num_of_neuron_groups_in_layer(Network *network, unsigned int layer, unsigned int *num_of_neuron_groups);
bool get_num_of_neurons_in_neuron_group(Network *network, unsigned int layer, unsigned int neuron_group, unsigned int *num_of_neurons);
unsigned int get_num_of_neurons_in_network(Network *network);
bool get_num_of_neuron_groups_in_network(Network *network, unsigned int *num_of_neuron_groups);
bool is_layer_connected_to_layer(Network *this_network, unsigned int this_layer, Network *target_network, unsigned int target_layer, bool *connected);
bool get_neuron_id_in_network(Network *network, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, unsigned int *neuron_id);
bool set_layer_type_of_the_neurons_in_layer(Network *network, unsigned int layer_num, LayerType layer_type);
unsigned int get_num_of_poisson_neurons_in_network_slow(Network *network);  // for faster implementation, create a variable in network main structure which saves the number of poisson neurons during addition of them to the net.
#endif
