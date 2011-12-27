 #include "Network.h"

struct __LayerConnectionsMatrix
{
	int 		**connections;
	int		matrix_size;
};

static struct __LayerConnectionsMatrix layer_connections_matrix = {NULL, 0};

bool initialize_network(void)
{
	all_network = NULL;
	all_network = g_new0(AllNetwork, 1);
	if (all_network == NULL)
	{
		printf("Network: ERROR: Couldn' t create network\n");
		return FALSE;
	}	
	return TRUE;
}

bool add_neurons_to_layer(int num_of_neuron, int layer, double v, double a, double b, double c,double d, double k, double C, double v_resting, double v_threshold, double v_peak, double I_inject, bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory, int randomize_params)
{	
	Layer		*ptr_layer = NULL;
	if ((layer > all_network->layer_count) || (layer < 0) )
	{
		printf ("Network: ERROR: Couldn't add layer %d to netwok.\n", layer);
		printf ("Network: ERROR: Layer number shouldn't be larger than %d or smaller than 0\n", all_network->layer_count);
		return FALSE;
	}
	
	if (layer == all_network->layer_count)		// New & valid layer request from user
	{
		ptr_layer = g_new0(Layer, 1);
		if (ptr_layer == NULL)
		{
			printf("Network: ERROR: Couldn' t create layer %d\n", layer);
			return FALSE;
		}
		all_network->layers[layer] = ptr_layer;
		all_network->layer_count++;
		if (!increment_layer_connections_matrix_size())		
			return FALSE;
	}	

	if (num_of_neuron <= 0)
	{
		printf("Network: ERROR: Num of neurons to add should be greater than ZERO\n");
		return FALSE;
	}
	
	if (!add_neuron_group_to_layer(layer, num_of_neuron, v, a, b, c, d, k, C, v_resting, v_threshold, v_peak, I_inject, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory))
		return FALSE;

	return TRUE;
}

bool add_neuron_group_to_layer(int layer, int num_of_neuron, double v, double a, double b, double c, double d, double k, double C, double v_resting, double v_threshold, double v_peak, double I_inject, bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory)
{
	int i;
	Layer		*ptr_layer = NULL;
	NeuronGroup	*ptr_neuron_group = NULL;
	
	ptr_neuron_group = g_new0(NeuronGroup, 1);
	if (ptr_neuron_group == NULL)
	{
		printf("Network: ERROR: Couldn' t create neuron group %d\n", ptr_layer->neuron_group_count);
		return FALSE;
	}	
	ptr_layer = all_network->layers[layer];			
	ptr_layer->neuron_groups[ptr_layer->neuron_group_count] = ptr_neuron_group;
	ptr_layer->neuron_group_count++;
		
	ptr_neuron_group->neurons = g_new0(Neuron, num_of_neuron);
	if (ptr_neuron_group->neurons == NULL)
	{
		printf("Network: ERROR: Couldn' t create %d neurons\n", num_of_neuron);
		return FALSE;
	}
	ptr_neuron_group->neuron_count = num_of_neuron;
	
	for (i=0; i<num_of_neuron; i++)
	{
		if( !initialize_neuron(&(ptr_neuron_group->neurons[i]), layer, 0, i, v, a, b, c, d, k, C, v_resting, v_threshold, v_peak, I_inject, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory))
			return FALSE;
	}
	return TRUE;
}

