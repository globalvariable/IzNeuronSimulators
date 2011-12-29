#include "Synapse.h"

int create_synapse(Neuron *this_neuron, Neuron *target_neuron, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, SynapticDelay EPSP_delay_min, SynapticDelay EPSP_delay_max, SynapticDelay IPSP_delay_min, SynapticDelay IPSP_delay_max, float connection_probability)
{

	int i;
	NeuronSynapseList	*ptr_neuron_synapse_list;
	Neuron 			**to = NULL;
	SynapticDelay		*delay = NULL;
	SynapticWeight		*weight = NULL;	
	
	srand ( time(NULL) );
	if (   ( rand() /  ((float) RAND_MAX))  <= connection_probability )
	{	
		ptr_neuron_synapse_list = this_neuron->syn_list;

		to = g_new0(Neuron *,ptr_neuron_synapse_list->num_of_synapses+1);
		delay = g_new0(SynapticDelay,ptr_neuron_synapse_list->num_of_synapses+1);
		weight = g_new0(SynapticWeight,ptr_neuron_synapse_list->num_of_synapses+1);
		if ((to == NULL) || (delay == NULL) || (weight == NULL))
		{
			printf("Synapse: ERROR: Couldn' t allocate synapse list for neuron (Layer: %d, Neuron Group: %d, Neuron Num: %d)\n", this_neuron->layer, this_neuron->neuron_group, this_neuron->neuron_num);
			return -1;
		}
	
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

		if (this_neuron->inhibitory)
		{
			ptr_neuron_synapse_list->delay[i]  = (SynapticDelay)((IPSP_delay_max-IPSP_delay_min) * (rand()/((double)RAND_MAX))) + IPSP_delay_min; 
			ptr_neuron_synapse_list->weight[i]  = -( (weight_inhibitory_max-weight_inhibitory_min) * (rand()/ ((double)RAND_MAX) )) - weight_inhibitory_min;
		}
		else
		{
			ptr_neuron_synapse_list->delay[i]  = (SynapticDelay)((EPSP_delay_max-EPSP_delay_min) * (rand()/((double)RAND_MAX))) + EPSP_delay_min; 
			ptr_neuron_synapse_list->weight[i] = (weight_excitatory_max-weight_excitatory_min) * (rand()/ ((double)RAND_MAX) ) + weight_excitatory_min;
		}					
		ptr_neuron_synapse_list->num_of_synapses++;
		
		if (increment_neuron_event_buffer_size(target_neuron))
			return 1;
		else
			return -1;
	}
	return 0;
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
