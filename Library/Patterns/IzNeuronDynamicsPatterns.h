#ifndef IZ_NEURON_DYNAMICS_PATTERNS_H
#define IZ_NEURON_DYNAMICS_PATTERNS_H


typedef struct __NetworkNeuronDynamicsPatterns NetworkNeuronDynamicsPatterns;
typedef struct __NetworkNeuronDynamicsPattern NetworkNeuronDynamicsPattern;
typedef struct __NeuronDynamicsPattern NeuronDynamicsPattern;


#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"

struct __NetworkIzNeuronDynamicsPatterns
{
	TrialStats						*trial_stats;	// get num of patterns from num of trials
	Network 						*network;    // whose pattern is this?
	NetworkNeuronDynamicsPattern		*head;
	NetworkNeuronDynamicsPattern		*tail;		// current pattern
}

struct __NetworkIzNeuronDynamicsPattern
{
	NeuronDynamicsPattern 			***neuron_dynamics_pattern;
	TimeStamp					*sampling_times;
	unsigned int					num_of_samples;
	NetworkNeuronDynamicsPattern		*prev;
	NetworkNeuronDynamicsPattern		*next;
};

struct __IzNeuronDynamicsPattern
{
	double *v;   /// *num_of_allocated_samples
	double *u;    /// * num_of_allocated_samples
	double *e;   /// * num_of_allocated_samples
	double *i;   /// * num_of_allocated_samples	
	double initial_v;   
	double initial_u;   
	double initial_e;   
	double initial_i;   	
};

struct __NetworkIzNeuronDynamicsSinglePattern
{
	Network 						*network;    // whose pattern is this?	
	IzNeuronDynamicsPattern 			***neuron_dynamics_pattern;
	TimeStamp					*sampling_times;
	unsigned int					write_idx;
	unsigned int					allocated_num_of_samples;
};

NetworkIzNeuronDynamicsPatterns* allocate_network_izneuron_dynamics_patterns(Network *network, TrialStats *trial_stats, NetworkIzNeuronDynamicsPatterns* network_neuron_dynamics_patterns);
NetworkIzNeuronDynamicsPatterns* deallocate_network_izneuron_dynamics_patterns(Network *network, TrialStats *trial_stats, NetworkIzNeuronDynamicsPatterns* network_neuron_dynamics_patterns);
NetworkIzNeuronDynamicsSinglePattern* allocate_network_izneuron_dynamics_single_pattern(Network *network, NetworkIzNeuronDynamicsSinglePattern* network_neuron_dynamics_single_pattern, unsigned int num_of_samples_to_allocate);
NetworkIzNeuronDynamicsSinglePattern* deallocate_network_izneuron_dynamics_single_pattern(Network *network, NetworkIzNeuronDynamicsSinglePattern* network_neuron_dynamics_single_pattern);
bool write_initial_neuron_dynamics_to_network_izneuron_dynamics_single_pattern(Network *network, NetworkIzNeuronDynamicsSinglePattern *neuron_dynamics_single_pattern, int layer, int neuron_group, int neuron_num, double v, double u, double e, double i);
bool add_izneuron_dynamics_to_network_izneuron_dynamics_single_pattern(Network *network, NetworkIzNeuronDynamicsSinglePattern *neuron_dynamics_single_pattern, int layer, int neuron_group, int neuron_num, TimeStamp sampling_time, double v, double u, double e, double i, bool first_sample);
bool add_network_izneuron_dynamics_single_pattern_to_network_izneuron_dynamics_patterns(Network *network, NetworkIzNeuronDynamicsPatterns* network_neuron_dynamics_patterns, NetworkIzNeuronDynamicsSinglePattern* network_neuron_dynamics_single_pattern);

#endif
