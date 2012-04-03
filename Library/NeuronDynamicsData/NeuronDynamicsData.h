#ifndef NEURON_DYNAMICS_DATA_H
#define NEURON_DYNAMICS_DATA_H




typedef struct __NeuronDynamics NeuronDynamics;
typedef struct __NetworkNeuronDynamics NetworkNeuronDynamics;
typedef struct __NeuronDynamicsBuffer NeuronDynamicsBuffer;

#include "../Network/Network.h"

struct __NeuronDynamics
{
	double dyn[MAX_NUM_OF_NEURON_DYNAMICS_TYPE];   /// *num_of_allocated_samples
};

struct __NetworkNeuronDynamics
{
	NeuronDynamics	***network_neuron_dyn;
};

struct __NeuronDynamicsBuffer
{
	pthread_mutex_t 			mutex;
	NetworkNeuronDynamics 	*buffer;
	unsigned int 				buff_write_idx;
	unsigned int 				buffer_size;
	TimeStamp				last_sample_time;
};


NeuronDynamicsBuffer* allocate_neuron_dynamics_buffer(Network *network, NeuronDynamicsBuffer* buffer, unsigned int buffer_size);
NeuronDynamicsBuffer* deallocate_neuron_dynamics_buffer(Network *network, NeuronDynamicsBuffer* buffer);
bool push_neuron_dynamics_to_neuron_dynamics_buffer(Network *network, NeuronDynamicsBuffer* neuron_dynamics_buffer, TimeStamp sampling_time) ;
bool get_neuron_dynamics_last_sample_time_and_write_idx(NeuronDynamicsBuffer *buffer, TimeStamp *last_sample_time, unsigned int *write_idx);
#endif
