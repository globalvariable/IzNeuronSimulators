#ifndef STDP_H
#define STDP_H


typedef struct 	__PsStdpPrePost	PsStdpPrePost;
typedef struct 	__PsStdpPostPre	PsStdpPostPre;

#define STDP_TYPE_PRE_POST	0
#define STDP_TYPE_POST_PRE	1
#define MAX_NUM_OF_STDP_TYPE	2

#include "../Network/Network.h"


struct __PsStdpPrePost
{
	double		now; 					
	double		change_amount;				
	double		decay_rate;			
	double 		iter_prev;			
	double 		iter_curr;			
	double 		*ps_pol_vals;			// size should be parker_sochacki_max_order + 1
	double		*decay_rate_ps_pol_vals;	//  size should be parker_sochacki_max_order + 1
};

struct __PsStdpPostPre
{
	double		now; 					
	double		change_amount;				
	double		decay_rate;			
	double 		iter_prev;			
	double 		iter_curr;			
	double 		*ps_pol_vals;			// size should be parker_sochacki_max_order + 1
	double		*decay_rate_ps_pol_vals;	//  size should be parker_sochacki_max_order + 1
};

bool create_ps_pre_post_stdp_for_neuron_group(Network *network, unsigned int layer, unsigned int nrn_grp, double change_min, double change_max, double tau_min, double tau_max);
bool create_ps_post_pre_stdp_for_neuron_group(Network *network, unsigned int layer, unsigned int nrn_grp, double change_min, double change_max, double tau_min, double tau_max);

bool create_ps_pre_post_stdp_for_neuron(Neuron* neuron , double change_min, double change_max, double tau_min, double tau_max);
bool create_ps_post_pre_stdp_for_neuron(Neuron* neuron , double change_min, double change_max, double tau_min, double tau_max);

bool create_ps_pre_post_stdp_for_synapse(Neuron* neuron , double change_min, double change_max, double tau_min, double tau_max, unsigned int synapse);
bool create_ps_post_pre_stdp_for_synapse(Neuron* neuron , double change_min, double change_max, double tau_min, double tau_max, unsigned int synapse);

bool allocate_ps_pre_post_stdp_for_neuron(Neuron* neuron);
bool allocate_ps_post_pre_stdp_for_neuron(Neuron* neuron);

bool submit_new_ps_pre_post_stdp_vals_for_neuron(Neuron* neuron , double change_min, double change_max, double tau_min, double tau_max);

bool submit_new_ps_pre_post_stdp_vals_for_synapse(Neuron* neuron , double change_min, double change_max, double tau_min, double tau_max, unsigned int synapse);

bool submit_new_ps_pre_post_stdp_vals_for_neuron_according_to_pre_synaptic_neuron_group(Neuron* neuron , Network *axon_from_network, unsigned int axon_from_layer, unsigned int axon_from_nrn_grp, double change_min, double change_max, double tau_min, double tau_max);

#endif
