#include "HybridNetRLBMIConfig.h"



bool add_neurons_for_external_and_in_silico_network(HybridNetRLBMIData *bmi_data)
{
	double v;
	double a;
	double b;
	double c;
	double d;
	double k;
	double C;
	double v_resting;
	double v_threshold;
	double v_peak;
//	double I_inject;
	bool inhibitory;
	double E_excitatory;
	double E_inhibitory;
	double tau_excitatory;
	double tau_inhibitory;
	unsigned int randomize_params = 0;
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
	get_neuron_type_parameters(NRN_TYPE_MEDIUM_SPINY, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, NUM_OF_OUTPUT_SPINY_PROJECTION_NEURONS, LAYER_BASE_SERVO_EXTENSOR_SPINY, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_MEDIUM_SPINY, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, NUM_OF_OUTPUT_SPINY_PROJECTION_NEURONS, LAYER_BASE_SERVO_FLEXOR_SPINY, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");
	
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

	bool connection_matrix[3][2] = {	{0,1},
								{1,0},
								{1,1}};


	synapse_data->blue_spike_2_in_silico_excitatory_connection_probability = 0.7;
	synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_max = 6.0;
	synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_min = 6.0;
	synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	synapse_data->blue_spike_2_in_silico_inhibitory_connection_probability = 0;
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_max = 12;
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_min = 12;
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j = 0; j < MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			for (k = 0; k < MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE; k++)
			{
				if (! (*bmi_data->sorted_spike_time_stamp)[i][j].included_units[k])
					continue;
				if ((connection_matrix[cntr % 3][0]))
				{
					if (! connect_neurons(bmi_data->blue_spike_network, i, j, k, bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_SPINY, 0, 0, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_min, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_min, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, TRUE, FALSE))
						return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_neurons().");
				}
				if ((connection_matrix[cntr % 3][1]))
				{
					if (! connect_neurons(bmi_data->blue_spike_network, i, j, k, bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_SPINY, 0, 0, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_min, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_min, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, TRUE, FALSE))
						return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_neurons().");
				}
				cntr++;
			}
		}
	}
	if (cntr == 0)
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "No connections has been made. no included units in bmi_data->sorted_spike_time_stamp.");
	return TRUE;
}



bool connect_medium_spiny_neurons(HybridNetRLBMIData *bmi_data)
{
	HybridNetRLBMISynapseData	*synapse_data = &(bmi_data->synapse_data);

	synapse_data->spiny_2_spiny_neuron_excitatory_connection_probability = 0.0;
	synapse_data->spiny_2_spiny_neuron_excitatory_connection_weight_max = 6;
	synapse_data->spiny_2_spiny_neuron_excitatory_connection_weight_min = 6;
	synapse_data->spiny_2_spiny_neuron_excitatory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->spiny_2_spiny_neuron_excitatory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	synapse_data->spiny_2_spiny_neuron_inhibitory_connection_probability = 1;
	synapse_data->spiny_2_spiny_neuron_inhibitory_connection_weight_max = 40;
	synapse_data->spiny_2_spiny_neuron_inhibitory_connection_weight_min = 40;
	synapse_data->spiny_2_spiny_neuron_inhibitory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->spiny_2_spiny_neuron_inhibitory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);


	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_SPINY, bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_SPINY, synapse_data->spiny_2_spiny_neuron_excitatory_connection_weight_max, synapse_data->spiny_2_spiny_neuron_excitatory_connection_weight_min, synapse_data->spiny_2_spiny_neuron_inhibitory_connection_weight_max, synapse_data->spiny_2_spiny_neuron_inhibitory_connection_weight_min, synapse_data->spiny_2_spiny_neuron_excitatory_connection_delay_max, synapse_data->spiny_2_spiny_neuron_excitatory_connection_delay_min, synapse_data->spiny_2_spiny_neuron_inhibitory_connection_delay_max, synapse_data->spiny_2_spiny_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->spiny_2_spiny_neuron_excitatory_connection_probability, synapse_data->spiny_2_spiny_neuron_inhibitory_connection_probability, FALSE, FALSE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_SPINY, bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_SPINY, synapse_data->spiny_2_spiny_neuron_excitatory_connection_weight_max, synapse_data->spiny_2_spiny_neuron_excitatory_connection_weight_min, synapse_data->spiny_2_spiny_neuron_inhibitory_connection_weight_max, synapse_data->spiny_2_spiny_neuron_inhibitory_connection_weight_min, synapse_data->spiny_2_spiny_neuron_excitatory_connection_delay_max, synapse_data->spiny_2_spiny_neuron_excitatory_connection_delay_min, synapse_data->spiny_2_spiny_neuron_inhibitory_connection_delay_max, synapse_data->spiny_2_spiny_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->spiny_2_spiny_neuron_excitatory_connection_probability, synapse_data->spiny_2_spiny_neuron_inhibitory_connection_probability, FALSE, FALSE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	return TRUE;
}

bool adjust_plastic_synaptic_weights(HybridNetRLBMIData *bmi_data)
{
	Neuron	**all_neurons;
	unsigned int num_of_all_neurons, i, j;
	double sum_weights, diff_weights;	
	Neuron *nrn;

	all_neurons = bmi_data->in_silico_network->all_neurons;

	num_of_all_neurons = bmi_data->in_silico_network->num_of_neurons;

	for (i = 0; i < num_of_all_neurons; i++) 
	{
		nrn = all_neurons[i];
		sum_weights = 0;
		for (j = 0; j < nrn->syn_list->num_of_synapses; j++)
		{
			if (! nrn->syn_list->synapses[j].plastic)
				continue;
			sum_weights += nrn->syn_list->synapses[j].weight;		
		}
		diff_weights = sum_weights - get_hybrid_net_rl_bmi_data()->total_synaptic_weights;

		for (j = 0; j < nrn->syn_list->num_of_synapses; j++)
		{
			if (! nrn->syn_list->synapses[j].plastic)
				continue;
			 nrn->syn_list->synapses[j].weight =  nrn->syn_list->synapses[j].weight - (( nrn->syn_list->synapses[j].weight/sum_weights)*diff_weights);
		}	
	}
	return TRUE;
}



