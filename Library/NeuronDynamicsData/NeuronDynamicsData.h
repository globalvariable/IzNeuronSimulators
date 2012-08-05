#ifndef NEURON_DYNAMICS_DATA_H
#define NEURON_DYNAMICS_DATA_H




typedef struct __NeuronDynamics NeuronDynamics;
typedef struct __NetworkNeuronDynamics NetworkNeuronDynamics;
typedef struct __NeuronDynamicsBuffer NeuronDynamicsBuffer;
typedef struct __SelectedNeuronDynamics SelectedNeuronDynamics;
typedef struct __NeuronDynamicsBufferLimited NeuronDynamicsBufferLimited;

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

struct __SelectedNeuronDynamics
{	
	pthread_mutex_t 	mutex;   // required for get_neuron_dynamics_limited_last_sample_time_and_write_idx()
	unsigned int		neuron_id;
	int				dynamics_type;
	double			*neuron_dynamic;	// buffer size
	unsigned int 		buff_write_idx;
	unsigned int 		buffer_size;
	TimeStamp		last_sample_time;
};

struct __NeuronDynamicsBufferLimited		// For faster handling of dynamics buffer.
{
	SelectedNeuronDynamics	*selected_dyns;
	unsigned int 				num_of_selected_neurons;
};

NeuronDynamicsBuffer* allocate_neuron_dynamics_buffer(Network *network, NeuronDynamicsBuffer* buffer, unsigned int buffer_size);
NeuronDynamicsBuffer* deallocate_neuron_dynamics_buffer(Network *network, NeuronDynamicsBuffer* buffer);
bool push_neuron_dynamics_to_neuron_dynamics_buffer(Network *network, NeuronDynamicsBuffer* neuron_dynamics_buffer, TimeStamp sampling_time) ;
bool get_neuron_dynamics_last_sample_time_and_write_idx(NeuronDynamicsBuffer *buffer, TimeStamp *last_sample_time, unsigned int *write_idx);

NeuronDynamicsBufferLimited* allocate_neuron_dynamics_buffer_limited(Network *network, NeuronDynamicsBufferLimited* buffer, unsigned int buffer_size, unsigned int num_of_selected_neurons);
NeuronDynamicsBufferLimited* deallocate_neuron_dynamics_buffer_limited(Network *network, NeuronDynamicsBufferLimited* buffer);
bool submit_selected_neuron_to_neuron_dynamics_buffer_limited(Network *network, NeuronDynamicsBufferLimited* buffer, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num,  int dynamics_type, unsigned int list_idx);
bool push_neuron_dynamics_to_neuron_dynamics_buffer_limited(Network *network, NeuronDynamicsBufferLimited* buffer, TimeStamp sampling_time, unsigned int arg_neuron_id);
bool get_neuron_dynamics_limited_last_sample_time_and_write_idx(NeuronDynamicsBufferLimited *buffer, unsigned int selected_neuron_list_idx, TimeStamp *last_sample_time, unsigned int *write_idx);
#endif
