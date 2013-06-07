#ifndef ELIGIBILITY_H
#define ELIGIBILITY_H


typedef struct 	__EligibilityList	EligibilityList;

#include "../Network/Network.h"

struct __EligibilityList
{
	double		*eligibility;					// num_of_synapses
	double		*eligibility_saved;				// num_of_synapses   // to be handled at the end of trial
	double		*eligibility_decay_rate;			// num_of_synapses
	double		*depol_eligibility_change; // num_of_synapses	// should be less than 'max_eligibility'
	double 		*eligibility_iter_prev;			// num_of_synapses 
	double 		*eligibility_iter_curr;			// num_of_synapses 
	double 		**eligibility_pol_vals;			// num_of_synapses * size should be parker_sochacki_max_order + 1
	double		**eligibility_decay_rate_pol_vals;	// num_of_synapses * size should be parker_sochacki_max_order + 1
	double		max_eligibility;	
	double		eligibility_rate;	
};



bool create_ps_eligibility_for_neuron_group(Network *network, unsigned int layer, unsigned int nrn_grp, unsigned int parker_sochacki_max_order,  double eligibility_tau_max, double  eligibility_tau_min, double depol_eligibility_change_max, double  depol_eligibility_change_min, double max_eligibility, double eligibility_rate);   // run this function before create_ps_stdp_for_neuron_group, stdp_change  shouldnt be larger than max_eligibility

bool create_ps_eligibility_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order,  double eligibility_tau_max, double  eligibility_tau_min, double depol_eligibility_change_max, double  depol_eligibility_change_min, double max_eligibility, double eligibility_rate);

bool allocate_ps_eligibility_for_neuron(Neuron* neuron , int parker_sochacki_max_order);
bool submit_ps_eligibility_for_synapse(Neuron* neuron , unsigned int syn_idx, double eligibility_tau, double depol_eligibility_change, double max_eligibility, double eligibility_rate);

bool submit_new_ps_eligibility_vals_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order,  double eligibility_tau_max, double  eligibility_tau_min, double depol_eligibility_change_max, double  depol_eligibility_change_min, double max_eligibility, double eligibility_rate);
bool submit_new_ps_eligibility_vals_for_synapse(Neuron* neuron , unsigned int parker_sochacki_max_order,  double eligibility_tau_max, double  eligibility_tau_min, unsigned int synapse, double depol_eligibility_change_max, double  depol_eligibility_change_min, double max_eligibility, double eligibility_rate);
void clear_eligibility_for_neuron(Neuron *neuron);
void save_eligibility_for_neuron(Neuron *neuron);

#endif
