#include "InjectionCurrentData.h"


CurrentTemplate* allocate_current_templates(Network *network, TrialsData *trials_data, CurrentTemplate* current_data, unsigned int num_of_trial_start_available_currents, unsigned int num_of_in_refractory_currents, unsigned int num_of_in_trial_currents)
{
	unsigned int i, j, k, m, n;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	if (network == NULL)
		return (CurrentTemplate*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "network == NULL.");
	if (trials_data == NULL)
		return (CurrentTemplate*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "trials_data == NULL.");
	if (current_data != NULL)
	{
		current_data = deallocate_current_templates(network, trials_data, current_data);
		current_data = allocate_current_templates(network, trials_data, current_data, num_of_trial_start_available_currents, num_of_in_refractory_currents, num_of_in_trial_currents);
		return current_data;
	}
	current_data = g_new0(CurrentTemplate,1);

	current_data->trial_start_available_currents = g_new0(CurrentPatternTemplate, num_of_trial_start_available_currents);
	current_data->num_of_trial_start_available_currents = num_of_trial_start_available_currents;
	for (m = 0; m < num_of_trial_start_available_currents; m++)
	{
		if (!get_num_of_layers_in_network(network, &num_of_layers))
			return (CurrentTemplate*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "Couldn' t retrieve number of layers. Already allocated some data. Take care of that data.");
		current_data->trial_start_available_currents[m].noise_params = g_new0(CurrentNoiseParams**, num_of_layers);	
		for (i = 0; i < num_of_layers; i++)
		{	
			if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
				return (CurrentTemplate*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "Couldn' t retrieve number of neuron groups. Already allocated some data. Take care of that data.");
			current_data->trial_start_available_currents[m].noise_params[i] = g_new0(CurrentNoiseParams*, num_of_neuron_groups_in_layer);
			for (j=0; j<num_of_neuron_groups_in_layer; j++)
			{
				if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
					return (CurrentTemplate*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "Couldn' t retrieve number of neurons. Already allocated some data. Take care of that data.");
				current_data->trial_start_available_currents[m].noise_params[i][j] = g_new0(CurrentNoiseParams, num_of_neurons_in_neuron_group);
			}
		}		
	}

	current_data->in_refractory_currents = g_new0(CurrentPatternTemplate, num_of_in_refractory_currents);
	current_data->num_of_in_refractory_currents = num_of_in_refractory_currents;
	for (m = 0; m < num_of_in_refractory_currents; m++)
	{
		if (!get_num_of_layers_in_network(network, &num_of_layers))
			return (CurrentTemplate*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "Couldn' t retrieve number of layers. Already allocated some data. Take care of that data.");
		current_data->in_refractory_currents[m].noise_params = g_new0(CurrentNoiseParams**, num_of_layers);	
		for (i = 0; i < num_of_layers; i++)
		{	
			if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
				return (CurrentTemplate*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "Couldn' t retrieve number of neuron groups. Already allocated some data. Take care of that data.");
			current_data->in_refractory_currents[m].noise_params[i] = g_new0(CurrentNoiseParams*, num_of_neuron_groups_in_layer);
			for (j=0; j<num_of_neuron_groups_in_layer; j++)
			{
				if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
					return (CurrentTemplate*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "Couldn' t retrieve number of neurons. Already allocated some data. Take care of that data.");
				current_data->in_refractory_currents[m].noise_params[i][j] = g_new0(CurrentNoiseParams, num_of_neurons_in_neuron_group);
			}
		}		
	}

	current_data->in_trial_currents = g_new0(CurrentPatternTemplate*, trials_data->trial_types_data.num_of_types);
	current_data->num_of_in_trial_currents = num_of_in_trial_currents;
	for (n = 0; n < trials_data->trial_types_data.num_of_types; n++)
	{
		current_data->in_trial_currents[n] = g_new0(CurrentPatternTemplate, num_of_in_trial_currents);
		for (m = 0; m < num_of_in_trial_currents; m++)
		{
			current_data->in_trial_currents[n][m].template_samples = g_new0(NeuronCurrentSample, trials_data->trial_types_data.type_data[n].constraints.max_trial_length/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
			current_data->in_trial_currents[n][m].num_of_template_samples = trials_data->trial_types_data.type_data[n].constraints.max_trial_length/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
			current_data->in_trial_currents[n][m].template_length = trials_data->trial_types_data.type_data[n].constraints.max_trial_length;
			for (k = 0; k < current_data->in_trial_currents[n][m].num_of_template_samples ; k++)
			{
				if (!get_num_of_layers_in_network(network, &num_of_layers))
					return (CurrentTemplate*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "Couldn' t retrieve number of layers. Already allocated some data. Take care of that data.");
				current_data->in_trial_currents[n][m].template_samples[k].current_sample = g_new0(InjectionCurrent**, num_of_layers);	
				for (i = 0; i < num_of_layers; i++)
				{	
					if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
						return (CurrentTemplate*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "Couldn' t retrieve number of neuron groups. Already allocated some data. Take care of that data.");
					current_data->in_trial_currents[n][m].template_samples[k].current_sample[i] = g_new0(InjectionCurrent*, num_of_neuron_groups_in_layer);
					for (j=0; j<num_of_neuron_groups_in_layer; j++)
					{
						if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
							return (CurrentTemplate*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "Couldn' t retrieve number of neurons. Already allocated some data. Take care of that data.");
						current_data->in_trial_currents[n][m].template_samples[k].current_sample[i][j] = g_new0(InjectionCurrent, num_of_neurons_in_neuron_group);
					}
				}
			}
		}
	}

	for (n = 0; n < trials_data->trial_types_data.num_of_types; n++)
	{
		for (m = 0; m < num_of_in_trial_currents; m++)
		{
			if (!get_num_of_layers_in_network(network, &num_of_layers))
				return (CurrentTemplate*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "Couldn' t retrieve number of layers. Already allocated some data. Take care of that data.");
			current_data->in_trial_currents[n][m].noise_params = g_new0(CurrentNoiseParams**, num_of_layers);	
			for (i = 0; i < num_of_layers; i++)
			{	
				if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
					return (CurrentTemplate*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "Couldn' t retrieve number of neuron groups. Already allocated some data. Take care of that data.");
				current_data->in_trial_currents[n][m].noise_params[i] = g_new0(CurrentNoiseParams*, num_of_neuron_groups_in_layer);
				for (j=0; j<num_of_neuron_groups_in_layer; j++)
				{
					if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
						return (CurrentTemplate*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "Couldn' t retrieve number of neurons. Already allocated some data. Take care of that data.");
					current_data->in_trial_currents[n][m].noise_params[i][j] = g_new0(CurrentNoiseParams, num_of_neurons_in_neuron_group);
				}
			}
		}
	}
	print_message(INFO_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "Created current_templates.");
	return current_data;

}
CurrentTemplate* deallocate_current_templates(Network *network, TrialsData *trials_data, CurrentTemplate* current_data)
{
	print_message(BUG_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "deallocate_current_templates", "Not implemented.");
	return NULL;
}
bool submit_current_length_trial_start_available_status(Network *network, CurrentTemplate* current_data, unsigned int trial_start_available_current_num, TimeStamp current_length, bool *has_unallocated_current_template)
{
	unsigned int i, j, k, m;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	char str [200];
	if (current_data == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_trial_start_available_status", "current_data == NULL.");
	if (trial_start_available_current_num >= current_data->num_of_trial_start_available_currents)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_trial_start_available_status", "trial_start_available_current_num >= current_data->current_templates.num_of_trial_start_available_currents.");
	if (current_data->trial_start_available_currents[trial_start_available_current_num].template_samples != NULL)
	{
		for (k = 0; k < current_data->trial_start_available_currents[trial_start_available_current_num].num_of_template_samples; k++)
		{
			get_num_of_layers_in_network(network, &num_of_layers);
			for (i = 0; i < num_of_layers; i++)
			{	
				get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
				for (j=0; j<num_of_neuron_groups_in_layer; j++)
				{
					get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
					g_free(current_data->trial_start_available_currents[trial_start_available_current_num].template_samples[k].current_sample[i][j]);
				}
				g_free(current_data->trial_start_available_currents[trial_start_available_current_num].template_samples[k].current_sample[i]);
			}
			g_free(current_data->trial_start_available_currents[trial_start_available_current_num].template_samples[k].current_sample);
		}
		g_free(current_data->trial_start_available_currents[trial_start_available_current_num].template_samples);	
		current_data->trial_start_available_currents[trial_start_available_current_num].template_samples = NULL;
	}	
	current_data->trial_start_available_currents[trial_start_available_current_num].template_samples = g_new0(NeuronCurrentSample, current_length/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
	current_data->trial_start_available_currents[trial_start_available_current_num].num_of_template_samples = current_length/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
	current_data->trial_start_available_currents[trial_start_available_current_num].template_length = current_length;
	for (k = 0; k < current_data->trial_start_available_currents[trial_start_available_current_num].num_of_template_samples; k++)
	{
		get_num_of_layers_in_network(network, &num_of_layers);
		current_data->trial_start_available_currents[trial_start_available_current_num].template_samples[k].current_sample = g_new0(InjectionCurrent**, num_of_layers);
		for (i = 0; i < num_of_layers; i++)
		{	
			get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
			current_data->trial_start_available_currents[trial_start_available_current_num].template_samples[k].current_sample[i] = g_new0(InjectionCurrent*, num_of_neuron_groups_in_layer);
			for (j=0; j<num_of_neuron_groups_in_layer; j++)
			{
				get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
				current_data->trial_start_available_currents[trial_start_available_current_num].template_samples[k].current_sample[i][j] = g_new0(InjectionCurrent, num_of_neurons_in_neuron_group);
			}
		}
	}
	*has_unallocated_current_template = FALSE;
	for (m = 0; m < current_data->num_of_trial_start_available_currents; m++)
	{	
		if (current_data->trial_start_available_currents[m].template_samples == NULL)
		{
			*has_unallocated_current_template = TRUE;
			sprintf (str, "Still there are unallocated current patterns for trial start available current %d.", m);
			print_message(INFO_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_trial_start_available_status", str);
		}
	}
	return TRUE;
}
bool submit_current_length_in_refractory_status(Network *network, CurrentTemplate* current_data, unsigned int in_refractory_current_num, TimeStamp current_length, bool *has_unallocated_current_template)
{
	unsigned int i, j, k, m;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	char str [200];
	if (current_data == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_in_refractory_status", "current_data == NULL.");
	if (in_refractory_current_num >= current_data->num_of_in_refractory_currents)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_in_refractory_status", "in_refractory_current_num >= current_data->current_templates.in_refractory_current_num.");
	if (current_data->in_refractory_currents[in_refractory_current_num].template_samples != NULL)
	{
		for (k = 0; k < current_data->in_refractory_currents[in_refractory_current_num].num_of_template_samples; k++)
		{
			get_num_of_layers_in_network(network, &num_of_layers);
			for (i = 0; i < num_of_layers; i++)
			{
				get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
				for (j=0; j<num_of_neuron_groups_in_layer; j++)
				{
					get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
					g_free(current_data->in_refractory_currents[in_refractory_current_num].template_samples[k].current_sample[i][j]);
				}
				g_free(current_data->in_refractory_currents[in_refractory_current_num].template_samples[k].current_sample[i]);
			}
			g_free(current_data->in_refractory_currents[in_refractory_current_num].template_samples[k].current_sample);
		}
		g_free(current_data->in_refractory_currents[in_refractory_current_num].template_samples);		
		current_data->in_refractory_currents[in_refractory_current_num].template_samples = NULL;	
	}

	current_data->in_refractory_currents[in_refractory_current_num].template_samples = g_new0(NeuronCurrentSample, current_length/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
	current_data->in_refractory_currents[in_refractory_current_num].num_of_template_samples = current_length/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
	current_data->in_refractory_currents[in_refractory_current_num].template_length = current_length;
	for (k = 0; k < current_data->in_refractory_currents[in_refractory_current_num].num_of_template_samples; k++)
	{
		get_num_of_layers_in_network(network, &num_of_layers);
		current_data->in_refractory_currents[in_refractory_current_num].template_samples[k].current_sample = g_new0(InjectionCurrent**, num_of_layers);
		for (i = 0; i < num_of_layers; i++)
		{
			get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
			current_data->in_refractory_currents[in_refractory_current_num].template_samples[k].current_sample[i] = g_new0(InjectionCurrent*, num_of_neuron_groups_in_layer);
			for (j=0; j<num_of_neuron_groups_in_layer; j++)
			{
				get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
				current_data->in_refractory_currents[in_refractory_current_num].template_samples[k].current_sample[i][j] = g_new0(InjectionCurrent, num_of_neurons_in_neuron_group);
			}
		}
	}
	*has_unallocated_current_template = FALSE;
	for (m = 0; m < current_data->num_of_in_refractory_currents; m++)
	{	
		if (current_data->in_refractory_currents[m].template_samples == NULL)
		{
			*has_unallocated_current_template = TRUE;
			sprintf (str, "Still there are unallocated current patterns for in refractory current %d.", m);
			print_message(INFO_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_in_refractory_status", str);
		}
	}

	return TRUE;
}

CurrentPatternBuffer* allocate_current_pattern_buffer(Network *network, CurrentPatternBuffer *buffer, unsigned int buffer_size)
{
	unsigned int i, j, k;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;

	if (network == NULL)
		return (CurrentPatternBuffer*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_templates", "network == NULL.");
	if (buffer != NULL)
	{
		buffer = deallocate_current_pattern_buffer(network, buffer);
		buffer = allocate_current_pattern_buffer(network, buffer, buffer_size);
		return buffer;
	}
	buffer = g_new0(CurrentPatternBuffer,1);
	buffer->current_buffer = g_new0(NeuronCurrentSample, buffer_size);

	for (k = 0; k <  buffer_size; k++)
	{
		if (!get_num_of_layers_in_network(network, &num_of_layers))
			return (CurrentPatternBuffer*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_pattern_buffer", "Couldn' t retrieve number of layers. Already allocated some data. Take care of that data.");
		buffer->current_buffer[k].current_sample = g_new0(InjectionCurrent**, num_of_layers);
		for (i = 0; i < num_of_layers; i++)
		{
			if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
				return (CurrentPatternBuffer*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_pattern_buffer", "Couldn' t retrieve number of neuron groups. Already allocated some data. Take care of that data.");
			buffer->current_buffer[k].current_sample[i] = g_new0(InjectionCurrent*, num_of_neuron_groups_in_layer);
			for (j=0; j<num_of_neuron_groups_in_layer; j++)
			{
				if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
					return (CurrentPatternBuffer*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_pattern_buffer", "Couldn' t retrieve number of neurons. Already allocated some data. Take care of that data.");	
				buffer->current_buffer[k].current_sample[i][j] = g_new0(InjectionCurrent, num_of_neurons_in_neuron_group);
			}
		}
	}	
	buffer->buffer_size = buffer_size;
	print_message(INFO_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_current_pattern_buffer", "Created current_pattern_buffer.");
	return buffer;
}
CurrentPatternBuffer* deallocate_current_pattern_buffer(Network *network, CurrentPatternBuffer *buffer)
{
	print_message(BUG_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "deallocate_current_pattern_buffer", "Not implemented.");	
	return NULL;
}

bool determine_in_trial_current_number_randomly(CurrentTemplate* current_template, unsigned int *current_num)	
{
	if (current_template == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "determine_in_trial_current_number_randomly", "current_template == NULL.");	
	*current_num = (unsigned int)(current_template->num_of_in_trial_currents* get_rand_number());
	return TRUE;
}

bool determine_trial_start_available_current_number_randomly(CurrentTemplate* current_template, unsigned int *current_num)	
{
	if (current_template == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "determine_in_trial_current_number_randomly", "current_template == NULL.");	
	*current_num = (unsigned int)(current_template->num_of_trial_start_available_currents* get_rand_number());
	return TRUE;
}

bool determine_in_refractory_current_number_randomly(CurrentTemplate* current_template, unsigned int *current_num)	
{
	if (current_template == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "determine_in_trial_current_number_randomly", "current_template == NULL.");	
	*current_num = (unsigned int)(current_template->num_of_in_refractory_currents* get_rand_number());
	return TRUE;
}

bool get_in_trial_current_pattern_template(CurrentTemplate* current_template, CurrentPatternTemplate** pattern_template, unsigned int trial_type_idx, unsigned int current_num)
{
	* pattern_template = &(current_template->in_trial_currents[trial_type_idx][current_num]);
	return TRUE;
}
bool get_trial_start_available_current_pattern_template(CurrentTemplate* current_template, CurrentPatternTemplate** pattern_template, unsigned int current_num)
{
	* pattern_template = &(current_template->trial_start_available_currents[current_num]);
	return TRUE;
}	
bool get_in_refractory_current_pattern_template(CurrentTemplate* current_template, CurrentPatternTemplate** pattern_template, unsigned int current_num)
{
	* pattern_template = &(current_template->in_refractory_currents[current_num]);
	return TRUE;
}

void reset_prev_noise_addition_times_for_current_template(Network *network, CurrentPatternTemplate* pattern_template)
{
	unsigned int i, j, k;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	get_num_of_layers_in_network(network, &num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{	
		get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{
			get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				pattern_template->noise_params[i][j][k].prev_noise_addition_time = 0;
			}
		}
	}
}

bool load_current_template_sample_to_neurons_with_noise(Network *network, CurrentPatternTemplate* pattern_template, unsigned int current_template_read_idx, TimeStamp now)
{
	unsigned int i, j, k;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	InjectionCurrent ***current_samples =  pattern_template->template_samples[current_template_read_idx].current_sample;
	CurrentNoiseParams *neuron_noise_params;
	get_num_of_layers_in_network(network, &num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{	
		get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{
			get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				neuron_noise_params = &(pattern_template->noise_params[i][j][k]);
				if ((now - neuron_noise_params->prev_noise_addition_time) >=  (neuron_noise_params->noise_addition_interval))
				{
					get_neuron_address(network, i, j, k)->iz_params->I_inject = current_samples[i][j][k] + randn_notrig(0, neuron_noise_params->noise_variance);
					neuron_noise_params->prev_noise_addition_time = now;
				}
			}
		}
	}
	return TRUE;
}

bool push_neuron_currents_to_current_pattern_buffer(Network *network, CurrentPatternBuffer* current_pattern_buffer)
{
	unsigned int i, j, k;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	InjectionCurrent ***current_samples =  current_pattern_buffer->current_buffer[current_pattern_buffer->buff_write_idx].current_sample;
	get_num_of_layers_in_network(network, &num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{	
		get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{
			get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				current_samples[i][j][k] = get_neuron_address(network, i, j, k)->iz_params->I_inject;
			}
		}
	}
	if ((current_pattern_buffer->buff_write_idx + 1) == current_pattern_buffer->buffer_size)
		current_pattern_buffer->buff_write_idx = 0;
	else
		current_pattern_buffer->buff_write_idx++;
	return TRUE;
}
