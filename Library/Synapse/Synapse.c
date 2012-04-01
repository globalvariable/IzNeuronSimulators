#include "Synapse.h"

bool create_synapse(Neuron *this_neuron, Neuron *target_neuron, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, SynapticDelay EPSP_delay_min, SynapticDelay EPSP_delay_max, SynapticDelay IPSP_delay_min, SynapticDelay IPSP_delay_max, SynapticDelay delay_hard_min, SynapticDelay delay_hard_max, double excitatory_connection_probability, double inhibitory_connection_probability, bool *did_connection)	
{

	unsigned int i;
	NeuronSynapseList	*ptr_neuron_synapse_list;
	Neuron				**to = NULL;
	SynapticDelay			*delay = NULL;
	SynapticWeight		*weight = NULL;	
	*did_connection = FALSE;
	if (EPSP_delay_min>EPSP_delay_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "EPSP_delay_min>EPSP_delay_max.");	 
	if (IPSP_delay_min>IPSP_delay_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "IPSP_delay_min>IPSP_delay_max.");	
	if (weight_excitatory_min > weight_excitatory_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "weight_excitatory_min > weight_excitatory_max.");	
 	if (weight_inhibitory_min > weight_inhibitory_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "weight_inhibitory_min > weight_inhibitory_max.");	
 	if (weight_excitatory_min<=0)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "weight_excitatory_min<=0.");	
 	if (weight_inhibitory_min<=0)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "weight_excitatory_min<=0.");	
 	if (weight_excitatory_max<=0)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "weight_excitatory_max<=0.");	
 	if (weight_inhibitory_max<=0)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "weight_excitatory_max<=0.");	
 	if (EPSP_delay_min < delay_hard_min)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "EPSP_delay_min < delay_hard_min.");	
 	if (EPSP_delay_max > delay_hard_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "EPSP_delay_max > delay_hard_max.");	
 	if (IPSP_delay_min < delay_hard_min)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "IPSP_delay_min < delay_hard_min.");	
 	if (IPSP_delay_max > delay_hard_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "IPSP_delay_max < delay_hard_max.");	

	if (this_neuron->inhibitory)
	{
		if (   ( rand() /  ((double) RAND_MAX))  <= inhibitory_connection_probability )
		{	
			ptr_neuron_synapse_list = this_neuron->syn_list;

			to = g_new0(Neuron *,ptr_neuron_synapse_list->num_of_synapses+1);
			delay = g_new0(SynapticDelay,ptr_neuron_synapse_list->num_of_synapses+1);
			weight = g_new0(SynapticWeight,ptr_neuron_synapse_list->num_of_synapses+1);
	
			for (i = 0; i < ptr_neuron_synapse_list->num_of_synapses; i++)
			{
				to[i] = ptr_neuron_synapse_list->to[i];
				delay[i] = ptr_neuron_synapse_list->delay[i];		
				weight[i] = ptr_neuron_synapse_list->weight[i];			
			}
			g_free(ptr_neuron_synapse_list->to);
			g_free(ptr_neuron_synapse_list->delay);		
			g_free(ptr_neuron_synapse_list->weight);	
	
			ptr_neuron_synapse_list->to = to;
			ptr_neuron_synapse_list->delay = delay;		
			ptr_neuron_synapse_list->weight = weight;	
	
			ptr_neuron_synapse_list->to[i] = target_neuron;
			ptr_neuron_synapse_list->delay[i]  = (SynapticDelay)((IPSP_delay_max-IPSP_delay_min) * (rand()/((double)RAND_MAX))) + IPSP_delay_min; 
			ptr_neuron_synapse_list->weight[i]  = -( (weight_inhibitory_max-weight_inhibitory_min) * (rand()/ ((double)RAND_MAX) )) - weight_inhibitory_min;
	
			ptr_neuron_synapse_list->num_of_synapses++;
			if (!increase_neuron_event_buffer_size(target_neuron, (ptr_neuron_synapse_list->delay[i]/1000000)+1))
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "! increment_neuron_event_buffer_size().");
			*did_connection = TRUE;		
		}
	}
	else
	{
		if (   ( rand() /  ((double) RAND_MAX))  <= excitatory_connection_probability )
		{	
			ptr_neuron_synapse_list = this_neuron->syn_list;

			to = g_new0(Neuron *,ptr_neuron_synapse_list->num_of_synapses+1);
			delay = g_new0(SynapticDelay,ptr_neuron_synapse_list->num_of_synapses+1);
			weight = g_new0(SynapticWeight,ptr_neuron_synapse_list->num_of_synapses+1);
	
			for (i = 0; i < ptr_neuron_synapse_list->num_of_synapses; i++)
			{
				to[i] = ptr_neuron_synapse_list->to[i];
				delay[i] = ptr_neuron_synapse_list->delay[i];		
				weight[i] = ptr_neuron_synapse_list->weight[i];			
			}
			g_free(ptr_neuron_synapse_list->to);
			g_free(ptr_neuron_synapse_list->delay);		
			g_free(ptr_neuron_synapse_list->weight);	
	
			ptr_neuron_synapse_list->to = to;
			ptr_neuron_synapse_list->delay = delay;		
			ptr_neuron_synapse_list->weight = weight;	
	
			ptr_neuron_synapse_list->to[i] = target_neuron;
			ptr_neuron_synapse_list->delay[i]  = (SynapticDelay)((EPSP_delay_max-EPSP_delay_min) * (rand()/((double)RAND_MAX))) + EPSP_delay_min; 
			ptr_neuron_synapse_list->weight[i] = (weight_excitatory_max-weight_excitatory_min) * (rand()/ ((double)RAND_MAX) ) + weight_excitatory_min;
				
			ptr_neuron_synapse_list->num_of_synapses++;
			if (!increase_neuron_event_buffer_size(target_neuron, (ptr_neuron_synapse_list->delay[i]/1000000)+1))
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "create_synapse", "! increment_neuron_event_buffer_size().");
			*did_connection = TRUE;	

			printf ("%u\n", ptr_neuron_synapse_list->delay[i]);	
		}
	}
	return TRUE;
}

void destroy_neuron_synapse_list(Neuron *neuron)
{
	NeuronSynapseList	*ptr_neuron_syn_list;
	
	ptr_neuron_syn_list = neuron->syn_list;	
	
	g_free(ptr_neuron_syn_list->to);
	g_free(ptr_neuron_syn_list->delay);	
 	g_free(ptr_neuron_syn_list->weight);
 	g_free(ptr_neuron_syn_list);	

	return;
}


