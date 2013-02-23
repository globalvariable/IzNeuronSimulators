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
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_BASE_SERVO_EXTENSOR_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_BASE_SERVO_EXTENSOR_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_BASE_SERVO_FLEXOR_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_BASE_SERVO_FLEXOR_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	C = 50.0; 
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, NUM_OF_ANGULAR_SPINDLES, LAYER_BASE_SERVO_ANGLE_SENS_SPINDLE, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	// SHOULDER SERVO
	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_SHOULDER_SERVO_EXTENSOR_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_SHOULDER_SERVO_EXTENSOR_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_SHOULDER_SERVO_FLEXOR_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_SHOULDER_SERVO_FLEXOR_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	C = 50.0; 
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, NUM_OF_ANGULAR_SPINDLES, LAYER_SHOULDER_SERVO_ANGLE_SENS_SPINDLE, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	// ELBOW SERVO
	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_ELBOW_SERVO_EXTENSOR_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_ELBOW_SERVO_EXTENSOR_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_ELBOW_SERVO_FLEXOR_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_ELBOW_SERVO_FLEXOR_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	C = 50.0; 
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, NUM_OF_ANGULAR_SPINDLES, LAYER_ELBOW_SERVO_ANGLE_SENS_SPINDLE, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
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
	SynapticWeight weight_excitatory_max = 10;
	SynapticWeight weight_excitatory_min = 1;
	SynapticWeight weight_inhibitory_max = 10;
	SynapticWeight weight_inhibitory_min = 1;
	AxonalDelay EPSP_delay_max = (AxonalDelay)(7.0 * 1000000);
	AxonalDelay EPSP_delay_min = (AxonalDelay)(7.0 * 1000000);
	AxonalDelay IPSP_delay_max = (AxonalDelay)(7.0 * 1000000);
	AxonalDelay IPSP_delay_min = (AxonalDelay)(7.0 * 1000000);
	double excitatory_connection_probability = 1.0;
	double inhibitory_connection_probability = 1.0;

	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		if (! connect_layers(bmi_data->blue_spike_network, i, bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_MOTOR, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_max, EPSP_delay_min, IPSP_delay_max, IPSP_delay_min, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, excitatory_connection_probability, inhibitory_connection_probability, TRUE, TRUE, TRUE))
			return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_layers().");
	}
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		if (! connect_layers(bmi_data->blue_spike_network, i, bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_MOTOR, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_max, EPSP_delay_min, IPSP_delay_max, IPSP_delay_min, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, excitatory_connection_probability, inhibitory_connection_probability, TRUE, TRUE, TRUE))
			return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_layers().");
	}
	return TRUE;
}

bool connect_angle_sensitive_spindles_to_in_silico_network(HybridNetRLBMIData *bmi_data)
{
	SynapticWeight weight_excitatory_max = 10;
	SynapticWeight weight_excitatory_min = 1;
	SynapticWeight weight_inhibitory_max = 10;
	SynapticWeight weight_inhibitory_min = 1;
	AxonalDelay EPSP_delay_max = (AxonalDelay)(7.0 * 1000000);
	AxonalDelay EPSP_delay_min = (AxonalDelay)(7.0 * 1000000);
	AxonalDelay IPSP_delay_max = (AxonalDelay)(7.0 * 1000000);
	AxonalDelay IPSP_delay_min = (AxonalDelay)(7.0 * 1000000);
	double excitatory_connection_probability = 1.0;
	double inhibitory_connection_probability = 1.0;

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_ANGLE_SENS_SPINDLE, bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_MOTOR, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_max, EPSP_delay_min, IPSP_delay_max, IPSP_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, excitatory_connection_probability, inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_angle_sensitive_spindles_to_in_silico_network", "! connect_layers().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_ANGLE_SENS_SPINDLE, bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_MOTOR, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_max, EPSP_delay_min, IPSP_delay_max, IPSP_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, excitatory_connection_probability, inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_angle_sensitive_spindles_to_in_silico_network", "! connect_layers().");

	return TRUE;
}

bool connect_inter_neurons_between_motor_neurons(HybridNetRLBMIData *bmi_data)
{
	SynapticWeight weight_excitatory_max = 10;
	SynapticWeight weight_excitatory_min = 10;
	SynapticWeight weight_inhibitory_max = 10;
	SynapticWeight weight_inhibitory_min = 10;
	AxonalDelay EPSP_delay_max = (AxonalDelay)(7.0 * 1000000);
	AxonalDelay EPSP_delay_min = (AxonalDelay)(7.0 * 1000000);
	AxonalDelay IPSP_delay_max = (AxonalDelay)(7.0 * 1000000);
	AxonalDelay IPSP_delay_min = (AxonalDelay)(7.0 * 1000000);
	double excitatory_connection_probability = 1.0;
	double inhibitory_connection_probability = 1.0;

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_INTER, bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_MOTOR, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_max, EPSP_delay_min, IPSP_delay_max, IPSP_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, excitatory_connection_probability, inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_INTER, bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_MOTOR, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_max, EPSP_delay_min, IPSP_delay_max, IPSP_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, excitatory_connection_probability, inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_angle_sensitive_spindles_to_in_silico_network().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_MOTOR, bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_INTER, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_max, EPSP_delay_min, IPSP_delay_max, IPSP_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, excitatory_connection_probability, inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	if (! connect_layers(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_MOTOR, bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_INTER, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_max, EPSP_delay_min, IPSP_delay_max, IPSP_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, excitatory_connection_probability, inhibitory_connection_probability, TRUE, TRUE, TRUE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	return TRUE;
}

bool submit_stdp_elig_for_layer_nrn_grp(HybridNetRLBMIData *bmi_data)
{
	if (! create_ps_stdp_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_INTER, 0, get_maximum_parker_sochacki_order(), 0, 0, 10, 10, 0, 0, 10, 10))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_stdp_for_neuron_group().");	
	if (! create_ps_eligibility_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_EXTENSOR_INTER, 0, get_maximum_parker_sochacki_order(),  10, 10, 0, 0))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_eligibility_for_neuron_group().");
	if (! create_ps_stdp_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_INTER, 0, get_maximum_parker_sochacki_order(), 0, 0, 10, 10, 0, 0, 10, 10))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_stdp_for_neuron_group().");	
	if (! create_ps_eligibility_for_neuron_group(bmi_data->in_silico_network, LAYER_BASE_SERVO_FLEXOR_INTER, 0, get_maximum_parker_sochacki_order(),  10, 10, 0, 0))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_stdp_elig_for_layer_nrn_grp", "! create_ps_eligibility_for_neuron_group().");
	return TRUE;
}
