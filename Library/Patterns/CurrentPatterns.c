#include "CurrentPatterns.h"



CurrentPatterns* allocate_current_patterns(Network *network, TrialStats *trial_stats, CurrentPatterns* current_patterns)
{
	if (network == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "allocate_current_patterns", "network == NULL");	
		return NULL;
	}
	if (trial_stats == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "allocate_current_patterns", "trial_stats == NULL");	
		return NULL;
	}
	if (current_patterns != NULL)
		current_patterns = deallocate_current_patterns(network, trial_stats, current_patterns);
		current_patterns = allocate_current_patterns(network, trial_stats, current_patterns, num_of_template_types);
		return current_patterns;
	}	
	current_patterns = g_new0(CurrentPatterns, 1);
	current_patterns->head = g_new0(CurrentPattern, 1);
	current_patterns->tail = current_patterns->head;
	print_message(INFO_MSG ,"NeuroSim", "CurrentPatterns", "allocate_current_patterns", "Created current_patterns.");
	return current_patterns;	
}

CurrentPatterns* deallocate_current_patterns(Network *network, TrialStats *trial_stats, CurrentPatterns* current_patterns)
{
	CurrentPattern *curr_current_pattern, *prev_current_pattern;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	unsigned int i, j, k, m;
	if (network == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "network == NULL");	
		return NULL;
	}
	if (trial_stats == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "trial_stats == NULL");	
		return NULL;
	}
	if (current_patterns == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "current_patterns == NULL");	
		return NULL;
	}
	curr_current_pattern = current_patterns->tail;
	for (i = 0; i < trial_stats->num_of_trials; i++)
	{
		prev_current_pattern = curr_current_pattern->prev;
		if (!get_num_of_layers_in_network(network, &num_of_layers))
			return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "Couldn' t retrieve number of layers.");			
		for (j=0; j < num_of_layers; j++)
		{
			if(!get_num_of_neuron_groups_in_layer(network, j, &num_of_neuron_groups_in_layer))
				return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "Couldn' t retrieve number of neuron groups.");
			for (k=0; k<num_of_neuron_groups_in_layer; k++)
			{
				if (!get_num_of_neurons_in_neuron_group(network, j, k, &num_of_neurons_in_neuron_group))
					return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "Couldn' t retrieve number of neurons.");	
				for (m = 0 ; m < num_of_neurons_in_neuron_group; m++)
				{
					g_free(curr_current_pattern->neuron_current_pattern[j][k][m].current);
				}
				g_free(curr_current_pattern->neuron_current_pattern[j][k]);
			}
			g_free(curr_current_pattern->neuron_current_pattern[j]);		
		}
		g_free(curr_current_pattern->neuron_current_pattern);		
		g_free(curr_current_pattern->sampling_times);
		curr_current_pattern = prev_current_pattern;
	}	
	if (curr_current_pattern != current_patterns->head)
	{
		print_message(BUG_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "curr_current_pattern != current_patterns->head");
	}
	g_free(current_patterns->head);
// deallocate templates
	for (i = 0; i < current_patterns->num_of_template_types; i++)
	{
		if (!get_num_of_layers_in_network(network, &num_of_layers))
			return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "Couldn' t retrieve number of layers.");			
		for (j=0; j < num_of_layers; j++)
		{
			if(!get_num_of_neuron_groups_in_layer(network, j, &num_of_neuron_groups_in_layer))
				return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "Couldn' t retrieve number of neuron groups.");
			for (k=0; k<num_of_neuron_groups_in_layer; k++)
			{
				if (!get_num_of_neurons_in_neuron_group(network, j, k, &num_of_neurons_in_neuron_group))
					return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "Couldn' t retrieve number of neurons.");	
				for (m = 0 ; m < num_of_neurons_in_neuron_group; m++)
				{
					g_free(current_patterns->current_pattern_templates[i].current_templates.neuron_current_templates[j][k][m].current);		
					g_free(current_patterns->current_pattern_templates[i].current_templates.neuron_current_templates[j][k][m]);		 /// Compiler should give error for here.		
				}
				g_free(current_patterns->current_pattern_templates[i].init_currents.init_current_neuron[j][k]);
				g_free(current_patterns->current_pattern_templates[i].current_templates.neuron_current_templates[j][k]);				
				g_free(current_patterns->current_pattern_templates[i].noise_params.neuron_noise_params[j][k]);								
			}
			g_free(current_patterns->current_pattern_templates[i].init_currents.init_current_neuron[j]);
			g_free(current_patterns->current_pattern_templates[i].current_templates.neuron_current_templates[j]);				
			g_free(current_patterns->current_pattern_templates[i].noise_params.neuron_noise_params[j]);			
		}
		g_free(current_patterns->current_pattern_templates[i].init_currents.init_current_neuron);
		g_free(current_patterns->current_pattern_templates[i].current_templates.neuron_current_templates);				
		g_free(current_patterns->current_pattern_templates[i].noise_params.neuron_noise_params);			
	}	
	g_free(current_patterns->current_pattern_templates);
	g_free(current_patterns);
	print_message(INFO_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "Destroyed current_patterns.");
	retun NULL;	
}

