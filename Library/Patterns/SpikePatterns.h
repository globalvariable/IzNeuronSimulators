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
	TrialStats			*trial_stats;	// get num of patterns from num of trials
	SpikePattern 		*head;		
	SpikePattern 		*tail;
};

struct __SpikePattern
{
	SpikeTimeStampItem *spikes;
	unsigned int num_of_spikes;
	SpikePattern *prev;
	SpikePattern *next;	
};

struct __SingleSpikePattern
{
	SpikeTimeStampItem *spikes;
	unsigned int write_idx;
	unsigned int allocated_num_of_spikes;
};

SpikePatterns* allocate_spike_patterns(TrialStats *trial_stats, SpikePatterns* spike_patterns);
SpikePatterns* deallocate_spike_patterns(TrialStats *trial_stats, SpikePatterns* spike_patterns);
SingleSpikePattern* allocate_single_spike_pattern(SingleSpikePattern* single_spike_pattern, unsigned int num_of_spikes_to_allocate);
SingleSpikePattern* deallocate_single_spike_pattern(SingleSpikePattern* single_spike_pattern);
bool get_used_num_of_spikes_of_single_spike_pattern(SingleSpikePattern* single_spike_pattern, unsigned int *used_num_of_spikes);
bool add_spike_time_stamp_to_single_spike_pattern(SingleSpikePattern* single_spike_pattern, SpikeTimeStampItem *spike_time_stamp, bool first_spike);
bool add_single_spike_pattern_to_spike_patterns(TrialStats *trial_stats, SpikePatterns* spike_patterns, SingleSpikePattern* single_spike_pattern);

#endif
