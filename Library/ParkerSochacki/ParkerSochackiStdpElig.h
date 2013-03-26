#ifndef PARKER_SOCHACKI_STDP_ELIG_H
#define PARKER_SOCHACKI_STDP_ELIG_H


#include "ParkerSochacki.h"

// PS INTEGRATION WITH STDP & ELIGIBILITY
bool evaluate_neuron_dyn_stdp_elig(Neuron *nrn, TimeStamp start_time, TimeStamp end_time, bool *spike_generated, TimeStamp *spike_time);


#endif
