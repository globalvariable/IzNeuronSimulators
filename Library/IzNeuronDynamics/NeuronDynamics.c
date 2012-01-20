#include "NeuronDynamics.h"


NeuronDynamics* allocate_neuron_dynamics(Network *network, PatternData *pattern_data, NeuronDynamics *neuron_dynamics)
{
	int i,j;
	int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	
	if (network == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_neuron_dynamics", "Network was not alocated.");		
	 	return NULL;
	}
	 
	if (neuron_dynamics == NULL)
	{
		neuron_dynamics = g_new0(NeuronDynamics, 1);
		if (neuron_dynamics == NULL)
		{
			print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_neuron_dynamics", "Couldn' t allocate neuron dynamics.");		
			return NULL;
		}
		print_message(INFO_MSG ,"NeuroSim", NULL, NULL, "Created Neuron_Dynamics.");	
	}
	else		
	{
		neuron_dynamics = deallocate_neuron_dynamics(network, pattern_data, neuron_dynamics);
		neuron_dynamics = allocate_neuron_dynamics(network, pattern_data, neuron_dynamics);
	}
	neuron_dynamics->network = network;
	if (!get_num_of_layers_in_network(network, &num_of_layers))
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_neuron_dynamics", "Couldn' t retrieve number of layers.");	
		return NULL;
	}	
		
	neuron_dynamics->patterns = g_new0(NeuronDynamicsPatterns**, num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
		{
			print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_neuron_dynamics", "Couldn' t retrieve number of neuron groups.");
			return NULL;
		}
		neuron_dynamics->patterns[i] = g_new0(NeuronDynamicsPatterns*, num_of_neuron_groups_in_layer);
		for (j = 0; j < num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
			{
				print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_neuron_dynamics", "Couldn' t retrieve number of neuron groups.");	
				return NULL;
			}			
			neuron_dynamics->patterns[i][j] = g_new0(NeuronDynamicsPatterns, num_of_neurons_in_neuron_group);
		}			
	}
	print_message(INFO_MSG ,"BlueSpike", NULL, NULL, "Allocated Neuron_Dynamics\n");		
	return neuron_dynamics;	
}

NeuronDynamics* deallocate_neuron_dynamics(Network *network, PatternData *pattern_data, NeuronDynamics *neuron_dynamics)
{
	int i,j,k, m;
	int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	
	if (network == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "deallocate_neuron_dynamics", "Network was not alocated.");		
		return NULL;
	}
	
	if (neuron_dynamics == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "deallocate_neuron_dynamics", "neuron_dynamics was not alocated.");		
		return NULL;
	}	
	
	if (network != neuron_dynamics->network)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "deallocate_neuron_dynamics", "Incorrect network pointer was given to destroy");		
		return NULL;
	}	
			
	if (!get_num_of_layers_in_network(network, &num_of_layers))
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "allocate_neuron_dynamics", "Couldn' t retrieve number of layers.");	
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
				for (m = 0; m < pattern_data->num_of_patterns; m++)
				{				
					g_free(neuron_dynamics->patterns[i][j][k].pattern[m].v);	
					g_free(neuron_dynamics->patterns[i][j][k].pattern[m].u);	
					g_free(neuron_dynamics->patterns[i][j][k].pattern[m].e);	
					g_free(neuron_dynamics->patterns[i][j][k].pattern[m].i);												
				}
				g_free(neuron_dynamics->patterns[i][j][k].pattern);
			}
			g_free(neuron_dynamics->patterns[i][j]);
		}
		g_free(neuron_dynamics->patterns[i]);
	}
	g_free(neuron_dynamics->patterns); 
	
	print_message(INFO_MSG ,"BlueSpike", NULL, NULL, "Destroyed Neuron_Dynamics\n");		
	return NULL;	
}

