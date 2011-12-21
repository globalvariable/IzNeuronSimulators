 #include "Network.h"

bool create_neural_network(int num_of_layer)
{
	int i;
	GPtrArray *NetworkLayer;	

	ptrArrayAllNetwork = g_ptr_array_new();
	for (i=0; i<num_of_layer; i++)
	{	
		NetworkLayer = g_ptr_array_new();
		g_ptr_array_add  (ptrArrayAllNetwork, NetworkLayer);
	}

	LayerConnectionsMatrix = g_new0(int *, num_of_layer);
	for (i=0; i<num_of_layer; i++)
	{	
		LayerConnectionsMatrix[i] = g_new0(int, num_of_layer);
	}	
	
	printf("Network created with %d layers\n", ptrArrayAllNetwork->len);
	printf("Layer Connection Matrix with %d x %d is created\n", num_of_layer, num_of_layer);
	return TRUE;
}

bool add_neuron_group_to_layer(int num_neuron, int layer, double v, double a, double b, double c,double d, double I_inject, bool inhibitory, int randomize_params, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory, int logging_buff_size)
{
	int i;
	neuron *nrn;
	GPtrArray *ptrArrayLayer, *ptrArrayNeuronGroup; 
	ptrArrayNeuronGroup = g_ptr_array_new();

	if ((layer >= ptrArrayAllNetwork->len) || (layer < 0) )
	{
		printf ("Couldn't add %d neurons to layer %d.\n", num_neuron , layer);
		printf ("Invalid layer number\n");
		return FALSE;
	}
	if (num_neuron <= 0)
	{
		printf("Num of neurons to add should be greater than ZERO\n");
		return FALSE;
	}
	ptrArrayLayer = g_ptr_array_index(ptrArrayAllNetwork,layer);	
	g_ptr_array_add  (ptrArrayLayer, ptrArrayNeuronGroup);	
	for (i=0; i<num_neuron; i++)
	{
		nrn = create_neuron( v, a, b, c, d, I_inject, inhibitory, C, E_excitatory, E_inhibitory, v_resting, v_threshold, v_peak, k, tau_excitatory, tau_inhibitory, logging_buff_size);
		if (nrn == NULL)
		{
			return FALSE;
		}
		g_ptr_array_add  (ptrArrayNeuronGroup, nrn);
	}
	return TRUE;
}

