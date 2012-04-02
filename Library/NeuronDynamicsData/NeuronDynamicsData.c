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
	print_message(INFO_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "allocate_neuron_dynamics_buffer", "Created current_pattern_buffer.");
	return buffer;
}
NeuronDynamicsBuffer* deallocate_neuron_dynamics_buffer(Network *network, NeuronDynamicsBuffer* buffer_data)
{
	print_message(BUG_MSG ,"IzNeuronSimulators", "NeuronDynamicsData", "deallocate_neuron_dynamics_buffer", "Not implemented.");	
	return NULL;
}

bool push_neuron_dynamics_to_neuron_dynamics_buffer(Network *network, NeuronDynamicsBuffer* neuron_dynamics_buffer, TimeStamp current_time)
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
				dynamics_samples[i][j][k].v = iz_params->v;
				dynamics_samples[i][j][k].u = iz_params->u;
				dynamics_samples[i][j][k].e = iz_params->conductance_excitatory;
				dynamics_samples[i][j][k].i = iz_params->conductance_inhibitory;
			}
		}
	}
	if ((neuron_dynamics_buffer->buff_write_idx + 1) == neuron_dynamics_buffer->buffer_size)
		neuron_dynamics_buffer->buff_write_idx = 0;
	else
		neuron_dynamics_buffer->buff_write_idx++;
	neuron_dynamics_buffer->last_sample_time = current_time;
	return TRUE;
}
