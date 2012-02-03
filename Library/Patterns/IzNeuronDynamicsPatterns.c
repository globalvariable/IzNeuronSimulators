#include "NeuronDynamicsPatterns.h"


NetworkIzNeuronDynamicsPatterns* allocate_network_izneuron_dynamics_patterns(Network *network, TrialStats *trial_stats, NetworkIzNeuronDynamicsPatterns* network_neuron_dynamics_patterns)
{
	if (network == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "allocate_network_neuron_dynamics_patterns", "network == NULL");	
		return NULL;
	}
	if (trial_stats == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "allocate_network_neuron_dynamics_patterns", "trial_stats == NULL");	
		return NULL;
	}
	if (network_neuron_dynamics_patterns != NULL)
		network_neuron_dynamics_patterns = deallocate_allocate_network_neuron_dynamics_patterns(network, trial_stats, network_neuron_dynamics_patterns);
		network_neuron_dynamics_patterns = allocate_allocate_network_neuron_dynamics_patterns(network, trial_stats, network_neuron_dynamics_patterns);
		return network_neuron_dynamics_patterns;
	}	
	network_neuron_dynamics_patterns = g_new0(NetworkIzNeuronDynamicsPatterns, 1);
	network_neuron_dynamics_patterns->head = g_new0(NetworkIzNeuronDynamicsPattern, 1);
	network_neuron_dynamics_patterns->tail = network_neuron_dynamics_patterns->head;
	print_message(INFO_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "allocate_network_neuron_dynamics_patterns", "Created network_neuron_dynamics_patterns.");
	return network_neuron_dynamics_patterns;	
}
NetworkIzNeuronDynamicsPatterns* deallocate_network_izneuron_dynamics_patterns(Network *network, TrialStats *trial_stats, NetworkIzNeuronDynamicsPatterns* network_neuron_dynamics_patterns)
{
	unsigned int i;
	NetworkIzNeuronDynamicsPattern *curr_pattern, *prev_pattern;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	unsigned int i, j, k, m;
	if (network == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_patterns", "network == NULL");	
		return NULL;
	}
	if (trial_stats == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_patterns", "trial_stats == NULL");	
		return NULL;
	}
	if (network_neuron_dynamics_patterns == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_patterns", "network_neuron_dynamics_patterns == NULL");	
		return NULL;
	}
	curr_pattern = network_neuron_dynamics_patterns->tail;
	for (i = 0; i < trial_stats->num_of_trials; i++)
	{
		prev_pattern = curr_pattern->prev;
		if (!get_num_of_layers_in_network(network, &num_of_layers))
			return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_patterns", "Couldn' t retrieve number of layers.");			
		for (j=0; j < num_of_layers; j++)
		{
			if(!get_num_of_neuron_groups_in_layer(network, j, &num_of_neuron_groups_in_layer))
				return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_patterns", "Couldn' t retrieve number of neuron groups.");
			for (k=0; k<num_of_neuron_groups_in_layer; k++)
			{
				if (!get_num_of_neurons_in_neuron_group(network, j, k, &num_of_neurons_in_neuron_group))
					return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_patterns", "Couldn' t retrieve number of neurons.");	
				for (m = 0 ; m < num_of_neurons_in_neuron_group; m++)
				{
					g_free(network_neuron_dynamics_patterns->tail->neuron_dynamics_pattern[j][k][m].v);
					g_free(network_neuron_dynamics_patterns->tail->neuron_dynamics_pattern[j][k][m].u);
					g_free(network_neuron_dynamics_patterns->tail->neuron_dynamics_pattern[j][k][m].e);
					g_free(network_neuron_dynamics_patterns->tail->neuron_dynamics_pattern[j][k][m].i);
					g_free(network_neuron_dynamics_patterns->tail->neuron_dynamics_pattern[j][k][m]);       /// Compiler should give error for here.
				}
				g_free(network_neuron_dynamics_patterns->tail->neuron_dynamics_pattern[j][k]);
			}
			g_free(network_neuron_dynamics_patterns->tail->neuron_dynamics_pattern[j]);		
		}
		g_free(network_neuron_dynamics_patterns->tail->neuron_dynamics_pattern);		
		g_free(network_neuron_dynamics_patterns->tail->sampling_times);
		curr_pattern = prev_pattern;
	}
	if (curr_pattern != network_neuron_dynamics_patterns->head)
	{
		print_message(BUG_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_patterns", "curr_pattern != network_neuron_dynamics_patterns->head");
	}
	g_free(network_neuron_dynamics_patterns->head);
	g_free(network_neuron_dynamics_patterns);
	print_message(INFO_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_patterns", "Destroyed network_neuron_dynamics_patterns.");
	retun NULL;
}
NetworkIzNeuronDynamicsSinglePattern* allocate_network_izneuron_dynamics_single_pattern(Network *network, NetworkIzNeuronDynamicsSinglePattern* network_neuron_dynamics_single_pattern, unsigned int num_of_samples_to_allocate)
{
	unsigned int i, j, k;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	if (network == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "allocate_network_izneuron_dynamics_single_pattern", "network == NULL");	
		return NULL;
	}
	if (network_neuron_dynamics_single_pattern != NULL)
	{
		network_neuron_dynamics_single_pattern = deallocate_network_izneuron_dynamics_single_pattern(network, network_neuron_dynamics_single_pattern);
		network_neuron_dynamics_single_pattern = allocate_network_izneuron_dynamics_single_pattern(network, network_neuron_dynamics_single_pattern);
		return network_neuron_dynamics_single_pattern;
	}
	network_neuron_dynamics_single_pattern = g_new0(NetworkIzNeuronDynamicsSinglePattern, 1);
	network_neuron_dynamics_single_pattern->sampling_times = g_new0(TimeStamp, num_of_samples_to_allocate);	
	network_neuron_dynamics_single_pattern->allocated_num_of_samples = num_of_samples_to_allocate;
	network_neuron_dynamics_single_pattern->neuron_dynamics_pattern = g_new0(IzNeuronDynamicsPattern**,network->layer_count);
	if (!get_num_of_layers_in_network(network, &num_of_layers))
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "allocate_network_izneuron_dynamics_single_pattern", "Couldn' t retrieve number of layers.");	
	for (i = 0; i < num_of_layers; i++)
	{
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
			return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "allocate_network_izneuron_dynamics_single_pattern", "Couldn' t retrieve number of neuron groups.");
		network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i] = g_new0(IzNeuronDynamicsPattern*,num_of_neuron_groups_in_layer;
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
				return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "allocate_network_izneuron_dynamics_single_pattern", "Couldn' t retrieve number of neurons.");	
			network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j] = g_new0(IzNeuronDynamicsPattern, num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].v = g_new0(double, num_of_samples_to_allocate);
				network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].u = g_new0(double, num_of_samples_to_allocate);
				network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].e = g_new0(double, num_of_samples_to_allocate);
				network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].i = g_new0(double, num_of_samples_to_allocate);
			}
		}
	}
	print_message(INFO_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "allocate_network_izneuron_dynamics_single_pattern", "Created network_neuron_dynamics_single_pattern.");
	return network_neuron_dynamics_single_pattern;	
}
NetworkIzNeuronDynamicsSinglePattern* deallocate_network_izneuron_dynamics_single_pattern(Network *network, NetworkIzNeuronDynamicsSinglePattern* network_neuron_dynamics_single_pattern)
{
	unsigned int i, j, k;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	if (network == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_single_pattern", "network == NULL");	
		return NULL;
	}
	if (network_neuron_dynamics_single_pattern == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_single_pattern", "network_neuron_dynamics_single_pattern == NULL");	
		return NULL;
	}
	if (!get_num_of_layers_in_network(network, &num_of_layers))
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_single_pattern", "Couldn' t retrieve number of layers.");	
	for (i = 0; i < num_of_layers; i++)
	{
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
			return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_single_pattern", "Couldn' t retrieve number of neuron groups.");
		for (j = 0; j < num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
				return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_single_pattern", "Couldn' t retrieve number of neurons.");	
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				g_free(network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].v); 
				g_free(network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].u); 
				g_free(network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].e); 
				g_free(network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].i); 
			}
			g_free(network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j]);
		}
		g_free(network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i]);	
	}
	g_free(network_neuron_dynamics_single_pattern->neuron_dynamics_pattern);	
	g_free(network_neuron_dynamics_single_pattern->sampling_times);
	g_free(network_neuron_dynamics_single_pattern);
	print_message(INFO_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_single_pattern", "Destroyed network_neuron_dynamics_single_pattern.");
	return NULL;	
}

