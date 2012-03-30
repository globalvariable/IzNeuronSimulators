#include "Neuron.h"

bool initialize_neuron_params(Neuron *nrn, int layer, int neuron_group, int neuron_num, double a, double b, double c, double d, double k, double C, double v_resting, double v_threshold, double v_peak,bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory) 
{
	if ((a<0) || ((inhibitory < 0) || (inhibitory > 1)) || (C<=0) ||  (k<0) || (tau_excitatory<=0) || (tau_inhibitory<=0))
	{
		printf("Neuron: ERROR : Invalid parameter submission for neuron initialization\n");
		printf("Neuron: ERROR : a = %f, inhibitory = %d, C = %f, k = %f, tau_excitatory = %f, tau_inhibitory = %f\n", a, inhibitory, C, k, tau_excitatory, tau_inhibitory);		
		return FALSE;
	}	
	nrn->layer = layer;
	nrn->neuron_group = neuron_group;
	nrn->neuron_num = neuron_num;
	nrn->a = a;
	nrn->b = b;
	nrn->c = c - v_resting;	
	nrn->d = d;
	nrn->k = k;	
	nrn->E = 1.0/C;
	nrn->v_resting = v_resting;			
	nrn->v_threshold = v_threshold - v_resting;
	nrn->v_peak = v_peak - v_resting;	
	nrn->inhibitory = inhibitory;
	nrn->E_excitatory = E_excitatory-v_resting;      // Vogels & Abbott 2005   eqtns 1,2
	nrn->E_inhibitory = E_inhibitory-v_resting;
	nrn->decay_rate_excitatory = -1.0/tau_excitatory;
	nrn->decay_rate_inhibitory = -1.0/tau_inhibitory;	
	nrn->conductance_excitatory = 0;
	nrn->conductance_inhibitory = 0;
	nrn->k_v_threshold = k * nrn->v_threshold;
	if (nrn->syn_list != NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "Neuron", "initialize_neuron_params", "nrn->syn_list was allocated before. Re-use of initialize_neuron_params");	
	nrn->syn_list = g_new0(NeuronSynapseList,1);
	if (nrn->event_buff != NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "Neuron", "initialize_neuron_params", "nrn->event_buff was allocated before. Re-use of initialize_neuron_params");		
	nrn->event_buff = g_new0(NeuronEventBuffer,1);	
	pthread_mutex_init(&(nrn->event_buff->mutex), NULL);
	if (!increment_neuron_event_buffer_size(nrn))	// if only one connection buffer write idx will be reset to zero(refer to neuron_event_buffer->write_idx++)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "! increment_neuron_event_buffer_size().");
	if (nrn->ps_vals != NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "Neuron", "initialize_neuron_params", "nrn->ps_vals was allocated before. Re-use of initialize_neuron_params");		
	nrn->ps_vals = g_new0(ParkerSochackiPolynomialVals,1);
	return TRUE;
}

