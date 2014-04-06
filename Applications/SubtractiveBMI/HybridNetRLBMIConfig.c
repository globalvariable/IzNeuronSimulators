#include "HybridNetRLBMIConfig.h"



bool add_neurons_for_external_and_in_silico_network(HybridNetRLBMIData *bmi_data)
{

	unsigned int i, j;
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j = 0; j < MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			if (!add_neuron_nodes_to_layer(bmi_data->blue_spike_network, MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE ,i, FALSE)) {
				return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "main", "add_neuron_nodes_to_layer()."); return -1; }				
		}
	}

	// BASE SERVO

	if (!add_neuron_nodes_to_layer(bmi_data->in_silico_network, 1, LAYER_BASE_SERVO_EXTENSOR_SPINY, FALSE)) {
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "main", "add_neuron_nodes_to_layer()."); return -1; }	

	if (!add_neuron_nodes_to_layer(bmi_data->in_silico_network, 1, LAYER_BASE_SERVO_FLEXOR_SPINY, FALSE)) {
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "main", "add_neuron_nodes_to_layer()."); return -1; }	



	return TRUE;
}

bool submit_parker_sochacki_integration_precision(HybridNetRLBMIData *bmi_data)
{
	if (! parker_sochacki_set_order_tolerance(bmi_data->in_silico_network, PS_INTEGRATION_MAX_ORDER, PS_INTEGRATION_ERR_TOL))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_parker_sochacki_integration_precision", "! parker_sochacki_set_order_tolerance().");
	return TRUE;
}

bool set_output_layers(HybridNetRLBMIData *bmi_data)
{
	set_layer_type_of_the_neurons_in_layer(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_SPINY, NEURON_LAYER_TYPE_OUTPUT);
	set_layer_type_of_the_neurons_in_layer(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_SPINY, NEURON_LAYER_TYPE_OUTPUT);
	return TRUE;	
}






