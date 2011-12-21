#include "Neuron.h"

neuron *create_neuron(double v, double a, double b, double c, double d, double I_inject, bool inhibitory, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory, int logging_buff_size)
{
	neuron * nrn;
	nrn = g_new0 (neuron, 1);
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

	nrn->axon_to = g_new0(neuron *, MAX_OUTPUT_PER_NEURON );
	nrn->axonal_delay = g_new0(unsigned int , MAX_OUTPUT_PER_NEURON );
	nrn->axonal_weight = g_new0(double , MAX_OUTPUT_PER_NEURON);

	nrn->event_time = g_new0(unsigned int,EVENT_BUFF_SIZE_PER_NEURON);
	nrn->event_from = g_new0(neuron * , EVENT_BUFF_SIZE_PER_NEURON);   
	nrn->event_weight = g_new0(double, EVENT_BUFF_SIZE_PER_NEURON);  

	nrn->E_pol_vals = g_new0(double,MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON +1);
	nrn->a_pol_vals = g_new0(double,MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON +1);
	nrn->conductance_decay_rate_excitatory_pol_vals = g_new0(double,MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON +1);
	nrn->conductance_decay_rate_inhibitory_pol_vals = g_new0(double,MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON +1);

	int i;
	for(i=0; i<MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON; i++)
	{
		nrn->E_pol_vals[i] = nrn->E/((double)(i+1));
		nrn->a_pol_vals[i] = nrn->a/((double)(i+1));
		nrn->conductance_decay_rate_excitatory_pol_vals[i] = nrn->decay_rate_excitatory/((double)(i+1));
		nrn->conductance_decay_rate_inhibitory_pol_vals[i] = nrn->decay_rate_inhibitory/((double)(i+1));
	}
	for(i=0;i<MAX_NUM_OF_VALUE_PER_NEURON_TO_LOG ; i++)
	{
		nrn->dynamics_log[i] = g_new0(double, logging_buff_size);
	}

	return nrn;
}

bool reset_neuron_params(neuron *nrn, double v, double a, double b, double c, double d, double I_inject, bool inhibitory, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory)
{

	int i;
	nrn->a = a;
	nrn->b = b;
	nrn->d = d;
	nrn->I_inject = I_inject;

	nrn->k = k;
	nrn->E = 1.0/C;
	nrn->v_resting = v_resting;
	
	nrn->decay_rate_excitatory = -1.0/tau_excitatory;
	nrn->decay_rate_inhibitory = -1.0/tau_inhibitory;
	nrn->conductance_excitatory = 0;
	nrn->conductance_inhibitory = 0;
	/*Initialise neuron structure, with voltages shifted relative to v_resting*/
//	nrn->v = v - v_resting;
	nrn->v = v;    // Retrieve 0 directly from the GUI
	nrn->u = nrn->b * nrn->v;
	nrn->v_threshold = v_threshold - v_resting;
	nrn->k_v_threshold = nrn->k * nrn->v_threshold ;
	nrn->c = c - v_resting;
	nrn->v_peak = v_peak - v_resting;
	nrn->E_excitatory = E_excitatory-v_resting;     
	nrn->E_inhibitory = E_inhibitory-v_resting;

	for (i=0;i<MAX_OUTPUT_PER_NEURON; i++)
	{
//		nrn->axon_to[i] = NULL;
//		nrn->axonal_delay[i] = 0;
		if (((nrn->inhibitory) && (!(inhibitory))) || ((!(nrn->inhibitory)) && (inhibitory)))
			nrn->axonal_weight[i] = (-1.0)* nrn->axonal_weight[i] ;
	}
	nrn->inhibitory = inhibitory;  
	if (nrn->inhibitory)
		printf("Neuron is now inhibitory\n");
	else
		printf("Neuron is now excitatory\n");		
	for (i=0;i<EVENT_BUFF_SIZE_PER_NEURON; i++)
	{
		nrn->event_time[i] = 0;
		nrn->event_from[i] = NULL;   
		nrn->event_weight[i] = 0;
	}  
	nrn->event_buffer_write_idx = 0;
	nrn->num_of_event = 0;

	for (i=0;i<MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON; i++)
	{
		nrn->E_pol_vals[i] = (nrn->E)/((double)(i+1));
		nrn->a_pol_vals[i] = (nrn->a)/((double)(i+1));
		nrn->conductance_decay_rate_excitatory_pol_vals[i] = (nrn->decay_rate_excitatory)/((double)(i+1));
		nrn->conductance_decay_rate_inhibitory_pol_vals[i] = (nrn->decay_rate_inhibitory)/((double)(i+1));
	}
	
	printf("Neuron parameters were reset\n");
	printf("Neuron connections were reserved\n");

	return TRUE;
}


void clear_neuron_event_buffer(neuron *nrn)
{
	int i;
	for (i=0;i<EVENT_BUFF_SIZE_PER_NEURON; i++)
	{
		nrn->event_time[i] = 0;
		nrn->event_from[i] = NULL;   
		nrn->event_weight[i] = 0;
	}  
	nrn->event_buffer_write_idx = 0;
	nrn->num_of_event = 0;	
	return;
}
