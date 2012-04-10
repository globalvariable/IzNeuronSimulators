#include "NeuronDynamicsData.h"



NeuronDynamicsBuffer* allocate_neuron_dynamics_buffer(Network *network, NeuronDynamicsBuffer* buffer, unsigned int buffer_size)
{
	unsigned int i, j, k;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;

	if (network == NULL)
		return (NeuronDynamicsBuffer*)print_message(ERROR_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "allocate_neuron_dynamics_buffer", "network == NULL.");
	if (buffer != NULL)
	{
		buffer = deallocate_neuron_dynamics_buffer(network, buffer);
		buffer = allocate_neuron_dynamics_buffer(network, buffer, buffer_size);
		return buffer;
	}
	buffer = g_new0(NeuronDynamicsBuffer,1);
	pthread_mutex_init(&(buffer->mutex), NULL);
	buffer->buffer = g_new0(NetworkNeuronDynamics, buffer_size);

	for (k = 0; k <  buffer_size; k++)
	{
		if (!get_num_of_layers_in_network(network, &num_of_layers))
			return (NeuronDynamicsBuffer*)print_message(ERROR_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "allocate_neuron_dynamics_buffer", "Couldn' t retrieve number of layers. Already allocated some data. Take care of that data.");
		buffer->buffer[k].network_neuron_dyn = g_new0(NeuronDynamics**, num_of_layers);
		for (i = 0; i < num_of_layers; i++)
		{
			if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
				return (NeuronDynamicsBuffer*)print_message(ERROR_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "allocate_neuron_dynamics_buffer","Couldn' t retrieve number of neuron groups. Already allocated some data. Take care of that data.");
			buffer->buffer[k].network_neuron_dyn[i] = g_new0(NeuronDynamics*, num_of_neuron_groups_in_layer);
			for (j=0; j<num_of_neuron_groups_in_layer; j++)
			{
				if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
					return (NeuronDynamicsBuffer*)print_message(ERROR_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "allocate_neuron_dynamics_buffer","Couldn' t retrieve number of neurons. Already allocated some data. Take care of that data.");	
				buffer->buffer[k].network_neuron_dyn[i][j] = g_new0(NeuronDynamics, num_of_neurons_in_neuron_group);
			}
		}
	}	
	buffer->buffer_size = buffer_size;
	print_message(INFO_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "allocate_neuron_dynamics_buffer", "Created neuron_dynamics_buffer.");
	return buffer;
}
NeuronDynamicsBuffer* deallocate_neuron_dynamics_buffer(Network *network, NeuronDynamicsBuffer* buffer_data)
{
	print_message(BUG_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "deallocate_neuron_dynamics_buffer", "Not implemented.");	
	return NULL;
}

bool push_neuron_dynamics_to_neuron_dynamics_buffer(Network *network, NeuronDynamicsBuffer* neuron_dynamics_buffer, TimeStamp sampling_time) 
{
	unsigned int i, j, k;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	NeuronDynamics ***dynamics_samples =  neuron_dynamics_buffer->buffer[neuron_dynamics_buffer->buff_write_idx].network_neuron_dyn;
	IzNeuronParams *iz_params;
	get_num_of_layers_in_network(network, &num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{	
		get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{
			get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				iz_params = get_neuron_address(network, i, j, k)->iz_params;
				dynamics_samples[i][j][k].dyn[DYNAMICS_TYPE_V] = iz_params->v;
				dynamics_samples[i][j][k].dyn[DYNAMICS_TYPE_U] = iz_params->u;
				dynamics_samples[i][j][k].dyn[DYNAMICS_TYPE_E] = iz_params->conductance_excitatory;
				dynamics_samples[i][j][k].dyn[DYNAMICS_TYPE_I] = iz_params->conductance_inhibitory;
			}
		}
	}
	pthread_mutex_lock(&(neuron_dynamics_buffer->mutex));
	if ((neuron_dynamics_buffer->buff_write_idx + 1) == neuron_dynamics_buffer->buffer_size)
		neuron_dynamics_buffer->buff_write_idx = 0;
	else
		neuron_dynamics_buffer->buff_write_idx++;
	neuron_dynamics_buffer->last_sample_time = sampling_time;
	pthread_mutex_unlock(&(neuron_dynamics_buffer->mutex));
	return TRUE;
}

bool get_neuron_dynamics_last_sample_time_and_write_idx(NeuronDynamicsBuffer *buffer, TimeStamp *last_sample_time, unsigned int *write_idx)
{
	pthread_mutex_lock(&(buffer->mutex));
	*write_idx = buffer->buff_write_idx;
	*last_sample_time = buffer->last_sample_time;
	pthread_mutex_unlock(&(buffer->mutex));
	return TRUE;
}

