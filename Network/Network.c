 #include "Network.h"


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
	if (num_of_neuron <= 0)
	{
		printf("Network: ERROR: Num of neurons to add should be greater than ZERO\n");
		return FALSE;
	}
	
	if (!add_layer_to_network(layer))
		return FALSE;
	if (!add_layer_synapse_list_to_main_synapse_list(layer))
		return FALSE;
	if (!add_layer_event_buffer_to_main_event_buffer(layer))
		return FALSE;

	if (!add_neuron_group_to_layer(layer, num_of_neuron))
		return FALSE;
	if (!add_neuron_group_synapse_list_to_layer_synapse_list(layer, num_of_neuron))
		return FALSE;
	if (!add_neuron_group_event_buffer_to_layer_event_buffer(layer, num_of_neuron))
		return FALSE;		
	if (!add_layer_to_layer_connections_matrix(all_network->layer_count))	
		return FALSE;
	
	return TRUE;
}

bool add_layer_to_network(int layer)
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
	}
	
	// if it is an existing layer, do not do anything
	return TRUE;
}	

bool add_neuron_group_to_layer(int layer, int num_of_neuron)
{
	int i;
	Layer		*ptr_layer = NULL;
	NeuronGroup	*ptr_neuron_group = NULL;
	Neuron		*ptr_neuron =NULL;
	
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

bool connect_layer_to_layer(int ThisLayer, int TargetLayer, );
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
	if (is_layer_free(ptrArrayThisLayer))
	{
		return FALSE;		
	}
	if (is_layer_free(ptrArrayTargetLayer))
	{
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
			ptr_this_neuron = &(ptr_this_neuron_group->neurons[i]);
			counter_neurons++;
			for (k=0; k< ptr_target_layer->neuron_group_count; k++)
			{
				ptr_target_neuron_group = ptr_target_layer->neuron_groups[k];
				for (m=0; m<ptr_target_neuron_group->neuron_count; m++)
				{		
					ptr_target_neuron = &(ptr_target_neuron_group->neurons[m]);
					if (ptr_this_neuron == ptr_target_neuron)
						continue; // Do not connect the neuron to itself
					retval = create_synapse(ptr_this_neuron, ptr_target_neuron, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_min, EPSP_delay_max, IPSP_delay_min, IPSP_delay_max, connection_probability);	
					if (retval < 0)
					{
						return FALSE;
					}
					else if (retval > 0)
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

bool is_layer_free (GPtrArray *layer)
{
	if (all_network->layer_count == 0)
	{
		printf ("Network: BUG: A layer created previously cannot be free.\n");
		printf ("Network: BUG: Take a look at add_neuron_group_to_layer func.\n");
		return TRUE;
	{
	else
		return FALSE;	
}
bool is_neuron_group_free (GPtrArray *nrn_group)
{
	if (nrn_group->len == 0)
		return TRUE;
	else
		return FALSE;		
}
bool reset_all_neurons (void)
{
	GPtrArray *ptrArrayLayer, *ptrArrayNeuronGroup; 
	neuron *nrn;
	int i, j, k;

	for (i=0; i<ptrArrayAllNetwork->len; i++)
	{	
		ptrArrayLayer = g_ptr_array_index(ptrArrayAllNetwork,i);	
		for (j=0; j<ptrArrayLayer->len; j++)
		{
			ptrArrayNeuronGroup = g_ptr_array_index(ptrArrayLayer,j);
			for (k=0; k<ptrArrayNeuronGroup->len; k++)
			{
				nrn = g_ptr_array_index(ptrArrayNeuronGroup, k);
				nrn->v = 0;
				nrn->u = 0;
				nrn->conductance_excitatory = 0;
				nrn->conductance_inhibitory = 0;
				clear_neuron_event_buffer(nrn);
				clear_neuron_dynamics_log(nrn);
			}
		}
	}
	return TRUE;
}
bool reset_all_synapses (double weight_excitatory_max, double weight_excitatory_min, double weight_inhibitory_max, double weight_inhibitory_min)
{
	GPtrArray *ptrArrayLayer, *ptrArrayNeuronGroup; 
	neuron *nrn; 
	int i, j, k,m;

	for (i=0; i<ptrArrayAllNetwork->len; i++)
	{	
		ptrArrayLayer = g_ptr_array_index(ptrArrayAllNetwork,i);	
		for (j=0; j<ptrArrayLayer->len; j++)
		{
			ptrArrayNeuronGroup = g_ptr_array_index(ptrArrayLayer,j);
			for (k=0; k<ptrArrayNeuronGroup->len; k++)
			{
				nrn = g_ptr_array_index(ptrArrayNeuronGroup, k);
				for (m=0; m<nrn->num_of_output; m++)
				{
					if (nrn->inhibitory)
					{
						nrn->axonal_weight[m] = -((weight_inhibitory_max-weight_inhibitory_min) * ((rand()+1)/32768.0)+weight_inhibitory_min);						
					}
					else
					{
						nrn->axonal_weight[m]= ((weight_excitatory_max-weight_excitatory_min) * ((rand()+1)/32768.0)+weight_excitatory_min);
					}
				}
				for (m=0; m<EVENT_BUFF_SIZE_PER_NEURON; m++)
				{
					nrn->event_time[m] = 0;
					nrn->event_from[m] = 0;   // interrogate ptrArrayEventFrom->len to reach buffer size from anywhere
					nrn->event_weight[m] = 0;
				}
				nrn->event_buffer_write_idx = 0;
				nrn->num_of_event = 0;
			}
		}
	}
	return TRUE;	
}

neuron* is_neuron_allocated(int layer, int nrn_grp, int nrn_num)
{
	GPtrArray *ptrArrayLayer, *ptrArrayNeuronGroup; 
	
	if  (layer >= ptrArrayAllNetwork->len)
	{
		printf ("ERROR: Invalid layer submitted\n");
		printf ("ERROR: Maximum layer # available is %d\n", ptrArrayAllNetwork->len-1);
		return NULL;
	} 
	ptrArrayLayer = g_ptr_array_index(ptrArrayAllNetwork,layer);
	if  (nrn_grp >= ptrArrayLayer->len)
	{
		printf ("ERROR: Invalid neuron group submitted\n");
		printf ("ERROR: Maximum neuron group # available is %d\n", ptrArrayLayer->len-1);
		return NULL;
	} 

	ptrArrayNeuronGroup = g_ptr_array_index(ptrArrayLayer,nrn_grp);
	if  (nrn_num >= ptrArrayNeuronGroup->len)
	{
		printf ("ERROR: Invalid neuron number submitted\n");
		printf ("ERROR: Maximum neuron # available is %d\n", ptrArrayNeuronGroup->len-1);
		return NULL;
	} 
	if ((layer<0) || (nrn_grp<0) || (nrn_num<0))
	{
		printf ("Layer, Neuron Group for Neuron # cannot be less than 0\n");
		return NULL;
	}
	return g_ptr_array_index(ptrArrayNeuronGroup, nrn_num);
}

bool reset_neurons_in_group(int nrn_grp, int layer, double v, double a, double b, double c,double d, double I_inject, bool inhibitory, int randomize_params, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory)
{
	GPtrArray *ptrArrayLayer, *ptrArrayNeuronGroup; 
	neuron *nrn;
	int i;
	ptrArrayLayer = g_ptr_array_index(ptrArrayAllNetwork,layer);
	ptrArrayNeuronGroup = g_ptr_array_index(ptrArrayLayer,nrn_grp);
	for (i=0;i<ptrArrayNeuronGroup->len; i++)
	{
		nrn = g_ptr_array_index(ptrArrayNeuronGroup,i);
		reset_neuron_params(nrn, v, a, b, c, d, I_inject, inhibitory, C, E_excitatory, E_inhibitory, v_resting, v_threshold, v_peak, k, tau_excitatory, tau_inhibitory);
	}
	printf("% d Neurons in Neuron Group: %d of Layer: %d have been reset\n", i, nrn_grp, layer);
	printf("Use 'Interrogate Neuron' to verify the modifications'\n");
	return TRUE;
}

bool reset_neuron(int nrn_num, int nrn_grp, int layer, double v, double a, double b, double c,double d, double I_inject, bool inhibitory, int randomize_params, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory)
{
	GPtrArray *ptrArrayLayer, *ptrArrayNeuronGroup; 
	neuron *nrn;

	ptrArrayLayer = g_ptr_array_index(ptrArrayAllNetwork,layer);
	ptrArrayNeuronGroup = g_ptr_array_index(ptrArrayLayer,nrn_grp);
	nrn = g_ptr_array_index(ptrArrayNeuronGroup,nrn_num);

	reset_neuron_params(nrn, v, a, b, c, d, I_inject, inhibitory, C, E_excitatory, E_inhibitory, v_resting, v_threshold, v_peak, k, tau_excitatory, tau_inhibitory);
	
	printf("Neuron #%d in Neuron Group: %d of Layer: %d have been reset\n", nrn_num, nrn_grp, layer);
	printf("Neuron address: %u\n", (unsigned int) nrn);
	printf("Use 'Interrogate Neuron' to verify the modifications'\n");
	return TRUE;
}
