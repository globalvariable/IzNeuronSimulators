#ifndef PARKER_SOCHACKI_SYNAPSE_COND_H
#define PARKER_SOCHACKI_SYNAPSE_COND_H


#include "ParkerSochacki.h"

// PS INTEGRATION WITH ONLY SYNAPTIC INTEGRATION (NO STDP)
bool evaluate_neuron_dyn(Neuron *nrn, TimeStamp start_time, TimeStamp end_time, bool *spike_generated, TimeStamp *spike_time);



#endif
