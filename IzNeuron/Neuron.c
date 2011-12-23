#include "Neuron.h"

Neuron *create_neuron(double v, double a, double b, double c, double d, double I_inject, bool inhibitory, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory, int logging_buff_size)
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
	nrn->a = a;
	nrn->b = b;
	nrn->d = d;
	nrn->I_inject = I_inject;
	nrn->inhibitory = inhibitory;
	nrn->k = k;
	nrn->E = 1.0/C;
	nrn->v_resting = v_resting;
	
	nrn->decay_rate_excitatory = -1.0/tau_excitatory;
	nrn->decay_rate_inhibitory = -1.0/tau_inhibitory;
	/*Initialise neuron structure, with voltages shifted relative to v_resting*/
//	nrn->v = v - v_resting;
	nrn->v = v;    // Retrieve 0 directly from the GUI
	nrn->u = nrn->b * nrn->v;
	nrn->v_threshold = v_threshold - v_resting;
	nrn->k_v_threshold = nrn->k * nrn->v_threshold ;
	nrn->c = c - v_resting;
	nrn->v_peak = v_peak - v_resting;
	nrn->E_excitatory = E_excitatory-v_resting;      // Vogels & Abbott 2005   eqtns 1,2
	nrn->E_inhibitory = E_inhibitory-v_resting;

	int i;
	for(i=0; i<MAX_PARKER_SOCHACKI_ITERATION_PER_NEURON; i++)
	{
		nrn->ps_vals.E_pol_vals[i] = nrn->E/((double)(i+1));
		nrn->ps_vals.a_pol_vals[i] = nrn->a/((double)(i+1));
		nrn->ps_vals.conductance_decay_rate_excitatory_pol_vals[i] = nrn->decay_rate_excitatory/((double)(i+1));
		nrn->ps_vals.conductance_decay_rate_inhibitory_pol_vals[i] = nrn->decay_rate_inhibitory/((double)(i+1));
	}
	return nrn;
}


