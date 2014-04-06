#ifndef HYBRID_NET_RL_BMI_CONFIG_H
#define HYBRID_NET_RL_BMI_CONFIG_H

#define MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE 2    // to decrease the number of synaptic connections to SNN simulators.

#define LAYER_BASE_SERVO_EXTENSOR_SPINY 			0		//   decreasing pulse width for servo)   ///  TURN TO RIGHT
#define LAYER_BASE_SERVO_FLEXOR_SPINY				1		//   incresing pulse width for servo)   ///  TURN TO LEFT

#define NUM_OF_IN_SILICO_NETWORK_LAYERS 				2	

#define PS_INTEGRATION_MAX_ORDER			40
#define PS_INTEGRATION_ERR_TOL			1e-3

#define REWARD_WINDOW_SIZE	4.0   /// keep it floating point
#define PREDICTION_ERROR_WINDOW_SIZE	3.0   /// keep it floating point

#include "HybridNetRLBMI.h"


bool add_neurons_for_external_and_in_silico_network(HybridNetRLBMIData *bmi_data);
bool submit_parker_sochacki_integration_precision(HybridNetRLBMIData *bmi_data);
bool set_output_layers(HybridNetRLBMIData *bmi_data);



#endif
