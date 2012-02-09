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
	{
		current_patterns = deallocate_current_patterns(network, trial_stats, current_patterns);
		current_patterns = allocate_current_patterns(network, trial_stats, current_patterns);
		return current_patterns;
	}	
	current_patterns = g_new0(CurrentPatterns, 1);
	current_patterns->head = g_new0(CurrentPattern, 1);
	current_patterns->tail = current_patterns->head;
	current_patterns->network = network;
	current_patterns->trial_stats = trial_stats;
	print_message(INFO_MSG ,"NeuroSim", "CurrentPatterns", "allocate_current_patterns", "Created current_patterns.");
	return current_patterns;	
}

CurrentPatterns* deallocate_current_patterns(Network *network, TrialStats *trial_stats, CurrentPatterns* current_patterns)
{
	CurrentPattern *curr_current_pattern, *prev_current_pattern;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	unsigned int i, j, k, m;
	if (network == NULL)
		return (CurrentPatterns*)print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "network == NULL");	
	if (trial_stats == NULL)
		return (CurrentPatterns*)print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "trial_stats == NULL");	
	if (current_patterns == NULL)
		return (CurrentPatterns*)print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "current_patterns == NULL");	
	
	curr_current_pattern = current_patterns->tail;
	for (i = 0; i < trial_stats->total_num_of_trials; i++)
	{
		prev_current_pattern = curr_current_pattern->prev;
		if (!get_num_of_layers_in_network(network, &num_of_layers))
			return (CurrentPatterns*)print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "Couldn' t retrieve number of layers.");			
		for (j=0; j < num_of_layers; j++)
		{
			if(!get_num_of_neuron_groups_in_layer(network, j, &num_of_neuron_groups_in_layer))
				return (CurrentPatterns*)print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "Couldn' t retrieve number of neuron groups.");
			for (k=0; k<num_of_neuron_groups_in_layer; k++)
			{
				if (!get_num_of_neurons_in_neuron_group(network, j, k, &num_of_neurons_in_neuron_group))
					return (CurrentPatterns*)print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "Couldn' t retrieve number of neurons.");	
				for (m = 0 ; m < num_of_neurons_in_neuron_group; m++)
				{
					g_free(curr_current_pattern->neuron_current_pattern[j][k][m].pattern);
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
		print_message(BUG_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "curr_current_pattern != current_patterns->head");
	g_free(current_patterns->head);
// deallocate templates
	for (i = 0; i < trial_stats->num_of_trial_types; i++)
	{
		if (!get_num_of_layers_in_network(network, &num_of_layers))
			return (CurrentPatterns*)print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "Couldn' t retrieve number of layers.");			
		for (j=0; j < num_of_layers; j++)
		{
			if(!get_num_of_neuron_groups_in_layer(network, j, &num_of_neuron_groups_in_layer))
				return (CurrentPatterns*)print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "Couldn' t retrieve number of neuron groups.");
			for (k=0; k<num_of_neuron_groups_in_layer; k++)
			{
				if (!get_num_of_neurons_in_neuron_group(network, j, k, &num_of_neurons_in_neuron_group))
					return (CurrentPatterns*)print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "Couldn' t retrieve number of neurons.");	
				for (m = 0 ; m < num_of_neurons_in_neuron_group; m++)
				{
					g_free(current_patterns->current_pattern_templates[i].neuron_current_templates[j][k][m].pattern_template.pattern);		
					g_free(current_patterns->buffer[i].neuron_current_pattern[j][k][m].pattern);		
				}
				g_free(current_patterns->current_pattern_templates[i].neuron_current_templates[j][k]);
				g_free(current_patterns->buffer[i].neuron_current_pattern[j][k]);		
			}
			g_free(current_patterns->current_pattern_templates[i].neuron_current_templates[j]);
			g_free(current_patterns->buffer[i].neuron_current_pattern[j]);		
		}
		g_free(current_patterns->current_pattern_templates[i].neuron_current_templates);
		g_free(current_patterns->current_pattern_templates[i].sampling_times);
		g_free(current_patterns->buffer[i].neuron_current_pattern);		
		g_free(current_patterns->buffer[i].sampling_times);		
	}	
	g_free(current_patterns->current_pattern_templates);
	g_free(current_patterns->buffer);		
	g_free(current_patterns);
	print_message(INFO_MSG ,"NeuroSim", "CurrentPatterns", "deallocate_current_patterns", "Destroyed current_patterns.");
	return NULL;	
}

