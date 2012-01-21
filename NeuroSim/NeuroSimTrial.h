#ifndef NEUROSIM_TRIAL_H
#define NEUROSIM_TRIAL_H

#include <stdio.h>

#include "../../BlueSpike/Library/TrialStats/TrialStats.h"

MainTrialStats * neurosim_get_main_trial_stats(void);
void neurosim_set_main_trial_stats(MainTrialStats* main_stats);


#endif
