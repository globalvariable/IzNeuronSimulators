#ifndef ELIGIBILITY_H
#define ELIGIBILITY_H


typedef struct 	__PsEligibility	PsEligibility;

#include "../Network/Network.h"

struct __PsEligibility
{
	double		now; 					
//	double		change_amount;	 it is determines by stdp	
	double		decay_rate;			
	double		max_eligibility;
	double		depol_eligibility;
	double 		iter_prev;			
	double 		iter_curr;			
	double 		*ps_pol_vals;			// size should be parker_sochacki_max_order + 1
	double		*decay_rate_ps_pol_vals;	//  size should be parker_sochacki_max_order + 1
};



bool create_ps_eligibility_for_neuron_group(Network *network, unsigned int layer, unsigned int nrn_grp, double tau_min, double  tau_max, double max_eligibility_min, double max_eligibility_max, double depol_eligibility_min, double depol_eligibility_max);  

bool create_ps_eligibility_for_neuron(Neuron* neuron, double tau_min, double tau_max, double max_eligibility_min, double max_eligibility_max, double depol_eligibility_min, double depol_eligibility_max);

bool create_ps_eligibility_for_synapse(Neuron* neuron , double tau_min, double tau_max, unsigned int synapse, double max_eligibility_min, double max_eligibility_max, double depol_eligibility_min, double depol_eligibility_max);

bool allocate_ps_eligibility_for_neuron(Neuron* neuron);

bool submit_new_ps_eligibility_vals_for_neuron(Neuron* neuron, double tau_min, double tau_max, double max_eligibility_min, double max_eligibility_max, double depol_eligibility_min, double depol_eligibility_max);

bool submit_new_ps_eligibility_vals_for_synapse(Neuron* neuron, double tau_min, double tau_max, unsigned int synapse, double max_eligibility_min, double max_eligibility_max, double depol_eligibility_min, double depol_eligibility_max);

bool submit_new_ps_eligibility_vals_for_neuron_according_to_pre_synaptic_neuron_group(Neuron* neuron, Network *axon_from_network, unsigned int axon_from_layer, unsigned int axon_from_nrn_grp, double tau_min, double  tau_max, double max_eligibility_min, double max_eligibility_max, double depol_eligibility_min, double depol_eligibility_max);

void clear_eligibility_for_neuron(Neuron *neuron);

#endif
