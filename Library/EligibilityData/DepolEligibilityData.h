#ifndef DEPOL_ELIGIBILITY_DATA_H
#define DEPOL_ELIGIBILITY_DATA_H


typedef struct __SelectedDepolEligibility SelectedDepolEligibility;
typedef struct __DepolEligibilityBufferLimited DepolEligibilityBufferLimited;

#include "../Network/Network.h"

struct __SelectedDepolEligibility
{	
	pthread_mutex_t 	mutex;   // required for get_neuron_dynamics_limited_last_sample_time_and_write_idx()
	unsigned int		neuron_id;
	unsigned int		synapse_num;
	double			*depol_eligibility;	// buffer_size
	unsigned int 		buff_write_idx;
	unsigned int 		buffer_size;
	TimeStamp		last_sample_time;
};

struct __DepolEligibilityBufferLimited		// For faster handling of depol_eligibility buffer.
{
	SelectedDepolEligibility		*selected_depol_eligibility;
	unsigned int 			num_of_selected_synapses;
};

DepolEligibilityBufferLimited* allocate_depol_eligibility_buffer_limited(Network *network, DepolEligibilityBufferLimited* buffer, unsigned int buffer_size, unsigned int num_of_selected_synapses);
DepolEligibilityBufferLimited* deallocate_depol_eligibility_buffer_limited(Network *network, DepolEligibilityBufferLimited* buffer);
bool submit_selected_synapse_to_depol_eligibility_buffer_limited(Network *network, DepolEligibilityBufferLimited* buffer, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, unsigned int synapse_num, unsigned int list_idx);
bool push_depol_eligibility_to_depol_eligibility_buffer_limited(Network *network, DepolEligibilityBufferLimited* buffer, TimeStamp sampling_time, unsigned int arg_neuron_id);
bool get_depol_eligibility_limited_last_sample_time_and_write_idx(DepolEligibilityBufferLimited *buffer, unsigned int selected_synapse_list_idx, TimeStamp *last_sample_time, unsigned int *write_idx);
#endif
