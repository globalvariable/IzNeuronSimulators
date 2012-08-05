#ifndef STDP_DATA_H
#define STDP_DATA_H


typedef struct __SelectedSTDP SelectedSTDP;
typedef struct __STDPBufferLimited STDPBufferLimited;

#include "../Network/Network.h"

struct __SelectedSTDP
{	
	pthread_mutex_t 	mutex;   // required for get_neuron_dynamics_limited_last_sample_time_and_write_idx()
	unsigned int		neuron_id;
	unsigned int		synapse_num;
	int				stdp_type;
	double			*stdp;	// buffer_size
	unsigned int 		buff_write_idx;
	unsigned int 		buffer_size;
	TimeStamp		last_sample_time;
};

struct __STDPBufferLimited		// For faster handling of stdp buffer.
{
	SelectedSTDP		*selected_stdp;
	unsigned int 			num_of_selected_synapses;
};

STDPBufferLimited* allocate_stdp_buffer_limited(Network *network, STDPBufferLimited* buffer, unsigned int buffer_size, unsigned int num_of_selected_synapses);
STDPBufferLimited* deallocate_stdp_buffer_limited(Network *network, STDPBufferLimited* buffer);
bool submit_selected_synapse_to_stdp_buffer_limited(Network *network, STDPBufferLimited* buffer, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, unsigned int synapse_num, int stdp_type, unsigned int list_idx);
bool push_stdp_to_stdp_buffer_limited(Network *network, STDPBufferLimited* buffer, TimeStamp sampling_time, unsigned int arg_neuron_id);
bool get_stdp_limited_last_sample_time_and_write_idx(STDPBufferLimited *buffer, unsigned int selected_synapse_list_idx, TimeStamp *last_sample_time, unsigned int *write_idx);
#endif
