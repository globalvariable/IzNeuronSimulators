#ifndef TRIAL_STATS
#define TRIAL_STATS



typedef struct __TrialStats TrialStats;
typedef struct __TrialData TrialData;

#include "../../../BlueSpike/Library/Misc/Misc.h"
#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/TimeStamp.h"
#include "TrialType.h"

struct __TrialStats
{
	TrialData		*trials_data;
	TrialData		*current_trial_data;	
	unsigned int 	num_of_trials;		// it is also current trial num
	TimeStamp	max_trial_length;   
	TimeStamp	min_trial_length;   	
	TimeStamp	max_trial_interval; 
	TimeStamp	min_trial_interval; 	  		
};

struct __TrialData
{
	TimeStamp	trial_start_time;   
	TimeStamp	trial_length;
	TimeStamp	trial_interval;     // according to previous pattern end time.
	TrialType		type;	// left, right, tracjectroy right etc. 
	TrialData		*next;
	TrialData		*prev;
};

struct __SingleTrialData
{
	TimeStamp	trial_start_time;   
	TimeStamp	trial_length;
	TimeStamp	trial_interval;     // according to previous pattern end time.
	TrialType		type;	// left, right, tracjectroy right etc. 
};

MainTrialStats* allocate_main_trial_stats(MainTrialStats* main_stats, unsigned int num_of_trials_to_allocate);
MainTrialStats* deallocate_main_trial_stats(MainTrialStats* main_stats);
bool reset_main_trial_stats_write_idx(MainTrialStats* main_stats);
bool write_to_main_trial_stats(MainTrialStats* main_stats, TimeStamp trial_start_ns , TimeStamp trial_duration_ns);
bool get_main_trial_stats_num_of_trials(MainTrialStats* main_stats,  unsigned int *num_of_trials_used, unsigned int *num_of_trials_allocated);
bool get_main_trial_stats_trial_times_ns(MainTrialStats* main_stats, unsigned int trial_num, TimeStamp *trial_start_ns, TimeStamp *trial_duration_ns);

MainSingleTrialStats* allocate_main_single_trial_stats(MainSingleTrialStats* main_single_stats);
MainSingleTrialStats* deallocate_main_single_trial_stats(MainSingleTrialStats* main_single_stats);
bool write_to_main_single_trial_stats(MainSingleTrialStats* main_single_stats, TimeStamp trial_start_ns , TimeStamp trial_duration_ns);
bool reset_main_single_trial_stats(MainSingleTrialStats* main_single_stats);
bool get_main_single_trial_stats_num_of_all_simulated_trials(MainSingleTrialStats* main_single_stats,  unsigned int *num_of_all_simulated_trials);
bool get_main_single_trial_stats_trial_times_ns(MainSingleTrialStats* main_single_stats, TimeStamp *trial_start_ns, TimeStamp *trial_duration_ns);

#endif