bool connect_layer_to_layer(int ThisLayer, int TargetLayer, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, 
						SynapticDelay EPSP_delay_min, SynapticDelay EPSP_delay_max, SynapticDelay IPSP_delay_min, SynapticDelay IPSP_delay_max, float connection_probability)
{
	Layer		*ptr_this_layer = NULL;
	Layer		*ptr_target_layer = NULL;	
	NeuronGroup	*ptr_this_neuron_group = NULL;
	NeuronGroup	*ptr_target_neuron_group = NULL;	
	Neuron		*ptr_this_neuron = NULL;
	Neuron		*ptr_target_neuron = NULL;
	int counter_neurons = 0, counter_synapses = 0;
	
	if ((all_network->layer_count <= ThisLayer) || (all_network->layer_count <= TargetLayer) || (ThisLayer<0) || (TargetLayer<0))
	{
		printf ("Network: ERROR: Couldn't connect layer %d to layer %d.\n", ThisLayer , TargetLayer);
		printf ("Network: ERROR: Layer # entered is greater than max layer # %d or less than zero.\n", all_network->layer_count-1);
		return FALSE;
	}

	if ((EPSP_delay_min>EPSP_delay_max) ||  (IPSP_delay_min >  IPSP_delay_max) || (weight_excitatory_min > weight_excitatory_max) || (weight_inhibitory_min > weight_inhibitory_max) || (weight_excitatory_min<=0) || (weight_inhibitory_min<=0) || (EPSP_delay_min < MINIMUM_EPSP_DELAY)  || (IPSP_delay_min <MINIMUM_IPSP_DELAY)) 
	{
		printf ("Network: ERROR: Minimum Maximum Value Inconsistency in synaptic weights of delays\n");
		return FALSE;
	}

	if (layer_connections_matrix.connections[ThisLayer][TargetLayer])
	{
		printf ("Layer %d is already connected to layer %d\n", ThisLayer, TargetLayer);
		return FALSE;		
	}
	
	ptr_this_layer  = all_network->layers[ThisLayer];
	ptr_target_layer  = all_network->layers[TargetLayer];	
	
	int i,j,k,m, ret_val;

	for (i=0; i<ptr_this_layer->neuron_group_count; i++)
	{
		ptr_this_neuron_group = ptr_this_layer->neuron_groups[i];
		for (j=0; j<ptr_this_neuron_group->neuron_count; j++)
		{
			ptr_this_neuron = &(ptr_this_neuron_group->neurons[j]);
			counter_neurons++;
			for (k=0; k< ptr_target_layer->neuron_group_count; k++)
			{
				ptr_target_neuron_group = ptr_target_layer->neuron_groups[k];
				for (m=0; m<ptr_target_neuron_group->neuron_count; m++)
				{		
					ptr_target_neuron = &(ptr_target_neuron_group->neurons[m]);
					if (ptr_this_neuron == ptr_target_neuron)
						continue; // Do not connect the neuron to itself
					ret_val = create_synapse(ptr_this_neuron, ptr_target_neuron, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_min, EPSP_delay_max, IPSP_delay_min, IPSP_delay_max, connection_probability);	
					if (ret_val < 0)
					{
						return FALSE;
					}
					else if (ret_val > 0)
					{
						counter_synapses++;
					}					
				}
			}
		}
	}
	layer_connections_matrix.connections[ThisLayer][TargetLayer] = 1;
	printf ("Connection of %d neurons in layer %d with %d synapses to layer %d is successful\n",counter_neurons, ThisLayer, counter_synapses, TargetLayer);	
	return TRUE;
}

bool increment_layer_connections_matrix_size(void)
{
	int 	**local_connections_matrix;
	int 	i,j;
	
	local_connections_matrix = g_new0(int *, layer_connections_matrix.matrix_size+1);
	if (local_connections_matrix == NULL)
	{
		printf("Synapse: ERROR: Couldn' t create connections_matrix\n");
		return FALSE;		
	}
	for (i=0; i< layer_connections_matrix.matrix_size+1; i++)
	{
		local_connections_matrix[i] = g_new0(int,  layer_connections_matrix.matrix_size+1);
		if (local_connections_matrix[i] == NULL)
		{
			printf("Synapse: ERROR: Couldn' t create connections_matrix\n");
			return FALSE;		
		}		
	} 
	
	for (i = 0; i <  layer_connections_matrix.matrix_size; i++)
	{
		for (j = 0; j <  layer_connections_matrix.matrix_size; j++)
		{
			local_connections_matrix[i][j] = layer_connections_matrix.connections[i][j];
		}		
	}
	
	for (i=0; i<layer_connections_matrix.matrix_size; i++)
	{
		g_free(layer_connections_matrix.connections[i]);
	} 		
	g_free(layer_connections_matrix.connections);	
		
	layer_connections_matrix.connections = local_connections_matrix;
	layer_connections_matrix.matrix_size++;	

	return TRUE;
}		

bool decrement_layer_connections_matrix_size(void)
{
	int 	**local_connections_matrix;
	int 	i,j;
	
	if (layer_connections_matrix.matrix_size > 0)
	{
		local_connections_matrix = g_new0(int *, layer_connections_matrix.matrix_size-1);   // if argument 2 of g_new0 is zero, it will return NULL
		if (local_connections_matrix == NULL)
		{
			printf("Synapse: ERROR: Couldn' t create connections_matrix\n");
			return FALSE;		
		}	
		for (i = 0; i <  layer_connections_matrix.matrix_size-1; i++)
		{
			local_connections_matrix[i] = g_new0(int,  layer_connections_matrix.matrix_size-1);
			if (local_connections_matrix[i] == NULL)
			{
				printf("Synapse: ERROR: Couldn' t create connections_matrix\n");
				return FALSE;		
			}			
			for (j = 0; j <  layer_connections_matrix.matrix_size-1; j++)
			{
				local_connections_matrix[i][j] = layer_connections_matrix.connections[i][j];
			}		
		}	
		for (i=0; i<layer_connections_matrix.matrix_size; i++)
		{
			g_free(layer_connections_matrix.connections[i]);
		} 		
		g_free(layer_connections_matrix.connections);	
		
		layer_connections_matrix.connections = local_connections_matrix;
		layer_connections_matrix.matrix_size--;
	}	

	return TRUE;
}

bool is_layer_free (int layer)
{
	if ((layer >= all_network->layer_count) || (layer < 0))
	{
		printf ("Network: ERROR: Current number of layers is : %d.\n", all_network->layer_count);
		printf ("Network: ERROR: Submitted number is : %d.\n", layer);
		return TRUE;
	}
	return FALSE;
}

