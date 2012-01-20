#include "NeuronDynamicsPatterns.h"


AllNeuronsDynamicsPatterns* allocate_all_neurons_dynamics_patterns(Network *network, unsigned int num_of_patterns_to_allocate, unsigned int num_of_samples_to_allocate, AllNeuronsDynamicsPatterns *all_neurons_dynamics_patterns)
{
	int i,j, k, m;
	int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	
	if (network == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_neuron_dynamics", "Network was not alocated.");		
	 	return NULL;
	}
	 
	if (all_neurons_dynamics_patterns == NULL)
	{
		all_neurons_dynamics_patterns = g_new0(AllNeuronsDynamicsPatterns, 1);
		print_message(INFO_MSG ,"NeuroSim", NULL, NULL, "Created Neuron_Dynamics.");	
	}
	else		
	{
		all_neurons_dynamics_patterns = deallocate_all_neurons_dynamics_patterns(network, all_neurons_dynamics_patterns);
		all_neurons_dynamics_patterns = allocate_all_neurons_dynamics_patterns(network, num_of_patterns_to_allocate, num_of_samples_to_allocate, all_neurons_dynamics_patterns);
	}
	all_neurons_dynamics_patterns->network = network;
	if (!get_num_of_layers_in_network(network, &num_of_layers))
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_neuron_dynamics", "Couldn' t retrieve number of layers.");	
		return NULL;
	}	
		
	all_neurons_dynamics_patterns->network_patterns = g_new0(NeuronDynamicsPatterns**, num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
		{
			print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_neuron_dynamics", "Couldn' t retrieve number of neuron groups.");
			return NULL;
		}
		all_neurons_dynamics_patterns->network_patterns[i] = g_new0(NeuronDynamicsPatterns*, num_of_neuron_groups_in_layer);
		for (j = 0; j < num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
			{
				print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_neuron_dynamics", "Couldn' t retrieve number of neuron groups.");	
				return NULL;
			}			
			all_neurons_dynamics_patterns->network_patterns[i][j] = g_new0(NeuronDynamicsPatterns, num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				all_neurons_dynamics_patterns->network_patterns[i][j][k].patterns = g_new0(NeuronDynamicsPattern, num_of_patterns_to_allocate);		
				for (m = 0; m < num_of_patterns_to_allocate; m++)
				{				
					all_neurons_dynamics_patterns->network_patterns[i][j][k].patterns[m].v = g_new0(double, num_of_samples_to_allocate);	
					all_neurons_dynamics_patterns->network_patterns[i][j][k].patterns[m].u = g_new0(double, num_of_samples_to_allocate);	
					all_neurons_dynamics_patterns->network_patterns[i][j][k].patterns[m].e = g_new0(double, num_of_samples_to_allocate);	
					all_neurons_dynamics_patterns->network_patterns[i][j][k].patterns[m].i = g_new0(double, num_of_samples_to_allocate);																
				}

			}			
		}			
	}
	all_neurons_dynamics_patterns->sampling_times = g_new0(TimeStamp*, num_of_patterns_to_allocate);	
	for (i = 0; i < num_of_patterns_to_allocate; i++)
	{		
		all_neurons_dynamics_patterns->sampling_times[i] = g_new0(TimeStamp, num_of_samples_to_allocate);
	}	
	all_neurons_dynamics_patterns->num_of_used_samples = g_new0(unsigned int, num_of_patterns_to_allocate);
	all_neurons_dynamics_patterns->num_of_allocated_samples = num_of_samples_to_allocate;
	all_neurons_dynamics_patterns->num_of_allocated_patterns = num_of_patterns_to_allocate;		
	print_message(INFO_MSG ,"BlueSpike", NULL, NULL, "Allocated all_neurons_dynamics_patterns\n");		
	return all_neurons_dynamics_patterns;	
}

