#ifndef NEURON_DYNAMICS_H
#define NEURON_DYNAMICS_H


typedef struct __NeuronDynamics NeuronDynamics;
typedef struct __NeuronDynamicsPattern NeuronDynamicsPattern;
typedef struct __NeuronDynamicsPatterns NeuronDynamicsPatterns;
typedef struct __CopierNeuronDynamics CopierNeuronDynamics;

#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"

struct __NeuronDynamics
{
	Network *network;
	NeuronDynamicsPatterns ***patterns; ///num_of_layers * num_of_neuron_groups * num_of_neurons 
};

struct __CopierNeuronDynamics
{
	Network *network;
	NeuronDynamicsPattern ***pattern; ///num_of_layers * num_of_neuron_groups * num_of_neurons 
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
	double initial_v;    
	double initial_u;   
	double initial_e;   
	double initial_i;    
};


NeuronDynamics* allocate_neuron_dynamics(Network *network, NeuronDynamics *neuron_dynamics);
NeuronDynamics* deallocate_neuron_dynamics(Network *network, NeuronDynamics *neuron_dynamics);
bool add_neuron_dynamics_pattern(Network *network, NeuronDynamics *neuron_dynamics, NeuronDynamics *neuron_dynamics_source, TimeStampMs pattern_length_ms) ;

#endif