bool interrogate_neuron(Network *network, int layer, int neuron_group, int neuron_num)
{
	Neuron		*ptr_neuron = NULL;
	NeuronEventBuffer *ptr_neuron_event_buffer;
	NeuronSynapseList	*ptr_neuron_syn_list;
	ParkerSochackiPolynomialVals	*ptr_ps_vals;	
	int i;
	
	
	ptr_neuron = get_neuron_address(network, layer, neuron_group, neuron_num);
	if (ptr_neuron == NULL)
	{
		printf("Neuron: ERROR: Neuron to interrogate was not found (Layer: %d Neuron_Group: %d Neuron: %d)\n", layer, neuron_group, neuron_num);
		return FALSE;
	}
	ptr_neuron_event_buffer = ptr_neuron->event_buff;		
	ptr_neuron_syn_list = ptr_neuron->syn_list;
	ptr_ps_vals = ptr_neuron->ps_vals;
					
	printf ("--------------Interrogating Neuron Dynamics ---------\n");		
	printf ("address:%lu\n",  (NeuronAddress)ptr_neuron);	
	printf ("layer:%d\n",  ptr_neuron->layer);
	printf ("neuron group:%d\n",  ptr_neuron->neuron_group);	
	printf ("neuron num:%d\n",  ptr_neuron->neuron_num);
	printf ("v:\t%f\n",  ptr_neuron->v+ptr_neuron->v_resting);
	printf ("u:\t%f\n",  ptr_neuron->u);	
	printf ("a:\t%f\n",  ptr_neuron->a);
	printf ("b:\t%f\n",  ptr_neuron->b);	
	printf ("c:\t%f\n",  ptr_neuron->c+ptr_neuron->v_resting);
	printf ("d:\t%f\n",  ptr_neuron->d);					
	printf ("k:\t%f\n",  ptr_neuron->k);
	printf ("E:\t%f\n",  ptr_neuron->E);	
	printf ("v_resting:\t%f\n",  ptr_neuron->v_resting);
	printf ("v_threshold:\t%f\n",  ptr_neuron->v_threshold+ptr_neuron->v_resting);					
	printf ("v_peak:\t\t%f\n",  ptr_neuron->v_peak+ptr_neuron->v_resting );
	printf ("I_inject:\t%f\n",  ptr_neuron->I_inject);
	printf ("inhibitory:\t%d\n",  ptr_neuron->inhibitory);
	printf ("E_excitatory:\t%f\n",  ptr_neuron->E_excitatory+ptr_neuron->v_resting);
	printf ("E_inhibitory:\t%f\n",  ptr_neuron->E_inhibitory+ptr_neuron->v_resting);	
	printf ("decay_rate_excitatory:\t%f\n",  ptr_neuron->decay_rate_excitatory);
	printf ("decay_rate_inhibitory:\t%f\n",  ptr_neuron->decay_rate_inhibitory);					
	printf ("conductance_excitatory:\t%f\n",  ptr_neuron->conductance_excitatory);
	printf ("conductance_inhibitory:\t%f\n",  ptr_neuron->conductance_inhibitory);
	printf ("--------------Interrogating Neuron Dynamics...Complete---------\n");
	
	printf ("--------------Interrogating Event Buffer ---------\n");		
	if (ptr_neuron_event_buffer != NULL)
	{ 
		printf ("Event buffer size: %d\n", ptr_neuron_event_buffer->buff_size);
		printf ("Write Index: %d\n", ptr_neuron_event_buffer->write_idx);		
		printf ("Read Index: %d\n", ptr_neuron_event_buffer->read_idx);
		printf ("Event TimeStamp / Event From / Synaptic Weight\n");		
		for (i = 0; i < ptr_neuron_event_buffer->buff_size; i++)
		{
			printf ("%llu\t\t", ptr_neuron_event_buffer->time[i]);
			printf ("%lu\t\t", (NeuronAddress) ptr_neuron_event_buffer->from[i]);		
			printf ("%.5f\n", ptr_neuron_event_buffer->weight[i]);				
		}
	}
	else
	{
		printf ("Event buffer is not allocated yet\n");			
	}		
	printf ("--------------Interrogating Event Buffer...Complete ---------\n");							

	printf ("--------------Interrogating Synapse List ---------\n");				
	if (ptr_neuron_syn_list != NULL)
	{
		printf ("Number of Synapses: %d\n", ptr_neuron_syn_list->num_of_synapses);
		printf ("Synapse To / Synaptic Delay / Synaptic Weight\n");		
		for (i = 0; i < ptr_neuron_event_buffer->buff_size; i++)
		{
			printf ("%lu\t\t", (NeuronAddress) ptr_neuron_syn_list->to[i]);
			printf ("%u\t\t", (SynapticDelay) ptr_neuron_syn_list->delay[i]);		
			printf ("%.5f\n", ptr_neuron_syn_list->weight[i]);				
		}
	}
	else
	{
		printf ("Synapse list is not allocated yet\n");					
	}
	printf ("--------------Interrogating Synapse List....Complete ---------\n");				
		
	printf ("--------------   Interrogating Parker Sochacki Ploynomial Values ---------\n");
	if (ptr_ps_vals != NULL)	
	{				
		printf ("Maximum ParkerSochacki Order: %d\n", get_maximum_parker_sochacki_order());
		printf ("Order equals to zero shows maximum order that the iterations reached.\n");		
		printf ("v / u / excitatory_conductance / inhibitory_conductance / chi / E / a / excitatory_conductance_decay_rate / inhibitory_conductance_decay_rate\n");		
		for (i = 0; i < get_maximum_parker_sochacki_order(); i++)
		{
			printf("%.5f\t\t", ptr_ps_vals->v_pol_vals[i]); 
			printf("%.5f\t\t", ptr_ps_vals->u_pol_vals[i]);
			printf("%.5f\t\t", ptr_ps_vals->conductance_excitatory_pol_vals[i]);
			printf("%.5f\t\t", ptr_ps_vals->conductance_inhibitory_pol_vals[i]); 
			printf("%.5f\t\t", ptr_ps_vals->chi_pol_vals[i]); 
			printf("%.5f\t\t", ptr_ps_vals->E_pol_vals[i]);
			printf("%.5f\t\t", ptr_ps_vals->a_pol_vals[i]); 
			printf("%.5f\t\t", ptr_ps_vals->conductance_decay_rate_excitatory_pol_vals[i]); 
			printf("%.5f\n", ptr_ps_vals->conductance_decay_rate_inhibitory_pol_vals[i]); 			
		}
	}
	else
	{
		printf ("Parker-Sochacki Polynomials were not allocated yet\n");							
	}
	printf ("-------------- Interrogating Parker Sochacki Ploynomial Values...Complete ---------\n");				
		
	return TRUE;
				
}

bool submit_new_neuron_params(Network *network, Neuron *nrn, double v, double a, double b, double c, double d, double k, double C, double v_resting, double v_threshold, double v_peak, double I_inject, bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory) 
{
	if ((a<0) || ((inhibitory < 0) || (inhibitory > 1)) || (C<=0) ||  (k<0) || (tau_excitatory<=0) || (tau_inhibitory<=0))
	{
		printf("Neuron: ERROR : Invalid parameter submission for neuron initialization\n");
		printf("Neuron: ERROR : a = %f, inhibitory = %d, C = %f, k = %f, tau_excitatory = %f, tau_inhibitory = %f\n", a, inhibitory, C, k, tau_excitatory, tau_inhibitory);		
		return FALSE;
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
	if (!parker_sochacki_set_order_tolerance(network, get_maximum_parker_sochacki_order(), get_maximum_parker_sochacki_error_tolerance()))
		return FALSE;
	return TRUE;
}

bool inject_current_to_neuron(Neuron *nrn, double I_inject)
{
	if (nrn == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "Neuron", "inject_current_to_neuron", "neuron was not allocated.");
	nrn->I_inject = I_inject;
	return TRUE;
}
