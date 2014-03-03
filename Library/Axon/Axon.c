#include "Axon.h"

bool create_axon(Neuron *this_neuron, Neuron *target_neuron, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, AxonalDelay EPSP_delay_min, AxonalDelay EPSP_delay_max, AxonalDelay IPSP_delay_min, AxonalDelay IPSP_delay_max, AxonalDelay delay_hard_min, AxonalDelay delay_hard_max, double excitatory_connection_probability, double inhibitory_connection_probability, bool *did_connection, bool excitatory_plastic, bool inhibitory_plastic)
{

	unsigned int i;
	NeuronAxonList	*ptr_neuron_axon_list;
	SynapseList		*ptr_post_neuron_synapse_list;   // post synaptic neurons synapses
	Neuron				**to = NULL;
	AxonalDelay			*delay = NULL;
	SynapseIndex			*syn_idx = NULL;
	Synapse				*synapses = NULL;
	unsigned int		*target_layer = NULL;
	unsigned int		*target_neuron_group = NULL;	
	unsigned int		*target_neuron_num = NULL;

	*did_connection = FALSE;
	if (EPSP_delay_min>EPSP_delay_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "EPSP_delay_min>EPSP_delay_max.");	 
	if (IPSP_delay_min>IPSP_delay_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "IPSP_delay_min>IPSP_delay_max.");	
 	if (EPSP_delay_min < delay_hard_min)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "EPSP_delay_min < delay_hard_min. delay_hard_min is placed due to multicore processing.");	
 	if (EPSP_delay_max > delay_hard_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "EPSP_delay_max > delay_hard_max.delay_hard_min is placed due to multicore processing.");	
 	if (IPSP_delay_min < delay_hard_min)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "IPSP_delay_min < delay_hard_min.delay_hard_min is placed due to multicore processing.");	
 	if (IPSP_delay_max > delay_hard_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "IPSP_delay_max < delay_hard_max.delay_hard_min is placed due to multicore processing.");
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
 	
	if (search_identical_axon (this_neuron, target_neuron))	// this neurons is previously connected to target neuron. no re-connection is allowed.
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "search_identical_axon (this_neuron, target_neuron).");

	if (this_neuron->inhibitory)
	{
		if (   ( get_rand_number())  <= inhibitory_connection_probability )
		{	
			ptr_neuron_axon_list = this_neuron->axon_list;
			ptr_post_neuron_synapse_list = target_neuron->syn_list;

			to = g_new0(Neuron *, ptr_neuron_axon_list->num_of_axons+1);
			delay = g_new0(AxonalDelay, ptr_neuron_axon_list->num_of_axons+1);
			syn_idx = g_new0(SynapseIndex, ptr_neuron_axon_list->num_of_axons+1);
			target_layer = g_new0(unsigned int, ptr_neuron_axon_list->num_of_axons+1);
			target_neuron_group = g_new0(unsigned int, ptr_neuron_axon_list->num_of_axons+1);	
			target_neuron_num = g_new0(unsigned int, ptr_neuron_axon_list->num_of_axons+1);

			synapses = g_new0(Synapse, ptr_post_neuron_synapse_list->num_of_synapses+1);	

			for (i = 0; i < ptr_neuron_axon_list->num_of_axons; i++)
			{
				to[i] = ptr_neuron_axon_list->to[i];
				delay[i] = ptr_neuron_axon_list->delay[i];
				syn_idx[i] =  ptr_neuron_axon_list->syn_idx[i];
				target_layer[i] = ptr_neuron_axon_list->target_layer[i];
				target_neuron_group[i] = ptr_neuron_axon_list->target_neuron_group[i];
				target_neuron_num[i] = ptr_neuron_axon_list->target_neuron_num[i];
			}
			g_free(ptr_neuron_axon_list->to);
			g_free(ptr_neuron_axon_list->delay);		
			g_free(ptr_neuron_axon_list->syn_idx);
			g_free(ptr_neuron_axon_list->target_layer);
			g_free(ptr_neuron_axon_list->target_neuron_group);
			g_free(ptr_neuron_axon_list->target_neuron_num);

			ptr_neuron_axon_list->to = to;
			ptr_neuron_axon_list->delay = delay;		
			ptr_neuron_axon_list->syn_idx = syn_idx;
			ptr_neuron_axon_list->target_layer = target_layer;
			ptr_neuron_axon_list->target_neuron_group = target_neuron_group;
			ptr_neuron_axon_list->target_neuron_num = target_neuron_num;

			ptr_neuron_axon_list->to[i] = target_neuron;
			ptr_neuron_axon_list->delay[i]  = (AxonalDelay) ((IPSP_delay_max-IPSP_delay_min) * get_rand_number() + IPSP_delay_min); 
			ptr_neuron_axon_list->syn_idx[i] = ptr_post_neuron_synapse_list->num_of_synapses;    // connected to which synapse (dendrite) of the post synaptic neuron
			ptr_neuron_axon_list->target_layer[i] = target_neuron->layer;
			ptr_neuron_axon_list->target_neuron_group[i] = target_neuron->neuron_group;
			ptr_neuron_axon_list->target_neuron_num[i] = target_neuron->neuron_num;

			for (i = 0; i < ptr_post_neuron_synapse_list->num_of_synapses; i++)
			{
				synapses[i].weight = ptr_post_neuron_synapse_list->synapses[i].weight;
				synapses[i].type = ptr_post_neuron_synapse_list->synapses[i].type;
				synapses[i].plastic = ptr_post_neuron_synapse_list->synapses[i].plastic;
				synapses[i].event_buffer =  ptr_post_neuron_synapse_list->synapses[i].event_buffer;
				synapses[i].axon_from_network = ptr_post_neuron_synapse_list->synapses[i].axon_from_network;
				synapses[i].axon_from_layer = ptr_post_neuron_synapse_list->synapses[i].axon_from_layer;
				synapses[i].axon_from_neuron_group = ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron_group;
				synapses[i].axon_from_neuron = ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron;
			}
			g_free(ptr_post_neuron_synapse_list->synapses);
		
			ptr_post_neuron_synapse_list->synapses = synapses;
			ptr_post_neuron_synapse_list->synapses[i].weight = -( (weight_inhibitory_max-weight_inhibitory_min) * get_rand_number() ) - weight_inhibitory_min;
			ptr_post_neuron_synapse_list->synapses[i].type = INHIBITORY_SYNAPSE;
			ptr_post_neuron_synapse_list->synapses[i].plastic = inhibitory_plastic;
			ptr_post_neuron_synapse_list->synapses[i].event_buffer = allocate_neuron_synaptic_event_buffer(ptr_neuron_axon_list->delay[ptr_neuron_axon_list->num_of_axons]);
			ptr_post_neuron_synapse_list->synapses[i].axon_from_network = this_neuron->network;
			ptr_post_neuron_synapse_list->synapses[i].axon_from_layer = this_neuron->layer;
			ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron_group = this_neuron->neuron_group;
			ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron = this_neuron->neuron_num;
	
			ptr_neuron_axon_list->num_of_axons++;
			ptr_post_neuron_synapse_list->num_of_synapses++;

			if (! update_neuron_sorted_event_buffer_size(target_neuron))		
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "! update_neuron_sorted_event_buffer_size().");

			*did_connection = TRUE;		
		}
	}
	else
	{
		if (   ( get_rand_number())  <= excitatory_connection_probability )
		{	
			ptr_neuron_axon_list = this_neuron->axon_list;
			ptr_post_neuron_synapse_list = target_neuron->syn_list;

			to = g_new0(Neuron *, ptr_neuron_axon_list->num_of_axons+1);
			delay = g_new0(AxonalDelay, ptr_neuron_axon_list->num_of_axons+1);
			syn_idx = g_new0(SynapseIndex, ptr_neuron_axon_list->num_of_axons+1);
			target_layer = g_new0(unsigned int, ptr_neuron_axon_list->num_of_axons+1);
			target_neuron_group = g_new0(unsigned int, ptr_neuron_axon_list->num_of_axons+1);	
			target_neuron_num = g_new0(unsigned int, ptr_neuron_axon_list->num_of_axons+1);

			synapses = g_new0(Synapse, ptr_post_neuron_synapse_list->num_of_synapses+1);	

			for (i = 0; i < ptr_neuron_axon_list->num_of_axons; i++)
			{
				to[i] = ptr_neuron_axon_list->to[i];
				delay[i] = ptr_neuron_axon_list->delay[i];
				syn_idx[i] =  ptr_neuron_axon_list->syn_idx[i];
				target_layer[i] = ptr_neuron_axon_list->target_layer[i];
				target_neuron_group[i] = ptr_neuron_axon_list->target_neuron_group[i];
				target_neuron_num[i] = ptr_neuron_axon_list->target_neuron_num[i];
			}
			g_free(ptr_neuron_axon_list->to);
			g_free(ptr_neuron_axon_list->delay);		
			g_free(ptr_neuron_axon_list->syn_idx);
			g_free(ptr_neuron_axon_list->target_layer);
			g_free(ptr_neuron_axon_list->target_neuron_group);
			g_free(ptr_neuron_axon_list->target_neuron_num);

			ptr_neuron_axon_list->to = to;
			ptr_neuron_axon_list->delay = delay;		
			ptr_neuron_axon_list->syn_idx = syn_idx;
			ptr_neuron_axon_list->target_layer = target_layer;
			ptr_neuron_axon_list->target_neuron_group = target_neuron_group;
			ptr_neuron_axon_list->target_neuron_num = target_neuron_num;
	
			ptr_neuron_axon_list->to[i] = target_neuron;
			ptr_neuron_axon_list->delay[i]  = (AxonalDelay)  ((EPSP_delay_max-EPSP_delay_min) * get_rand_number() + EPSP_delay_min); 
			ptr_neuron_axon_list->syn_idx[i] = ptr_post_neuron_synapse_list->num_of_synapses;    // connected to which synapse (dendrite) of the post synaptic neuron
			ptr_neuron_axon_list->target_layer[i] = target_neuron->layer;
			ptr_neuron_axon_list->target_neuron_group[i] = target_neuron->neuron_group;
			ptr_neuron_axon_list->target_neuron_num[i] = target_neuron->neuron_num;

			for (i = 0; i < ptr_post_neuron_synapse_list->num_of_synapses; i++)
			{
				synapses[i].weight = ptr_post_neuron_synapse_list->synapses[i].weight;
				synapses[i].type = ptr_post_neuron_synapse_list->synapses[i].type;
				synapses[i].plastic = ptr_post_neuron_synapse_list->synapses[i].plastic;
				synapses[i].event_buffer =  ptr_post_neuron_synapse_list->synapses[i].event_buffer;
				synapses[i].axon_from_network = ptr_post_neuron_synapse_list->synapses[i].axon_from_network;
				synapses[i].axon_from_layer = ptr_post_neuron_synapse_list->synapses[i].axon_from_layer;
				synapses[i].axon_from_neuron_group = ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron_group;
				synapses[i].axon_from_neuron = ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron;
			}
			g_free(ptr_post_neuron_synapse_list->synapses);
		
			ptr_post_neuron_synapse_list->synapses = synapses;
			ptr_post_neuron_synapse_list->synapses[i].weight = ( (weight_excitatory_max-weight_excitatory_min) * get_rand_number() ) + weight_excitatory_min;
			ptr_post_neuron_synapse_list->synapses[i].type = EXCITATORY_SYNAPSE;
			ptr_post_neuron_synapse_list->synapses[i].plastic = excitatory_plastic;
			ptr_post_neuron_synapse_list->synapses[i].event_buffer = allocate_neuron_synaptic_event_buffer(ptr_neuron_axon_list->delay[ptr_neuron_axon_list->num_of_axons]);
			ptr_post_neuron_synapse_list->synapses[i].axon_from_network = this_neuron->network;
			ptr_post_neuron_synapse_list->synapses[i].axon_from_layer = this_neuron->layer;
			ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron_group = this_neuron->neuron_group;
			ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron = this_neuron->neuron_num;

			ptr_neuron_axon_list->num_of_axons++;
			ptr_post_neuron_synapse_list->num_of_synapses++;

			if (! update_neuron_sorted_event_buffer_size(target_neuron))		
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "! update_neuron_sorted_event_buffer_size().");

			*did_connection = TRUE;	
		}
	}
	return TRUE;
}