bool connect_layer_to_layer(int ThisLayer, int TargetLayer, double weight_excitatory_max, double weight_excitatory_min, double weight_inhibitory_max, double weight_inhibitory_min, unsigned int EPSP_delay_min, unsigned int EPSP_delay_max, unsigned int IPSP_delay_min, unsigned int IPSP_delay_max, float connection_probability)
{
	neuron *nrn;
	GPtrArray *ptrArrayThisLayer;
	GPtrArray *ptrArrayTargetLayer;
	GPtrArray *ptrArrayThisNeuronGroup;
	GPtrArray *ptrArrayTargetNeuronGroup;

	int counter_neurons = 0, counter_synapses = 0;
	ptrArrayThisLayer = g_ptr_array_index(ptrArrayAllNetwork,ThisLayer);
	ptrArrayTargetLayer = g_ptr_array_index(ptrArrayAllNetwork,TargetLayer);

	if ((ptrArrayAllNetwork->len <= ThisLayer) || (ptrArrayAllNetwork->len <= TargetLayer) || (ThisLayer<0) || (TargetLayer<0))
	{
		printf ("Couldn't connect layer %d to layer %d.\n", ThisLayer , TargetLayer);
		printf ("Layer # entered is greater than max layer # %d or less than zero.\n", ptrArrayAllNetwork->len-1);
		return FALSE;
	}
	if (is_layer_free(ptrArrayThisLayer))
	{
		printf ("Couldn't connect layer %d to layer %d.\n", ThisLayer , TargetLayer);
		printf ("Source Layer %d has NO NEURON.\n", ThisLayer);
		return FALSE;		
	}
	if (is_layer_free(ptrArrayTargetLayer))
	{
		printf ("Couldn't connect layer %d to layer %d.\n", ThisLayer, TargetLayer);
		printf ("Target Layer %d has NO NEURON.\n", TargetLayer);
		return FALSE;		
	}
	if ((EPSP_delay_min>EPSP_delay_max) ||  (IPSP_delay_min >  IPSP_delay_max) || (weight_excitatory_min > weight_excitatory_max) || (weight_inhibitory_min > weight_inhibitory_max) || (weight_excitatory_min<=0) || (weight_inhibitory_min<=0) || (EPSP_delay_min < 1000000)  || (IPSP_delay_min <1000000)) 
	{
		printf ("Minimum Maximum Value Inconsistency in synaptic weights of delays\n");
		return FALSE;
	}

	if (LayerConnectionsMatrix[ThisLayer][TargetLayer])
	{
		printf ("Layer %d is already connected to layer %d\n", ThisLayer, TargetLayer);
		return FALSE;		
	}

	int i,j,k,m;

	srand ( time(NULL) );
			
	for (i=0; i<ptrArrayThisLayer ->len; i++)
	{
		ptrArrayThisNeuronGroup = g_ptr_array_index(ptrArrayThisLayer,i);
		for (j=0; j<ptrArrayThisNeuronGroup->len; j++)
		{
			nrn = g_ptr_array_index(ptrArrayThisNeuronGroup,j);
			counter_neurons++;
			for (k=0; k<ptrArrayTargetLayer->len; k++)
			{
				ptrArrayTargetNeuronGroup = g_ptr_array_index(ptrArrayTargetLayer,k);
				for (m=0; m<ptrArrayTargetNeuronGroup->len; m++)
				{				
					if (nrn == g_ptr_array_index(ptrArrayTargetNeuronGroup,m))
						continue; // Do not connect the neuron to itself
					if (   ( rand() /  ((float) RAND_MAX))  <= connection_probability )
					{
						if (nrn->num_of_output >= MAX_OUTPUT_PER_NEURON)
						{
							printf("ERROR: Max number of output per neuron set as %d\n", MAX_OUTPUT_PER_NEURON);
							printf("ERROR: Tried to access %d th output to connect neuron\n", nrn->num_of_output+1);
							return FALSE;
						}
						nrn->axon_to[nrn->num_of_output] = g_ptr_array_index(ptrArrayTargetNeuronGroup,m);

						if (nrn->inhibitory)
						{
							nrn->axonal_delay[nrn->num_of_output]  = (unsigned int)((IPSP_delay_max-IPSP_delay_min) * (rand()/((double)RAND_MAX))) + IPSP_delay_min; 
							nrn->axonal_weight[nrn->num_of_output] = -( (weight_inhibitory_max-weight_inhibitory_min) * (rand()/ ((double)RAND_MAX) )) - weight_inhibitory_min;
						}
						else
						{
							nrn->axonal_delay[nrn->num_of_output]  = (unsigned int)((EPSP_delay_max-EPSP_delay_min) * (rand()/((double)RAND_MAX))) + EPSP_delay_min; 
							nrn->axonal_weight[nrn->num_of_output] = (weight_excitatory_max-weight_excitatory_min) * (rand()/ ((double)RAND_MAX) ) + weight_excitatory_min;
						}					
						nrn->num_of_output++;
						counter_synapses++;
					}
				}
			}
		}
	}
	LayerConnectionsMatrix[ThisLayer][TargetLayer] = 1;
	printf ("Connection of %d neurons in layer %d with %d synapses to layer %d is successful\n",counter_neurons, ThisLayer, counter_synapses, TargetLayer);	
	return TRUE;
}

bool is_layer_free (GPtrArray *layer)
{
	if (layer->len == 0)
		return TRUE;
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
