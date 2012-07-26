#ifndef DEPOL_ELIGIBILITY_H
#define DEPOL_ELIGIBILITY_H


typedef struct 	__DepolEligibilityList	DepolEligibilityList;

#include "../Network/Network.h"

struct __DepolEligibilityList
{
	double		*depol_eligibility;					// num_of_synapses
	double		*depol_eligibility_saved;				// num_of_synapses   // to be handled at the end of trial
	double		*depol_eligibility_decay_rate;			// num_of_synapses
	double 		*depol_eligibility_iter_prev;			// num_of_synapses 
	double 		*depol_eligibility_iter_curr;			// num_of_synapses 
	double 		**depol_eligibility_pol_vals;			// num_of_synapses * size should be parker_sochacki_max_order + 1
	double		**depol_eligibility_decay_rate_pol_vals;	// num_of_synapses * size should be parker_sochacki_max_order + 1
};



bool create_ps_depol_eligibility_for_neuron_group(Network *network, unsigned int layer, unsigned int nrn_grp, unsigned int parker_sochacki_max_order,  double depol_eligibility_tau_max, double  depol_eligibility_tau_min);

bool create_ps_depol_eligibility_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order,  double depol_eligibility_tau_max, double  depol_eligibility_tau_min);

void clear_depol_eligibility_for_neuron(Neuron *neuron);
void save_depol_eligibility_for_neuron(Neuron *neuron);

#endif