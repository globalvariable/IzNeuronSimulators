#include "ExtNetwork.h"


static bool increment_number_of_external_network_layers(ExtNetwork *ext_network);
static bool add_neuron_group_to_external_network_layer(ExtNetwork *ext_network, int layer, int num_of_neuron, bool inhibitory);
static bool increment_number_of_neuron_group_in_external_network_layer(ExtNetwork *ext_network, int layer);

ExtNetwork* allocate_external_network(ExtNetwork *network)
{
	if (network != NULL)
	{
		network = deallocate_external_network(network);
		network = allocate_external_network(network);
		return network;
	}	
	network = g_new0(ExtNetwork,1);
	print_message(INFO_MSG ,"IzNeuronSimulators", "ExtNetwork", "allocate_external_network", "Created external_network.");
	return network;			
}

ExtNetwork* deallocate_external_network(ExtNetwork *network)
{
	ExtLayer		*ptr_layer = NULL;
	ExtNeuronGroup	*ptr_neuron_group = NULL;
	ExtNeuron	*ptr_neuron = NULL;
	unsigned int i, j, k;
	
	if (network == NULL)
		return (ExtNetwork*)print_message(BUG_MSG ,"IzNeuronSimulators", "Network", "deallocate_external_network", "network == NULL.");		

	for (i = 0; i < network->layer_count; i++)
	{
		ptr_layer = network->layers[i];
		for (j = 0; j < ptr_layer->neuron_group_count; j++)
		{
			ptr_neuron_group = ptr_layer->neuron_groups[j];
			for (k=0; k<ptr_neuron_group->neuron_count; k++)
			{
				ptr_neuron = &(ptr_neuron_group->neurons[k]);
				destroy_ext_neuron_event_buffer(ptr_neuron);
				destroy_ext_neuron_synapse_list(ptr_neuron);	
			}
			g_free(ptr_neuron_group->neurons);
			g_free(ptr_neuron_group);
		}
		g_free(ptr_layer->neuron_groups);
		g_free(ptr_layer);		
	}
	g_free(network->layers);
	g_free(network);
	print_message(INFO_MSG ,"IzNeuronSimulators", "ExtNetwork", "deallocate_external_network", "Destroyed external_network.");
	return NULL;		
}

