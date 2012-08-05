#ifndef ELIGIBILITY_DATA_H
#define ELIGIBILITY_DATA_H


typedef struct __SelectedEligibility SelectedEligibility;
typedef struct __EligibilityBufferLimited EligibilityBufferLimited;

#include "../Network/Network.h"

struct __SelectedEligibility
{	
	pthread_mutex_t 	mutex;   // required for get_neuron_dynamics_limited_last_sample_time_and_write_idx()
	unsigned int		neuron_id;
	unsigned int		synapse_num;
	double			*eligibility;	// buffer_size
	unsigned int 		buff_write_idx;
	unsigned int 		buffer_size;
	TimeStamp		last_sample_time;
};

struct __EligibilityBufferLimited		// For faster handling of eligibility buffer.
{
	SelectedEligibility		*selected_eligibility;
	unsigned int 			num_of_selected_synapses;
};

EligibilityBufferLimited* allocate_eligibility_buffer_limited(Network *network, EligibilityBufferLimited* buffer, unsigned int buffer_size, unsigned int num_of_selected_synapses);
EligibilityBufferLimited* deallocate_eligibility_buffer_limited(Network *network, EligibilityBufferLimited* buffer);
bool submit_selected_synapse_to_eligibility_buffer_limited(Network *network, EligibilityBufferLimited* buffer, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, unsigned int synapse_num, unsigned int list_idx);
bool push_eligibility_to_eligibility_buffer_limited(Network *network, EligibilityBufferLimited* buffer, TimeStamp sampling_time, unsigned int arg_neuron_id);
bool get_eligibility_limited_last_sample_time_and_write_idx(EligibilityBufferLimited *buffer, unsigned int selected_synapse_list_idx, TimeStamp *last_sample_time, unsigned int *write_idx);
#endif