AllNeuronsDynamicsPatterns* deallocate_all_neurons_dynamics_patterns(Network *network, AllNeuronsDynamicsPatterns *all_neurons_dynamics_patterns)
{
	int i,j,k, m;
	int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	
	if (network == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "deallocate_all_neurons_dynamics_patterns", "Network was not alocated.");		
		return NULL;
	}
	
	if (all_neurons_dynamics_patterns == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "deallocate_all_neurons_dynamics_patterns", "neuron_dynamics was not alocated.");		
		return NULL;
	}	
	
	if (network != all_neurons_dynamics_patterns->network)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "deallocate_all_neurons_dynamics_patterns", "Incorrect network pointer was given to destroy");		
		return NULL;
	}	
			
	if (!get_num_of_layers_in_network(network, &num_of_layers))
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "deallocate_all_neurons_dynamics_patterns", "Couldn' t retrieve number of layers.");	
		return NULL;
	}
	for (i = 0; i < num_of_layers; i++)
	{
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
		{
			print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_neuron_dynamics", "Couldn' t retrieve number of neuron groups.");
			return NULL;
		}
		for (j = 0; j < num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
			{
				print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "add_neuron_dynamics_pattern", "Couldn' t retrieve number of neurons.");	
				return FALSE;
			}		
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				for (m = 0; m < all_neurons_dynamics_patterns->num_of_allocated_patterns; m++)
				{				
					g_free(all_neurons_dynamics_patterns->network_patterns[i][j][k].patterns[m].v);	
					g_free(all_neurons_dynamics_patterns->network_patterns[i][j][k].patterns[m].u);	
					g_free(all_neurons_dynamics_patterns->network_patterns[i][j][k].patterns[m].e);	
					g_free(all_neurons_dynamics_patterns->network_patterns[i][j][k].patterns[m].i);												
				}
				g_free(all_neurons_dynamics_patterns->network_patterns[i][j][k].patterns);
			}
			g_free(all_neurons_dynamics_patterns->network_patterns[i][j]);
		}
		g_free(all_neurons_dynamics_patterns->network_patterns[i]);
	}
	g_free(all_neurons_dynamics_patterns->network_patterns); 
	for (m = 0; m < all_neurons_dynamics_patterns->num_of_allocated_patterns; m++)
	{				
		g_free(all_neurons_dynamics_patterns->sampling_times[m]);	
	}
	g_free(all_neurons_dynamics_patterns->sampling_times); 	
	g_free(all_neurons_dynamics_patterns->num_of_used_samples); 	
	g_free(all_neurons_dynamics_patterns); 	
	print_message(INFO_MSG ,"BlueSpike", NULL, NULL, "Destroyed all_neurons_dynamics_patterns\n");		
	return NULL;	
}

AllNeuronsDynamicsSinglePattern* allocate_all_neurons_dynamics_single_pattern(Network *network, unsigned int num_of_samples_to_allocate, AllNeuronsDynamicsSinglePattern *all_neurons_dynamics_single_pattern)
{
	int i,j, k;
	int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	
	if (network == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_all_neurons_dynamics_single_pattern", "Network was not alocated.");		
	 	return NULL;
	}
	 
	if (all_neurons_dynamics_single_pattern == NULL)
	{
		all_neurons_dynamics_single_pattern = g_new0(AllNeuronsDynamicsSinglePattern, 1);
		print_message(INFO_MSG ,"NeuroSim", NULL, NULL, "Created all_neurons_dynamics_single_pattern.");	
	}
	else		
	{
		all_neurons_dynamics_single_pattern = deallocate_all_neurons_dynamics_single_pattern(network, all_neurons_dynamics_single_pattern);
		all_neurons_dynamics_single_pattern = allocate_all_neurons_dynamics_single_pattern(network, num_of_samples_to_allocate, all_neurons_dynamics_single_pattern);
	}
	all_neurons_dynamics_single_pattern->network = network;
	if (!get_num_of_layers_in_network(network, &num_of_layers))
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_all_neurons_dynamics_single_pattern", "Couldn' t retrieve number of layers.");	
		return NULL;
	}	
		
	all_neurons_dynamics_single_pattern->network_pattern = g_new0(NeuronDynamicsPattern**, num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
		{
			print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_all_neurons_dynamics_single_pattern", "Couldn' t retrieve number of neuron groups.");
			return NULL;
		}
		all_neurons_dynamics_single_pattern->network_pattern[i] = g_new0(NeuronDynamicsPattern*, num_of_neuron_groups_in_layer);
		for (j = 0; j < num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
			{
				print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_all_neurons_dynamics_single_pattern", "Couldn' t retrieve number of neuron groups.");	
				return NULL;
			}			
			all_neurons_dynamics_single_pattern->network_pattern[i][j] = g_new0(NeuronDynamicsPattern, num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				all_neurons_dynamics_single_pattern->network_pattern[i][j][k].v = g_new0(double, num_of_samples_to_allocate);
				all_neurons_dynamics_single_pattern->network_pattern[i][j][k].u = g_new0(double, num_of_samples_to_allocate);
				all_neurons_dynamics_single_pattern->network_pattern[i][j][k].e = g_new0(double, num_of_samples_to_allocate);
				all_neurons_dynamics_single_pattern->network_pattern[i][j][k].i = g_new0(double, num_of_samples_to_allocate);				
			}			
		}			
	}
	all_neurons_dynamics_single_pattern->sampling_times =g_new0(TimeStamp, num_of_samples_to_allocate);
	all_neurons_dynamics_single_pattern->num_of_allocated_samples = num_of_samples_to_allocate;

	print_message(INFO_MSG ,"BlueSpike", NULL, NULL, "Allocated all_neurons_dynamics_single_pattern\n");		
	return all_neurons_dynamics_single_pattern;	
}

