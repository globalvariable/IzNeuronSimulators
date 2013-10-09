#ifndef HYBRID_NET_RL_BMI_CONFIG_H
#define HYBRID_NET_RL_BMI_CONFIG_H

#define MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE 2    // to decrease the number of synaptic connections to SNN simulators.

#define LAYER_BASE_SERVO_EXTENSOR_SPINY 			0		//   decreasing pulse width for servo)   ///  TURN TO RIGHT
#define LAYER_BASE_SERVO_EXTENSOR_BABBLE			1		//   to be suitable for mov obj handler output layer
#define LAYER_BASE_SERVO_FLEXOR_SPINY				2		//   incresing pulse width for servo)   ///  TURN TO LEFT
#define LAYER_BASE_SERVO_FLEXOR_BABBLE				3		//   to be suitable for mov obj handler output layer
#define LAYER_POISSON_NEURONS						4		//   to be suitable for mov obj handler output layer

#define NUM_OF_IN_SILICO_NETWORK_LAYERS 				5	

#define PS_INTEGRATION_MAX_ORDER			40
#define PS_INTEGRATION_ERR_TOL			1e-3

#include "HybridNetRLBMI.h"


bool add_neurons_for_external_and_in_silico_network(HybridNetRLBMIData *bmi_data);
bool submit_parker_sochacki_integration_precision(HybridNetRLBMIData *bmi_data);
bool set_output_layers(HybridNetRLBMIData *bmi_data);
bool connect_external_to_in_silico_network(HybridNetRLBMIData *bmi_data);
bool connect_medium_spiny_neurons(HybridNetRLBMIData *bmi_data);
bool connect_babling_neurons(HybridNetRLBMIData *bmi_data);
bool connect_babling_2_medium_spiny_neurons(HybridNetRLBMIData *bmi_data);

#endif
