#include "STDPData.h"


STDPBufferLimited* allocate_stdp_buffer_limited(Network *network, STDPBufferLimited* buffer, unsigned int buffer_size, unsigned int num_of_selected_synapses)
{
	unsigned int i;
	if (network == NULL)
		return (STDPBufferLimited*)print_message(ERROR_MSG ,"IzNeuronSimulators", "STDPData", "allocate_stdp_buffer_limited", "network == NULL.");
	if (buffer != NULL)
	{
		buffer = deallocate_stdp_buffer_limited(network, buffer);
		buffer = allocate_stdp_buffer_limited(network, buffer, buffer_size, num_of_selected_synapses);
		return buffer;
	}
	buffer = g_new0(STDPBufferLimited,1);
	buffer->selected_stdp = g_new0(SelectedSTDP, num_of_selected_synapses);
	for(i = 0; i < num_of_selected_synapses; i++)
	{
		pthread_mutex_init(&(buffer->selected_stdp[i].mutex), NULL);	
		buffer->selected_stdp[i].stdp = g_new0(double, buffer_size);
		buffer->selected_stdp[i].buffer_size = buffer_size;
	}
	buffer->num_of_selected_synapses = num_of_selected_synapses;
	print_message(INFO_MSG ,"IzNeuronSimulators", "STDPData", "allocate_stdp_buffer_limited", "Created stdp_buffer_limited.");
	return buffer;
}

STDPBufferLimited* deallocate_stdp_buffer_limited(Network *network, STDPBufferLimited* buffer)
{
	print_message(BUG_MSG ,"IzNeuronSimulators", "STDPData", "deallocate_stdp_limited", "Not implemented.");	
	return NULL;
}

bool submit_selected_synapse_to_stdp_buffer_limited(Network *network, STDPBufferLimited* buffer, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, unsigned int synapse_num, int stdp_type, unsigned int list_idx)
{
	unsigned int neuron_id;
	Neuron *neuron;
	if ((neuron = get_neuron_address(network, layer, neuron_group, neuron_num)) == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "STDPData", "submit_selected_neuron_to_stdp_buffer_limited", "get_neuron_address() = NULL.");
	if (synapse_num >= neuron->syn_list->num_of_synapses)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "STDPData", "submit_selected_neuron_to_stdp_buffer_limited", "(synapse_num >= neuron->syn_list->num_of_synapses).");
	if ((stdp_type >= MAX_NUM_OF_STDP_TYPE) || (stdp_type < 0))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "STDPData", "submit_selected_neuron_to_stdp_buffer_limited", "((stdp_type >= MAX_NUM_OF_STDP_TYPE) || (stdp_type < 0)).");
	if (list_idx >= buffer->num_of_selected_synapses)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "STDPData", "submit_selected_neuron_to_stdp_buffer_limited", "list_idx >= buffer->num_of_selected_synapses.");	
	if (! get_neuron_id_in_network(network, layer, neuron_group, neuron_num, &neuron_id))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "STDPData", "submit_selected_neuron_to_stdp_buffer_limited", "! get_neuron_id_in_network().");	
	pthread_mutex_lock(&(buffer->selected_stdp->mutex));  
	buffer->selected_stdp[list_idx].neuron_id = neuron_id;
	buffer->selected_stdp[list_idx].synapse_num = synapse_num;
	buffer->selected_stdp[list_idx].stdp_type = stdp_type;
	pthread_mutex_unlock(&(buffer->selected_stdp->mutex));  
	return TRUE;
}

bool push_stdp_to_stdp_buffer_limited(Network *network, STDPBufferLimited* buffer, TimeStamp sampling_time, unsigned int arg_neuron_id)
{
	unsigned int i;
	unsigned int neuron_id;
	SelectedSTDP	*selected_stdp;
	Synapse *synapse;
	for (i = 0; i < buffer->num_of_selected_synapses; i++)
	{
		neuron_id = buffer->selected_stdp[i].neuron_id;   // re-get neuron id for writing into buffer. invalid synapse id due to simulatenous submit_selected_synapse_to_stdp_buffer_limited leads to TRAP.
		if (arg_neuron_id != neuron_id)	
			continue;
		selected_stdp = &(buffer->selected_stdp[i]);
		pthread_mutex_lock(&(selected_stdp->mutex));   //  // it is within mutex for getting valid synapse_num for the specified neuron_id  // re-get neuron_id for simultaneous submit_selected_synapse_to_stdp_buffer_limited
		switch (selected_stdp->stdp_type)
		{
			case STDP_TYPE_PRE_POST:
				synapse = &(network->all_neurons[selected_stdp->neuron_id]->syn_list->synapses[selected_stdp->synapse_num]);
				if (synapse->plastic)
 					selected_stdp->stdp[selected_stdp->buff_write_idx] = synapse->ps_stdp_pre_post->now;
				else
					selected_stdp->stdp[selected_stdp->buff_write_idx] = 0;
				break;
			case STDP_TYPE_POST_PRE:
				synapse = &(network->all_neurons[selected_stdp->neuron_id]->syn_list->synapses[selected_stdp->synapse_num]);
				if (synapse->plastic)
 					selected_stdp->stdp[selected_stdp->buff_write_idx] = synapse->ps_stdp_post_pre->now;
				else
					selected_stdp->stdp[selected_stdp->buff_write_idx] = 0;
				break;
			default: 
				pthread_mutex_unlock(&(selected_stdp->mutex));
				return print_message(BUG_MSG ,"IzNeuronSimulators", "STDPData", "push_stdp_to_stdp_buffer_limited", "Unknown dynamics type.");		
		}
		if ((selected_stdp->buff_write_idx + 1) == selected_stdp->buffer_size)
			selected_stdp->buff_write_idx = 0;
		else
			selected_stdp->buff_write_idx++;
		selected_stdp->last_sample_time = sampling_time;		 // it is within mutex for get_stdp_limited_last_sample_time_and_write_idx
		pthread_mutex_unlock(&(selected_stdp->mutex));
	}
	return TRUE;
}

bool get_stdp_limited_last_sample_time_and_write_idx(STDPBufferLimited *buffer, unsigned int selected_synapse_list_idx, TimeStamp *last_sample_time, unsigned int *write_idx)
{
	SelectedSTDP	*selected_stdp = &(buffer->selected_stdp[selected_synapse_list_idx]);
	pthread_mutex_lock(&(selected_stdp->mutex));
	*write_idx = selected_stdp->buff_write_idx;
	*last_sample_time = selected_stdp->last_sample_time;
	pthread_mutex_unlock(&(selected_stdp->mutex));
	return TRUE;
}
