#ifndef PS_PRE_POST_RESET_STDP_ELIG_H
#define PS_PRE_POST_RESET_STDP_ELIG_H

#include "ParkerSochacki.h"

// PS INTEGRATION WITH ONLY PRE POST STDP & ELIGIBILITY AND SYNAPTIC WEIGHT UPDATE DURING INTEGRATION
bool evaluate_neuron_dyn_pre_post_w_resetting_stdp_elig(Neuron *nrn, TimeStamp start_time, TimeStamp end_time, bool *spike_generated, TimeStamp *spike_time);




#endif
