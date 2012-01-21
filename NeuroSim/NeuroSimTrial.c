#include "NeuroSimTrial.h"


static MainTrialStats *neurosim_main_trial_stats = NULL;

MainTrialStats * neurosim_get_main_trial_stats(void)
{
	return neurosim_main_trial_stats;
}

void neurosim_set_main_trial_stats(MainTrialStats *main_stats)
{
	neurosim_main_trial_stats = main_stats;
}

