#ifndef NEURON_DYNAMICS_DATA_H
#define NEURON_DYNAMICS_DATA_H


typedef struct __NeuronDynamics NeuronDynamics;
typedef struct __NetworkNeuronDynamics NetworkNeuronDynamics;
typedef struct __NeuronDynamicsBuffer NeuronDynamicsBuffer;

#include "../Network/Network.h"

struct __NeuronDynamics
{
	double v;   /// *num_of_allocated_samples
	double u;    /// * num_of_allocated_samples
	double e;   /// * num_of_allocated_samples
	double i;   /// * num_of_allocated_samples	
};

struct __NetworkNeuronDynamics
{
	NeuronDynamics	***network_neuron_dyn;
};

struct __NeuronDynamicsBuffer
{
	NetworkNeuronDynamics 	*buffer;
	unsigned int 				buff_write_idx;
};


NeuronDynamicsBuffer* allocate_neuron_dynamics_buffer(Network *network, NeuronDynamicsBuffer* buffer_data);
NeuronDynamicsBuffer* deallocate_neuron_dynamics_buffer(Network *network, NeuronDynamicsBuffer* buffer_data);
#endif
