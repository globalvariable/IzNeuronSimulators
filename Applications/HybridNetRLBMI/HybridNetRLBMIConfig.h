#ifndef HYBRID_NET_RL_BMI_CONFIG_H
#define HYBRID_NET_RL_BMI_CONFIG_H

#define LAYER_BASE_SERVO_EXTENSOR_MOTOR 			0		//   decreasing pulse width for servo)   ///  TURN TO RIGHT
#define LAYER_BASE_SERVO_EXTENSOR_INTER			1
#define LAYER_BASE_SERVO_FLEXOR_MOTOR				2		//   incresing pulse width for servo)   ///  TURN TO LEFT
#define LAYER_BASE_SERVO_FLEXOR_INTER				3
#define LAYER_BASE_SERVO_ANGLE_SENS_SPINDLE		4

#define LAYER_SHOULDER_SERVO_EXTENSOR_MOTOR 		5		//   decreasing pulse width for servo)
#define LAYER_SHOULDER_SERVO_EXTENSOR_INTER		6	
#define LAYER_SHOULDER_SERVO_FLEXOR_MOTOR			7		//   incresing pulse width for servo)
#define LAYER_SHOULDER_SERVO_FLEXOR_INTER			8
#define LAYER_SHOULDER_SERVO_ANGLE_SENS_SPINDLE	9	

#define LAYER_ELBOW_SERVO_EXTENSOR_MOTOR 			10		//   decreasing pulse width for servo)
#define LAYER_ELBOW_SERVO_EXTENSOR_INTER			11	
#define LAYER_ELBOW_SERVO_FLEXOR_MOTOR			12		//   incresing pulse width for servo)
#define LAYER_ELBOW_SERVO_FLEXOR_INTER				13
#define LAYER_ELBOW_SERVO_ANGLE_SENS_SPINDLE		14	

#define NUM_OF_IN_SILICO_NETWORK_LAYERS 				15	

#define NUM_OF_ANGULAR_SPINDLES						16

#define NUM_OF_FLEXOR_IA_SPINDLES						1
#define NUM_OF_EXTENSOR_IA_SPINDLES					NUM_OF_FLEXOR_IA_SPINDLES

#define PS_INTEGRATION_MAX_ORDER			40
#define PS_INTEGRATION_ERR_TOL			1e-3

#include "HybridNetRLBMI.h"


bool add_neurons_for_external_and_in_silico_network(HybridNetRLBMIData *bmi_data);
bool submit_parker_sochacki_integration_precision(HybridNetRLBMIData *bmi_data);
bool set_output_layers(HybridNetRLBMIData *bmi_data);
bool connect_external_to_in_silico_network(HybridNetRLBMIData *bmi_data);
bool connect_angle_sensitive_spindles_to_in_silico_network(HybridNetRLBMIData *bmi_data);
bool connect_inter_neurons_between_motor_neurons(HybridNetRLBMIData *bmi_data);
bool submit_stdp_elig_for_layer_nrn_grp(HybridNetRLBMIData *bmi_data);
#endif
