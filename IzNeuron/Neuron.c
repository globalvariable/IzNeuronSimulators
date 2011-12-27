#include "Neuron.h"

bool initialize_neuron(Neuron *nrn, int layer, int neuron_group, int neuron_num, double v, double a, double b, double c, double d, double k, double C, double v_resting, double v_threshold, double v_peak, double I_inject, bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory) 
{
	if ((a<=0) || ((inhibitory!=0) && (inhibitory!=1)) ||(C<=0) ||  (k<=0) || (tau_excitatory<=0) || (tau_inhibitory<=0))
	{
		printf("ERROR : Invalid parameter submission for neuron initialization\n");
		return FALSE;
	}	
	nrn->layer = layer;
	nrn->neuron_group = neuron_group;
	nrn->neuron_num = neuron_num;
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

	return TRUE;
}

bool interrogate_neuron(int layer, int neuron_group, int neuron_num)
{
	Neuron		*ptr_neuron = NULL;

	ptr_neuron = get_neuron_address(layer, neuron_group, neuron_num);
	if (ptr_neuron == NULL)
		return FALSE;
	printf ("--------------Interrogating Neuron Dynamics ---------\n");		
	printf ("address:%u\n",  (unsigned int)ptr_neuron);	
	printf ("layer:%d\n",  ptr_neuron->layer);
	printf ("neuron group:%d\n",  ptr_neuron->neuron_group);	
	printf ("neuron num:%d\n",  ptr_neuron->neuron_num);
	printf ("v:\t%f\n",  ptr_neuron->v);
	printf ("u:\t%f\n",  ptr_neuron->u);	
	printf ("a:\t%f\n",  ptr_neuron->a);
	printf ("b:\t%f\n",  ptr_neuron->b);	
	printf ("c:\t%f\n",  ptr_neuron->c);
	printf ("d:\t%f\n",  ptr_neuron->d);					
	printf ("k:\t%f\n",  ptr_neuron->k);
	printf ("E:\t%f\n",  ptr_neuron->E);	
	printf ("v_resting:\t%f\n",  ptr_neuron->v_resting);
	printf ("v_threshold:\t%f\n",  ptr_neuron->v_threshold+ptr_neuron->v_resting);					
	printf ("v_peak:\t%f\n",  ptr_neuron->v_peak+ptr_neuron->v_resting );
	printf ("I_inject:\t%f\n",  ptr_neuron->I_inject);
	printf ("inhibitory:\t%d\n",  ptr_neuron->inhibitory);
	printf ("E_excitatory:\t%f\n",  ptr_neuron->E_excitatory);
	printf ("E_inhibitory:\t%f\n",  ptr_neuron->E_inhibitory);	
	printf ("decay_rate_excitatory:\t%f\n",  ptr_neuron->decay_rate_excitatory);
	printf ("decay_rate_inhibitory:\t%f\n",  ptr_neuron->decay_rate_inhibitory);					
	printf ("conductance_excitatory:\t%f\n",  ptr_neuron->conductance_excitatory);
	printf ("conductance_inhibitory:\t%f\n",  ptr_neuron->conductance_inhibitory);
	printf ("--------------Interrogating Neuron Dynamics Complete---------\n");		
}
