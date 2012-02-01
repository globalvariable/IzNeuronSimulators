#include "SpikePatterns.h"


SpikePatterns* allocate_spike_patterns(SpikePatterns* spike_patterns, unsigned int num_of_patterns_to_allocate, unsigned int num_of_spikes_to_allocate)
{
	unsigned int i;
	if (spike_patterns != NULL)
	{
		spike_patterns = deallocate_spike_patterns(spike_patterns);
		spike_patterns = allocate_spike_patterns(spike_patterns, num_of_patterns_to_allocate, num_of_spikes_to_allocate);
		return spike_patterns;
	}	
	
	spike_patterns = g_new0(SpikePatterns, 1);
	print_message(INFO_MSG ,"BlueSpike", "SpikePatterns", "allocate_spike_patterns", "Created Spike Patterns");
	spike_patterns->patterns = g_new0(SpikePattern, num_of_patterns_to_allocate);
	for (i = 0; i < num_of_patterns_to_allocate; i++ )
	{
		spike_patterns->patterns[i].spikes = g_new0(SpikeTimeStampItem, num_of_spikes_to_allocate);
	}	
	spike_patterns->num_of_allocated_patterns = num_of_patterns_to_allocate;
	spike_patterns->allocated_num_of_spikes = num_of_spikes_to_allocate;
	return spike_patterns;
}

SpikePatterns* deallocate_spike_patterns(SpikePatterns* spike_patterns)
{
	unsigned int i;
	if (spike_patterns == NULL)
	{
		print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "deallocate_single_spike_pattern", "spike_patterns is already NULL");	
		return NULL;	
	}
	for (i = 0; i < spike_patterns->num_of_allocated_patterns; i++)
	{
		g_free(spike_patterns->patterns[i].spikes);
	}	
	g_free(spike_patterns->patterns);
	g_free(spike_patterns);
	print_message(INFO_MSG ,"BlueSpike", "SpikePatterns", "deallocate_spike_patterns", "Destroyed Spike_Patterns");	
	return NULL;	
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
	print_message(INFO_MSG ,"BlueSpike", "SpikePatterns", "allocate_single_spike_pattern", "Created Single Spike Pattern.");

	single_spike_pattern->spikes = g_new0(SpikeTimeStampItem, num_of_spikes_to_allocate);	
	single_spike_pattern->allocated_num_of_spikes = num_of_spikes_to_allocate;
	return single_spike_pattern;
}

SingleSpikePattern* deallocate_single_spike_pattern(SingleSpikePattern* single_spike_pattern)
{
	if (single_spike_pattern == NULL)
	{
		print_message(WARNING_MSG ,"BlueSpike", "SpikePatterns", "deallocate_single_spike_pattern", "Single Spike Pattern is already NULL");	
		return NULL;	
	}
	g_free(single_spike_pattern->spikes);
	g_free(single_spike_pattern);
	print_message(INFO_MSG ,"BlueSpike", "SpikePatterns", "deallocate_single_spike_pattern", "Destroyed Single Spike Pattern");	
	return NULL;	
}

bool reset_single_spike_pattern_write_idx(SingleSpikePattern* single_spike_pattern)
{
	if (single_spike_pattern == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "reset_spike_pattern_write_idx", "single_spike_pattern was not allocated.");
	single_spike_pattern->used_num_of_spikes = 0;
	return TRUE;
}

bool get_used_num_of_spikes_of_single_spike_pattern(SingleSpikePattern* single_spike_pattern, unsigned int *used_num_of_spikes)
{
	*used_num_of_spikes = 0;
	if (single_spike_pattern == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "get_used_num_of_spikes_of_single_spike_pattern", "single_spike_pattern was not allocated.");	
	*used_num_of_spikes = single_spike_pattern->used_num_of_spikes;
	return TRUE;
}

bool get_allocated_num_of_spikes_of_single_spike_pattern(SingleSpikePattern* single_spike_pattern, unsigned int *allocated_num_of_spikes)
{
	*allocated_num_of_spikes = 0;
	if (single_spike_pattern == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "get_allocated_num_of_spikes_of_single_spike_pattern", "single_spike_pattern was not allocated.");	
	*allocated_num_of_spikes = single_spike_pattern->allocated_num_of_spikes;
	return TRUE;
}