AllNeuronsDynamicsSinglePattern* deallocate_all_neurons_dynamics_single_pattern(Network *network, AllNeuronsDynamicsSinglePattern *all_neurons_dynamics_single_pattern)
{
	int i,j,k;
	int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	
	if (network == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "deallocate_all_neurons_dynamics_single_pattern", "Network was not alocated.");		
		return NULL;
	}
	
	if (all_neurons_dynamics_single_pattern == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "deallocate_all_neurons_dynamics_single_pattern", "neuron_dynamics was not alocated.");		
		return NULL;
	}	
	
	if (network != all_neurons_dynamics_single_pattern->network)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "deallocate_all_neurons_dynamics_single_pattern", "Incorrect network pointer was given to destroy");		
		return NULL;
	}	
			
	if (!get_num_of_layers_in_network(network, &num_of_layers))
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "deallocate_all_neurons_dynamics_single_pattern", "Couldn' t retrieve number of layers.");	
		return NULL;
	}
	for (i = 0; i < num_of_layers; i++)
	{
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
		{
			print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_neuron_dynamics", "Couldn' t retrieve number of neuron groups.");
			return NULL;
		}
		for (j = 0; j < num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
			{
				print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "add_neuron_dynamics_pattern", "Couldn' t retrieve number of neurons.");	
				return FALSE;
			}		
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				g_free(all_neurons_dynamics_single_pattern->network_pattern[i][j][k].v);	
				g_free(all_neurons_dynamics_single_pattern->network_pattern[i][j][k].u);	
				g_free(all_neurons_dynamics_single_pattern->network_pattern[i][j][k].e);	
				g_free(all_neurons_dynamics_single_pattern->network_pattern[i][j][k].i);																				
			}
			g_free(all_neurons_dynamics_single_pattern->network_pattern[i][j]);
		}
		g_free(all_neurons_dynamics_single_pattern->network_pattern[i]);
	}
	g_free(all_neurons_dynamics_single_pattern->network_pattern); 
	g_free(all_neurons_dynamics_single_pattern->sampling_times);	
	g_free(all_neurons_dynamics_single_pattern); 	

	print_message(INFO_MSG ,"BlueSpike", NULL, NULL, "Destroyed all_neurons_dynamics_single_pattern.");		
	return NULL;	
}


