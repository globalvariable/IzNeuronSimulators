#ifndef STDP_H
#define STDP_H


typedef struct 	__STDPList		STDPList;


#define STDP_TYPE_PRE_POST	0
#define STDP_TYPE_POST_PRE	1
#define MAX_NUM_OF_STDP_TYPE	2

#include "../Network/Network.h"

struct __STDPList
{
	double		*stdp_pre_post; 					// num_of_synapses
	double		*change_stdp_pre_post;			// num_of_synapses       // should be less than 'max_eligibility'
	double		*decay_rate_stdp_pre_post;			// num_of_synapses
	double 		*stdp_pre_post_iter_prev;			// num_of_synapses 
	double 		*stdp_pre_post_iter_curr;			// num_of_synapses 
	double 		**stdp_pre_post_pol_vals;			// num_of_synapses * size should be parker_sochacki_max_order + 1
	double		**stdp_pre_post_decay_rate_pol_vals;	// num_of_synapses * size should be parker_sochacki_max_order + 1
	double		*stdp_post_pre;					// num_of_synapses
	double		*change_stdp_post_pre;			// num_of_synapses	// should be less than 'max_eligibility'
	double		*decay_rate_stdp_post_pre;			// num_of_synapses
	double 		*stdp_post_pre_iter_prev;			// num_of_synapses 
	double 		*stdp_post_pre_iter_curr;			// num_of_synapses 
	double 		**stdp_post_pre_pol_vals;			// num_of_synapses * size should be parker_sochacki_max_order + 1
	double		**stdp_post_pre_decay_rate_pol_vals;	// num_of_synapses * size should be parker_sochacki_max_order + 1	
};

bool create_ps_stdp_for_neuron_group(Network *network, unsigned int layer, unsigned int nrn_grp, unsigned int parker_sochacki_max_order, double STDP_pre_post_change_max, double STDP_pre_post_change_min, double STDP_pre_post_tau_min, double STDP_pre_post_tau_max, double  STDP_post_pre_change_max, double  STDP_post_pre_change_min, double STDP_post_pre_tau_max, double  STDP_post_pre_tau_min);

bool create_ps_stdp_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order, double STDP_pre_post_change_max, double STDP_pre_post_change_min, double STDP_pre_post_tau_min, double STDP_pre_post_tau_max, double  STDP_post_pre_change_max, double  STDP_post_pre_change_min, double STDP_post_pre_tau_max, double  STDP_post_pre_tau_min);

bool allocate_ps_stdp_for_neuron(Neuron* neuron , int parker_sochacki_max_order);
bool submit_ps_stdp_for_synapse(Neuron* neuron , unsigned int syn_idx, double STDP_pre_post_change, double STDP_pre_post_tau, double  STDP_post_pre_change, double STDP_post_pre_tau);

bool submit_new_ps_stdp_vals_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order, double STDP_pre_post_change_max, double STDP_pre_post_change_min, double STDP_pre_post_tau_min, double STDP_pre_post_tau_max, double  STDP_post_pre_change_max, double  STDP_post_pre_change_min, double STDP_post_pre_tau_max, double  STDP_post_pre_tau_min);

bool submit_new_ps_stdp_vals_for_synapse(Neuron* neuron , unsigned int parker_sochacki_max_order, double STDP_pre_post_change_max, double STDP_pre_post_change_min, double STDP_pre_post_tau_min, double STDP_pre_post_tau_max, double  STDP_post_pre_change_max, double  STDP_post_pre_change_min, double STDP_post_pre_tau_max, double  STDP_post_pre_tau_min, unsigned int synapse);

#endif
