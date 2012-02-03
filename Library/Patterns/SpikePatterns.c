#include "SpikePatterns.h"

SpikePatterns* allocate_spike_patterns(TrialStats *trial_stats, SpikePatterns* spike_patterns)
{
	if (trial_stats == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "SpikePatterns", "allocate_spike_patterns", "trial_stats == NULL");	
		return NULL;
	}
	if (spike_patterns != NULL)
		spike_patterns = deallocate_spike_patterns(trial_stats, spike_patterns);
		spike_patterns = allocate_trial_stats(trial_stats, spike_patterns);
		return spike_patterns;
	}	
	spike_patterns = g_new0(SpikePatterns, 1);
	spike_patterns->head = g_new0(SpikePattern, 1);	
	spike_patterns->tail = spike_patterns->head;
	print_message(INFO_MSG ,"NeuroSim", "SpikePatterns", "allocate_spike_patterns", "Created spike_patterns.");
	return spike_patterns;
}
SpikePatterns* deallocate_spike_patterns(TrialStats *trial_stats, SpikePatterns* spike_patterns)
{
	unsigned int i;
	SpikePattern *curr_spike_pattern, *prev_spike_pattern;
	if (trial_stats == NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "SpikePatterns", "deallocate_spike_patterns", "trial_stats == NULL");	
		return NULL;
	}
	curr_spike_pattern = spike_patterns->tail;
	for (i = 0; i < trial_stats->num_of_trials; i++)
	{
		prev_spike_pattern = curr_spike_pattern->prev;
		g_free(curr_spike_pattern->spikes);
		g_free(curr_spike_pattern);
		curr_spike_pattern = prev_spike_pattern;
	}
	if (curr_spike_pattern != spike_patterns->head)
	{
		print_message(BUG_MSG ,"NeuroSim", "SpikePatterns", "deallocate_spike_patterns", "curr_spike_pattern != trial_stats->head");
	}
	g_free(spike_patterns->head);
	g_free(spike_patterns);
	print_message(INFO_MSG ,"NeuroSim", "SpikePatterns", "deallocate_spike_patterns", "Destroyed spike_patterns.");
	retun NULL;
}
SingleSpikePattern* allocate_single_spike_pattern(SingleSpikePattern* single_spike_pattern, unsigned int num_of_spikes_to_allocate)
{
	if (single_spike_pattern != NULL)
	{
		single_spike_pattern = deallocate_single_spike_pattern(single_spike_pattern);
		single_spike_pattern = allocate_single_spike_pattern(single_spike_pattern, num_of_spikes_to_allocate);
		return single_spike_pattern;
	}	
	single_spike_pattern = g_new0(SingleSpikePattern, 1);
	single_spike_pattern->spikes = g_new0(SpikeTimeStampItem, num_of_spikes_to_allocate);	
	single_spike_pattern->allocated_pattern_size = num_of_spikes_to_allocate;
	print_message(INFO_MSG ,"NeuroSim", "SpikePatterns", "allocate_single_spike_pattern", "Created single_spike_pattern.");
	return single_spike_pattern;
}
SingleSpikePattern* deallocate_single_spike_pattern(SingleSpikePattern* single_spike_pattern)
{
	if (single_spike_pattern == NULL)
	{
		print_message(WARNING_MSG ,"NeuroSim", "SpikePatterns", "deallocate_single_spike_pattern", "single_spike_pattern == NULL");	
		return NULL;	
	}
	g_free(single_spike_pattern->spikes);
	g_free(single_spike_pattern);
	print_message(INFO_MSG ,"NeuroSim", "SpikePatterns", "deallocate_single_spike_pattern", "Destroyed single_spike_pattern");	
	return NULL;	
}
bool add_spike_time_stamp_to_single_spike_pattern(SingleSpikePattern* single_spike_pattern, SpikeTimeStampItem *spike_time_stamp, bool first_spike)
{
	unsigned int i;
	if (single_spike_pattern == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "SpikePatterns", "write_spike_time_stamp_to_spike_pattern", "SpikePatterns *single_spike_pattern was not allocated\n");
	
	if (single_spike_pattern->write_idx == single_spike_pattern->allocated_pattern_size)
		return print_message(ERROR_MSG ,"NeuroSim", "SpikePatterns", "write_spike_time_stamp_to_spike_pattern", "(single_spike_pattern->write_idx == single_spike_pattern->allocated_pattern_size.");	

	if (first_spike)
		single_spike_pattern->write_idx = 0;
	i = single_spike_pattern->write_idx;
	single_spike_pattern->write_idx ++;
	// sort spike time
	while(1) 
	{
		i --;
		if (i <0)
		{
			single_spike_pattern->spikes[0].peak_time = spike_time_stamp->peak_time;
			single_spike_pattern->spikes[0].mwa_or_layer = spike_time_stamp->mwa_or_layer;		
			single_spike_pattern->spikes[0].channel_or_neuron_group = spike_time_stamp->channel_or_neuron_group;
			single_spike_pattern->spikes[0].unit_or_neuron = spike_time_stamp->unit_or_neuron;		
			break;
		}			
		if (spike_time_stamp->peak_time <  single_spike_pattern->spikes[i].peak_time)		// push item to the next index
		{
			single_spike_pattern->spikes[i+1].peak_time = single_spike_pattern->spikes[i].peak_time;
			single_spike_pattern->spikes[i+1].mwa_or_layer =single_spike_pattern->spikes[i].mwa_or_layer;		
			single_spike_pattern->spikes[i+1].channel_or_neuron_group = single_spike_pattern->spikes[i].channel_or_neuron_group;
			single_spike_pattern->spikes[i+1].unit_or_neuron = single_spike_pattern->spikes[i].unit_or_neuron;					
		}
		else
		{
			single_spike_pattern->spikes[i+1].peak_time = spike_time_stamp->peak_time;
			single_spike_pattern->spikes[i+1].mwa_or_layer = spike_time_stamp->mwa_or_layer;		
			single_spike_pattern->spikes[i+1].channel_or_neuron_group = spike_time_stamp->channel_or_neuron_group;
			single_spike_pattern->spikes[i+1].unit_or_neuron = spike_time_stamp->unit_or_neuron;		
			break;
		}
	} 
	return TRUE;				
}