bool write_initial_neuron_dynamics_to_network_izneuron_dynamics_single_pattern(Network *network, NetworkIzNeuronDynamicsSinglePattern *neuron_dynamics_single_pattern, int layer, int neuron_group, int neuron_num, double v, double u, double e, double i)
{
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	if (network == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_izneuron_dynamics_to_network_izneuron_dynamics_single_pattern", "network == NULL.");	
	if (network_neuron_dynamics_single_pattern == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_izneuron_dynamics_to_network_izneuron_dynamics_single_pattern", "network_neuron_dynamics_single_pattern == NULL.");	
	if (network != network_neuron_dynamics_single_pattern->network)
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_izneuron_dynamics_to_network_izneuron_dynamics_single_pattern", "network != network_neuron_dynamics_single_pattern->network.");	
	if (!is_neuron(network, layer, neuron_group, neuron_num))
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_izneuron_dynamics_to_network_izneuron_dynamics_single_pattern", " ! is_neuron(network, layer, neuron_group, neuron_num).");	
	neuron_dynamics_single_pattern->neuron_dynamics_pattern[layer][neuron_group][neuron_num].initial_v = v;
	neuron_dynamics_single_pattern->neuron_dynamics_pattern[layer][neuron_group][neuron_num].initial_u = u;
	neuron_dynamics_single_pattern->neuron_dynamics_pattern[layer][neuron_group][neuron_num].initial_e = e;
	neuron_dynamics_single_pattern->neuron_dynamics_pattern[layer][neuron_group][neuron_num].initial_i = i;
	return TRUE;
}

bool add_izneuron_dynamics_to_network_izneuron_dynamics_single_pattern(Network *network, NetworkIzNeuronDynamicsSinglePattern *neuron_dynamics_single_pattern, int layer, int neuron_group, int neuron_num, TimeStamp sampling_time, double v, double u, double e, double i, bool first_sample) 
{
	unsigned int idx;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	if (network == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_izneuron_dynamics_to_network_izneuron_dynamics_single_pattern", "network == NULL.");	
	if (network_neuron_dynamics_single_pattern == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_izneuron_dynamics_to_network_izneuron_dynamics_single_pattern", "network_neuron_dynamics_single_pattern == NULL.");	
	if (network != network_neuron_dynamics_single_pattern->network)
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_izneuron_dynamics_to_network_izneuron_dynamics_single_pattern", "network != network_neuron_dynamics_single_pattern->network.");	
	if (!is_neuron(network, layer, neuron_group, neuron_num))
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_izneuron_dynamics_to_network_izneuron_dynamics_single_pattern", " ! is_neuron(network, layer, neuron_group, neuron_num).");	
	if (neuron_dynamics_single_pattern->write_idx == neuron_dynamics_single_pattern->allocated_num_of_samples)
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_izneuron_dynamics_to_network_izneuron_dynamics_single_pattern", " neuron_dynamics_single_pattern->write_idx == neuron_dynamics_single_pattern->allocated_num_of_samples.");			
	if (first_sample)
		neuron_dynamics_single_pattern->write_idx = 0;
	idx = network_neuron_dynamics_single_pattern->write_idx;

	neuron_dynamics_single_pattern->neuron_dynamics_pattern[layer][neuron_group][neuron_num].v[idx] = v;
	neuron_dynamics_single_pattern->neuron_dynamics_pattern[layer][neuron_group][neuron_num].u[idx] = u;
	neuron_dynamics_single_pattern->neuron_dynamics_pattern[layer][neuron_group][neuron_num].e[idx] = e;
	neuron_dynamics_single_pattern->neuron_dynamics_pattern[layer][neuron_group][neuron_num].i[idx] = i;

	if (!get_num_of_layers_in_network(network, &num_of_layers))
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_izneuron_dynamics_to_network_izneuron_dynamics_single_pattern", "Couldn' t retrieve number of layers.");		
	if(!get_num_of_neuron_groups_in_layer(network, num_of_layers-1, &num_of_neuron_groups_in_layer))
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_izneuron_dynamics_to_network_izneuron_dynamics_single_pattern", "Couldn' t retrieve number of neuron groups.");
	if (!get_num_of_neurons_in_neuron_group(network, num_of_layers-1, num_of_neuron_groups_in_layer-1, &num_of_neurons_in_neuron_group))
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "deallocate_network_izneuron_dynamics_single_pattern", "Couldn' t retrieve number of neurons.");	
	if (( layer == (num_of_layers-1)) && (neuron_group == (num_of_neuron_groups_in_layer -1)) && (neuron_num == (num_of_neurons_in_neuron_group - 1)) )	// last neuron in network is being handled		
	{
		network_neuron_dynamics_single_pattern->sampling_times[idx] = sampling_time;
		network_neuron_dynamics_single_pattern->write_idx++;	
	}
	return TRUE;
}

bool add_network_izneuron_dynamics_single_pattern_to_network_izneuron_dynamics_patterns(Network *network, NetworkIzNeuronDynamicsPatterns* network_neuron_dynamics_patterns, NetworkIzNeuronDynamicsSinglePattern* network_neuron_dynamics_single_pattern)
{
	unsigned int i,j,k, m;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	unsigned int num_of_samples;	
	NetworkIzNeuronDynamicsPattern curr_patt;
	if (network == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_network_izneuron_dynamics_single_pattern_to_network_izneuron_dynamics_patterns", "network == NULL.");		
	if (network_neuron_dynamics_patterns == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_network_izneuron_dynamics_single_pattern_to_network_izneuron_dynamics_patterns", "network_neuron_dynamics_patterns == NULL.");		
	if (network_neuron_dynamics_single_pattern == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_network_izneuron_dynamics_single_pattern_to_network_izneuron_dynamics_patterns", "network_neuron_dynamics_single_pattern == NULL.");		
	if (network != network_neuron_dynamics_patterns->network)
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_network_izneuron_dynamics_single_pattern_to_network_izneuron_dynamics_patterns", "network != network_neuron_dynamics_patterns->network.");		
	if (network != network_neuron_dynamics_single_pattern->network)
		return print_message(ERROR_MSG ,"NeuroSim", "IzNeuronDynamicsPatterns", "add_network_izneuron_dynamics_single_pattern_to_network_izneuron_dynamics_patterns", "network != network_neuron_dynamics_single_pattern->network.");		

	network_neuron_dynamics_patterns->tail->next = gnew_0(NetworkIzNeuronDynamicsPatterns, 1);
	network_neuron_dynamics_patterns->tail->next->prev = network_neuron_dynamics_patterns->tail;
	network_neuron_dynamics_patterns->tail = network_neuron_dynamics_patterns->tail->next;
	curr_patt = network_neuron_dynamics_patterns->tail;
	num_of_samples = network_neuron_dynamics_single_pattern->write_idx;

	if (!get_num_of_layers_in_network(network, &num_of_layers))
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_all_neurons_dynamics_single_pattern_to_all_neurons_dynamics_patterns", "Couldn' t retrieve number of layers.");	
	for (i = 0; i < num_of_layers; i++)
	{
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
			return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_all_neurons_dynamics_single_pattern_to_all_neurons_dynamics_patterns", "Couldn' t retrieve number of neuron groups.");

		for (j = 0; j < num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
				return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_all_neurons_dynamics_single_pattern_to_all_neurons_dynamics_patterns", "Couldn' t retrieve number of neurons.");	

			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				curr_patt->neuron_dynamics_pattern[i][j][k].initial_v = network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].initial_v;
				curr_patt->neuron_dynamics_pattern[i][j][k].initial_u = network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].initial_u;
				curr_patt->neuron_dynamics_pattern[i][j][k].initial_e = network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].initial_e;
				curr_patt->neuron_dynamics_pattern[i][j][k].initial_i = network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].initial_i;
				for (m = 0; m < num_of_samples; m++)
				{			
					curr_patt->neuron_dynamics_pattern[i][j][k].v[m] = network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].v[m];
					curr_patt->neuron_dynamics_pattern[i][j][k].u[m] = network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].u[m];
					curr_patt->neuron_dynamics_pattern[i][j][k].e[m] = network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].e[m];
					curr_patt->neuron_dynamics_pattern[i][j][k].i[m] = network_neuron_dynamics_single_pattern->neuron_dynamics_pattern[i][j][k].i[m];
				}
			}
		}
	}  
	curr_patt->sampling_times = g_new0(TimeStamp, num_of_samples);
	for (i = 0; i < num_of_samples; i++)
	{			
		curr_patt->sampling_times[i] = network_neuron_dynamics_single_pattern->sampling_times[i];
	}	
	curr_patt->num_of_samples = num_of_samples;
	return TRUE;
}





