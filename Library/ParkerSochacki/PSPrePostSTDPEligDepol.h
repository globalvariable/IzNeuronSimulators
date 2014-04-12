#ifndef PS_PRE_POST_STDP_ELIG_DEPOL_H
#define PS_PRE_POST_STDP_ELIG_DEPOL_H

#include "ParkerSochacki.h"

// PS INTEGRATION WITH ONLY PRE POST STDP & ELIGIBILITY AND SYNAPTIC WEIGHT UPDATE DURING INTEGRATION
bool evaluate_neuron_dyn_pre_post_stdp_elig_depol(Neuron *nrn, TimeStamp start_time, TimeStamp end_time, bool *spike_generated, TimeStamp *spike_time);




#endif