bool add_single_spike_pattern_to_spike_patterns(TrialStats *trial_stats, SpikePatterns* spike_patterns, SingleSpikePattern* single_spike_pattern)
{
	unsigned int i;
	unsigned int num_of_spikes;
	if (trial_stats == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "SpikePatterns", "add_single_spike_pattern_to_spike_patterns", "trial_stats == NULL.");	
	if (spike_patterns == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "SpikePatterns", "add_single_spike_pattern_to_spike_patterns", "spike_patterns == NULL.");	
	if (single_spike_pattern == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "SpikePatterns", "add_single_spike_pattern_to_spike_patterns", "single_spike_pattern == NULL");

	spike_patterns->tail->next = gnew_0(SpikePattern, 1);
	spike_patterns->tail->next->prev = spike_patterns->tail;
	spike_patterns->tail = spike_patterns->tail->next;
	num_of_spikes = single_spike_pattern->write_idx;
	spike_patterns->tail->spikes = g_new0(SpikeTimeStampItem, num_of_spikes);		

	for (i=0; i < num_of_spikes; i++)
	{
		spike_patterns->tail->spikes[i].peak_time = single_spike_pattern->spikes[i].peak_time;
		spike_patterns->tail->spikes[i].mwa_or_layer = single_spike_pattern->spikes[i].mwa_or_layer;		
		spike_patterns->tail->spikes[i].channel_or_neuron_group = single_spike_pattern->spikes[i].channel_or_neuron_group;
		spike_patterns->tail->spikes[i].unit_or_neuron = single_spike_pattern->spikes[i].unit_or_neuron;				
	}	
	spike_patterns->tail->num_of_spikes = num_of_spikes;	

	return TRUE;				
}

