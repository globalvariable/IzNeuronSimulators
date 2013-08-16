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
	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 20, LAYER_BASE_SERVO_EXTENSOR_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 10, LAYER_BASE_SERVO_EXTENSOR_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 20, LAYER_BASE_SERVO_FLEXOR_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 10, LAYER_BASE_SERVO_FLEXOR_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 20, LAYER_BASE_SERVO_STATION_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 10, LAYER_BASE_SERVO_STATION_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 10, LAYER_BASE_SERVO_EXTENSOR_EXCI, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 10, LAYER_BASE_SERVO_FLEXOR_EXCI, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 10, LAYER_BASE_SERVO_STATION_EXCI, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
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
	set_layer_type_of_the_neurons_in_layer(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_MOTOR, NEURON_LAYER_TYPE_OUTPUT);
	set_layer_type_of_the_neurons_in_layer(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_MOTOR, NEURON_LAYER_TYPE_OUTPUT);
	return TRUE;	
}

bool connect_external_to_in_silico_network(HybridNetRLBMIData *bmi_data)
{
	unsigned int i;
	HybridNetRLBMISynapseData	*synapse_data = &(bmi_data->synapse_data);
	synapse_data->blue_spike_2_in_silico_excitatory_connection_probability = 0.5;
	synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_max = 4;
	synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_min = 4;
	synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	synapse_data->blue_spike_2_in_silico_inhibitory_connection_probability = 0.5;
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_max = 4;
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_min = 4;
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		if (! connect_layers(bmi_data->blue_spike_network, i, bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_MOTOR, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_min, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_min, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, synapse_data->blue_spike_2_in_silico_excitatory_connection_probability, synapse_data->blue_spike_2_in_silico_inhibitory_connection_probability, TRUE, TRUE, TRUE))
			return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_layers().");
	}

	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		if (! connect_layers(bmi_data->blue_spike_network, i, bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_MOTOR, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_min, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_min, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, synapse_data->blue_spike_2_in_silico_excitatory_connection_probability, synapse_data->blue_spike_2_in_silico_inhibitory_connection_probability, TRUE, TRUE, TRUE))
			return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_layers().");
	}

	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		if (! connect_layers(bmi_data->blue_spike_network, i, bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_MOTOR, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_min, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_min, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, synapse_data->blue_spike_2_in_silico_excitatory_connection_probability, synapse_data->blue_spike_2_in_silico_inhibitory_connection_probability, TRUE, TRUE, TRUE))
			return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_layers().");
	}

	return TRUE;
}

