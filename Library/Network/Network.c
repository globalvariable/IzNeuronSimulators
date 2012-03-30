 #include "Network.h"

static bool increment_number_of_layers(Network *network);
static bool increment_number_of_neuron_group_in_layer(Network *network, int layer);
static bool add_neuron_group_to_layer(Network *network, int layer, int num_of_neuron, double a, double b, double c, double d, double k, double C, double v_resting, double v_threshold, double v_peak, bool inhibitory, double E_excitatory, 
								double E_inhibitory, double tau_excitatory, double tau_inhibitory);	


Network* allocate_network(Network *network)
{
	if (network != NULL)
	{
		network = deallocate_network(network);
		network = allocate_network(network);
		return network;
	}	
	network = g_new0(Network,1);
	print_message(INFO_MSG ,"IzNeuronSimulators", "Network", "allocate_network", "Created network.");
	return network;			
}

Network* deallocate_network(Network *network)
{
	int i, j, k;
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
	Neuron		*ptr_neuron;

	if (network == NULL)
		return (Network*)print_message(BUG_MSG ,"IzNeuronSimulators", "Network", "deallocate_network", "network == NULL.");	
	
	for (i=0; i<network->layer_count; i++)
	{
		ptr_layer = network->layers[i];
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
			g_free(ptr_neuron_group);				
		}
		g_free(ptr_layer->neuron_groups);
		g_free(ptr_layer);
	}
	g_free(network->layers);
	g_free(network);
	print_message(INFO_MSG ,"IzNeuronSimulators", "Network", "deallocate_network", "Destroyed network.");
	return NULL;
}

