#include "Synapse.h"

static MainSynapseList	*main_synapse_list;		 // Use dynamic allocation for each neuron.    Layer ----> Neuron Group ---> Neuron   3D array. 

static struct __LayerConnectionsMatrix
{
	int 		**connections;
	int		matrix_size;
} layer_connections_matrix;


bool initialize_main_synapse_list(void)
{
	main_synapse_list == NULL;
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
		printf ("Synapse: ERROR: Layer number shouldn't be larger than %d and smaller than 0\n", all_network->count);
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

bool add_neuron_group_synapse_list_to_layer_synapse_list(int layer, int num_of _neuron)
{
	LayerSynapseList		*ptr_layer_syn_list = NULL;
	NeuroGroupSynapseList	*ptr_neuron_group_syn_list = NULL;
	NeuronSynapseList		*ptr_neuron_syn_list =NULL;
	
	ptr_neuron_group_syn_list = g_new0(NeuroGroupSynapseList, 1);
	if (ptr_neuron_group_syn_list == NULL)
	{
		printf("Synapse: ERROR: Couldn' t create neuron group synapse list\n");
		return FALSE;
	}
	ptr_layer_syn_list = main_synapse_list->layer_syn_list[layer];			
	ptr_layer_syn_list->neuron_groups_syn_list[ptr_layer_syn_list->neuron_group_syn_list_count] = ptr_neuron_group_syn_list;
	ptr_layer_syn_list->neuron_group_syn_list_count++;
		
	ptr_neuron_group_syn_list->neuron_syn_list = g_new0(NeuronSynapseList, num_of_neuron);
	if (ptr_neuron_group_syn_list->neuron_syn_list == NULL)
	{
		printf("Synapse: ERROR: Couldn' t create %d neuron synapse list\n", num_of_neuron);
		return FALSE;
	}
	ptr_neuron_group_syn_list->neuron_syn_list_count = num_of_neuron;
	
	return TRUE;
}

void initialize_layer_connections_matrix(void)
{
	layer_connections_matrix.matrix_size = 0;
	layer_connections_matrix.connections = NULL;
}

bool add_layer_to_layer_connections_matrix(int new_num_of_layer);				
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
	
	for (i = 0; i < layer_connections_matrix.matrix_size, i++)
	{
		for (j = 0; j < layer_connections_matrix.matrix_size, j++)
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