bool connect_inter_neurons_between_motor_neurons(HybridNetRLBMIData *bmi_data)
{
	HybridNetRLBMISynapseData	*synapse_data = &(bmi_data->synapse_data);

	synapse_data->motor_2_inter_neuron_excitatory_connection_probability = 0.4;
	synapse_data->motor_2_inter_neuron_excitatory_connection_weight_max = 5;
	synapse_data->motor_2_inter_neuron_excitatory_connection_weight_min = 5;
	synapse_data->motor_2_inter_neuron_excitatory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->motor_2_inter_neuron_excitatory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	synapse_data->motor_2_inter_neuron_inhibitory_connection_probability = 0;
	synapse_data->motor_2_inter_neuron_inhibitory_connection_weight_max = 7;
	synapse_data->motor_2_inter_neuron_inhibitory_connection_weight_min = 7;
	synapse_data->motor_2_inter_neuron_inhibitory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->motor_2_inter_neuron_inhibitory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	synapse_data->inter_2_motor_neuron_excitatory_connection_probability = 0;
	synapse_data->inter_2_motor_neuron_excitatory_connection_weight_max = 0.5;
	synapse_data->inter_2_motor_neuron_excitatory_connection_weight_min = 0.5;
	synapse_data->inter_2_motor_neuron_excitatory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->inter_2_motor_neuron_excitatory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	synapse_data->inter_2_motor_neuron_inhibitory_connection_probability = 0.8;
	synapse_data->inter_2_motor_neuron_inhibitory_connection_weight_max = 7;
	synapse_data->inter_2_motor_neuron_inhibitory_connection_weight_min = 7;
	synapse_data->inter_2_motor_neuron_inhibitory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->inter_2_motor_neuron_inhibitory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_MOTOR, bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_INTER, synapse_data->motor_2_inter_neuron_excitatory_connection_weight_max, synapse_data->motor_2_inter_neuron_excitatory_connection_weight_min, synapse_data->motor_2_inter_neuron_inhibitory_connection_weight_max, synapse_data->motor_2_inter_neuron_inhibitory_connection_weight_min, synapse_data->motor_2_inter_neuron_excitatory_connection_delay_max, synapse_data->motor_2_inter_neuron_excitatory_connection_delay_min, synapse_data->motor_2_inter_neuron_inhibitory_connection_delay_max, synapse_data->motor_2_inter_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->motor_2_inter_neuron_excitatory_connection_probability, synapse_data->motor_2_inter_neuron_inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_MOTOR, bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_INTER, synapse_data->motor_2_inter_neuron_excitatory_connection_weight_max, synapse_data->motor_2_inter_neuron_excitatory_connection_weight_min, synapse_data->motor_2_inter_neuron_inhibitory_connection_weight_max, synapse_data->motor_2_inter_neuron_inhibitory_connection_weight_min, synapse_data->motor_2_inter_neuron_excitatory_connection_delay_max, synapse_data->motor_2_inter_neuron_excitatory_connection_delay_min, synapse_data->motor_2_inter_neuron_inhibitory_connection_delay_max, synapse_data->motor_2_inter_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->motor_2_inter_neuron_excitatory_connection_probability, synapse_data->motor_2_inter_neuron_inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_MOTOR, bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_INTER, synapse_data->motor_2_inter_neuron_excitatory_connection_weight_max, synapse_data->motor_2_inter_neuron_excitatory_connection_weight_min, synapse_data->motor_2_inter_neuron_inhibitory_connection_weight_max, synapse_data->motor_2_inter_neuron_inhibitory_connection_weight_min, synapse_data->motor_2_inter_neuron_excitatory_connection_delay_max, synapse_data->motor_2_inter_neuron_excitatory_connection_delay_min, synapse_data->motor_2_inter_neuron_inhibitory_connection_delay_max, synapse_data->motor_2_inter_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->motor_2_inter_neuron_excitatory_connection_probability, synapse_data->motor_2_inter_neuron_inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_INTER, bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_MOTOR, synapse_data->inter_2_motor_neuron_excitatory_connection_weight_max, synapse_data->inter_2_motor_neuron_excitatory_connection_weight_min, synapse_data->inter_2_motor_neuron_inhibitory_connection_weight_max, synapse_data->inter_2_motor_neuron_inhibitory_connection_weight_min, synapse_data->inter_2_motor_neuron_excitatory_connection_delay_max, synapse_data->inter_2_motor_neuron_excitatory_connection_delay_min, synapse_data->inter_2_motor_neuron_inhibitory_connection_delay_max, synapse_data->inter_2_motor_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->inter_2_motor_neuron_excitatory_connection_probability , synapse_data->inter_2_motor_neuron_inhibitory_connection_probability , TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_INTER, bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_MOTOR, synapse_data->inter_2_motor_neuron_excitatory_connection_weight_max, synapse_data->inter_2_motor_neuron_excitatory_connection_weight_min, synapse_data->inter_2_motor_neuron_inhibitory_connection_weight_max, synapse_data->inter_2_motor_neuron_inhibitory_connection_weight_min, synapse_data->inter_2_motor_neuron_excitatory_connection_delay_max, synapse_data->inter_2_motor_neuron_excitatory_connection_delay_min, synapse_data->inter_2_motor_neuron_inhibitory_connection_delay_max, synapse_data->inter_2_motor_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->inter_2_motor_neuron_excitatory_connection_probability , synapse_data->inter_2_motor_neuron_inhibitory_connection_probability , TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_INTER, bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_MOTOR, synapse_data->inter_2_motor_neuron_excitatory_connection_weight_max, synapse_data->inter_2_motor_neuron_excitatory_connection_weight_min, synapse_data->inter_2_motor_neuron_inhibitory_connection_weight_max, synapse_data->inter_2_motor_neuron_inhibitory_connection_weight_min, synapse_data->inter_2_motor_neuron_excitatory_connection_delay_max, synapse_data->inter_2_motor_neuron_excitatory_connection_delay_min, synapse_data->inter_2_motor_neuron_inhibitory_connection_delay_max, synapse_data->inter_2_motor_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->inter_2_motor_neuron_excitatory_connection_probability, synapse_data->inter_2_motor_neuron_inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_angle_sensitive_spindles_to_in_silico_network().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_INTER, bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_MOTOR, synapse_data->inter_2_motor_neuron_excitatory_connection_weight_max, synapse_data->inter_2_motor_neuron_excitatory_connection_weight_min, synapse_data->inter_2_motor_neuron_inhibitory_connection_weight_max, synapse_data->inter_2_motor_neuron_inhibitory_connection_weight_min, synapse_data->inter_2_motor_neuron_excitatory_connection_delay_max, synapse_data->inter_2_motor_neuron_excitatory_connection_delay_min, synapse_data->inter_2_motor_neuron_inhibitory_connection_delay_max, synapse_data->inter_2_motor_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->inter_2_motor_neuron_excitatory_connection_probability, synapse_data->inter_2_motor_neuron_inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_angle_sensitive_spindles_to_in_silico_network().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_INTER, bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_MOTOR, synapse_data->inter_2_motor_neuron_excitatory_connection_weight_max, synapse_data->inter_2_motor_neuron_excitatory_connection_weight_min, synapse_data->inter_2_motor_neuron_inhibitory_connection_weight_max, synapse_data->inter_2_motor_neuron_inhibitory_connection_weight_min, synapse_data->inter_2_motor_neuron_excitatory_connection_delay_max, synapse_data->inter_2_motor_neuron_excitatory_connection_delay_min, synapse_data->inter_2_motor_neuron_inhibitory_connection_delay_max, synapse_data->inter_2_motor_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->inter_2_motor_neuron_excitatory_connection_probability, synapse_data->inter_2_motor_neuron_inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_angle_sensitive_spindles_to_in_silico_network().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_INTER, bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_MOTOR, synapse_data->inter_2_motor_neuron_excitatory_connection_weight_max, synapse_data->inter_2_motor_neuron_excitatory_connection_weight_min, synapse_data->inter_2_motor_neuron_inhibitory_connection_weight_max, synapse_data->inter_2_motor_neuron_inhibitory_connection_weight_min, synapse_data->inter_2_motor_neuron_excitatory_connection_delay_max, synapse_data->inter_2_motor_neuron_excitatory_connection_delay_min, synapse_data->inter_2_motor_neuron_inhibitory_connection_delay_max, synapse_data->inter_2_motor_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->inter_2_motor_neuron_excitatory_connection_probability, synapse_data->inter_2_motor_neuron_inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_angle_sensitive_spindles_to_in_silico_network().");

	return TRUE;
}