bool add_neurons_to_layer(Network *network, unsigned int num_of_neuron, unsigned int layer, double a, double b, double c,double d, double k, double C, double v_resting, double v_threshold, double v_peak, bool inhibitory, 	 double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory, unsigned int randomize_params)
{
	if (network == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Network", "add_neurons_to_layer", "network == NULL.");	

	if ((layer > network->layer_count) || (layer < 0) )
	{
		printf ("Network: ERROR: Couldn't add layer %d to network.\n", layer);
		printf ("Network: ERROR: Layer number shouldn't be larger than %d or smaller than 0\n", network->layer_count);
		return FALSE;
	}

	if (num_of_neuron <= 0)
	{
		printf("Network: ERROR: Num of neurons to add should be greater than ZERO\n");
		return FALSE;
	}
	
	if (layer == network->layer_count)		// New & valid layer request from user
	{
		if (!increment_number_of_layers(network))
			return FALSE;
	}	
	
	if (!add_neuron_group_to_layer(network, layer, num_of_neuron, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory))
		return FALSE;

	return TRUE;
}

static bool increment_number_of_layers(Network *network)
{
	unsigned int i, j;
	Layer		**ptr_layers = NULL;
	Layer		*ptr_layer = NULL;	
	Layer		**connected_to_network_layer=NULL;			
	ptr_layers = g_new0(Layer*, network->layer_count+1);
	if (ptr_layers == NULL)
	{
		printf("Network: ERROR: Couldn' t create layers array\n");
		return FALSE;
	}
	for (i=0; i< network->layer_count; i++)
	{
		ptr_layers[i] = network->layers[i];
	}
	g_free(network->layers);
	network->layers = ptr_layers;
	
	ptr_layer = g_new0(Layer,1);	// add new layer
	network->layers[network->layer_count] = ptr_layer;

	network->layers[network->layer_count]->connected_to_network_layer = g_new0(Layer*, network->layer_count + 1);
	for (i = 0; i < network->layer_count; i++)		// copy layer connection matrix
	{
		connected_to_network_layer = g_new0(Layer*, network->layer_count+1);
		for (j = 0; j < network->layer_count; j++)
		{	
			connected_to_network_layer[j] = network->layers[i]->connected_to_network_layer[j];
		}
		g_free(network->layers[i]->connected_to_network_layer);
		network->layers[i]->connected_to_network_layer = connected_to_network_layer;
	}
	if (!increment_ext_to_int_network_layer_connection_matrix(network->connection_from_ext_network))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Network", "add_neurons_to_layer", "increment_ext_to_int_network_layer_connection_matrix().");		

	network->layer_count++;

	printf("Network: INFO: Incremented number of layers from %d to %d\n", network->layer_count -1 , network->layer_count);		
	return TRUE;			
}

static bool add_neuron_group_to_layer(Network *network, int layer, int num_of_neuron, double a, double b, double c, double d, double k, double C, double v_resting, double v_threshold, double v_peak, bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory)
{
	int i;
	Layer		*ptr_layer = NULL;
	NeuronGroup	*ptr_neuron_group = NULL;
	
	if (!increment_number_of_neuron_group_in_layer(network, layer))
		return FALSE;

	ptr_layer = network->layers[layer];			
	ptr_neuron_group = ptr_layer->neuron_groups[ptr_layer->neuron_group_count-1];

	ptr_neuron_group->neurons = g_new0(Neuron, num_of_neuron);
	if (ptr_neuron_group->neurons == NULL)
	{
		printf("Network: ERROR: Couldn' t create %d neurons\n", num_of_neuron);
		return FALSE;
	}
	ptr_neuron_group->neuron_count = num_of_neuron;

	for (i=0; i<num_of_neuron; i++)
	{
		if( !initialize_neuron_params(&(ptr_neuron_group->neurons[i]), layer, 0, i, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory))
			return FALSE;
	}
	printf("Network: Additon of %d neurons to layer %d is successful\n", num_of_neuron, layer);
	return TRUE;
}

static bool increment_number_of_neuron_group_in_layer(Network *network, int layer)
{
	int i;
	Layer		*ptr_layer = NULL;
	NeuronGroup	**ptr_neuron_groups = NULL;	
	NeuronGroup	*ptr_neuron_group = NULL;
	
	ptr_layer = network->layers[layer];			
	ptr_neuron_groups = g_new0(NeuronGroup*, ptr_layer->neuron_group_count+1);
	if (ptr_neuron_groups == NULL)
	{
		printf("Network: ERROR: Couldn' t create neuron groups array for layer %d\n", layer);
		return FALSE;
	}	
	for (i=0; i < ptr_layer->neuron_group_count; i++)
	{
		ptr_neuron_groups[i] = ptr_layer->neuron_groups[i];
	}	
	g_free(ptr_layer->neuron_groups);
	ptr_layer->neuron_groups = ptr_neuron_groups;	
	
	ptr_neuron_group = g_new0(NeuronGroup,1);	
	if (ptr_neuron_group == NULL)
	{
		printf("Network: ERROR: Couldn' t create neuron group\n");
		return FALSE;
	}	
	ptr_layer->neuron_groups[ptr_layer->neuron_group_count] = ptr_neuron_group;
	printf("Network: INFO: Incremented number of neuron groups from %d to %d in layer %d\n", ptr_layer->neuron_group_count, ptr_layer->neuron_group_count +1, layer);		
	ptr_layer->neuron_group_count++;

	return TRUE;		
}



bool connect_network_layer_to_network_layer(Network *network, unsigned int this_layer, unsigned int target_layer, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, SynapticDelay EPSP_delay_max, SynapticDelay EPSP_delay_min, SynapticDelay IPSP_delay_max, SynapticDelay IPSP_delay_min, double excitatory_connection_probability, double inhibitory_connection_probability)
{
	Layer		*ptr_this_layer = NULL;
	Layer		*ptr_target_layer = NULL;	
	NeuronGroup	*ptr_this_neuron_group = NULL;
	NeuronGroup	*ptr_target_neuron_group = NULL;	
	Neuron		*ptr_this_neuron = NULL;
	Neuron		*ptr_target_neuron = NULL;
	unsigned int counter_neurons = 0, counter_synapses = 0;
	bool layers_connected = FALSE, neuron_connected = FALSE;
	unsigned int i, j, k, m;

	if (network->layer_count <= this_layer)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Network", "connect_network_layer_to_network_layer", "network->layer_count <= this_layer.");	 
	if (network->layer_count <= target_layer)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Network", "connect_network_layer_to_network_layer", "network->layer_count <= target_layer.");	 

	ptr_this_layer  = network->layers[this_layer];
	ptr_target_layer  = network->layers[target_layer];
	
	if (! is_layer_connected_to_layer(network, this_layer, target_layer, &layers_connected))	
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Network", "connect_network_layer_to_network_layer", "! is_layer_connected_to_layer().");	
	if (layers_connected)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Network", "connect_network_layer_to_network_layer", "this_layer already connected to target_layer.");		

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
					neuron_connected = FALSE;
					if (!create_synapse(ptr_this_neuron, ptr_target_neuron, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_min, EPSP_delay_max, IPSP_delay_min, IPSP_delay_max, excitatory_connection_probability, inhibitory_connection_probability, &neuron_connected))
						return print_message(ERROR_MSG ,"IzNeuronSimulators", "Network", "connect_network_layer_to_network_layer", "! create_synapse().");		
					if (neuron_connected)
						counter_synapses++;
				}
			}
		}
	}
	
	ptr_this_layer->connected_to_network_layer[target_layer] = ptr_target_layer;  

	printf ("Connection of %d neurons in layer %d with %d synapses to layer %d is successful\n",counter_neurons, this_layer, counter_synapses, target_layer);	
	return TRUE;
}