bool increment_current_template_types_in_current_pattern_templates(CurrentPatterns* current_patterns, TimeStamp sampling_interval)
{
	unsigned int i,j,k;
	unsigned int num_of_samples;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	CurrentPatternTemplate	*lcl_current_pattern_templates;
	CurrentPatternBuffer		*lcl_current_pattern_buffer;

	if (current_patterns == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "increment_current_template_types_in_current_pattern_templates", "current_patterns == NULL.");	
	if (current_patterns->trial_stats == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "increment_current_template_types_in_current_pattern_templates", "current_patterns->trial_stats == NULL.");	
	if (current_patterns->network == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "increment_current_template_types_in_current_pattern_templates", "current_patterns->network == NULL.");		
	lcl_current_pattern_templates = g_new0(CurrentPatternTemplate, current_patterns->trial_stats->num_of_trial_types);
	lcl_current_pattern_buffer = g_new0(CurrentPatternBuffer, current_patterns->trial_stats->num_of_trial_types);
	for (i = 0; i < current_patterns->trial_stats->num_of_trial_types-1; i++)
	{
		lcl_current_pattern_templates[i] = current_patterns->current_pattern_templates[i];
		lcl_current_pattern_buffer[i] = current_patterns->buffer[i];
	}
	g_free(current_patterns->current_pattern_templates);
	g_free(current_patterns->buffer);
	current_patterns->current_pattern_templates = lcl_current_pattern_templates;
	current_patterns->buffer = lcl_current_pattern_buffer;
	lcl_current_pattern_templates[current_patterns->trial_stats->num_of_trial_types-1].sampling_interval = sampling_interval;
	num_of_samples = current_patterns->trial_stats->trial_type_stats[current_patterns->trial_stats->num_of_trial_types-1].max_trial_type_length_assigned / sampling_interval;
	lcl_current_pattern_templates[current_patterns->trial_stats->num_of_trial_types-1].num_of_samples = num_of_samples;
	lcl_current_pattern_templates[current_patterns->trial_stats->num_of_trial_types-1].sampling_times = g_new0(TimeStamp, num_of_samples);
	lcl_current_pattern_buffer[current_patterns->trial_stats->num_of_trial_types-1].sampling_times = g_new0(TimeStamp, num_of_samples);	
	for (i = 0; i < num_of_samples; i++)
	{	
		lcl_current_pattern_templates[current_patterns->trial_stats->num_of_trial_types-1].sampling_times[i] = i * sampling_interval;
	}
	if (!get_num_of_layers_in_network(current_patterns->network, &num_of_layers))
		return (CurrentPatterns*)print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "increment_current_template_types_in_current_pattern_templates", "Couldn' t retrieve number of layers.");	
	lcl_current_pattern_templates[current_patterns->trial_stats->num_of_trial_types-1].neuron_current_templates = g_new0(NeuronCurrentTemplateData**, num_of_layers);
	lcl_current_pattern_buffer[current_patterns->trial_stats->num_of_trial_types-1].neuron_current_pattern = g_new0(NeuronCurrent**, num_of_layers);
	for (i=0; i < num_of_layers; i++)
	{
		if(!get_num_of_neuron_groups_in_layer(current_patterns->network, i, &num_of_neuron_groups_in_layer))
			return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "increment_current_template_types_in_current_pattern_templates", "Couldn' t retrieve number of neuron groups.");
		lcl_current_pattern_templates[current_patterns->trial_stats->num_of_trial_types-1].neuron_current_templates[i] = g_new0(NeuronCurrentTemplateData*, num_of_neuron_groups_in_layer);
		lcl_current_pattern_buffer[current_patterns->trial_stats->num_of_trial_types-1].neuron_current_pattern[i] = g_new0(NeuronCurrent*, num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{
			if (!get_num_of_neurons_in_neuron_group(current_patterns->network, i, j, &num_of_neurons_in_neuron_group))
				return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "increment_current_template_types_in_current_pattern_templates", "Couldn' t retrieve number of neurons.");	
			lcl_current_pattern_templates[current_patterns->trial_stats->num_of_trial_types-1].neuron_current_templates[i][j] = g_new0(NeuronCurrentTemplateData, num_of_neurons_in_neuron_group);
			lcl_current_pattern_buffer[current_patterns->trial_stats->num_of_trial_types-1].neuron_current_pattern[i][j] = g_new0(NeuronCurrent, num_of_neurons_in_neuron_group);
			for (k = 0 ; k < num_of_neurons_in_neuron_group; k++)
			{
				lcl_current_pattern_templates[current_patterns->trial_stats->num_of_trial_types-1].neuron_current_templates[i][j][k].pattern_template.pattern = g_new0(double, num_of_samples);
				lcl_current_pattern_buffer[current_patterns->trial_stats->num_of_trial_types-1].neuron_current_pattern[i][j][k].pattern = g_new0(double, num_of_samples);
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
	if (pattern_type_num >= current_patterns->trial_stats->num_of_trial_types)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_initialization_current", "pattern_type_num >= current_patterns->num_of_template_types");
	current_patterns->current_pattern_templates[pattern_type_num].neuron_current_templates[layer][neuron_group][neuron_num].init_params.max_current = max_current;
	current_patterns->current_pattern_templates[pattern_type_num].neuron_current_templates[layer][neuron_group][neuron_num].init_params.duration = duration;
	return TRUE;
}

bool submit_noise_params(CurrentPatterns* current_patterns, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, TimeStamp	noise_addition_interval, double noise_variance)
{
	if (current_patterns == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_noise_params", "current_patterns == NULL");	
	if (!is_neuron(current_patterns->network, layer, neuron_group, neuron_num))
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_noise_params", "! is_neuron()");
	if (pattern_type_num >= current_patterns->trial_stats->num_of_trial_types)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_noise_params", "pattern_type_num >= trial_stats->num_of_trial_types");
	if (noise_addition_interval < current_patterns->current_pattern_templates[pattern_type_num].sampling_interval)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_noise_params", "noise_addition_interval < current_patterns->current_pattern_templates[pattern_type_num].sampling_interval");	
	current_patterns->current_pattern_templates[pattern_type_num].neuron_current_templates[layer][neuron_group][neuron_num].noise_params.noise_addition_interval = noise_addition_interval;
	current_patterns->current_pattern_templates[pattern_type_num].neuron_current_templates[layer][neuron_group][neuron_num].noise_params.noise_variance = noise_variance;
	return TRUE;
}

bool submit_current_template_sample(CurrentPatterns* current_patterns, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, TimeStamp sampling_time, double current)
{
	TimeStamp sampling_interval; 
	if (current_patterns == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_current_template_sample", "current_patterns == NULL");	
	if (!is_neuron(current_patterns->network, layer, neuron_group, neuron_num))
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_current_template_sample", "! is_neuron()");
	if (pattern_type_num >= current_patterns->trial_stats->num_of_trial_types)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_current_template_sample", "pattern_type_num >= trial_stats->num_of_trial_types");
	if (sampling_time >= current_patterns->trial_stats->trial_type_stats[pattern_type_num].max_trial_type_length_assigned)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "submit_current_template_sample", "sampling_time >= trial_stats->trial_type_stats[pattern_type_num].max_trial_type_length_assigned");
	sampling_interval = current_patterns->current_pattern_templates[pattern_type_num].sampling_interval;
	current_patterns->current_pattern_templates[pattern_type_num].neuron_current_templates[layer][neuron_group][neuron_num].pattern_template.pattern[sampling_time/sampling_interval] =  current;
	return TRUE;
}

bool interrogate_neuron_current_pattern_params(CurrentPatterns* current_patterns, unsigned int trial_num, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num)
{
/*	char temp[2000];
	char temp_str[100];
	double init_current;
	if (current_patterns == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "interrogate_neuron_current_pattern_params", "current_patterns == NULL");	
	if (!is_neuron(current_patterns->network, layer, neuron_group, neuron_num))
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "interrogate_neuron_current_pattern_params", "! is_neuron()");
	if (pattern_type_num >= current_patterns->trial_stats->num_of_trial_types)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "interrogate_neuron_current_pattern_params", "pattern_type_num >= trial_stats->num_of_trial_types");
	if (trial_num >= current_patterns->trial_stats->total_num_of_trials)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "interrogate_neuron_current_pattern_params", "trial_num >= current_patterns->trial_stats->total_num_of_trials");

	init_current =  spike_pattern_generator_get_current_patterns()->current_pattern_templates[trial_type].neuron_current_templates[layer][neuron_group][neuron].init_params.max_current;
	char temp_str[40];
      	sprintf(temp_str, "%.2f", init_current_max);
	strcpy(temp, "Neuron Current Pattern Params:\n");
	print_message(INFO_MSG ,"NeuroSim", "CurrentPatterns", "interrogate_neuron_current_pattern_params", "Neuron Current Pattern Params:");	
	
*/
	return TRUE;
}

bool get_current_pattern(CurrentPatterns* current_patterns, unsigned int trial_num, CurrentPattern **current_pattern)
{
	unsigned int i;
	CurrentPattern *curr_current_pattern, *next_current_pattern;
	*current_pattern = NULL;
	if (current_patterns == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "get_current_pattern", "current_patterns == NULL.");
	if (current_patterns->trial_stats == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "get_current_pattern", "current_patterns->trial_stats == NULL.");	
	if (trial_num >= current_patterns->trial_stats->total_num_of_trials)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "get_current_pattern", "trial_num >= trial_stats->total_num_of_trials.");		
	curr_current_pattern = current_patterns->head;
	for (i = 0; i <= trial_num; i++)
	{
		curr_current_pattern = curr_current_pattern->next;
	}
	*current_pattern = curr_current_pattern;
	if (*current_pattern == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatterns", "get_current_pattern", "*current_pattern == NULL.");				
	return TRUE;		
}