bool create_axon_with_values(Neuron *this_neuron, Neuron *target_neuron, SynapticWeight weight, AxonalDelay arg_delay, AxonalDelay delay_hard_min, AxonalDelay delay_hard_max, bool plastic)
{
	unsigned int i;
	NeuronAxonList	*ptr_neuron_axon_list;
	SynapseList		*ptr_post_neuron_synapse_list;   // post synaptic neurons synapses
	Neuron				**to = NULL;
	AxonalDelay			*delay = NULL;
	SynapseIndex			*syn_idx = NULL;
	Synapse				*synapses = NULL;
	unsigned int		*target_layer = NULL;
	unsigned int		*target_neuron_group = NULL;	
	unsigned int		*target_neuron_num = NULL;

 	if (arg_delay < delay_hard_min)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon_with_values", "arg_delay < delay_hard_min.");	
 	if (arg_delay > delay_hard_max)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon_with_values", "arg_delay > delay_hard_max.");	
	if ((this_neuron->inhibitory) && (weight >= 0))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon_with_values", "(this_neuron->inhibitory) && (weight >= 0).");
	if ((! this_neuron->inhibitory) && (weight <= 0))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon_with_values", "(! this_neuron->inhibitory) && (weight <= 0).");

	if (search_identical_axon (this_neuron, target_neuron))	// this neurons is previously connected to target neuron. no re-connection is allowed.
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon_with_values", "search_identical_axon (this_neuron, target_neuron).");

	if (this_neuron->inhibitory)
	{
		ptr_neuron_axon_list = this_neuron->axon_list;
		ptr_post_neuron_synapse_list = target_neuron->syn_list;

		to = g_new0(Neuron *, ptr_neuron_axon_list->num_of_axons+1);
		delay = g_new0(AxonalDelay, ptr_neuron_axon_list->num_of_axons+1);
		syn_idx = g_new0(SynapseIndex, ptr_neuron_axon_list->num_of_axons+1);
		target_layer = g_new0(unsigned int, ptr_neuron_axon_list->num_of_axons+1);
		target_neuron_group = g_new0(unsigned int, ptr_neuron_axon_list->num_of_axons+1);	
		target_neuron_num = g_new0(unsigned int, ptr_neuron_axon_list->num_of_axons+1);

		synapses = g_new0(Synapse, ptr_post_neuron_synapse_list->num_of_synapses+1);	

		for (i = 0; i < ptr_neuron_axon_list->num_of_axons; i++)
		{
			to[i] = ptr_neuron_axon_list->to[i];
			delay[i] = ptr_neuron_axon_list->delay[i];
			syn_idx[i] =  ptr_neuron_axon_list->syn_idx[i];
			target_layer[i] = ptr_neuron_axon_list->target_layer[i];
			target_neuron_group[i] = ptr_neuron_axon_list->target_neuron_group[i];
			target_neuron_num[i] = ptr_neuron_axon_list->target_neuron_num[i];
		}
		g_free(ptr_neuron_axon_list->to);
		g_free(ptr_neuron_axon_list->delay);		
		g_free(ptr_neuron_axon_list->syn_idx);
		g_free(ptr_neuron_axon_list->target_layer);
		g_free(ptr_neuron_axon_list->target_neuron_group);
		g_free(ptr_neuron_axon_list->target_neuron_num);

		ptr_neuron_axon_list->to = to;
		ptr_neuron_axon_list->delay = delay;		
		ptr_neuron_axon_list->syn_idx = syn_idx;
		ptr_neuron_axon_list->target_layer = target_layer;
		ptr_neuron_axon_list->target_neuron_group = target_neuron_group;
		ptr_neuron_axon_list->target_neuron_num = target_neuron_num;

		ptr_neuron_axon_list->to[i] = target_neuron;
		ptr_neuron_axon_list->delay[i]  = arg_delay; 
		printf ("%u\n", ptr_neuron_axon_list->delay[i]);	
		ptr_neuron_axon_list->syn_idx[i] = ptr_post_neuron_synapse_list->num_of_synapses;    // connected to which synapse (dendrite) of the post synaptic neuron
		ptr_neuron_axon_list->target_layer[i] = target_neuron->layer;
		ptr_neuron_axon_list->target_neuron_group[i] = target_neuron->neuron_group;
		ptr_neuron_axon_list->target_neuron_num[i] = target_neuron->neuron_num;

		for (i = 0; i < ptr_post_neuron_synapse_list->num_of_synapses; i++)
		{
			synapses[i].weight = ptr_post_neuron_synapse_list->synapses[i].weight;
			synapses[i].type = ptr_post_neuron_synapse_list->synapses[i].type;
			synapses[i].plastic = ptr_post_neuron_synapse_list->synapses[i].plastic;
			synapses[i].event_buffer =  ptr_post_neuron_synapse_list->synapses[i].event_buffer;
			synapses[i].axon_from_layer = ptr_post_neuron_synapse_list->synapses[i].axon_from_layer;
			synapses[i].axon_from_neuron_group = ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron_group;
			synapses[i].axon_from_neuron = ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron;
		}
		g_free(ptr_post_neuron_synapse_list->synapses);
		
		ptr_post_neuron_synapse_list->synapses = synapses;
		ptr_post_neuron_synapse_list->synapses[i].weight = weight;  
		ptr_post_neuron_synapse_list->synapses[i].type = INHIBITORY_SYNAPSE;
		ptr_post_neuron_synapse_list->synapses[i].plastic = plastic;
		ptr_post_neuron_synapse_list->synapses[i].event_buffer = allocate_neuron_synaptic_event_buffer(ptr_neuron_axon_list->delay[ptr_neuron_axon_list->num_of_axons]);
		ptr_post_neuron_synapse_list->synapses[i].axon_from_layer = this_neuron->layer;
		ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron_group = this_neuron->neuron_group;
		ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron = this_neuron->neuron_num;
	
		ptr_neuron_axon_list->num_of_axons++;
		ptr_post_neuron_synapse_list->num_of_synapses++;

		if (! update_neuron_sorted_event_buffer_size(target_neuron))		
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon_with_values", "! update_neuron_sorted_event_buffer_size().");
	}
	else
	{
		ptr_neuron_axon_list = this_neuron->axon_list;
		ptr_post_neuron_synapse_list = target_neuron->syn_list;

		to = g_new0(Neuron *, ptr_neuron_axon_list->num_of_axons+1);
		delay = g_new0(AxonalDelay, ptr_neuron_axon_list->num_of_axons+1);
		syn_idx = g_new0(SynapseIndex, ptr_neuron_axon_list->num_of_axons+1);
		target_layer = g_new0(unsigned int, ptr_neuron_axon_list->num_of_axons+1);
		target_neuron_group = g_new0(unsigned int, ptr_neuron_axon_list->num_of_axons+1);	
		target_neuron_num = g_new0(unsigned int, ptr_neuron_axon_list->num_of_axons+1);

		synapses = g_new0(Synapse, ptr_post_neuron_synapse_list->num_of_synapses+1);	

		for (i = 0; i < ptr_neuron_axon_list->num_of_axons; i++)
		{
			to[i] = ptr_neuron_axon_list->to[i];
			delay[i] = ptr_neuron_axon_list->delay[i];
			syn_idx[i] =  ptr_neuron_axon_list->syn_idx[i];
			target_layer[i] = ptr_neuron_axon_list->target_layer[i];
			target_neuron_group[i] = ptr_neuron_axon_list->target_neuron_group[i];
			target_neuron_num[i] = ptr_neuron_axon_list->target_neuron_num[i];
		}
		g_free(ptr_neuron_axon_list->to);
		g_free(ptr_neuron_axon_list->delay);		
		g_free(ptr_neuron_axon_list->syn_idx);
		g_free(ptr_neuron_axon_list->target_layer);
		g_free(ptr_neuron_axon_list->target_neuron_group);
		g_free(ptr_neuron_axon_list->target_neuron_num);

		ptr_neuron_axon_list->to = to;
		ptr_neuron_axon_list->delay = delay;		
		ptr_neuron_axon_list->syn_idx = syn_idx;
		ptr_neuron_axon_list->target_layer = target_layer;
		ptr_neuron_axon_list->target_neuron_group = target_neuron_group;
		ptr_neuron_axon_list->target_neuron_num = target_neuron_num;
	
		ptr_neuron_axon_list->to[i] = target_neuron;
		ptr_neuron_axon_list->delay[i]  = arg_delay; 
		printf ("%u\n", ptr_neuron_axon_list->delay[i]);	
		ptr_neuron_axon_list->syn_idx[i] = ptr_post_neuron_synapse_list->num_of_synapses;    // connected to which synapse (dendrite) of the post synaptic neuron
		ptr_neuron_axon_list->target_layer[i] = target_neuron->layer;
		ptr_neuron_axon_list->target_neuron_group[i] = target_neuron->neuron_group;
		ptr_neuron_axon_list->target_neuron_num[i] = target_neuron->neuron_num;

		for (i = 0; i < ptr_post_neuron_synapse_list->num_of_synapses; i++)
		{
			synapses[i].weight = ptr_post_neuron_synapse_list->synapses[i].weight;
			synapses[i].type = ptr_post_neuron_synapse_list->synapses[i].type;
			synapses[i].plastic = ptr_post_neuron_synapse_list->synapses[i].plastic;
			synapses[i].event_buffer =  ptr_post_neuron_synapse_list->synapses[i].event_buffer;
			synapses[i].axon_from_layer = ptr_post_neuron_synapse_list->synapses[i].axon_from_layer;
			synapses[i].axon_from_neuron_group = ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron_group;
			synapses[i].axon_from_neuron = ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron;
		}
		g_free(ptr_post_neuron_synapse_list->synapses);
		
		ptr_post_neuron_synapse_list->synapses = synapses;
		ptr_post_neuron_synapse_list->synapses[i].weight = weight;
		ptr_post_neuron_synapse_list->synapses[i].type = EXCITATORY_SYNAPSE;
		ptr_post_neuron_synapse_list->synapses[i].plastic = plastic;
		ptr_post_neuron_synapse_list->synapses[i].event_buffer = allocate_neuron_synaptic_event_buffer(ptr_neuron_axon_list->delay[ptr_neuron_axon_list->num_of_axons]);
		ptr_post_neuron_synapse_list->synapses[i].axon_from_layer = this_neuron->layer;
		ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron_group = this_neuron->neuron_group;
		ptr_post_neuron_synapse_list->synapses[i].axon_from_neuron = this_neuron->neuron_num;

		ptr_neuron_axon_list->num_of_axons++;
		ptr_post_neuron_synapse_list->num_of_synapses++;

		if (! update_neuron_sorted_event_buffer_size(target_neuron))		
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "Axon", "create_axon", "! update_neuron_sorted_event_buffer_size().");
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

bool search_identical_axon (Neuron *this_neuron, Neuron *target_neuron)
{
	unsigned int		i, num_of_axons;	
	Neuron 			**to;

	num_of_axons = this_neuron->axon_list->num_of_axons;
	to = this_neuron->axon_list->to;

	for (i = 0; i < num_of_axons; i++)
	{
		if (to[i] == target_neuron)
			return TRUE;		// this neuron is connected to target neuron previously.
	}
	return FALSE;
}
