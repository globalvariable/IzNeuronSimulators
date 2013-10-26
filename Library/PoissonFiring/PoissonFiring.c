#include "PoissonFiring.h"


ThreePhasePoissonFiring* allocate_three_phase_poisson_firing_data(Network *network, TrialHandParadigmRobotReach *paradigm, ThreePhasePoissonFiring* firing_data)
{
	unsigned int i, j, n;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	if (network == NULL)
		return (ThreePhasePoissonFiring*)print_message(ERROR_MSG ,"IzNeuronSimulators", "PoissonFiring", "allocate_three_phase_poisson_firing_data", "network == NULL.");
	if (paradigm == NULL)
		return (ThreePhasePoissonFiring*)print_message(ERROR_MSG ,"IzNeuronSimulators", "PoissonFiring", "allocate_three_phase_poisson_firing_data", "trial_types_data == NULL.");
	if (firing_data != NULL)
	{
		firing_data = deallocate_three_phase_poisson_firing_data(network, paradigm, firing_data);
		firing_data = allocate_three_phase_poisson_firing_data(network, paradigm, firing_data);
		return firing_data;
	}
	firing_data = g_new0(ThreePhasePoissonFiring,1);

	if (!get_num_of_layers_in_network(network, &num_of_layers))
		return (ThreePhasePoissonFiring*)print_message(ERROR_MSG ,"IzNeuronSimulators", "PoissonFiring", "allocate_three_phase_poisson_firing_data", "Couldn' t retrieve number of layers. Already allocated some data. Take care of that data.");
	firing_data->trial_start_available_firing.rates = g_new0(double**, num_of_layers);	
	for (i = 0; i < num_of_layers; i++)
	{	
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
			return (ThreePhasePoissonFiring*)print_message(ERROR_MSG ,"IzNeuronSimulators", "PoissonFiring", "allocate_three_phase_poisson_firing_data", "Couldn' t retrieve number of neuron groups. Already allocated some data. Take care of that data.");
		firing_data->trial_start_available_firing.rates[i] = g_new0(double*, num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
				return (ThreePhasePoissonFiring*)print_message(ERROR_MSG ,"IzNeuronSimulators", "PoissonFiring", "allocate_three_phase_poisson_firing_data", "Couldn' t retrieve number of neurons. Already allocated some data. Take care of that data.");
			firing_data->trial_start_available_firing.rates[i][j] = g_new0(double, num_of_neurons_in_neuron_group);
		}		
	}

	if (!get_num_of_layers_in_network(network, &num_of_layers))
		return (ThreePhasePoissonFiring*)print_message(ERROR_MSG ,"IzNeuronSimulators", "PoissonFiring", "allocate_three_phase_poisson_firing_data", "Couldn' t retrieve number of layers. Already allocated some data. Take care of that data.");
	firing_data->in_refractory_firing.rates = g_new0(double**, num_of_layers);	
	for (i = 0; i < num_of_layers; i++)
	{	
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
			return (ThreePhasePoissonFiring*)print_message(ERROR_MSG ,"IzNeuronSimulators", "PoissonFiring", "allocate_three_phase_poisson_firing_data", "Couldn' t retrieve number of neuron groups. Already allocated some data. Take care of that data.");
		firing_data->in_refractory_firing.rates[i] = g_new0(double*, num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
				return (ThreePhasePoissonFiring*)print_message(ERROR_MSG ,"IzNeuronSimulators", "PoissonFiring", "allocate_three_phase_poisson_firing_data", "Couldn' t retrieve number of neurons. Already allocated some data. Take care of that data.");
			firing_data->in_refractory_firing.rates[i][j] = g_new0(double, num_of_neurons_in_neuron_group);
		}		
	}

	firing_data->in_trial_firing = g_new0(NetworkPoissonFiring, paradigm->num_of_robot_target_positions);
	for (n = 0; n < paradigm->num_of_robot_target_positions; n++)
	{
		firing_data->in_trial_firing[n].rates = g_new0(double**, num_of_layers);
		for (i = 0; i < num_of_layers; i++)
		{	
			if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
				return (ThreePhasePoissonFiring*)print_message(ERROR_MSG ,"IzNeuronSimulators", "PoissonFiring", "allocate_three_phase_poisson_firing_data", "Couldn' t retrieve number of neuron groups. Already allocated some data. Take care of that data.");
			firing_data->in_trial_firing[n].rates[i] = g_new0(double*, num_of_neuron_groups_in_layer);
			for (j=0; j<num_of_neuron_groups_in_layer; j++)
			{
				if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
					return (ThreePhasePoissonFiring*)print_message(ERROR_MSG ,"IzNeuronSimulators", "PoissonFiring", "allocate_three_phase_poisson_firing_data", "Couldn' t retrieve number of neurons. Already allocated some data. Take care of that data.");
				firing_data->in_trial_firing[n].rates[i][j] = g_new0(double, num_of_neurons_in_neuron_group);
			}
		}
	}

	print_message(INFO_MSG ,"IzNeuronSimulators", "PoissonFiring", "allocate_three_phase_poisson_firing_data", "Created current_templates.");
	return firing_data;

}


ThreePhasePoissonFiring* deallocate_three_phase_poisson_firing_data(Network *network, TrialHandParadigmRobotReach *paradigm, ThreePhasePoissonFiring* firing_data)
{
	print_message(BUG_MSG ,"IzNeuronSimulators", "PoissonFiring", "deallocate_three_phase_poisson_firing_data", "Not implemented.");
	return NULL;
}

NetworkPoissonFiring* get_in_trial_firing_data(ThreePhasePoissonFiring *firing_data, unsigned int idx)
{
	return &(firing_data->in_trial_firing[idx]);
}

NetworkPoissonFiring* get_trial_start_available_firing_data(ThreePhasePoissonFiring *firing_data)
{
	return &(firing_data->trial_start_available_firing);
}
NetworkPoissonFiring* get_in_refractory_firing_data(ThreePhasePoissonFiring *firing_data)
{
	return &(firing_data->in_refractory_firing);
}

void apply_firing_data_to_network(Network *network, NetworkPoissonFiring *firing_data, TimeStamp interval)   // interval = evaluation preiod in nanosec
{	
	double firing_rate;
	unsigned int i, j, k;
	for (i = 0; i < network->layer_count; i++)
	{ 
		for (j = 0; j < network->layers[i]->neuron_group_count; j++)
		{
			for (k = 0; k < network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				firing_rate = firing_data->rates[i][j][k] * (((double)(interval))/1000000000.0);
				submit_firing_rate_to_poisson_neuron (&(network->layers[i]->neuron_groups[j]->neurons[k]), firing_rate);
			}
		}
	}

}
