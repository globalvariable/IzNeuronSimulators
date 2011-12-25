#include "Neuron.h"

Neuron *create_neuron(double v, double a, double b, double c, double d, double I_inject, bool inhibitory, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory) 
{
	Neuron * nrn;
	nrn = g_new0 (Neuron, 1);
	if (nrn == NULL) 
	{	
		printf ("ERROR : Couldn't allocate neuron\n");
		return NULL;
	}
	if ((a<=0) || ((inhibitory!=0) && (inhibitory!=1)) ||(C<=0) ||  (k<=0) || (tau_excitatory<=0) || (tau_inhibitory<=0))
	{
		printf("ERROR : Invalid parameter submission\n");
		printf ("ERROR : Didn''t create neuron\n");
		return NULL;
	}	
	
	nrn->v = v - v_resting; 
	nrn->u = b * nrn->v;		
	nrn->a = a;
	nrn->b = b;
	nrn->c = c - v_resting;	
	nrn->d = d;
	nrn->k = k;	
	nrn->E = 1.0/C;
	nrn->v_resting = v_resting;			
	nrn->v_threshold = v_threshold - v_resting;
	nrn->v_peak = v_peak - v_resting;	
	nrn->I_inject = I_inject;
	nrn->inhibitory = inhibitory;
	nrn->E_excitatory = E_excitatory-v_resting;      // Vogels & Abbott 2005   eqtns 1,2
	nrn->E_inhibitory = E_inhibitory-v_resting;
	nrn->decay_rate_excitatory = -1.0/tau_excitatory;
	nrn->decay_rate_inhibitory = -1.0/tau_inhibitory;	
	nrn->conductance_excitatory = 0;
	nrn->conductance_inhibitory = 0;
	nrn->k_v_threshold = k * nrn->v_threshold;

	return nrn;
}


