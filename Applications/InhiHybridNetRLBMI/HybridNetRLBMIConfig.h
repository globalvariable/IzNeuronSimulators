#ifndef HYBRID_NET_RL_BMI_CONFIG_H
#define HYBRID_NET_RL_BMI_CONFIG_H

#define MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE 2    // to decrease the number of synaptic connections to SNN simulators.

#define LAYER_BASE_SERVO_EXTENSOR_MOTOR 			0		//   decreasing pulse width for servo)   ///  TURN TO RIGHT
#define LAYER_BASE_SERVO_EXTENSOR_INTER			1
#define LAYER_BASE_SERVO_FLEXOR_MOTOR				2		//   incresing pulse width for servo)   ///  TURN TO LEFT
#define LAYER_BASE_SERVO_FLEXOR_INTER				3
#define LAYER_BASE_SERVO_STATION_MOTOR				4		//   incresing pulse width for servo)   ///  TURN TO LEFT
#define LAYER_BASE_SERVO_STATION_INTER				5
#define LAYER_BASE_SERVO_EXTENSOR_EXCI				6
#define LAYER_BASE_SERVO_FLEXOR_EXCI				7
#define LAYER_BASE_SERVO_STATION_EXCI				8


#define NUM_OF_IN_SILICO_NETWORK_LAYERS 				9	

#define PS_INTEGRATION_MAX_ORDER			40
#define PS_INTEGRATION_ERR_TOL			1e-3

#include "HybridNetRLBMI.h"


bool add_neurons_for_external_and_in_silico_network(HybridNetRLBMIData *bmi_data);
bool submit_parker_sochacki_integration_precision(HybridNetRLBMIData *bmi_data);
bool set_output_layers(HybridNetRLBMIData *bmi_data);
bool pre_simulate_network(HybridNetRLBMIData *bmi_data);
bool connect_external_to_in_silico_network(HybridNetRLBMIData *bmi_data);
bool connect_inter_neurons_between_motor_neurons(HybridNetRLBMIData *bmi_data);
bool connect_excitatory_neurons_of_motor_neurons(HybridNetRLBMIData *bmi_data);
bool submit_stdp_elig_for_layer_nrn_grp(HybridNetRLBMIData *bmi_data);
#endif