bool add_neurons_to_external_network_layer(ExtNetwork *ext_network, unsigned int num_of_neuron, unsigned int layer, bool inhibitory)
{
	if (ext_network == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Network", "add_neurons_to_external_network_layer", "network == NULL.");		

	if ((layer > ext_network->layer_count) || (layer < 0) )
	{
		printf ("ExtNetwork: ERROR: Couldn't add layer %d to network.\n", layer);
		printf ("ExtNetwork: ERROR: Layer number shouldn't be larger than %d or smaller than 0\n", ext_network->layer_count);
		return FALSE;
	}

	if (num_of_neuron <= 0)
	{
		printf("ExtNetwork: ERROR: Num of neurons to add should be greater than ZERO\n");
		return FALSE;
	}	

	if (layer == ext_network->layer_count)		// New & valid layer request from user
	{
		if (!increment_number_of_external_network_layers(ext_network))
			return FALSE;
	}	

	if (!add_neuron_group_to_external_network_layer(ext_network, layer, num_of_neuron, inhibitory))
		return FALSE;
	return TRUE;	
}

static bool increment_number_of_external_network_layers(ExtNetwork *ext_network)
{
	int i;
	ExtLayer		**ptr_layers = NULL;
	ExtLayer		*ptr_layer = NULL;	
		
	ptr_layers = g_new0(ExtLayer*,ext_network->layer_count+1);
	if (!is_allocated(ptr_layers, TRUE, "ExtNetwork", "increment_number_of_external_network_layers", ALLOCATION_FAILED, "ptr_layers", EXPECTS_ALLOCATED))
		return FALSE;	
		
	for (i=0; i< ext_network->layer_count; i++)
	{
		ptr_layers[i] = ext_network->layers[i];
	}
	g_free(ext_network->layers);
	ext_network->layers = ptr_layers;
	
	ptr_layer = g_new0(ExtLayer,1);
	if (!is_allocated(ptr_layer, TRUE, "ExtNetwork", "increment_number_of_external_network_layers", ALLOCATION_FAILED, "ptr_layer", EXPECTS_ALLOCATED))
		return FALSE;	
	ext_network->layers[ext_network->layer_count] = ptr_layer;
	
//	printf("ExtNetwork: INFO: Incremented number of layers from %d to %d\n", ext_network->layer_count, ext_network->layer_count+1);	
		
	ext_network->layer_count++;

	return TRUE;			
}

static bool add_neuron_group_to_external_network_layer(ExtNetwork *ext_network, int layer, int num_of_neuron, bool inhibitory)
{
	int i;
	ExtLayer		*ptr_layer = NULL;
	ExtNeuronGroup	*ptr_neuron_group = NULL;
	
	if (!increment_number_of_neuron_group_in_external_network_layer(ext_network, layer))
		return FALSE;

	ptr_layer = ext_network->layers[layer];			
	ptr_neuron_group = ptr_layer->neuron_groups[ptr_layer->neuron_group_count-1];

	ptr_neuron_group->neurons = g_new0(ExtNeuron, num_of_neuron);

	if (!is_allocated(ptr_neuron_group->neurons, TRUE, "ExtNetwork", "add_neuron_group_to_external_network_layer", ALLOCATION_FAILED, "ptr_neuron_group->neurons", EXPECTS_ALLOCATED))
		return FALSE;	
		
	ptr_neuron_group->neuron_count = num_of_neuron;

	for (i=0; i<num_of_neuron; i++)
	{
		if( !initialize_ext_neuron(&(ptr_neuron_group->neurons[i]), inhibitory))
			return FALSE;
	}
//	printf("ExtNetwork: Additon of %d neurons to layer %d is successful\n", num_of_neuron, layer);
	return TRUE;
		
}

static bool increment_number_of_neuron_group_in_external_network_layer(ExtNetwork *ext_network, int layer)
{
	unsigned int i;
	ExtLayer		*ptr_layer = NULL;
	ExtNeuronGroup	**ptr_neuron_groups = NULL;	
	ExtNeuronGroup	*ptr_neuron_group = NULL;
	
	ptr_layer = ext_network->layers[layer];			
	ptr_neuron_groups = g_new0(ExtNeuronGroup*, ptr_layer->neuron_group_count+1);

	if (!is_allocated(ptr_neuron_groups, TRUE, "ExtNetwork", "increment_number_of_neuron_group_in_external_network_layer", ALLOCATION_FAILED, "ptr_neuron_groups", EXPECTS_ALLOCATED))
		return FALSE;	

	for (i=0; i < ptr_layer->neuron_group_count; i++)
	{
		ptr_neuron_groups[i] = ptr_layer->neuron_groups[i];
	}	
	
	g_free(ptr_layer->neuron_groups);
	ptr_layer->neuron_groups = ptr_neuron_groups;	
	
	ptr_neuron_group = g_new0(ExtNeuronGroup,1);	

	if (!is_allocated(ptr_neuron_group, TRUE, "ExtNetwork", "increment_number_of_neuron_group_in_external_network_layer", ALLOCATION_FAILED, "ptr_neuron_group", EXPECTS_ALLOCATED))
		return FALSE;	
		
	ptr_layer->neuron_groups[ptr_layer->neuron_group_count] = ptr_neuron_group;
	
//	printf("ExtNetwork: INFO: Incremented number of neuron groups from %d to %d in layer %d\n", ptr_layer->neuron_group_count, ptr_layer->neuron_group_count +1, layer);	
		
	ptr_layer->neuron_group_count++;

	return TRUE;		
}

bool increment_ext_to_int_network_layer_connection_matrix(ExtNetwork *ext_network)
{
	unsigned int i, j;
	Layer		**connected_to_internal_network_layer;		
	if (ext_network == NULL) // no ext network so no connection from ext_network
		return TRUE;
	for (i = 0; i < ext_network->layer_count; i++)		// copy layer connection matrix
	{
		connected_to_internal_network_layer = g_new0(Layer*, ext_network->connected_to_internal_network->layer_count+1);
		for (j = 0; j < ext_network->connected_to_internal_network->layer_count; j++)
		{
			connected_to_internal_network_layer[j] = ext_network->layers[i]->connected_to_internal_network_layer[j];
		}
		g_free(ext_network->layers[i]->connected_to_internal_network_layer);
		ext_network->layers[i]->connected_to_internal_network_layer = connected_to_internal_network_layer;
	}

	return TRUE;		
}


bool connect_ext_network_layer_to_int_network_layer(ExtNetwork *ext_network, unsigned int this_layer, unsigned int target_layer, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, SynapticDelay EPSP_delay_max, SynapticDelay EPSP_delay_min, SynapticDelay IPSP_delay_max, SynapticDelay IPSP_delay_min, double excitatory_connection_probability, double inhibitory_connection_probability)
{
	ExtLayer		*ptr_this_layer = NULL;
	Layer		*ptr_target_layer = NULL;	
	ExtNeuronGroup	*ptr_this_neuron_group = NULL;
	NeuronGroup	*ptr_target_neuron_group = NULL;	
	ExtNeuron		*ptr_this_neuron = NULL;
	Neuron		*ptr_target_neuron = NULL;
	unsigned int counter_neurons = 0, counter_synapses = 0;
	bool layers_connected = FALSE, neuron_connected = FALSE;
	unsigned int i, j, k, m;

	if (ext_network->layer_count <= this_layer)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "ExtNetwork", "connect_ext_network_layer_to_int_network_layer", "ext_network->layer_count <= this_layer.");	 
	if (ext_network->connected_to_internal_network->layer_count <= target_layer)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "ExtNetwork", "connect_ext_network_layer_to_int_network_layer", "ext_network->connected_to_internal_network->layer_count <= target_layer.");	 

	ptr_this_layer = ext_network->layers[this_layer];
	ptr_target_layer = ext_network->connected_to_internal_network->layers[target_layer];
	
	if (! is_ext_network_layer_connected_to_int_network_layer(ext_network, this_layer, target_layer, &layers_connected))	
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "ExtNetwork", "connect_ext_network_layer_to_int_network_layer", "! is_ext_network_layer_connected_to_int_network_layer().");	
	if (layers_connected)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "ExtNetwork", "connect_ext_network_layer_to_int_network_layer", "this_layer already connected to target_layer.");		

	for (i=0; i < ptr_this_layer->neuron_group_count; i++)
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
					neuron_connected = FALSE;
					if (!create_synapse_from_ext_neuron_to_int_neuron(ptr_this_neuron, ptr_target_neuron, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_min, EPSP_delay_max, IPSP_delay_min, IPSP_delay_max, excitatory_connection_probability, inhibitory_connection_probability, &neuron_connected))
						return print_message(ERROR_MSG ,"IzNeuronSimulators", "ExtNetwork", "connect_ext_network_layer_to_int_network_layerr", "! create_synapse_from_ext_neuron_to_int_neuron().");		
					if (neuron_connected)
						counter_synapses++;
				}
			}
		}
	}
	
	ptr_this_layer->connected_to_internal_network_layer[target_layer] = ptr_target_layer;  

	printf ("Connection of %u neurons in ext network layer %u with %u synapses to internal network layer %u is successful\n",counter_neurons, this_layer, counter_synapses, target_layer);	
	return TRUE;
}

bool is_ext_network_layer_connected_to_int_network_layer(ExtNetwork *ext_network, unsigned int this_layer, unsigned int target_layer, bool *connected)
{
	unsigned int i;
	ExtLayer		*ptr_this_layer = NULL;
	Layer		*ptr_target_layer = NULL;	

	*connected = FALSE;
	if (ext_network->layer_count <= this_layer)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "ExtNetwork", "is_ext_networkl_layer_connected_to_int_network_layer", "network->layer_count <= this_layer.");	 
	if (ext_network->connected_to_internal_network->layer_count <= target_layer)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "ExtNetwork", "is_ext_networkl_layer_connected_to_int_network_layer", "ext_network->connected_to_internal_network->layer_count <= target_layer.");	

	ptr_this_layer  = ext_network->layers[this_layer];
	ptr_target_layer  = ext_network->connected_to_internal_network->layers[target_layer];
	
	for (i = 0; i < ext_network->connected_to_internal_network->layer_count; i++)
	{
		if (ptr_this_layer->connected_to_internal_network_layer[i] == ptr_target_layer)
			*connected = TRUE;
	}
	return TRUE;	
}