bool add_trial_type_to_current_pattern_templates(CurrentPatterns* current_patterns, TrialType pattern_type, TimeStamp sampling_interval)
{
	unsigned int i;
	CurrentPatternTemplate	*lcl_current_pattern_templates;
	CurrentPatternTemplate	*ptr_current_pattern_template;	
	if (current_patterns == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "add_trial_type_to_current_pattern_templates", "current_patterns == NULL");	

	lcl_current_pattern_templates = g_new0(CurrentPatternTemplate, current_patterns->num_of_template_types+1);
	for (i = 0; i < current_patterns->num_of_template_types; i++)
	{
		lcl_current_pattern_templates[i] = current_patterns->current_pattern_templates[i];
	}
	g_free(current_patterns->current_pattern_templates);
	current_patterns->current_pattern_templates = lcl_current_pattern_templates;
	current_patterns->num_of_template_types++;
	ptr_current_pattern_template = &(current_patterns->current_pattern_templates[current_patterns->num_of_template_types - 1]);
	(*ptr_current_pattern_template).template_type = pattern_type;
	(*ptr_current_pattern_template).current_templates.sampling_interval = sampling_interval;
	(*ptr_current_pattern_template).num_of_samples = current_patterns->trial_stats->max_trial_length_assigned / sampling_interval;

	if (!get_num_of_layers_in_network(network, &num_of_layers))
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "add_trial_type_to_current_pattern_templates", "Couldn' t retrieve number of layers.");	
	(*ptr_current_pattern_template).init_currents.init_current_neuron = g_new0(InitializationCurrentNeuron**,num_of_layers);
	(*ptr_current_pattern_template).current_templates.neuron_current_templates = g_new0(NeuronCurrentTemplate**,num_of_layers);
	(*ptr_current_pattern_template).noise_params.neuron_noise_params = g_new0(NeuronCurrentNoiseParams**,num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{
		if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
			return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "add_trial_type_to_current_pattern_templates", "Couldn' t retrieve number of neuron groups.");
		(*ptr_current_pattern_template).init_currents.init_current_neuron[i] = g_new0(InitializationCurrentNeuron*,num_of_neuron_groups_in_layer);
		(*ptr_current_pattern_template).current_templates.neuron_current_templates[i] = g_new0(NeuronCurrentTemplate*,num_of_neuron_groups_in_layer);
		(*ptr_current_pattern_template).noise_params.neuron_noise_params[i] = g_new0(NeuronCurrentNoiseParams*,num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
				return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "add_trial_type_to_current_pattern_templates", "Couldn' t retrieve number of neurons.");	
			(*ptr_current_pattern_template).init_currents.init_current_neuron[i][j] = g_new0(InitializationCurrentNeuron, num_of_neurons_in_neuron_group);
			(*ptr_current_pattern_template).current_templates.neuron_current_templates[i][j] = g_new0(NeuronCurrentTemplate, num_of_neurons_in_neuron_group);
			(*ptr_current_pattern_template).noise_params.neuron_noise_params[i][j] = g_new0(NeuronCurrentNoiseParams, num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				(*ptr_current_pattern_template).current_templates.neuron_current_templates[i][j][k].current = g_new0(double, (*ptr_current_pattern_template).num_of_samples);
			}
		}
	}
	return TRUE;
}

bool submit_initialization_current(CurrentPatterns* current_patterns, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, TimeStamp duration, double max_current)
{
	if (current_patterns == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_initialization_current", "current_patterns == NULL");	
	if (!is_neuron(current_patterns->network, layer, neuron_group, neuron_num))
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_initialization_current", "! is_neuron()");
	if (pattern_type_num >= current_patterns->num_of_template_types)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_initialization_current", "pattern_type_num >= current_patterns->num_of_template_types");
	current_patterns->current_pattern_templates[pattern_type_num].init_currents.init_current_neuron[layer][neuron_group][neuron_num].max_current = max_current;
	current_patterns->current_pattern_templates[pattern_type_num].init_currents.init_current_neuron[layer][neuron_group][neuron_num].max_current = duration;
	return TRUE;
}

bool submit_noise_params(CurrentPatterns* current_patterns, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, TimeStamp	noise_addition_interval, double noise_variance)
{
	if (current_patterns == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_initialization_current", "current_patterns == NULL");	
	if (!is_neuron(current_patterns->network, layer, neuron_group, neuron_num))
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_initialization_current", "! is_neuron()");
	if (pattern_type_num >= trial_stats->num_of_trial_types)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_initialization_current", "pattern_type_num >= trial_stats->num_of_trial_types");
	current_patterns->current_pattern_templates[pattern_type_num].noise_params.neuron_noise_params[layer][neuron_group][neuron_num].noise_addition_interval = noise_addition_interval;
	current_patterns->current_pattern_templates[pattern_type_num].noise_params.neuron_noise_params[layer][neuron_group][neuron_num].noise_variance = noise_variance;
	return TRUE;
}

bool submit_current_template_sample(CurrentPatterns* current_patterns, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, TimeStamp sampling_time, double current)
{
	TimeStamp sampling_interval; 
	if (current_patterns == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_initialization_current", "current_patterns == NULL");	
	if (!is_neuron(current_patterns->network, layer, neuron_group, neuron_num))
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_initialization_current", "! is_neuron()");
	if (sampling_time >= trial_stats->trial_type_stats[pattern_type_num].max_trial_type_length_assigned)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_initialization_current", "sampling_time >= trial_stats->trial_type_stats[pattern_type_num].max_trial_type_length_assigned");
	sampling_interval = current_patterns->current_pattern_templates[pattern_type_num].current_templates.neuron_current_templates[layer][neuron_group][neuron_num].sampling_interval;
	current_patterns->current_pattern_templates[pattern_type_num].current_templates.neuron_current_templates[layer][neuron_group][neuron_num].current[sampling_time/sampling_interval] =  current;
	return TRUE;
}

