#include "Neuron.h"

bool initialize_iz_neuron_params(Neuron *nrn, Network *network, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, double a, double b, double c, double d, double k, double C, double v_resting, double v_threshold, double v_peak,bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory)
{
	if ((a<0) || ((inhibitory < 0) || (inhibitory > 1)) || (C<=0) ||  (k<0) || (tau_excitatory<=0) || (tau_inhibitory<=0))
	{
		printf("Neuron: ERROR : Invalid parameter submission for neuron initialization\n");
		printf("Neuron: ERROR : a = %f, inhibitory = %d, C = %f, k = %f, tau_excitatory = %f, tau_inhibitory = %f\n", a, inhibitory, C, k, tau_excitatory, tau_inhibitory);		
		return FALSE;
	}
	nrn->network = network;
	nrn->layer = layer;
	nrn->neuron_group = neuron_group;
	nrn->neuron_num = neuron_num;
	if (nrn->iz_params != NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Neuron", "initialize_neuron_params", "nrn->iz_params was allocated before. Re-use of initialize_neuron_params");	
	nrn->iz_params = g_new0(IzNeuronParams,1);
	nrn->iz_params->a = a;
	nrn->iz_params->b = b;
	nrn->iz_params->c = c - v_resting;	
	nrn->iz_params->d = d;
	nrn->iz_params->k = k;	
	nrn->iz_params->E = 1.0/C;
	nrn->iz_params->v_resting = v_resting;			
	nrn->iz_params->v_threshold = v_threshold - v_resting;
	nrn->iz_params->v_peak = v_peak - v_resting;	
	nrn->iz_params->E_excitatory = E_excitatory-v_resting;      // Vogels & Abbott 2005   eqtns 1,2
	nrn->iz_params->E_inhibitory = E_inhibitory-v_resting;
	nrn->iz_params->decay_rate_excitatory = -1.0/tau_excitatory;
	nrn->iz_params->decay_rate_inhibitory = -1.0/tau_inhibitory;	
	nrn->iz_params->conductance_excitatory = 0;
	nrn->iz_params->conductance_inhibitory = 0;
	nrn->iz_params->k_v_threshold = k * nrn->iz_params->v_threshold;
	nrn->inhibitory = inhibitory;
	if (nrn->axon_list != NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Neuron", "initialize_neuron_params", "nrn->axon_list was allocated before. Re-use of initialize_neuron_params");	
	nrn->axon_list = g_new0(NeuronAxonList,1);
	if (nrn->ps_vals != NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Neuron", "initialize_neuron_params", "nrn->ps_vals was allocated before. Re-use of initialize_neuron_params");		
	nrn->ps_vals = g_new0(ParkerSochackiPolynomialVals,1);
	nrn->syn_list = g_new0(SynapseList,1);


	if (nrn->sorted_event_buffer != NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Neuron", "initialize_neuron_params", "nrn->sorted_event_buffer  was allocated before. Re-use of initialize_neuron_params");	
	nrn->sorted_event_buffer = g_new0(NeuronSortedEventBuffer, 1);


	return TRUE;
}

bool initialize_neuron_node(Neuron *nrn, Network *network, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, bool inhibitory)	// to represent blue spike neurons to connect to in silico network
{
	nrn->network = network;
	nrn->layer = layer;
	nrn->neuron_group = neuron_group;
	nrn->neuron_num = neuron_num;
	nrn->inhibitory = inhibitory;
	if (nrn->axon_list != NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Neuron", "initialize_neuron_params", "nrn->axon_list was allocated before. Re-use of initialize_neuron_params");	
	nrn->axon_list = g_new0(NeuronAxonList,1);
	nrn->syn_list = g_new0(SynapseList,1);

	if (nrn->sorted_event_buffer != NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Neuron", "initialize_neuron_params", "nrn->sorted_event_buffer  was allocated before. Re-use of initialize_neuron_params");	
	nrn->sorted_event_buffer = g_new0(NeuronSortedEventBuffer, 1);


	return TRUE;
}

bool initialize_poisson_neuron(Neuron *nrn, Network *network, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, bool inhibitory, double firing_rate)
{
	nrn->network = network;
	nrn->layer = layer;
	nrn->neuron_group = neuron_group;
	nrn->neuron_num = neuron_num;
	nrn->inhibitory = inhibitory;
	if (nrn->axon_list != NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Neuron", "initialize_neuron_params", "nrn->axon_list was allocated before. Re-use of initialize_neuron_params");	
	nrn->axon_list = g_new0(NeuronAxonList,1);
	if (nrn->poisson_params != NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Neuron", "initialize_neuron_params", "nrn->poisson_params was allocated before. Re-use of initialize_neuron_params");		
	nrn->poisson_params = g_new0(PoissonNeuronParams,1);
	nrn->poisson_params->firing_rate = firing_rate;
	nrn->syn_list = g_new0(SynapseList,1);	// so that num of synapses will be zero.
	return TRUE;
}

bool interrogate_neuron(Network *network, int layer, int neuron_group, int neuron_num)
{
	Neuron		*ptr_neuron = NULL;
	IzNeuronParams				*ptr_iz_params;
	NeuronAxonList	*ptr_neuron_axon_list;
	ParkerSochackiPolynomialVals	*ptr_ps_vals;	
	SynapseList					*ptr_syn_list;
	int i;
	
	
	ptr_neuron = get_neuron_address(network, layer, neuron_group, neuron_num);
	if (ptr_neuron == NULL)
	{
		printf("Neuron: ERROR: Neuron to interrogate was not found (Layer: %d Neuron_Group: %d Neuron: %d)\n", layer, neuron_group, neuron_num);
		return FALSE;
	}
	ptr_iz_params = ptr_neuron->iz_params;
	ptr_neuron_axon_list = ptr_neuron->axon_list;
	ptr_ps_vals = ptr_neuron->ps_vals;
	ptr_syn_list = ptr_neuron->syn_list;
					

	printf ("address:%llu\n",  (NeuronAddress)ptr_neuron);	
	printf ("layer:%u\n",  ptr_neuron->layer);
	printf ("neuron group:%u\n",  ptr_neuron->neuron_group);	
	printf ("neuron num:%u\n",  ptr_neuron->neuron_num);
	printf ("inhibitory:\t%d\n",  ptr_neuron->inhibitory);
	printf ("--------------Interrogating Izhikevich Neuron Dynamics ---------\n");		
	if (ptr_iz_params != NULL)
	{
		printf ("v:\t%f\n",  ptr_iz_params->v+ptr_iz_params->v_resting);
		printf ("u:\t%f\n",  ptr_iz_params->u);	
		printf ("a:\t%f\n",  ptr_iz_params->a);
		printf ("b:\t%f\n",  ptr_iz_params->b);	
		printf ("c:\t%f\n",  ptr_iz_params->c+ptr_iz_params->v_resting);
		printf ("d:\t%f\n",  ptr_iz_params->d);					
		printf ("k:\t%f\n",  ptr_iz_params->k);
		printf ("E:\t%f\n",  ptr_iz_params->E);	
		printf ("v_resting:\t%f\n",  ptr_iz_params->v_resting);
		printf ("v_threshold:\t%f\n",  ptr_iz_params->v_threshold+ptr_iz_params->v_resting);					
		printf ("v_peak:\t\t%f\n",  ptr_iz_params->v_peak+ptr_iz_params->v_resting );
		printf ("I_inject:\t%f\n",  ptr_iz_params->I_inject);
		printf ("E_excitatory:\t%f\n",  ptr_iz_params->E_excitatory+ptr_iz_params->v_resting);
		printf ("E_inhibitory:\t%f\n",  ptr_iz_params->E_inhibitory+ptr_iz_params->v_resting);	
		printf ("decay_rate_excitatory:\t%f\n",  ptr_iz_params->decay_rate_excitatory);
		printf ("decay_rate_inhibitory:\t%f\n",  ptr_iz_params->decay_rate_inhibitory);					
		printf ("conductance_excitatory:\t%f\n",  ptr_iz_params->conductance_excitatory);
		printf ("conductance_inhibitory:\t%f\n",  ptr_iz_params->conductance_inhibitory);
	}
	printf ("--------------Interrogating Neuron Dynamics...Complete---------\n");
	
	printf ("--------------Interrogating Event Buffer ---------\n");		
/*	if (ptr_neuron_event_buffer != NULL)
	{ 
		printf ("Event buffer size: %d\n", ptr_neuron_event_buffer->buff_size);
		printf ("Write Index: %d\n", ptr_neuron_event_buffer->write_idx);		
		printf ("Read Index: %d\n", ptr_neuron_event_buffer->read_idx);
		printf ("Event TimeStamp / Event From / Synaptic Weight\n");		
		for (i = 0; i < ptr_neuron_event_buffer->buff_size; i++)
		{
			pthread_mutex_lock(&(ptr_neuron_event_buffer->mutex));
			printf ("%llu\t\t", ptr_neuron_event_buffer->time[i]);
			printf ("%u\t\t",  ptr_neuron_event_buffer->syn_idx[i]);	
			pthread_mutex_unlock(&(ptr_neuron_event_buffer->mutex));	
		}
	}
	else
	{
		printf ("Event buffer is not allocated yet\n");			
	}		
*/	printf ("--------------Interrogating Event Buffer...Complete ---------\n");							
	printf ("--------------Interrogating Synapse List ---------\n");		
	if (ptr_syn_list != NULL)
	{
		printf ("Number of Synapses: %d\n", ptr_syn_list->num_of_synapses);
		printf ("Synapse Type / Weight\n");
		for (i = 0; i < ptr_syn_list->num_of_synapses; i++)
		{
			if (ptr_syn_list->synapses[i].type == EXCITATORY_SYNAPSE)
				printf ("EXCI\t");
			else if (ptr_syn_list->synapses[i].type == INHIBITORY_SYNAPSE)
				printf ("INHI\t");
			else
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "Neuron", "interrogate_neuron", "Invalid synapse type.");	
			printf ("%f\n", ptr_syn_list->synapses[i].weight);		
		}		
	}
	else
	{
		printf ("Synapse list is not allocated yet\n");					
	}
	printf ("--------------Interrogating Synapse List... Complete ---------\n");				

	printf ("--------------Interrogating Axon List ---------\n");				
	if (ptr_neuron_axon_list != NULL)
	{
		printf ("Number of Axons: %d\n", ptr_neuron_axon_list->num_of_axons);
		printf ("Axon To / Axonal Delay\n");		
		for (i = 0; i < ptr_neuron_axon_list->num_of_axons; i++)
		{
			printf ("%llu\t\t", (NeuronAddress) ptr_neuron_axon_list->to[i]);
			printf ("%u\t\t", (AxonalDelay) ptr_neuron_axon_list->delay[i]);		
		}
	}
	else
	{
		printf ("Axon list is not allocated yet\n");					
	}
	printf ("--------------Interrogating Axon List....Complete ---------\n");				
		
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

bool submit_new_iz_neuron_params(Network *network, Neuron *nrn, double a, double b, double c, double d, double k, double C, double v_resting, double v_threshold, double v_peak, bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory) 
{
	IzNeuronParams	*ptr_iz_params;
	ptr_iz_params = nrn->iz_params;
	if ((a<0) || ((inhibitory < 0) || (inhibitory > 1)) || (C<=0) ||  (k<0) || (tau_excitatory<=0) || (tau_inhibitory<=0))
	{
		printf("Neuron: ERROR : Invalid parameter submission for neuron initialization\n");
		printf("Neuron: ERROR : a = %f, inhibitory = %d, C = %f, k = %f, tau_excitatory = %f, tau_inhibitory = %f\n", a, inhibitory, C, k, tau_excitatory, tau_inhibitory);		
		return FALSE;
	}	
	nrn->inhibitory = inhibitory;
	ptr_iz_params->v = 0; 
	ptr_iz_params->u = 0;		
	ptr_iz_params->a = a;
	ptr_iz_params->b = b;
	ptr_iz_params->c = c - v_resting;	
	ptr_iz_params->d = d;
	ptr_iz_params->k = k;	
	ptr_iz_params->E = 1.0/C;
	ptr_iz_params->v_resting = v_resting;			
	ptr_iz_params->v_threshold = v_threshold - v_resting;
	ptr_iz_params->v_peak = v_peak - v_resting;	
	ptr_iz_params->E_excitatory = E_excitatory-v_resting;      // Vogels & Abbott 2005   eqtns 1,2
	ptr_iz_params->E_inhibitory = E_inhibitory-v_resting;
	ptr_iz_params->decay_rate_excitatory = -1.0/tau_excitatory;
	ptr_iz_params->decay_rate_inhibitory = -1.0/tau_inhibitory;	
	ptr_iz_params->conductance_excitatory = 0;
	ptr_iz_params->conductance_inhibitory = 0;
	ptr_iz_params->k_v_threshold = k * ptr_iz_params->v_threshold;
	if (!parker_sochacki_set_order_tolerance(network, get_maximum_parker_sochacki_order(), get_maximum_parker_sochacki_error_tolerance()))
		return FALSE;
	return TRUE;
}

bool inject_current_to_neuron(Neuron *nrn, double I_inject)
{
	if (nrn == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Neuron", "inject_current_to_neuron", "neuron was not allocated.");
	nrn->iz_params->I_inject = I_inject;
	return TRUE;
}

bool get_iz_neuron_parameters(Neuron *neuron, double *a, double *b, double *c, double *d, double *k, double *C, double *v_resting, double *v_threshold, double *v_peak, bool *inhibitory, double *E_excitatory, double *E_inhibitory, double *tau_excitatory, double *tau_inhibitory)
{
	IzNeuronParams	*ptr_iz_params;			
	if (neuron == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Neuron", "get_iz_neuron_parameters", "neuron == NULL.");
	ptr_iz_params = neuron->iz_params;
	if (ptr_iz_params == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Neuron", "get_iz_neuron_parameters", "iz_params == NULL.");
	*inhibitory = neuron->inhibitory;
	*a = ptr_iz_params->a;
	*b = ptr_iz_params->b;
	*c = ptr_iz_params->c + ptr_iz_params->v_resting;
	*d = ptr_iz_params->d;
	*k = ptr_iz_params->k;
	*C = 1/ptr_iz_params->E;
	*v_resting = ptr_iz_params->v_resting;
	*v_threshold = ptr_iz_params->v_threshold + ptr_iz_params->v_resting;
	*v_peak = ptr_iz_params->v_peak + ptr_iz_params->v_resting;
	*E_excitatory =  ptr_iz_params->E_excitatory + ptr_iz_params->v_resting;
	*E_inhibitory =  ptr_iz_params->E_inhibitory + ptr_iz_params->v_resting;
	*tau_excitatory =  -1.0/ptr_iz_params->decay_rate_excitatory;
	*tau_inhibitory =  -1.0/ptr_iz_params->decay_rate_inhibitory;
	return TRUE;
}
