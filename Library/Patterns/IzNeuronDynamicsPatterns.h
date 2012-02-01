#ifndef NEURON_DYNAMICS_PATTERNS_H
#define NEURON_DYNAMICS_PATTERNS_H


typedef struct __NetworkNeuronDynamicsPatterns NetworkNeuronDynamicsPatterns;
typedef struct __NetworkNeuronDynamicsPattern NetworkNeuronDynamicsPattern;
typedef struct __NeuronDynamicsPattern NeuronDynamicsPattern;


#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"

struct __NetworkNeuronDynamicsPatterns
{
	Network 						*network;    // whose pattern is this?
	NetworkNeuronDynamicsPattern		*patterns;
	NetworkNeuronDynamicsPattern		*curr_pattern;
}

struct __NetworkNeuronDynamicsPattern
{
	NeuronDynamicsPattern 			***neuron_dynamics_in_network;
	TimeStamp					*sampling_times;
	unsigned int					*num_of_samples;
	NetworkNeuronDynamicsPattern		*prev;
	NetworkNeuronDynamicsPattern		*next;
};

struct __NeuronDynamicsPattern
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

AllNeuronsDynamicsPatterns* allocate_all_neurons_dynamics_patterns(Network *network, unsigned int num_of_patterns_to_allocate, unsigned int num_of_samples_to_allocate, AllNeuronsDynamicsPatterns *all_neurons_dynamics_patterns);
AllNeuronsDynamicsPatterns* deallocate_all_neurons_dynamics_patterns(Network *network, AllNeuronsDynamicsPatterns *all_neurons_dynamics_patterns);

AllNeuronsDynamicsSinglePattern* allocate_all_neurons_dynamics_single_pattern(Network *network, unsigned int num_of_samples_to_allocate, AllNeuronsDynamicsSinglePattern *all_neurons_dynamics_single_pattern);
AllNeuronsDynamicsSinglePattern* deallocate_all_neurons_dynamics_single_pattern(Network *network, AllNeuronsDynamicsSinglePattern *all_neurons_dynamics_single_pattern);

bool write_all_neurons_dynamics_single_pattern_to_all_neurons_dynamics_patterns(Network *network, AllNeuronsDynamicsSinglePattern *all_neurons_dynamics_single_pattern, AllNeuronsDynamicsPatterns *all_neurons_dynamics_patterns);
bool reset_all_neurons_dynamics_single_pattern_write_idx(Network *network, AllNeuronsDynamicsSinglePattern *all_neurons_dynamics_single_pattern);
bool reset_all_neurons_dynamics_patterns_write_idx(Network *network, AllNeuronsDynamicsPatterns *all_neurons_dynamics_patterns);
bool write_neuron_dynamics_to_all_neurons_dynamics_single_pattern(Network *network, AllNeuronsDynamicsSinglePattern *all_neurons_dynamics_single_pattern, int layer, int neuron_group, int neuron_num, TimeStamp sampling_time, double v, double u, double e, double i);
bool write_initial_neuron_dynamics_to_all_neurons_dynamics_single_pattern(Network *network, AllNeuronsDynamicsSinglePattern *all_neurons_dynamics_single_pattern, int layer, int neuron_group, int neuron_num, TimeStamp inital_time, double v, double u, double e, double i);
#endif
