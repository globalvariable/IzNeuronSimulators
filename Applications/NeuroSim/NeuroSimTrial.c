#include "NeuroSimTrial.h"


static MainTrialStats *neurosim_main_trial_stats = NULL;
static MainSingleTrialStats *neurosim_main_single_trial_stats = NULL;

MainTrialStats * neurosim_get_main_trial_stats(void)
{
	return neurosim_main_trial_stats;
}

void neurosim_set_main_trial_stats(MainTrialStats *main_stats)
{
	neurosim_main_trial_stats = main_stats;
}


MainSingleTrialStats * neurosim_get_main_single_trial_stats(void)
{
	return neurosim_main_single_trial_stats;
}

void neurosim_set_main_single_trial_stats(MainSingleTrialStats *main_single_stats)
{
	neurosim_main_single_trial_stats = main_single_stats;
}

