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
	unsigned int i, j;
	
	if (network == NULL)
		return (ExtNetwork*)print_message(BUG_MSG ,"IzNeuronSimulators", "Network", "deallocate_external_network", "network == NULL.");		

	for (i = 0; i < network->layer_count; i++)
	{
		ptr_layer = network->layers[i];
		for (j = 0; j < ptr_layer->neuron_group_count; j++)
		{
			ptr_neuron_group = ptr_layer->neuron_groups[j];
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
	for (i = 0; i < ext_network->connected_to_internal_network->layer_count; i++)		// copy layer connection matrix
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