bool add_neuron_dynamics_to_pattern(Network *network, PatternData *pattern_data, NeuronDynamics *neuron_dynamics, int layer, int neuron_group, int neuron_num, int pattern_num, NeuronDynamicsPattern *dynamics_pattern)
{
/*	pattern num max a eşitse yeni allocation yap. 
	int i,j,k, m, n;
	int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	NeuronDynamicsPattern *lcl_pattern;
	int num_of_patterns;
	TimeStampMs pattern_length_ms;
	if (network == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "add_neuron_dynamics_pattern", "Network was not alocated.");		

	if (neuron_dynamics == NULL)
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "add_neuron_dynamics_pattern", "Target neuron_dynamics was not allocated.");	

	if (network != neuron_dynamics->network)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "deallocate_neuron_dynamics", "Incorrect network pointer was given to add neuron dynamics pattern");		

	if (neuron_dynamics_source == NULL)
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "add_neuron_dynamics_pattern", "Source neuron_dynamics was not allocated.");
						
	if (!get_num_of_layers_in_network(network, &num_of_layers))
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "add_neuron_dynamics_pattern", "Couldn' t retrieve number of layers.");	
		return FALSE;
	}
	if (neuron_dynamics->patterns == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "add_neuron_dynamics_pattern", "Couldn' t retrieve pointer for neuron_dynamics->patterns.");	
		return FALSE;					
	}	
	
	num_of_patterns = pattern_data->num_of_patterns;   // to be faster
	for (i = 0; i < num_of_layers; i++)
	{
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
		{
			print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "add_neuron_dynamics_pattern", "Couldn' t retrieve number of neuron groups.");
			return FALSE;
		}
		if (neuron_dynamics->patterns[i] == NULL)
		{
			print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "add_neuron_dynamics_pattern", "Couldn' t retrieve pointer for neuron_dynamics->patterns[i].");	
			return FALSE;					
		}			
		for (j = 0; j < num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
			{
				print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "add_neuron_dynamics_pattern", "Couldn' t retrieve number of neurons.");	
				return FALSE;
			}	
			if (neuron_dynamics->patterns[i][j] == NULL)
			{
				print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "add_neuron_dynamics_pattern", "Couldn' t retrieve pointer for neuron_dynamics->patterns[i][j].");	
				return FALSE;					
			}			
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				lcl_pattern = g_new0(NeuronDynamicsPattern, num_of_patterns+1);	
				lcl_pattern[num_of_patterns].v = g_new0(double, pattern_length_ms);
				lcl_pattern[num_of_patterns].u = g_new0(double, pattern_length_ms);
				lcl_pattern[num_of_patterns].e = g_new0(double, pattern_length_ms);
				lcl_pattern[num_of_patterns].i = g_new0(double, pattern_length_ms);						
				for (m = 0; m < num_of_patterns; m++)        // copy previous patterns. 
				{				
					lcl_pattern[m].v = neuron_dynamics->patterns[i][j][k].pattern[m].v;
					lcl_pattern[m].u = neuron_dynamics->patterns[i][j][k].pattern[m].u;
					lcl_pattern[m].e = neuron_dynamics->patterns[i][j][k].pattern[m].e;
					lcl_pattern[m].i = neuron_dynamics->patterns[i][j][k].pattern[m].i;	
					if (!get_pattern_length_ms(pattern_data, m, &pattern_length_ms))	
						return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamics", "add_neuron_dynamics_pattern", NULL);		
				}
				g_free(neuron_dynamics->patterns[i][j][k].pattern);
				neuron_dynamics->patterns[i][j][k].pattern = lcl_pattern;

				lcl_pattern[num_of_patterns].pattern_length_ms = pattern_length_ms;	
				for (n = 0; n < pattern_length_ms; n++)
				{				
					lcl_pattern[num_of_patterns].v[n] = neuron_dynamics_source->patterns[i][j][k].pattern[0].v[n];
					lcl_pattern[num_of_patterns].u[n] = neuron_dynamics->patterns[i][j][k].pattern[0].u[n];
					lcl_pattern[num_of_patterns].e[n] = neuron_dynamics->patterns[i][j][k].pattern[0].e[n];
					lcl_pattern[num_of_patterns].i[n] = neuron_dynamics->patterns[i][j][k].pattern[0].i[n];	
				}
				lcl_pattern[num_of_patterns].initial_v = neuron_dynamics->patterns[i][j][k].pattern[0].initial_v;
				lcl_pattern[num_of_patterns].initial_u = neuron_dynamics->patterns[i][j][k].pattern[0].initial_u;
				lcl_pattern[num_of_patterns].initial_e = neuron_dynamics->patterns[i][j][k].pattern[0].initial_e;
				lcl_pattern[num_of_patterns].initial_i = neuron_dynamics->patterns[i][j][k].pattern[0].initial_i;	

			}
			neuron_dynamics->num_of_patterns++;
		}
	}  
	
	
	
	
	
	 */
	return TRUE;
}


