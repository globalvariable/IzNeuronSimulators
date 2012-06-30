#ifndef STDP_H
#define STDP_H


typedef struct 	__STDPList		STDPList;

#include "../Network/Network.h"

struct __STDPList
{
	double		*stdp_pre_post; 					// num_of_synapses
	double		*change_stdp_pre_post;			// num_of_synapses
	double		*decay_rate_stdp_pre_post;			// num_of_synapses
	double 		*stdp_pre_post_iter_prev;			// num_of_synapses 
	double 		*stdp_pre_post_iter_curr;			// num_of_synapses 
	double 		**stdp_pre_post_pol_vals;			// num_of_synapses * size should be parker_sochacki_max_order + 1
	double		**stdp_pre_post_decay_rate_pol_vals;	// num_of_synapses * size should be parker_sochacki_max_order + 1
	double		*stdp_post_pre;					// num_of_synapses
	double		*change_stdp_post_pre;			// num_of_synapses
	double		*decay_rate_stdp_post_pre;			// num_of_synapses
	double 		*stdp_post_pre_iter_prev;			// num_of_synapses 
	double 		*stdp_post_pre_iter_curr;			// num_of_synapses 
	double 		**stdp_post_pre_pol_vals;			// num_of_synapses * size should be parker_sochacki_max_order + 1
	double		**stdp_post_pre_decay_rate_pol_vals;	// num_of_synapses * size should be parker_sochacki_max_order + 1	
};

bool create_ps_stdp_for_neuron_group(Network *network, unsigned int layer, unsigned int nrn_grp, unsigned int parker_sochacki_max_order, double STDP_pre_post_change_max, double STDP_pre_post_change_min, double STDP_pre_post_tau_min, double STDP_pre_post_tau_max, double  STDP_post_pre_change_max, double  STDP_post_pre_change_min, double STDP_post_pre_tau_max, double  STDP_post_pre_tau_min);

bool create_ps_stdp_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order, double STDP_pre_post_change_max, double STDP_pre_post_change_min, double STDP_pre_post_tau_min, double STDP_pre_post_tau_max, double  STDP_post_pre_change_max, double  STDP_post_pre_change_min, double STDP_post_pre_tau_max, double  STDP_post_pre_tau_min);

#endif
