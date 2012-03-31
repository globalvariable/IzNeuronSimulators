#ifndef EXTNETWORK_H
#define EXTNETWORK_H


typedef struct 	__ExtNeuronGroup 	ExtNeuronGroup;
typedef struct 	__ExtLayer 		ExtLayer;
typedef struct 	__ExtNetwork 	ExtNetwork;

#include "../ExtNeuron/ExtNeuron.h"
#include "../Network/Network.h"

struct __ExtNetwork
{
	Network		*connected_to_internal_network;
	ExtLayer		**layers;
	unsigned int	layer_count;
};

struct __ExtLayer
{
	ExtNeuronGroup	**neuron_groups;
	unsigned int		neuron_group_count;
	Layer			**connected_to_internal_network_layer;
};

struct __ExtNeuronGroup
{
	ExtNeuron	*neurons;
	unsigned int	neuron_count;
};



ExtNetwork* allocate_external_network(ExtNetwork *network);
ExtNetwork* deallocate_external_network(ExtNetwork *network);
bool add_neurons_to_external_network_layer(ExtNetwork *ext_network, unsigned int num_of_neuron, unsigned int layer, bool inhibitory);
bool increment_ext_to_int_network_layer_connection_matrix(ExtNetwork *ext_network);
bool connect_ext_network_layer_to_int_network_layer(ExtNetwork *ext_network, unsigned int this_layer, unsigned int target_layer, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, SynapticDelay EPSP_delay_max, SynapticDelay EPSP_delay_min, SynapticDelay IPSP_delay_max, SynapticDelay IPSP_delay_min, double excitatory_connection_probability, double inhibitory_connection_probability);
bool is_ext_network_layer_connected_to_int_network_layer(ExtNetwork *ext_network, unsigned int this_layer, unsigned int target_layer, bool *connected);
ExtNeuron* get_ext_neuron_address(ExtNetwork *network, unsigned int layer, unsigned int nrn_grp, unsigned int nrn_num);
#endif
