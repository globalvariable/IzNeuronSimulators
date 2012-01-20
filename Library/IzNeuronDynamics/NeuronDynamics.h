#ifndef NEURON_DYNAMICS_H
#define NEURON_DYNAMICS_H


typedef struct __NeuronDynamics NeuronDynamics;
typedef struct __NeuronDynamicsPattern NeuronDynamicsPattern;
typedef struct __NeuronDynamicsPatterns NeuronDynamicsPatterns;


#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../BlueSpike/Library/PatternData/PatternData.h"
#include "../Network/Network.h"

struct __NeuronDynamics
{
	Network *network;
	NeuronDynamicsPatterns ***patterns; ///num_of_layers * num_of_neuron_groups * num_of_neurons 
};

struct __NeuronDynamicsPatterns
{
	NeuronDynamicsPattern *pattern;  //  num_of_patterns
	NeuronDynamicsPattern *next_pattern; 
};

struct __NeuronDynamicsPattern
{
	double *v;   /// *pattern_length
	double *u;    /// * pattern_length
	double *e;   /// * pattern_length
	double *i;   /// * pattern_length	
};


NeuronDynamics* allocate_neuron_dynamics(Network *network, PatternData *pattern_data, NeuronDynamics *neuron_dynamics);
NeuronDynamics* deallocate_neuron_dynamics(Network *network, PatternData *pattern_data, NeuronDynamics *neuron_dynamics);
bool add_neuron_dynamics_to_pattern(Network *network, PatternData *pattern_data, NeuronDynamics *neuron_dynamics, int layer, int neuron_group, int neuron_num, int pattern_num, NeuronDynamicsPattern *dynamics_pattern);

#endif