bool set_used_num_of_spikes_of_single_spike_pattern(SingleSpikePattern* single_spike_pattern, unsigned int used_num_of_spikes)
{
	if (single_spike_pattern == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "set_used_num_of_spikes_of_single_spike_pattern", "single_spike_pattern was not allocated.");	
	single_spike_pattern->used_num_of_spikes = used_num_of_spikes;
	return TRUE;	
}

bool get_spikes_ptr_of_single_spike_pattern(SingleSpikePattern* single_spike_pattern, SpikeTimeStampItem **spikes)
{
	*spikes = NULL;
	if (single_spike_pattern == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "get_spikes_ptr_of_single_spike_pattern", "single_spike_pattern was not allocated.");	
	*spikes = 	single_spike_pattern->spikes;
	return TRUE;
}

bool write_spike_time_stamp_to_single_spike_pattern(SingleSpikePattern* single_spike_pattern, SpikeTimeStampItem *spike_time_stamp)
{
	// reset before every use
	int i;
	
	if (single_spike_pattern == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "write_spike_time_stamp_to_spike_pattern", "SpikePatterns *single_spike_pattern was not allocated\n");
	
	if (single_spike_pattern->used_num_of_spikes == single_spike_pattern->allocated_num_of_spikes)
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "write_spike_time_stamp_to_spike_pattern", "single_spike_pattern->used_num_of_spikes == single_spike_pattern->allocated_num_of_spikes");	
	
	i = single_spike_pattern->used_num_of_spikes;
	single_spike_pattern->used_num_of_spikes ++;
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

bool add_single_spike_pattern_to_spike_patterns(SingleSpikePattern* single_spike_pattern, SpikePatterns* spike_patterns)
{
	unsigned int i;
	unsigned int used_pattern_num = spike_patterns->num_of_used_patterns;
	unsigned int used_num_of_spikes = single_spike_pattern->used_num_of_spikes;
	if (single_spike_pattern == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "add_single_spike_pattern_to_spike_patterns", "single_spike_pattern was not allocated\n");
	if (spike_patterns == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "add_single_spike_pattern_to_spike_patterns", "spike_patterns was not allocated");	
	if (single_spike_pattern->allocated_num_of_spikes != spike_patterns->allocated_num_of_spikes)
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "add_single_spike_pattern_to_spike_patterns", "single_spike_pattern->allocated_num_of_spikes != spike_patterns>allocated_num_of_spikes");			
	if (used_pattern_num == spike_patterns->num_of_allocated_patterns)
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "add_single_spike_pattern_to_spike_patterns", "spike_patterns->num_of_used_patterns == spike_patterns->num_of_allocated_patterns");					

	for (i=0; i < used_num_of_spikes; i++)
	{
		spike_patterns->patterns[used_pattern_num].spikes[0].peak_time = single_spike_pattern->spikes[0].peak_time;
		spike_patterns->patterns[used_pattern_num].spikes[i].mwa_or_layer = single_spike_pattern->spikes[i].mwa_or_layer;		
		spike_patterns->patterns[used_pattern_num].spikes[i].channel_or_neuron_group = single_spike_pattern->spikes[i].channel_or_neuron_group;
		spike_patterns->patterns[used_pattern_num].spikes[i].unit_or_neuron = single_spike_pattern->spikes[i].unit_or_neuron;				
	}	
	spike_patterns->patterns[used_pattern_num].used_num_of_spikes = single_spike_pattern->used_num_of_spikes;	
	spike_patterns->num_of_used_patterns ++;

	return TRUE;				
}

bool reset_spike_patterns_write_idx(SpikePatterns* spike_patterns)
{
	if (spike_patterns == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "reset_spike_patterns", "spike_patterns was not allocated");	
	spike_patterns->num_of_used_patterns = 0;
	return TRUE;
}
