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
	Layer	**layers;
	int 		layer_count;
};

struct __Layer
{
	NeuronGroup	**neuron_groups;
	int 			neuron_group_count;
	Layer		*connected_to_network_layer;	
	ExtLayer		*connected_to_ext_network_layer;
};

struct __NeuronGroup
{
	Neuron	*neurons;
	int 		neuron_count;
};

Network* allocate_network(Network *network);
Network* deallocate_network(Network *network);
bool add_neurons_to_layer(Network *network, int num_of_neuron, int layer, double v, double a, double b, double c,double d, double k, double C, double v_resting, double v_threshold, double v_peak, double I_inject, bool inhibitory, 					
				 	double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory, int randomize_params);

bool connect_network_layer_to_network_layer(Network *network, int ThisLayer, int TargetLayer, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, 
						SynapticDelay EPSP_delay_min, SynapticDelay EPSP_delay_max, SynapticDelay IPSP_delay_min, SynapticDelay IPSP_delay_max, float connection_probability);

bool is_network_allocated(Network *network);
bool is_layer_free (Network *network, int layer);
bool is_neuron_group_free (Network *network, int layer, int neuron_group_num);
bool is_neuron(Network *network, int layer, int nrn_grp, int nrn_num);
Neuron* get_neuron_address(Network *network, int layer, int nrn_grp, int nrn_num);					
void reset_all_network_neuron_dynamics (Network *network);
void interrogate_network(Network *network);


#endif