bool is_neuron_group_free (int layer, int neuron_group_num)
{
	Layer *ptr_layer; 
	
	if (is_layer_free (layer))
		return TRUE;
		
	ptr_layer  = all_network->layers[layer];
	if ((neuron_group_num >= ptr_layer->neuron_group_count) || (neuron_group_num < 0))
	{
		printf ("Network: ERROR: Current number of Neuron Group in Layer %d is : %d.\n", layer, ptr_layer->neuron_group_count);
		printf ("Network: ERROR: Submitted number is : %d.\n", neuron_group_num);		
		return TRUE;
	}
	return FALSE;
}


bool is_neuron(int layer, int nrn_grp, int nrn_num)
{
	Layer *ptr_layer; 
	NeuronGroup *ptr_neuron_group;
	
	if (is_layer_free (layer))
		return FALSE;
	if (is_neuron_group_free (layer, nrn_grp))
		return FALSE;
	ptr_layer  = all_network->layers[layer];
	ptr_neuron_group = ptr_layer->neuron_groups[nrn_grp];
	if ((nrn_num >= ptr_neuron_group->neuron_count) || (nrn_num<0))
	{
		printf ("Network: ERROR: Current number of Neuron in Layer %d, Neuron Group %d is : %d.\n", layer, nrn_grp, ptr_neuron_group->neuron_count);
		printf ("Network: ERROR: Submitted number is : %d.\n", nrn_num);		
		return FALSE;
	}
	return FALSE;
}

Neuron* get_neuron_address(int layer, int nrn_grp, int nrn_num)
{
	Layer *ptr_layer; 
	NeuronGroup *ptr_neuron_group;
	Neuron		*ptr_neuron = NULL;	
	if (is_layer_free (layer))
		return NULL;
	if (is_neuron_group_free (layer, nrn_grp))
		return NULL;
	if (!is_neuron(layer, nrn_grp, nrn_num))
		return NULL;	
	ptr_layer  = all_network->layers[layer];
	ptr_neuron_group = ptr_layer->neuron_groups[nrn_grp];
	ptr_neuron = &(ptr_neuron_group->neurons[nrn_num]);
	if (ptr_neuron == NULL)
	{
		printf ("Network: BUG: This is not allocated: Layer: %d Neuron Group:%d Neuron:%d", layer, nrn_grp, nrn_num);
	}
	return ptr_neuron ;
}

void reset_all_neuron_dynamics (void)
{
	int i, j, k;
	Layer		*ptr_layer = NULL;
	NeuronGroup	*ptr_neuron_group = NULL;
	Neuron		*ptr_neuron = NULL;

	
	for (i=0; i<all_network->layer_count; i++)
	{
		ptr_layer = all_network->layers[i];
		for (j=0; j<ptr_layer->neuron_group_count; j++)
		{
			ptr_neuron_group = ptr_layer->neuron_groups[j];
			for (k=0; k<ptr_neuron_group->neuron_count; k++)
			{
				ptr_neuron = &(ptr_neuron_group->neurons[k]);
				ptr_neuron->v = 0;	// according to ParkerSochacki method.   v_resting is accepted as 0;  // Take a look at initialize_neuron
				ptr_neuron->u = ptr_neuron->b * ptr_neuron->v;
				ptr_neuron->conductance_excitatory = 0;
				ptr_neuron->conductance_inhibitory = 0;	
				clear_neuron_event_buffer(ptr_neuron);		
			}					
		}
	}	
}

void interrogate_network(void)
{
	int i, j;
	Layer		*ptr_layer = NULL;
	NeuronGroup	*ptr_neuron_group = NULL;
	printf("Num of layer is network: %d", all_network->layer_count);
	for (i = 0; i < all_network->layer_count; i++)
	{
		ptr_layer = all_network->layers[i];		
		printf("Layer: %d\t has %d\t neuron groups.\n", i, ptr_layer->neuron_group_count);
	}
	for (i = 0; i < all_network->layer_count; i++)
	{
		for (j=0; j<ptr_layer->neuron_group_count; j++)
		{
			ptr_neuron_group = ptr_layer->neuron_groups[j];	
			printf("Layer: %d\t Neuron Group: %d\t has %d\t neurons.\n", i, j, ptr_neuron_group->neuron_count);
		}
	}	
}


bool destroy_network(void)
{
	int i, j, k;
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
	Neuron		*ptr_neuron;
	
	for (i=0; i<all_network->layer_count; i++)
	{
		ptr_layer = all_network->layers[i];
		for (j=0; j<ptr_layer->neuron_group_count; j++)
		{
			ptr_neuron_group = ptr_layer->neuron_groups[j];
			for (k=0; k<ptr_neuron_group->neuron_count; k++)
			{
				ptr_neuron = &(ptr_neuron_group->neurons[k]);
				destroy_neuron_event_buffer(ptr_neuron);
				destroy_neuron_synapse_list(ptr_neuron);	
				destroy_neuron_parker_sochacki_pol_vals(ptr_neuron);		
			}
			g_free(ptr_neuron_group->neurons);					
		}
		g_free(ptr_layer->neuron_groups);
		if (!decrement_layer_connections_matrix_size())
			return FALSE;
	}
	g_free(all_network->layers);
	g_free(all_network);
	return TRUE;
}