bool is_network_allocated(Network *network)
{
	if (network == NULL)
	{
		printf ("Network: ERROR: The network is not allocated yet.\n");		
		return FALSE;
	}
	return TRUE;
}

bool is_layer_free (Network *network, int layer)
{
	if ((layer >= network->layer_count) || (layer < 0))
	{
		printf ("Network: ERROR: Current number of layers is : %d.\n", network->layer_count);
		printf ("Network: ERROR: Submitted number is : %d.\n", layer);
		return TRUE;
	}
	return FALSE;
}

bool is_neuron_group_free (Network *network, int layer, int neuron_group_num)
{
	Layer *ptr_layer; 
	
	if (is_layer_free (network, layer))
		return TRUE;
		
	ptr_layer  = network->layers[layer];
	if ((neuron_group_num >= ptr_layer->neuron_group_count) || (neuron_group_num < 0))
	{
		printf ("Network: ERROR: Current number of Neuron Group in Layer %d is : %d.\n", layer, ptr_layer->neuron_group_count);
		printf ("Network: ERROR: Submitted number is : %d.\n", neuron_group_num);		
		return TRUE;
	}
	return FALSE;
}


bool is_neuron(Network *network, int layer, int nrn_grp, int nrn_num)
{
	Layer *ptr_layer; 
	NeuronGroup *ptr_neuron_group;
	if (!is_network_allocated(network))
		return FALSE;	
	if (is_layer_free (network, layer))
		return FALSE;
	if (is_neuron_group_free (network, layer, nrn_grp))
		return FALSE;
	ptr_layer  = network->layers[layer];
	ptr_neuron_group = ptr_layer->neuron_groups[nrn_grp];
	if ((nrn_num >= ptr_neuron_group->neuron_count) || (nrn_num<0))
	{
		printf ("Network: ERROR: Current number of Neuron in Layer %d, Neuron Group %d is : %d.\n", layer, nrn_grp, ptr_neuron_group->neuron_count);
		printf ("Network: ERROR: Submitted number is : %d.\n", nrn_num);		
		return FALSE;
	}
	return TRUE;
}

Neuron* get_neuron_address(Network *network, int layer, int nrn_grp, int nrn_num)
{
	Layer *ptr_layer; 
	NeuronGroup *ptr_neuron_group;
	Neuron		*ptr_neuron = NULL;	
	if (!is_network_allocated(network))
		return NULL;
	if (is_layer_free (network, layer))
		return NULL;
	if (is_neuron_group_free (network, layer, nrn_grp))
		return NULL;
	if (!is_neuron(network, layer, nrn_grp, nrn_num))
		return NULL;	
	ptr_layer  = network->layers[layer];
	ptr_neuron_group = ptr_layer->neuron_groups[nrn_grp];
	ptr_neuron = &(ptr_neuron_group->neurons[nrn_num]);
	if (ptr_neuron == NULL)
	{
		printf ("Network: BUG: This is not allocated: Layer: %d Neuron Group:%d Neuron:%d", layer, nrn_grp, nrn_num);
	}
	return ptr_neuron ;
}

