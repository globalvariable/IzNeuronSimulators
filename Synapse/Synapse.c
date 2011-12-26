#include "Synapse.h"


bool initialize_main_synapse_list(void)
{
	main_synapse_list = NULL;
	main_synapse_list = g_new0(MainSynapseList, 1);
	if (main_synapse_list == NULL)
	{
		printf("Synapse: ERROR: Couldn' t create main synapse list\n");
		return FALSE;
	}	
	return TRUE;

}

bool add_layer_synapse_list_to_main_synapse_list(int layer)
{
	LayerSynapseList		*ptr_layer_syn_list = NULL;
	
	if ((layer > main_synapse_list->layer_syn_list_count) || (layer < 0) )
	{
		printf ("Synapse: ERROR: Couldn't add layer synapse list %d.\n", layer);
		printf ("Synapse: ERROR: Layer number shouldn't be larger than %d and smaller than 0\n", main_synapse_list->layer_syn_list_count);
		return FALSE;
	}
	
	if (layer == main_synapse_list->layer_syn_list_count)		// New & valid synapse list layer request from user
	{
		ptr_layer_syn_list = g_new0(LayerSynapseList, 1);
		if (ptr_layer_syn_list == NULL)
		{
			printf("Synapse: ERROR: Couldn' t create layer synapse list%d\n", layer);
			return FALSE;
		}
		main_synapse_list->layer_syn_list[layer] = ptr_layer_syn_list;
		main_synapse_list->layer_syn_list_count++;		
	}
	
	// if it is an existing layer, do not do anything
	return TRUE;

}	

bool add_neuron_group_synapse_list_to_layer_synapse_list(int layer, int num_of_neuron)
{
	int i;
	LayerSynapseList		*ptr_layer_syn_list = NULL;
	NeuronGroupSynapseList	*ptr_neuron_group_syn_list = NULL;
	NeuronSynapseList		*ptr_neuron_syn_list;
	Neuron				*neuron;		
	ptr_neuron_group_syn_list = g_new0(NeuronGroupSynapseList, 1);
	if (ptr_neuron_group_syn_list == NULL)
	{
		printf("Synapse: ERROR: Couldn' t create neuron group synapse list\n");
		return FALSE;
	}
	ptr_layer_syn_list = main_synapse_list->layer_syn_list[layer];			
	ptr_layer_syn_list->neuron_group_syn_list[ptr_layer_syn_list->neuron_group_syn_list_count] = ptr_neuron_group_syn_list;
			
	ptr_neuron_group_syn_list->neuron_syn_list = g_new0(NeuronSynapseList, num_of_neuron);
	if (ptr_neuron_group_syn_list->neuron_syn_list == NULL)
	{
		printf("Synapse: ERROR: Couldn' t create %d neuron synapse list\n", num_of_neuron);
		return FALSE;
	}
	ptr_neuron_group_syn_list->neuron_syn_list_count = num_of_neuron;

	for (i = 0; i < num_of_neuron; i++)
	{
		neuron = &(all_network->layers[layer]->neuron_groups[ptr_layer_syn_list->neuron_group_syn_list_count]->neurons[i]);			// For faster data reach in event scheduling and insertion
		ptr_neuron_syn_list = &(ptr_neuron_group_syn_list->neuron_syn_list[i]);	
		neuron->syn_list = ptr_neuron_syn_list;	
	}

	ptr_layer_syn_list->neuron_group_syn_list_count++;	
	
	return TRUE;
}

void initialize_layer_connections_matrix(void)
{
	layer_connections_matrix.matrix_size = 0;
	layer_connections_matrix.connections = NULL;
}

bool add_layer_to_layer_connections_matrix(int new_num_of_layer)				
{
	int 	**local_connections_matrix;
	int 	i,j;
	
	local_connections_matrix = g_new0(int *, new_num_of_layer);
	if (local_connections_matrix == NULL)
	{
		printf("Synapse: ERROR: Couldn' t create connections_matrix\n");
		return FALSE;		
	}
	for (i=0; i<new_num_of_layer; i++)
	{
		local_connections_matrix[i] = g_new0(int, new_num_of_layer);
		if (local_connections_matrix[i] == NULL)
		{
			printf("Synapse: ERROR: Couldn' t create connections_matrix\n");
			return FALSE;		
		}		
	} 
	
	for (i = 0; i < layer_connections_matrix.matrix_size; i++)
	{
		for (j = 0; j < layer_connections_matrix.matrix_size; j++)
		{
			local_connections_matrix[i][j] = layer_connections_matrix.connections[i][j];
		}		
	}
	
	for (i=0; i<new_num_of_layer; i++)
	{
		g_free(layer_connections_matrix.connections[i]);
	} 		
	g_free(layer_connections_matrix.connections);	
		
	layer_connections_matrix.connections = local_connections_matrix;
	layer_connections_matrix.matrix_size = new_num_of_layer;	

	return TRUE;
}		

int create_synapse(Neuron *this_neuron, Neuron *target_neuron, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, SynapticDelay EPSP_delay_min, SynapticDelay EPSP_delay_max, SynapticDelay IPSP_delay_min, SynapticDelay IPSP_delay_max, float connection_probability)
{

	int i;
	int this_layer = this_neuron->layer;
	int this_neuron_group = this_neuron->neuron_group;
	int this_neuron_num = this_neuron->neuron_num;
		
	int target_layer = target_neuron->layer;
	int target_neuron_group = target_neuron->neuron_group;
	int target_neuron_num = target_neuron->neuron_num;	
	
	NeuronSynapseList	*ptr_neuron_synapse_list;
	Neuron 			**to = NULL;
	SynapticDelay		*delay = NULL;
	SynapticWeight		*weight = NULL;	
	
	srand ( time(NULL) );
	if (   ( rand() /  ((float) RAND_MAX))  <= connection_probability )
	{	
		ptr_neuron_synapse_list = &(main_synapse_list->layer_syn_list[this_layer]->neuron_group_syn_list[this_neuron_group]->neuron_syn_list[this_neuron_num]);

		to = g_new0(Neuron *,ptr_neuron_synapse_list->num_of_synapses+1);
		delay = g_new0(SynapticDelay,ptr_neuron_synapse_list->num_of_synapses+1);
		weight = g_new0(SynapticWeight,ptr_neuron_synapse_list->num_of_synapses+1);
		if ((to == NULL) || (delay == NULL) || (weight == NULL))
		{
			printf("Synapse: ERROR: Couldn' t allocate synapse list for neuron (Layer: %d, Neuron Group: %d, Neuron Num: %d)\n", this_layer, this_neuron_group, this_neuron_num);
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