bool write_all_neurons_dynamics_single_pattern_to_all_neurons_dynamics_patterns(Network *network, AllNeuronsDynamicsSinglePattern *all_neurons_dynamics_single_pattern, AllNeuronsDynamicsPatterns *all_neurons_dynamics_patterns)
{
	int i,j,k, m;
	int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	unsigned int num_of_patterns;

	if (network == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_all_neurons_dynamics_single_pattern_to_all_neurons_dynamics_patterns", "Network was not alocated.");		

	if (all_neurons_dynamics_single_pattern == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_all_neurons_dynamics_single_pattern_to_all_neurons_dynamics_patterns", "all_neurons_dynamics_single_pattern was not allocated.");	

	if (all_neurons_dynamics_patterns == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_all_neurons_dynamics_single_pattern_to_all_neurons_dynamics_patterns", "all_neurons_dynamics_patterns was not allocated.");	

	if (network != all_neurons_dynamics_single_pattern->network)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_all_neurons_dynamics_single_pattern_to_all_neurons_dynamics_patterns", "Incorrect network pointer was given for all_neurons_dynamics_single_pattern");		

	if (network != all_neurons_dynamics_patterns->network)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_all_neurons_dynamics_single_pattern_to_all_neurons_dynamics_patterns", "Incorrect network pointer was given for all_neurons_dynamics_patterns");	

	if (all_neurons_dynamics_single_pattern->num_of_allocated_samples != all_neurons_dynamics_patterns->num_of_allocated_samples)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_all_neurons_dynamics_single_pattern_to_all_neurons_dynamics_patterns", "all_neurons_dynamics_single_pattern->num_of_allocated_samples != all_neurons_dynamics_patterns->num_of_allocated_samples");			
	
	num_of_patterns = all_neurons_dynamics_patterns->num_of_allocated_patterns;   
		
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
				for (m = 0; m < all_neurons_dynamics_single_pattern->num_of_used_samples; m++)
				{			
					all_neurons_dynamics_patterns->network_patterns[i][j][k].patterns[all_neurons_dynamics_patterns->num_of_used_patterns].v[m] = all_neurons_dynamics_single_pattern->network_pattern[i][j][k].v[m];
					all_neurons_dynamics_patterns->network_patterns[i][j][k].patterns[all_neurons_dynamics_patterns->num_of_used_patterns].u[m] = all_neurons_dynamics_single_pattern->network_pattern[i][j][k].u[m];
					all_neurons_dynamics_patterns->network_patterns[i][j][k].patterns[all_neurons_dynamics_patterns->num_of_used_patterns].e[m] = all_neurons_dynamics_single_pattern->network_pattern[i][j][k].e[m];
					all_neurons_dynamics_patterns->network_patterns[i][j][k].patterns[all_neurons_dynamics_patterns->num_of_used_patterns].i[m] = all_neurons_dynamics_single_pattern->network_pattern[i][j][k].i[m];
				}
			}
		}
	}  
	
	for (i = 0; i < all_neurons_dynamics_single_pattern->num_of_used_samples; i++)
	{			
		all_neurons_dynamics_patterns->sampling_times[all_neurons_dynamics_patterns->num_of_used_patterns][i] = all_neurons_dynamics_single_pattern->sampling_times[i];
	}	
	all_neurons_dynamics_patterns->num_of_used_samples[all_neurons_dynamics_patterns->num_of_used_patterns] = all_neurons_dynamics_single_pattern->num_of_used_samples;
	all_neurons_dynamics_patterns->num_of_used_patterns++;
	if (all_neurons_dynamics_patterns->num_of_used_patterns == all_neurons_dynamics_patterns->num_of_allocated_patterns)
		return print_message(WARNING_MSG ,"NeuroSim", "NeuronDynamics", "write_all_neurons_dynamics_single_pattern_to_all_neurons_dynamics_patterns", "all_neurons_dynamics_patterns->num_of_used_patterns == all_neurons_dynamics_patterns->num_of_allocated_patterns");
		 
	return TRUE;
}

bool reset_all_neurons_dynamics_single_pattern_write_idx(Network *network, AllNeuronsDynamicsSinglePattern *all_neurons_dynamics_single_pattern)
{
	if (network == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "reset_all_neurons_dynamics_single_pattern_write_idx", "Network was not alocated.");		

	if (all_neurons_dynamics_single_pattern == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "reset_all_neurons_dynamics_single_pattern_write_idx", "all_neurons_dynamics_single_pattern was not allocated.");	

	if (network != all_neurons_dynamics_single_pattern->network)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "reset_all_neurons_dynamics_single_pattern_write_idx", "Incorrect network pointer was given for all_neurons_dynamics_single_pattern");		
		
	all_neurons_dynamics_single_pattern->num_of_used_samples = 0;
	
	return TRUE;	
}