NeuronDynamicsBufferLimited* allocate_neuron_dynamics_buffer_limited(Network *network, NeuronDynamicsBufferLimited* buffer, unsigned int buffer_size, unsigned int num_of_selected_neurons)
{
	unsigned int i;
	if (network == NULL)
		return (NeuronDynamicsBufferLimited*)print_message(ERROR_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "allocate_neuron_dynamics_buffer_limited", "network == NULL.");
	if (buffer != NULL)
	{
		buffer = deallocate_neuron_dynamics_buffer_limited(network, buffer);
		buffer = allocate_neuron_dynamics_buffer_limited(network, buffer, buffer_size, num_of_selected_neurons);
		return buffer;
	}
	buffer = g_new0(NeuronDynamicsBufferLimited,1);
	pthread_mutex_init(&(buffer->mutex), NULL);
	buffer->buffer = g_new0(SelectedNeuronDynamics, buffer_size);
	for (i = 0; i <  buffer_size; i++)
		buffer->buffer[i].neuron_dynamic = g_new0(double, num_of_selected_neurons);
	buffer->selected_dyns = g_new0(SelectedNeuronDynList, num_of_selected_neurons);
	buffer->num_of_selected_neurons = num_of_selected_neurons;
	buffer->buffer_size = buffer_size;
	print_message(INFO_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "allocate_neuron_dynamics_buffer_limited", "Created neuron_dynamics_buffer_limited.");
	return buffer;
}

NeuronDynamicsBufferLimited* deallocate_neuron_dynamics_buffer_limited(Network *network, NeuronDynamicsBufferLimited* buffer)
{
	print_message(BUG_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "deallocate_neuron_dynamics_buffer_limited", "Not implemented.");	
	return NULL;
}

bool submit_selected_neuron_to_neuron_dynamics_buffer_limited(Network *network, NeuronDynamicsBufferLimited* buffer, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num,  int dynamics_type, unsigned int list_idx)
{
	if (!is_neuron(network, layer, neuron_group, neuron_num))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "submit_selected_neuron_to_neuron_dynamics_buffer_limited", "! is_neuron().");
	if ((dynamics_type >= MAX_NUM_OF_NEURON_DYNAMICS_TYPE) || (dynamics_type < 0))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "submit_selected_neuron_to_neuron_dynamics_buffer_limited", "(dynamics_type >= MAX_NUM_OF_NEURON_DYNAMICS_TYPE) || (dynamics_type < 0).");
	if (list_idx >= buffer->num_of_selected_neurons)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "submit_selected_neuron_to_neuron_dynamics_buffer_limited", "list_idx >= buffer->num_of_selected_neurons.");	
	pthread_mutex_lock(&(buffer->mutex));	
	buffer->selected_dyns[list_idx].layer = layer;
	buffer->selected_dyns[list_idx].neuron_group = neuron_group;
	buffer->selected_dyns[list_idx].neuron_num = neuron_num;
	buffer->selected_dyns[list_idx].dynamics_type = dynamics_type;
	pthread_mutex_unlock(&(buffer->mutex));	
	return TRUE;
}

bool push_neuron_dynamics_to_neuron_dynamics_buffer_limited(Network *network, NeuronDynamicsBufferLimited* buffer, TimeStamp sampling_time) 
{
	unsigned int i;
	SelectedNeuronDynList	*selected_dyns = buffer->selected_dyns;
	unsigned int num_of_selected_neurons = buffer->num_of_selected_neurons;
	double *neuron_dynamic = buffer->buffer[buffer->buff_write_idx].neuron_dynamic;
	IzNeuronParams		*iz_params;

	pthread_mutex_lock(&(buffer->mutex));
	for (i = 0; i < num_of_selected_neurons; i++)
	{
		iz_params = network->layers[selected_dyns[i].layer]->neuron_groups[selected_dyns[i].neuron_group]->neurons[selected_dyns[i].neuron_num].iz_params;
		switch (selected_dyns[i].dynamics_type)
		{
			case DYNAMICS_TYPE_V:
				 neuron_dynamic[i] = iz_params->v;
				break;
			case DYNAMICS_TYPE_U:
				 neuron_dynamic[i] = iz_params->u;
				break;
			case DYNAMICS_TYPE_E:
				 neuron_dynamic[i] = iz_params->conductance_excitatory;
				break;
			case DYNAMICS_TYPE_I:
				neuron_dynamic[i] = iz_params->conductance_inhibitory;
				break;
			default: 
				return print_message(BUG_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "push_neuron_dynamics_to_neuron_dynamics_buffer_limited", "Unknown dynamics type.");		
		}
	}
	if ((buffer->buff_write_idx + 1) == buffer->buffer_size)
		buffer->buff_write_idx = 0;
	else
		buffer->buff_write_idx++;
	buffer->last_sample_time = sampling_time;
	pthread_mutex_unlock(&(buffer->mutex));
	return TRUE;
}

bool get_neuron_dynamics_limited_last_sample_time_and_write_idx(NeuronDynamicsBufferLimited *buffer, TimeStamp *last_sample_time, unsigned int *write_idx)
{
	pthread_mutex_lock(&(buffer->mutex));
	*write_idx = buffer->buff_write_idx;
	*last_sample_time = buffer->last_sample_time;
	pthread_mutex_unlock(&(buffer->mutex));
	return TRUE;
}
