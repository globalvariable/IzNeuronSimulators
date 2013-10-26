#include "EligibilityData.h"


EligibilityBufferLimited* allocate_eligibility_buffer_limited(Network *network, EligibilityBufferLimited* buffer, unsigned int buffer_size, unsigned int num_of_selected_synapses)
{
	unsigned int i;
	if (network == NULL)
		return (EligibilityBufferLimited*)print_message(ERROR_MSG ,"IzNeuronSimulators", "EligibilityData", "allocate_eligibility_buffer_limited", "network == NULL.");
	if (buffer != NULL)
	{
		buffer = deallocate_eligibility_buffer_limited(network, buffer);
		buffer = allocate_eligibility_buffer_limited(network, buffer, buffer_size, num_of_selected_synapses);
		return buffer;
	}
	buffer = g_new0(EligibilityBufferLimited,1);
	buffer->selected_eligibility = g_new0(SelectedEligibility, num_of_selected_synapses);
	for(i = 0; i < num_of_selected_synapses; i++)
	{
		pthread_mutex_init(&(buffer->selected_eligibility[i].mutex), NULL);	
		buffer->selected_eligibility[i].eligibility = g_new0(double, buffer_size);
		buffer->selected_eligibility[i].buffer_size = buffer_size;
	}
	buffer->num_of_selected_synapses = num_of_selected_synapses;
	print_message(INFO_MSG ,"IzNeuronSimulators", "EligibilityData", "allocate_eligibility_buffer_limited", "Created eligibility_buffer_limited.");
	return buffer;
}

EligibilityBufferLimited* deallocate_eligibility_buffer_limited(Network *network, EligibilityBufferLimited* buffer)
{
	print_message(BUG_MSG ,"IzNeuronSimulators", "EligibilityData", "deallocate_eligibility_limited", "Not implemented.");	
	return NULL;
}

bool submit_selected_synapse_to_eligibility_buffer_limited(Network *network, EligibilityBufferLimited* buffer, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, unsigned int synapse_num, unsigned int list_idx)
{
	unsigned int neuron_id;
	Neuron *neuron;
	if ((neuron = get_neuron_address(network, layer, neuron_group, neuron_num)) == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "EligibilityData", "submit_selected_neuron_to_eligibility_buffer_limited", "get_neuron_address() = NULL.");
	if (synapse_num >= neuron->syn_list->num_of_synapses)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "EligibilityData", "submit_selected_neuron_to_eligibility_buffer_limited", "(synapse_num >= neuron->syn_list->num_of_synapses).");
	if (list_idx >= buffer->num_of_selected_synapses)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "EligibilityData", "submit_selected_neuron_to_eligibility_buffer_limited", "list_idx >= buffer->num_of_selected_synapses.");	
	if (! get_neuron_id_in_network(network, layer, neuron_group, neuron_num, &neuron_id))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "EligibilityData", "submit_selected_neuron_to_eligibility_buffer_limited", "! get_neuron_id_in_network().");	
	pthread_mutex_lock(&(buffer->selected_eligibility->mutex));  
	buffer->selected_eligibility[list_idx].neuron_id = neuron_id;
	buffer->selected_eligibility[list_idx].synapse_num = synapse_num;
	pthread_mutex_unlock(&(buffer->selected_eligibility->mutex));  
	return TRUE;
}

bool push_eligibility_to_eligibility_buffer_limited(Network *network, EligibilityBufferLimited* buffer, TimeStamp sampling_time, unsigned int arg_neuron_id)
{
	unsigned int i;
	unsigned int neuron_id;
	SelectedEligibility	*selected_eligibility;
	Synapse *synapse;

	for (i = 0; i < buffer->num_of_selected_synapses; i++)
	{
		neuron_id = buffer->selected_eligibility[i].neuron_id;   // re-get neuron id for writing into buffer. invalid synapse id due to simulatenous submit_selected_synapse_to_eligibility_buffer_limited leads to TRAP.
		if (arg_neuron_id != neuron_id)	
			continue;
		selected_eligibility = &(buffer->selected_eligibility[i]);
		pthread_mutex_lock(&(selected_eligibility->mutex));   //  // it is within mutex for getting valid synapse_num for the specified neuron_id  // re-get neuron_id for simultaneous submit_selected_synapse_to_eligibility_buffer_limited

		synapse = &(network->all_neurons[selected_eligibility->neuron_id]->syn_list->synapses[selected_eligibility->synapse_num]);
		if (synapse->plastic)
 			selected_eligibility->eligibility[selected_eligibility->buff_write_idx] = synapse->ps_eligibility->now;
		else
			selected_eligibility->eligibility[selected_eligibility->buff_write_idx] = 0;

		if ((selected_eligibility->buff_write_idx + 1) == selected_eligibility->buffer_size)
			selected_eligibility->buff_write_idx = 0;
		else
			selected_eligibility->buff_write_idx++;
		selected_eligibility->last_sample_time = sampling_time;		 // it is within mutex for get_eligibility_limited_last_sample_time_and_write_idx

		pthread_mutex_unlock(&(selected_eligibility->mutex));
	}
	return TRUE;
}

bool get_eligibility_limited_last_sample_time_and_write_idx(EligibilityBufferLimited *buffer, unsigned int selected_synapse_list_idx, TimeStamp *last_sample_time, unsigned int *write_idx)
{
	SelectedEligibility	*selected_eligibility = &(buffer->selected_eligibility[selected_synapse_list_idx]);
	pthread_mutex_lock(&(selected_eligibility->mutex));
	*write_idx = selected_eligibility->buff_write_idx;
	*last_sample_time = selected_eligibility->last_sample_time;
	pthread_mutex_unlock(&(selected_eligibility->mutex));
	return TRUE;
}
