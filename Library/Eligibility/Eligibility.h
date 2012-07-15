#ifndef ELIGIBILITY_H
#define ELIGIBILITY_H


typedef struct 	__EligibilityList	EligibilityList;

#include "../Network/Network.h"

struct __EligibilityList
{
	double		*eligibility;					// num_of_synapses
	double		*eligibility_decay_rate;			// num_of_synapses
	double 		*eligibility_iter_prev;			// num_of_synapses 
	double 		*eligibility_iter_curr;			// num_of_synapses 
	double 		**eligibility_pol_vals;			// num_of_synapses * size should be parker_sochacki_max_order + 1
	double		**eligibility_decay_rate_pol_vals;	// num_of_synapses * size should be parker_sochacki_max_order + 1
};



bool create_ps_eligiblity_for_neuron_group(Network *network, unsigned int layer, unsigned int nrn_grp, unsigned int parker_sochacki_max_order,  double eligibility_tau_max, double  eligibility_tau_min);

bool create_ps_eligiblity_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order,  double eligibility_tau_max, double  eligibility_tau_min);

void clear_eligibility_for_neuron(Neuron *neuron);


#endif