bool write_neuron_dynamics_to_all_neurons_dynamics_single_pattern(Network *network, AllNeuronsDynamicsSinglePattern *all_neurons_dynamics_single_pattern, int layer, int neuron_group, int neuron_num, TimeStamp sampling_time, double v, double u, double e, double i)
{
	int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	unsigned int idx;

	if (network == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "rwrite_neuron_dynamics_to_all_neurons_dynamics_single_pattern", "Network was not alocated.");		

	if (all_neurons_dynamics_single_pattern == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_neuron_dynamics_to_all_neurons_dynamics_single_pattern", "all_neurons_dynamics_single_pattern was not allocated.");	

	if (network != all_neurons_dynamics_single_pattern->network)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_neuron_dynamics_to_all_neurons_dynamics_single_pattern", "Incorrect network pointer was given for all_neurons_dynamics_single_pattern");		
	

	if (!get_num_of_layers_in_network(network, &num_of_layers))
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_neuron_dynamics_to_all_neurons_dynamics_single_pattern", "Couldn' t retrieve number of layers.");	

	if (num_of_layers <= layer)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_neuron_dynamics_to_all_neurons_dynamics_single_pattern", "Invalid layer number.");	
				
	if(!get_num_of_neuron_groups_in_layer(network, layer, &num_of_neuron_groups_in_layer))
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_neuron_dynamics_to_all_neurons_dynamics_single_pattern", "Couldn' t retrieve number of neuron groups.");

	if (num_of_neuron_groups_in_layer <= neuron_group)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_neuron_dynamics_to_all_neurons_dynamics_single_pattern", "Invalid neuron group number.");		

	if (!get_num_of_neurons_in_neuron_group(network, layer, neuron_group, &num_of_neurons_in_neuron_group))
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_neuron_dynamics_to_all_neurons_dynamics_single_patterns", "Couldn' t retrieve number of neurons.");	

	if (num_of_neurons_in_neuron_group <= neuron_num)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_neuron_dynamics_to_all_neurons_dynamics_single_pattern", "Invalid neuron number..");		
		
	idx = all_neurons_dynamics_single_pattern->num_of_used_samples;
	
	all_neurons_dynamics_single_pattern->network_pattern[layer][neuron_group][neuron_num].v[idx] = v;
	all_neurons_dynamics_single_pattern->network_pattern[layer][neuron_group][neuron_num].v[idx] = u;
	all_neurons_dynamics_single_pattern->network_pattern[layer][neuron_group][neuron_num].v[idx] = e;
	all_neurons_dynamics_single_pattern->network_pattern[layer][neuron_group][neuron_num].v[idx] = i;
	
	if(!get_num_of_neuron_groups_in_layer(network, num_of_layers-1, &num_of_neuron_groups_in_layer))
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_neuron_dynamics_to_all_neurons_dynamics_single_pattern", "Couldn' t retrieve number of neuron groups.");
		
	if (!get_num_of_neurons_in_neuron_group(network, num_of_layers-1, num_of_neuron_groups_in_layer -1 , &num_of_neurons_in_neuron_group))
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_neuron_dynamics_to_all_neurons_dynamics_single_patterns", "Couldn' t retrieve number of neurons.");		
	
	if (( layer == (num_of_layers-1)) && (neuron_group == (num_of_neuron_groups_in_layer -1)) && (neuron_num == (num_of_neurons_in_neuron_group - 1)) )	// last neuron in network is being handled		
	{
		all_neurons_dynamics_single_pattern->sampling_times[idx] = sampling_time;
		all_neurons_dynamics_single_pattern->num_of_used_samples++;	
		if (all_neurons_dynamics_single_pattern->num_of_used_samples == all_neurons_dynamics_single_pattern->num_of_allocated_samples)
			return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "write_neuron_dynamics_to_all_neurons_dynamics_single_patterns", "all_neurons_dynamics_single_pattern->num_of_used_samples == all_neurons_dynamics_single_pattern->num_of_allocated_samples.");		
	}
	return TRUE;	
}