void reset_all_network_neuron_dynamics (Network *network)
{
	int i, j, k;
	Layer		*ptr_layer = NULL;
	NeuronGroup	*ptr_neuron_group = NULL;
	Neuron		*ptr_neuron = NULL;

	
	for (i=0; i<network->layer_count; i++)
	{
		ptr_layer = network->layers[i];
		for (j=0; j<ptr_layer->neuron_group_count; j++)
		{
			ptr_neuron_group = ptr_layer->neuron_groups[j];
			for (k=0; k<ptr_neuron_group->neuron_count; k++)
			{
				ptr_neuron = &(ptr_neuron_group->neurons[k]);
				ptr_neuron->v = 0;	// according to ParkerSochacki method.   v_resting is accepted as 0;  // Take a look at initialize_neuron
				ptr_neuron->u = 0;
				ptr_neuron->conductance_excitatory = 0;
				ptr_neuron->conductance_inhibitory = 0;	
				clear_neuron_event_buffer(ptr_neuron);		
			}					
		}
	}	
}

void interrogate_network(Network *network)
{
	int i, j;
	Layer		*ptr_layer = NULL;
	NeuronGroup	*ptr_neuron_group = NULL;
	printf("------------ Interrogating Network ----------------\n");	
	if (network == NULL)
	{
		printf("Network: ERROR: Network has not been created yet.\n");
		return;
	}
	printf("Num of layer in network: %d\n", network->layer_count);
	for (i = 0; i < network->layer_count; i++)
	{
		ptr_layer = network->layers[i];		
		printf("Layer: %d\t has %d\t neuron groups.\n", i, ptr_layer->neuron_group_count);
	}
	for (i = 0; i < network->layer_count; i++)
	{
		ptr_layer = network->layers[i];	
		for (j=0; j<ptr_layer->neuron_group_count; j++)
		{
			ptr_neuron_group = ptr_layer->neuron_groups[j];	
			printf("Layer: %d\t Neuron Group: %d\t has %d\t neurons.\n", i, j, ptr_neuron_group->neuron_count);
		}
	}
	for (i = 0; i < network->layer_count; i++)
	{
		printf("Layer: %u\t is connected to Network Layer (address): %lu.\n", i, (unsigned long int)ptr_layer->connected_to_network_layer);
	}
	printf("------------ Interrogating Network...Complete ----------------\n");	
}


bool get_num_of_layers_in_network(Network *network, unsigned int *num_of_layers)
{
	*num_of_layers = network->layer_count; 
	return TRUE;	
}

bool get_num_of_neuron_groups_in_layer(Network *network, unsigned int layer, unsigned int *num_of_neuron_groups)
{
	*num_of_neuron_groups = network->layers[layer]->neuron_group_count;
	return TRUE;
}


bool get_num_of_neurons_in_neuron_group(Network *network, unsigned int layer, unsigned int neuron_group,unsigned int *num_of_neurons)
{
	*num_of_neurons = network->layers[layer]->neuron_groups[neuron_group]->neuron_count;
	return TRUE;
}

bool get_num_of_neurons_in_network(Network *network, unsigned int *num_of_neurons)
{
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	unsigned int i, j;
	*num_of_neurons = 0;
	get_num_of_layers_in_network(network, &num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{	
		get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{
			get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
			*num_of_neurons = *num_of_neurons + num_of_neurons_in_neuron_group;
		}
	}
	return TRUE;	
}

bool get_num_of_neuron_groups_in_network(Network *network, unsigned int *num_of_neuron_groups)
{
	unsigned int num_of_layers, num_of_neuron_groups_in_layer;
	unsigned int i;
	*num_of_neuron_groups = 0;
	get_num_of_layers_in_network(network, &num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{	
		get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
		*num_of_neuron_groups = *num_of_neuron_groups + num_of_neuron_groups_in_layer;
	}
	return TRUE;	
}

bool is_layer_connected_to_layer(Network *network, unsigned int this_layer, unsigned int target_layer, bool *connected)
{
	unsigned int i;
	Layer		*ptr_this_layer = NULL;
	Layer		*ptr_target_layer = NULL;	

	*connected = FALSE;
	if (network->layer_count <= this_layer)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Network", "is_layer_connected_to_layer", "network->layer_count <= this_layer.");	 
	if (network->layer_count <= target_layer)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Network", "is_layer_connected_to_layer", "network->layer_count <= target_layer.");	

	ptr_this_layer  = network->layers[this_layer];
	ptr_target_layer  = network->layers[target_layer];
	
	for (i = 0; i < network->layer_count; i++)
	{
		if (ptr_this_layer->connected_to_network_layer[i] == ptr_target_layer)
			*connected = TRUE;
	}
	return TRUE;	
}
