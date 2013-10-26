#ifndef PARKER_SOCHACKI_STDP_PSDDP_H
#define PARKER_SOCHACKI_STDP_PSDDP_H
// PS INTEGRATION WITH STDP & ELIGIBILITY & DEPOLARIZATION BASED ELIGIBILITY (WITHOUT DEPENDENCY TO MEMBRANE VOLTAGE)

//stdp: spike timing_dependent plasticity
//psddp: post synaptic depolarization dependent plasticity 

#include "ParkerSochacki.h"

bool evaluate_neuron_dyn_stdp_psddp_elig(Neuron *nrn, TimeStamp start_time, TimeStamp end_time, bool *spike_generated, TimeStamp *spike_time);  

THERE APPEARS NEWTON RAPHSON DIVERGENCE with 10-3 error tolerance. ParkerSochackiSynapseCond.h does not suffer from it.

#endif


