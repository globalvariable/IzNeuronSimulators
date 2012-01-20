#ifndef NEURON_DYNAMICS_PATTERNS_H
#define NEURON_DYNAMICS_PATTERNS_H


typedef struct __AllNeuronsDynamicsPatterns AllNeuronsDynamicsPatterns;
typedef struct __NeuronDynamicsPatterns NeuronDynamicsPatterns;
typedef struct __NeuronDynamicsPattern NeuronDynamicsPattern;
typedef struct __AllNeuronsDynamicsSinglePattern AllNeuronsDynamicsSinglePattern;


#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"

struct __AllNeuronsDynamicsPatterns
{
	Network 				*network;
	NeuronDynamicsPatterns 	***network_patterns; ///num_of_layers * num_of_neuron_groups * num_of_neurons 
	TimeStamp			**sampling_times; //  num_of_patterns *   num_of_allocated_samples
	unsigned int			*num_of_used_samples; //  num_of_patterns 
	unsigned int			num_of_allocated_samples; //  num_of_patterns 
	unsigned int			num_of_allocated_patterns; //  num_of_patterns 	
	unsigned int			num_of_used_patterns; //  num_of_patterns 		
};

struct __NeuronDynamicsPatterns
{
	NeuronDynamicsPattern *patterns;  //  num_of_patterns
};

struct __NeuronDynamicsPattern
{
	double *v;   /// *num_of_allocated_samples
	double *u;    /// * num_of_allocated_samples
	double *e;   /// * num_of_allocated_samples
	double *i;   /// * num_of_allocated_samples	
};

struct __AllNeuronsDynamicsSinglePattern
{
	Network 				*network;
	NeuronDynamicsPattern 	***network_pattern; ///num_of_layers * num_of_neuron_groups * num_of_neurons 
	TimeStamp			*sampling_times; //  num_of_allocated_samples
	unsigned int			num_of_used_samples; //  num_of_patterns 
	unsigned int			num_of_allocated_samples; //  num_of_patterns 
};

AllNeuronsDynamicsPatterns* allocate_all_neurons_dynamics_patterns(Network *network, unsigned int num_of_patterns_to_allocate, unsigned int num_of_samples_to_allocate, AllNeuronsDynamicsPatterns *all_neurons_dynamics_patterns);
AllNeuronsDynamicsPatterns* deallocate_all_neurons_dynamics_patterns(Network *network, AllNeuronsDynamicsPatterns *all_neurons_dynamics_patterns);

AllNeuronsDynamicsSinglePattern* allocate_all_neurons_dynamics_single_pattern(Network *network, unsigned int num_of_samples_to_allocate, AllNeuronsDynamicsSinglePattern *all_neurons_dynamics_single_pattern);
AllNeuronsDynamicsSinglePattern* deallocate_all_neurons_dynamics_single_pattern(Network *network, AllNeuronsDynamicsSinglePattern *all_neurons_dynamics_single_pattern);

bool write_all_neurons_dynamics_single_pattern_to_all_neurons_dynamics_patterns(Network *network, AllNeuronsDynamicsSinglePattern *all_neurons_dynamics_single_pattern, AllNeuronsDynamicsPatterns *all_neurons_dynamics_patterns);


#endif