bool connect_excitatory_neurons_of_motor_neurons(HybridNetRLBMIData *bmi_data)
{
	HybridNetRLBMISynapseData	*synapse_data = &(bmi_data->synapse_data);

	synapse_data->motor_2_exci_neuron_excitatory_connection_probability = 0.5;
	synapse_data->motor_2_exci_neuron_excitatory_connection_weight_max = 3;
	synapse_data->motor_2_exci_neuron_excitatory_connection_weight_min = 3;
	synapse_data->motor_2_exci_neuron_excitatory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->motor_2_exci_neuron_excitatory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	synapse_data->motor_2_exci_neuron_inhibitory_connection_probability = 0;
	synapse_data->motor_2_exci_neuron_inhibitory_connection_weight_max = 5;
	synapse_data->motor_2_exci_neuron_inhibitory_connection_weight_min = 5;
	synapse_data->motor_2_exci_neuron_inhibitory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->motor_2_exci_neuron_inhibitory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	synapse_data->exci_2_motor_neuron_excitatory_connection_probability = 0.5;
	synapse_data->exci_2_motor_neuron_excitatory_connection_weight_max = 3;
	synapse_data->exci_2_motor_neuron_excitatory_connection_weight_min = 3;
	synapse_data->exci_2_motor_neuron_excitatory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->exci_2_motor_neuron_excitatory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	synapse_data->exci_2_motor_neuron_inhibitory_connection_probability = 0;
	synapse_data->exci_2_motor_neuron_inhibitory_connection_weight_max = 5;
	synapse_data->exci_2_motor_neuron_inhibitory_connection_weight_min = 5;
	synapse_data->exci_2_motor_neuron_inhibitory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->exci_2_motor_neuron_inhibitory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_MOTOR, bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_EXCI, synapse_data->motor_2_exci_neuron_excitatory_connection_weight_max, synapse_data->motor_2_exci_neuron_excitatory_connection_weight_min, synapse_data->motor_2_exci_neuron_inhibitory_connection_weight_max, synapse_data->motor_2_exci_neuron_inhibitory_connection_weight_min, synapse_data->motor_2_exci_neuron_excitatory_connection_delay_max, synapse_data->motor_2_exci_neuron_excitatory_connection_delay_min, synapse_data->motor_2_exci_neuron_inhibitory_connection_delay_max, synapse_data->motor_2_exci_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->motor_2_exci_neuron_excitatory_connection_probability, synapse_data->motor_2_exci_neuron_inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");


	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_EXCI, bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_MOTOR, synapse_data->exci_2_motor_neuron_excitatory_connection_weight_max, synapse_data->exci_2_motor_neuron_excitatory_connection_weight_min, synapse_data->exci_2_motor_neuron_inhibitory_connection_weight_max, synapse_data->exci_2_motor_neuron_inhibitory_connection_weight_min, synapse_data->exci_2_motor_neuron_excitatory_connection_delay_max, synapse_data->exci_2_motor_neuron_excitatory_connection_delay_min, synapse_data->exci_2_motor_neuron_inhibitory_connection_delay_max, synapse_data->exci_2_motor_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->exci_2_motor_neuron_excitatory_connection_probability, synapse_data->exci_2_motor_neuron_inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_MOTOR, bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_EXCI, synapse_data->motor_2_exci_neuron_excitatory_connection_weight_max, synapse_data->motor_2_exci_neuron_excitatory_connection_weight_min, synapse_data->motor_2_exci_neuron_inhibitory_connection_weight_max, synapse_data->motor_2_exci_neuron_inhibitory_connection_weight_min, synapse_data->motor_2_exci_neuron_excitatory_connection_delay_max, synapse_data->motor_2_exci_neuron_excitatory_connection_delay_min, synapse_data->motor_2_exci_neuron_inhibitory_connection_delay_max, synapse_data->motor_2_exci_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->motor_2_exci_neuron_excitatory_connection_probability, synapse_data->motor_2_exci_neuron_inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_EXCI, bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_MOTOR, synapse_data->exci_2_motor_neuron_excitatory_connection_weight_max, synapse_data->exci_2_motor_neuron_excitatory_connection_weight_min, synapse_data->exci_2_motor_neuron_inhibitory_connection_weight_max, synapse_data->exci_2_motor_neuron_inhibitory_connection_weight_min, synapse_data->exci_2_motor_neuron_excitatory_connection_delay_max, synapse_data->exci_2_motor_neuron_excitatory_connection_delay_min, synapse_data->exci_2_motor_neuron_inhibitory_connection_delay_max, synapse_data->exci_2_motor_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->exci_2_motor_neuron_excitatory_connection_probability, synapse_data->exci_2_motor_neuron_inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_MOTOR, bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_EXCI, synapse_data->motor_2_exci_neuron_excitatory_connection_weight_max, synapse_data->motor_2_exci_neuron_excitatory_connection_weight_min, synapse_data->motor_2_exci_neuron_inhibitory_connection_weight_max, synapse_data->motor_2_exci_neuron_inhibitory_connection_weight_min, synapse_data->motor_2_exci_neuron_excitatory_connection_delay_max, synapse_data->motor_2_exci_neuron_excitatory_connection_delay_min, synapse_data->motor_2_exci_neuron_inhibitory_connection_delay_max, synapse_data->motor_2_exci_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->motor_2_exci_neuron_excitatory_connection_probability, synapse_data->motor_2_exci_neuron_inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_EXCI, bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_MOTOR, synapse_data->exci_2_motor_neuron_excitatory_connection_weight_max, synapse_data->exci_2_motor_neuron_excitatory_connection_weight_min, synapse_data->exci_2_motor_neuron_inhibitory_connection_weight_max, synapse_data->exci_2_motor_neuron_inhibitory_connection_weight_min, synapse_data->exci_2_motor_neuron_excitatory_connection_delay_max, synapse_data->exci_2_motor_neuron_excitatory_connection_delay_min, synapse_data->exci_2_motor_neuron_inhibitory_connection_delay_max, synapse_data->exci_2_motor_neuron_inhibitory_connection_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, synapse_data->exci_2_motor_neuron_excitatory_connection_probability, synapse_data->exci_2_motor_neuron_inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	return TRUE;
}


