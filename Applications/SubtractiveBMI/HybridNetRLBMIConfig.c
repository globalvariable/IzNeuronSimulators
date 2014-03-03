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

bool connect_external_to_in_silico_network(HybridNetRLBMIData *bmi_data)
{
	HybridNetRLBMISynapseData	*synapse_data = &(bmi_data->synapse_data);
	unsigned int i, j, k, cntr = 0;
	TemplateMatchingData			*template_matching_data = bmi_data->template_matching_data;
	
	bool spike_sorter_on = FALSE;

	synapse_data->blue_spike_2_in_silico_excitatory_connection_probability = 0.7;
	synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_max = 7;
	synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_min = 7;
	synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	synapse_data->blue_spike_2_in_silico_inhibitory_connection_probability = 0;
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_max = 7;
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_min = 7;
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j = 0; j < MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			for (k = 0; k < 2; k++)
			{
				if (! ((*template_matching_data)[i][j][k].include_unit))
					continue;
				if ((cntr % 2) == 0)
				{
					if (! connect_neurons(bmi_data->blue_spike_network, i, j, k, bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_SPINY, 0, 0, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_min, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_min, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, TRUE, FALSE))
						return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_neurons().");
				}
				else 
				{
					if (! connect_neurons(bmi_data->blue_spike_network, i, j, k, bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_SPINY, 0, 0, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_min, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_min, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, TRUE, FALSE))
						return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_neurons().");
				}
				cntr++;
				spike_sorter_on = TRUE;
			}
		}
	}
	if (! spike_sorter_on)
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! spike_sorter_on.");	
	return TRUE;
}





