#include "Axon.h"

bool create_axon(Neuron *this_neuron, Neuron *target_neuron, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, AxonalDelay EPSP_delay_min, AxonalDelay EPSP_delay_max, AxonalDelay IPSP_delay_min, AxonalDelay IPSP_delay_max, AxonalDelay delay_hard_min, AxonalDelay delay_hard_max, double excitatory_connection_probability, double inhibitory_connection_probability, bool *did_connection)
{

	unsigned int i;
	NeuronAxonList	*ptr_neuron_axon_list;
	NeuronSynapseList	*ptr_post_neuron_synapse_list;   // post synaptic neurons synapses
	Neuron				**to = NULL;
	AxonalDelay			*delay = NULL;
	SynapseIndex			*syn_idx = NULL;
	SynapticWeight		*weight = NULL;
	SynapseType			*type = NULL;
	*did_connection = FALSE;
	if (EPSP_delay_min>EPSP_delay_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "EPSP_delay_min>EPSP_delay_max.");	 
	if (IPSP_delay_min>IPSP_delay_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "IPSP_delay_min>IPSP_delay_max.");	
 	if (EPSP_delay_min < delay_hard_min)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "EPSP_delay_min < delay_hard_min.");	
 	if (EPSP_delay_max > delay_hard_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "EPSP_delay_max > delay_hard_max.");	
 	if (IPSP_delay_min < delay_hard_min)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "IPSP_delay_min < delay_hard_min.");	
 	if (IPSP_delay_max > delay_hard_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "IPSP_delay_max < delay_hard_max.");
	if (weight_excitatory_min > weight_excitatory_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "weight_excitatory_min > weight_excitatory_max.");
	if (weight_inhibitory_min > weight_inhibitory_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "weight_inhibitory_min > weight_inhibitory_max.");
	if (weight_excitatory_min<=0)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "weight_excitatory_min<=0.");
	if (weight_inhibitory_min<=0)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "weight_excitatory_min<=0.");
	if (weight_excitatory_max<=0)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "weight_excitatory_max<=0.");
	if (weight_inhibitory_max<=0)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "weight_excitatory_max<=0.");
 	

	if (this_neuron->inhibitory)
	{
		if (   ( rand() /  ((double) RAND_MAX))  <= inhibitory_connection_probability )
		{	
			ptr_neuron_axon_list = this_neuron->axon_list;
			ptr_post_neuron_synapse_list = target_neuron->syn_list;

			to = g_new0(Neuron *, ptr_neuron_axon_list->num_of_axons+1);
			delay = g_new0(AxonalDelay, ptr_neuron_axon_list->num_of_axons+1);
			syn_idx = g_new0(SynapseIndex, ptr_neuron_axon_list->num_of_axons+1);

			weight = g_new0(SynapticWeight, ptr_post_neuron_synapse_list->num_of_synapses+1);	
			type = g_new0(SynapseType, ptr_post_neuron_synapse_list->num_of_synapses+1);	

			for (i = 0; i < ptr_neuron_axon_list->num_of_axons; i++)
			{
				to[i] = ptr_neuron_axon_list->to[i];
				delay[i] = ptr_neuron_axon_list->delay[i];
				syn_idx[i] =  ptr_neuron_axon_list->syn_idx[i];
			}
			g_free(ptr_neuron_axon_list->to);
			g_free(ptr_neuron_axon_list->delay);		
			g_free(ptr_neuron_axon_list->syn_idx);

			ptr_neuron_axon_list->to = to;
			ptr_neuron_axon_list->delay = delay;		
			ptr_neuron_axon_list->syn_idx = syn_idx;
	
			ptr_neuron_axon_list->to[i] = target_neuron;
			ptr_neuron_axon_list->delay[i]  = (AxonalDelay) ((IPSP_delay_max-IPSP_delay_min) * get_rand_number() + IPSP_delay_min); 
			printf ("%u\n", ptr_neuron_axon_list->delay[i]);	
			ptr_neuron_axon_list->syn_idx[i] = ptr_post_neuron_synapse_list->num_of_synapses;    // connected to which synapse (dendrite) of the post synaptic neuron

			for (i = 0; i < ptr_post_neuron_synapse_list->num_of_synapses; i++)
			{
				weight[i] = ptr_post_neuron_synapse_list->weight[i];
				type[i] = ptr_post_neuron_synapse_list->type[i];
			}
			g_free(ptr_post_neuron_synapse_list->weight);
			g_free(ptr_post_neuron_synapse_list->type);
		
			ptr_post_neuron_synapse_list->weight = weight;
			ptr_post_neuron_synapse_list->type = type;
			ptr_post_neuron_synapse_list->weight[i] = -( (weight_inhibitory_max-weight_inhibitory_min) * get_rand_number() ) - weight_inhibitory_min;
			ptr_post_neuron_synapse_list->type[i] = INHIBITORY_SYNAPSE;
	
			ptr_neuron_axon_list->num_of_axons++;
			ptr_post_neuron_synapse_list->num_of_synapses++;

			if (!increase_neuron_event_buffer_size(target_neuron, (ptr_neuron_axon_list->delay[i]/1000000)+1))		// assuming neuron cannot fire more than 1000 Hz
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "! increment_neuron_event_buffer_size().");
			*did_connection = TRUE;		

			printf ("%u\n", ptr_neuron_axon_list->delay[i]);	
		}
	}
	else
	{
		if (   ( rand() /  ((double) RAND_MAX))  <= excitatory_connection_probability )
		{	
			ptr_neuron_axon_list = this_neuron->axon_list;
			ptr_post_neuron_synapse_list = target_neuron->syn_list;

			to = g_new0(Neuron *, ptr_neuron_axon_list->num_of_axons+1);
			delay = g_new0(AxonalDelay, ptr_neuron_axon_list->num_of_axons+1);
			syn_idx = g_new0(SynapseIndex, ptr_neuron_axon_list->num_of_axons+1);

			weight = g_new0(SynapticWeight, ptr_post_neuron_synapse_list->num_of_synapses+1);	
			type = g_new0(SynapseType, ptr_post_neuron_synapse_list->num_of_synapses+1);	

			for (i = 0; i < ptr_neuron_axon_list->num_of_axons; i++)
			{
				to[i] = ptr_neuron_axon_list->to[i];
				delay[i] = ptr_neuron_axon_list->delay[i];
				syn_idx[i] =  ptr_neuron_axon_list->syn_idx[i];
			}
			g_free(ptr_neuron_axon_list->to);
			g_free(ptr_neuron_axon_list->delay);		
			g_free(ptr_neuron_axon_list->syn_idx);

			ptr_neuron_axon_list->to = to;
			ptr_neuron_axon_list->delay = delay;		
			ptr_neuron_axon_list->syn_idx = syn_idx;
	
			ptr_neuron_axon_list->to[i] = target_neuron;
			ptr_neuron_axon_list->delay[i]  = (AxonalDelay)  ((EPSP_delay_max-EPSP_delay_min) * get_rand_number() + EPSP_delay_min); 
			printf ("%u\n", ptr_neuron_axon_list->delay[i]);	
			ptr_neuron_axon_list->syn_idx[i] = ptr_post_neuron_synapse_list->num_of_synapses;    // connected to which synapse (dendrite) of the post synaptic neuron

			for (i = 0; i < ptr_post_neuron_synapse_list->num_of_synapses; i++)
			{
				weight[i] = ptr_post_neuron_synapse_list->weight[i];
				type[i] = ptr_post_neuron_synapse_list->type[i];
			}
			g_free(ptr_post_neuron_synapse_list->weight);
			g_free(ptr_post_neuron_synapse_list->type);
		
			ptr_post_neuron_synapse_list->weight = weight;
			ptr_post_neuron_synapse_list->type = type;
			ptr_post_neuron_synapse_list->weight[i] = ( (weight_excitatory_max-weight_excitatory_min) * get_rand_number() ) + weight_excitatory_min;
			ptr_post_neuron_synapse_list->type[i] = EXCITATORY_SYNAPSE;
	
			ptr_neuron_axon_list->num_of_axons++;
			ptr_post_neuron_synapse_list->num_of_synapses++;

			if (!increase_neuron_event_buffer_size(target_neuron, (ptr_neuron_axon_list->delay[i]/1000000)+1))		// assuming neuron cannot fire more than 1000 Hz
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "! increment_neuron_event_buffer_size().");
			*did_connection = TRUE;	
		}
	}
	return TRUE;
}

void destroy_neuron_axon_list(Neuron *neuron)
{
	NeuronAxonList	*ptr_neuron_axon_list;
	
	ptr_neuron_axon_list = neuron->axon_list;	
	
	g_free(ptr_neuron_axon_list->to);
	g_free(ptr_neuron_axon_list->delay);	
 	g_free(ptr_neuron_axon_list);	

	return;
}