bool submit_stdp_elig_for_layer_nrn_grp(HybridNetRLBMIData *bmi_data)
{
	if (! create_ps_stdp_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_INTER, 0, get_maximum_parker_sochacki_order(), 0, 0, 10, 10, 0, 0, 10, 10))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_stdp_for_neuron_group().");	
	if (! create_ps_eligibility_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_INTER, 0, get_maximum_parker_sochacki_order(),  10, 10, 0, 0, 1, 0, -50, -50))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_eligibility_for_neuron_group().");

	if (! create_ps_stdp_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_INTER, 0, get_maximum_parker_sochacki_order(), 0, 0, 10, 10, 0, 0, 10, 10))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_stdp_for_neuron_group().");	
	if (! create_ps_eligibility_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_INTER, 0, get_maximum_parker_sochacki_order(),  10, 10, 0, 0, 1, 0, -50, -50))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_eligibility_for_neuron_group().");

	if (! create_ps_stdp_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_INTER, 0, get_maximum_parker_sochacki_order(), 0, 0, 10, 10, 0, 0, 10, 10))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_stdp_for_neuron_group().");	
	if (! create_ps_eligibility_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_INTER, 0, get_maximum_parker_sochacki_order(),  10, 10, 0, 0, 1, 0, -50, -50))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_eligibility_for_neuron_group().");

	if (! create_ps_stdp_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_EXCI, 0, get_maximum_parker_sochacki_order(), 0, 0, 10, 10, 0, 0, 10, 10))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_stdp_for_neuron_group().");	
	if (! create_ps_eligibility_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_EXCI, 0, get_maximum_parker_sochacki_order(),  10, 10, 0, 0, 1, 0, -50, -50))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_eligibility_for_neuron_group().");

	if (! create_ps_stdp_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_EXCI, 0, get_maximum_parker_sochacki_order(), 0, 0, 10, 10, 0, 0, 10, 10))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_stdp_for_neuron_group().");	
	if (! create_ps_eligibility_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_EXCI, 0, get_maximum_parker_sochacki_order(),  10, 10, 0, 0, 1, 0, -50, -50))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_eligibility_for_neuron_group().");

	if (! create_ps_stdp_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_EXCI, 0, get_maximum_parker_sochacki_order(), 0, 0, 10, 10, 0, 0, 10, 10))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_stdp_for_neuron_group().");	
	if (! create_ps_eligibility_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_STATION_EXCI, 0, get_maximum_parker_sochacki_order(),  10, 10, 0, 0, 1, 0, -50, -50))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_eligibility_for_neuron_group().");


	return TRUE;
}


