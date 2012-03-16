#include "InjectionCurrentData.h"


InjectionCurrentData* allocate_injection_current_data(Network *network, TrialsData *trials_data, InjectionCurrentData* injection_current_data, unsigned int num_of_trial_start_available_currents, unsigned int num_of_in_refractory_currents, unsigned int num_of_in_trial_currents)
{
	unsigned int i, j, k, m, n;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	if (network == NULL)
		return (InjectionCurrentData*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_injection_current_data", "network == NULL.");
	if (trials_data == NULL)
		return (InjectionCurrentData*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_injection_current_data", "trials_data == NULL.");
	if (injection_current_data != NULL)
	{
		injection_current_data = deallocate_injection_current_data(network, trials_data, injection_current_data);
		injection_current_data = allocate_injection_current_data(network, trials_data, injection_current_data, num_of_trial_start_available_currents, num_of_in_refractory_currents, num_of_in_trial_currents);
		return injection_current_data;
	}
	injection_current_data = g_new0(InjectionCurrentData,1);
	injection_current_data->current_templates = g_new0(CurrentTemplate,1);
	injection_current_data->current_templates->trial_start_available_currents = g_new0(CurrentPatternTemplate, num_of_trial_start_available_currents);
	injection_current_data->current_templates->num_of_trial_start_available_currents = num_of_trial_start_available_currents;
	injection_current_data->current_templates->in_refractory_currents = g_new0(CurrentPatternTemplate, num_of_in_refractory_currents);
	injection_current_data->current_templates->num_of_in_refractory_currents = num_of_in_refractory_currents;

	injection_current_data->current_templates->in_trial_currents = g_new0(CurrentPatternTemplate*, trials_data->trial_types_data.num_of_types);
	for (n = 0; n < trials_data->trial_types_data.num_of_types; n++)
	{
		injection_current_data->current_templates->in_trial_currents[n] = g_new0(CurrentPatternTemplate, num_of_in_trial_currents);
	}
	injection_current_data->current_templates->num_of_in_trial_currents = num_of_in_trial_currents;
 
	if (!get_num_of_layers_in_network(network, &num_of_layers))
		return (InjectionCurrentData*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_injection_current_data", "Couldn' t retrieve number of layers. Already allocated some data. Take care of that data.");
	for (m = 0; m < num_of_trial_start_available_currents; m++)
	{	
		injection_current_data->current_templates->trial_start_available_currents[m].templates = g_new0(NeuronCurrentTemplate**, num_of_layers);
	}
	for (m = 0; m < num_of_in_refractory_currents; m++)
	{	
		injection_current_data->current_templates->in_refractory_currents[m].templates = g_new0(NeuronCurrentTemplate**, num_of_layers);
	}
	for (n = 0; n < trials_data->trial_types_data.num_of_types; n++)
	{
		for (m = 0; m < num_of_in_trial_currents; m++)
		{	
			injection_current_data->current_templates->in_trial_currents[n][m].templates = g_new0(NeuronCurrentTemplate**, num_of_layers);
		}
	}
	for (i = 0; i < num_of_layers; i++)
	{
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
			return (InjectionCurrentData*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_injection_current_data", "Couldn' t retrieve number of neuron groups. Already allocated some data. Take care of that data.");
		for (m = 0; m < num_of_trial_start_available_currents; m++)
		{	
			injection_current_data->current_templates->trial_start_available_currents[m].templates[i] = g_new0(NeuronCurrentTemplate*, num_of_neuron_groups_in_layer);
		}
		for (m = 0; m < num_of_in_refractory_currents; m++)
		{	
			injection_current_data->current_templates->in_refractory_currents[m].templates[i] = g_new0(NeuronCurrentTemplate*, num_of_neuron_groups_in_layer);
		}
		for (n = 0; n < trials_data->trial_types_data.num_of_types; n++)
		{
			for (m = 0; m < num_of_in_trial_currents; m++)
			{	
				injection_current_data->current_templates->in_trial_currents[n][m].templates[i] = g_new0(NeuronCurrentTemplate*, num_of_neuron_groups_in_layer);
			}
		}

		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
				return (InjectionCurrentData*)print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_injection_current_data", "Couldn' t retrieve number of neurons. Already allocated some data. Take care of that data.");	
			for (m = 0; m < num_of_trial_start_available_currents; m++)
			{	
				injection_current_data->current_templates->trial_start_available_currents[m].templates[i][j] = g_new0(NeuronCurrentTemplate, num_of_neurons_in_neuron_group);
			}
			for (m = 0; m < num_of_in_refractory_currents; m++)
			{	
				injection_current_data->current_templates->in_refractory_currents[m].templates[i][j] = g_new0(NeuronCurrentTemplate, num_of_neurons_in_neuron_group);
			}
			for (n = 0; n < trials_data->trial_types_data.num_of_types; n++)
			{
				for (m = 0; m < num_of_in_trial_currents; m++)
				{	
					injection_current_data->current_templates->in_trial_currents[n][m].templates[i][j] = g_new0(NeuronCurrentTemplate, num_of_neurons_in_neuron_group);
				}
			}

			for (k = 0; k < num_of_neurons_in_neuron_group; k++)	// allocate in trial current lengths since those are determined by trialcontroller. submit lengths for others separately.
			{
				for (n = 0; n < trials_data->trial_types_data.num_of_types; n++)
				{
					for (m = 0; m < num_of_in_trial_currents; m++)
					{	
						injection_current_data->current_templates->in_trial_currents[n][m].templates[i][j][k].current = g_new0(InjectionCurrent, trials_data->trial_types_data.type_data[n].constraints.max_trial_length/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
						injection_current_data->current_templates->in_trial_currents[n][m].num_of_current_samples = trials_data->trial_types_data.type_data[n].constraints.max_trial_length/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
					}
				}
			}
		}
	}	
	print_message(INFO_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "allocate_injection_current_data", "Created injection_current_data.");
	return injection_current_data;

}
InjectionCurrentData* deallocate_injection_current_data(Network *network, TrialsData *trials_data, InjectionCurrentData* injection_current_data)
{
	print_message(INFO_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "deallocate_injection_current_data", "Destroyed injection_current_data.");
	return NULL;
}
bool submit_current_length_trial_start_available_status(Network *network, InjectionCurrentData* current_data, unsigned int trial_start_available_current_num, TimeStamp current_length)
{
	unsigned int i, j, k, m;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	char str [200];
	if (current_data == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_trial_start_available_status", "current_data == NULL.");
	if (trial_start_available_current_num >= current_data->current_templates->num_of_trial_start_available_currents)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_trial_start_available_status", "trial_start_available_current_num >= current_data->current_templates.num_of_trial_start_available_currents.");
	if (!get_num_of_layers_in_network(network, &num_of_layers))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_trial_start_available_status", "Couldn' t retrieve number of layers. Already allocated some data. Take care of that data.");
	for (i = 0; i < num_of_layers; i++)
	{
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_trial_start_available_status", "Couldn' t retrieve number of neuron groups. Already allocated some data. Take care of that data.");
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_trial_start_available_status", "Couldn' t retrieve number of neurons. Already allocated some data. Take care of that data.");
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)	// allocate in trial current lengths since those are determined by trialcontroller. submit lengths for others separately.
			{
				if (current_data->current_templates->trial_start_available_currents[trial_start_available_current_num].templates[i][j][k].current != NULL)
					g_free(current_data->current_templates->trial_start_available_currents[trial_start_available_current_num].templates[i][j][k].current);
				current_data->current_templates->trial_start_available_currents[trial_start_available_current_num].templates[i][j][k].current = g_new0(InjectionCurrent, current_length/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
				current_data->current_templates->trial_start_available_currents[trial_start_available_current_num].num_of_current_samples = current_length/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
			}	
		}
	}

	for (m = 0; m < current_data->current_templates->num_of_trial_start_available_currents; m++)
	{	
		if (current_data->current_templates->trial_start_available_currents[m].templates[0][0][0].current == NULL)
		{
			sprintf (str, "Still there are unallocated current patterns for trial start available current %d.", m);
			print_message(INFO_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_trial_start_available_status", str);
		}
	}
	return TRUE;
}
bool submit_current_length_in_refractory_status(Network *network, InjectionCurrentData* current_data, unsigned int in_refractory_current_num, TimeStamp current_length)
{
	unsigned int i, j, k, m;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	char str [200];
	if (current_data == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_in_refractory_status", "current_data == NULL.");
	if (in_refractory_current_num >= current_data->current_templates->num_of_in_refractory_currents)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_trial_start_available_status", "in_refractory_current_num >= current_data->current_templates.in_refractory_current_num.");
	if (!get_num_of_layers_in_network(network, &num_of_layers))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_in_refractory_status", "Couldn' t retrieve number of layers. Already allocated some data. Take care of that data.");
	for (i = 0; i < num_of_layers; i++)
	{
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_in_refractory_status", "Couldn' t retrieve number of neuron groups. Already allocated some data. Take care of that data.");
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_in_refractory_status", "Couldn' t retrieve number of neurons. Already allocated some data. Take care of that data.");
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)	// allocate in trial current lengths since those are determined by trialcontroller. submit lengths for others separately.
			{
				if (current_data->current_templates->in_refractory_currents[in_refractory_current_num].templates[i][j][k].current != NULL)
					g_free(current_data->current_templates->in_refractory_currents[in_refractory_current_num].templates[i][j][k].current);
				current_data->current_templates->in_refractory_currents[in_refractory_current_num].templates[i][j][k].current = g_new0(InjectionCurrent, current_length/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
				current_data->current_templates->in_refractory_currents[in_refractory_current_num].num_of_current_samples = current_length/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
			}	
		}
	}
	for (m = 0; m < current_data->current_templates->num_of_in_refractory_currents; m++)
	{	
		if (current_data->current_templates->in_refractory_currents[m].templates[0][0][0].current == NULL)
		{
			sprintf (str, "Still there are unallocated current patterns for in refractory current %d.", m);
			print_message(INFO_MSG ,"IzNeuronSimulators", "InjectionCurrentData", "submit_current_length_in_refractory_status", str);
		}
	}

	return TRUE;
}
