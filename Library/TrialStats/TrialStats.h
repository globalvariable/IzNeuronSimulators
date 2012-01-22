#ifndef TRIAL_STATS
#define TRIAL_STATS



typedef struct __MainTrialStats MainTrialStats;
typedef struct __MainSingleTrialStats MainSingleTrialStats;

#include "../../../BlueSpike/Library/Misc/Misc.h"
#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/TimeStamp.h"

struct __MainTrialStats
{
	TimeStamp	*trial_starts_ns;   // num_of_trials
	TimeStamp	*trial_durations_ns;   // num_of_trials
	unsigned int 	num_of_trials_allocated;
	unsigned int 	num_of_trials_used;	
};

struct __MainSingleTrialStats
{
	TimeStamp	trial_start_ns;   // num_of_trials
	TimeStamp	trial_duration_ns;   // num_of_trials
	TimeStamp	num_of_all_simulated_trials;   // num_of_trials
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
