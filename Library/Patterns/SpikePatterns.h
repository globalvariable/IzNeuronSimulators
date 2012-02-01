#ifndef SPIKE_PATTERNS
#define SPIKE_PATTERNS


typedef struct __SingleSpikePattern SingleSpikePattern;
typedef struct __SpikePattern SpikePattern;
typedef struct __SpikePatterns SpikePatterns;

#include "../../SpikeTimeStamp.h"
#include "../Misc/Misc.h"
#include <stdbool.h>
#include <gtk/gtk.h>

struct __SpikePatterns
{
	SpikePattern *patterns;
	unsigned int num_of_used_patterns;
	unsigned int num_of_allocated_patterns;
	unsigned int allocated_num_of_spikes;
};

struct __SpikePattern
{
	SpikeTimeStampItem *spikes;
	unsigned int used_num_of_spikes;
};

struct __SingleSpikePattern
{
	SpikeTimeStampItem *spikes;
	unsigned int used_num_of_spikes;
	unsigned int allocated_num_of_spikes;	
};

SpikePatterns* allocate_spike_patterns(SpikePatterns* spike_patterns, unsigned int num_of_patterns_to_allocate, unsigned int num_of_samples_to_allocate);
SpikePatterns* deallocate_spike_patterns(SpikePatterns* spike_patterns);
SingleSpikePattern* allocate_single_spike_pattern(SingleSpikePattern* single_spike_pattern, unsigned int num_of_spikes_to_allocate);
SingleSpikePattern* deallocate_single_spike_pattern(SingleSpikePattern* single_spike_pattern);
bool reset_spike_patterns_write_idx(SpikePatterns* spike_patterns);
bool reset_single_spike_pattern_write_idx(SingleSpikePattern* single_spike_pattern);
bool write_spike_time_stamp_to_single_spike_pattern(SingleSpikePattern* single_spike_pattern, SpikeTimeStampItem *spike_time_stamp);
bool add_single_spike_pattern_to_spike_patterns(SingleSpikePattern* single_spike_pattern, SpikePatterns* spike_patterns);
bool get_used_num_of_spikes_of_single_spike_pattern(SingleSpikePattern* single_spike_pattern, unsigned int *used_num_of_spikes);
bool get_allocated_num_of_spikes_of_single_spike_pattern(SingleSpikePattern* single_spike_pattern, unsigned int *allocated_num_of_spikes);
bool set_used_num_of_spikes_of_single_spike_pattern(SingleSpikePattern* single_spike_pattern, unsigned int used_num_of_spikes);
bool get_spikes_ptr_of_single_spike_pattern(SingleSpikePattern* single_spike_pattern, SpikeTimeStampItem **spikes);












#endif
