#ifndef HYBRID_NET_RL_BMI_H
#define HYBRID_NET_RL_BMI_H

#define LAYER_BASE_SERVO_EXTENSOR_MOTOR 			0		//   decreasing pulse width for servo)   ///  TURN TO RIGHT
#define LAYER_BASE_SERVO_EXTENSOR_INHI				1
#define LAYER_BASE_SERVO_EXTENSOR_INTER			2
#define LAYER_BASE_SERVO_EXTENSOR_SPINDLE			3
#define LAYER_BASE_SERVO_FLEXOR_MOTOR				4		//   incresing pulse width for servo)   ///  TURN TO LEFT
#define LAYER_BASE_SERVO_FLEXOR_INHI					5		
#define LAYER_BASE_SERVO_FLEXOR_INTER				6
#define LAYER_BASE_SERVO_FLEXOR_SPINDLE				7
#define LAYER_BASE_SERVO_SPINDLE						8

#define LAYER_SHOULDER_SERVO_EXTENSOR_MOTOR 		9		//   decreasing pulse width for servo)
#define LAYER_SHOULDER_SERVO_EXTENSOR_INHI			10
#define LAYER_SHOULDER_SERVO_EXTENSOR_INTER		11	
#define LAYER_SHOULDER_SERVO_EXTENSOR_SPINDLE	12	
#define LAYER_SHOULDER_SERVO_FLEXOR_MOTOR			13		//   incresing pulse width for servo)
#define LAYER_SHOULDER_SERVO_FLEXOR_INHI			14		
#define LAYER_SHOULDER_SERVO_FLEXOR_INTER			15
#define LAYER_SHOULDER_SERVO_FLEXOR_SPINDLE		16	
#define LAYER_SHOULDER_SERVO_SPINDLE				17	

#define LAYER_ELBOW_SERVO_EXTENSOR_MOTOR 			18		//   decreasing pulse width for servo)
#define LAYER_ELBOW_SERVO_EXTENSOR_INHI			19
#define LAYER_ELBOW_SERVO_EXTENSOR_INTER			20	
#define LAYER_ELBOW_SERVO_EXTENSOR_SPINDLE		21		
#define LAYER_ELBOW_SERVO_FLEXOR_MOTOR			22		//   incresing pulse width for servo)
#define LAYER_ELBOW_SERVO_FLEXOR_INHI				23		
#define LAYER_ELBOW_SERVO_FLEXOR_INTER				24
#define LAYER_ELBOW_SERVO_FLEXOR_SPINDLE			25	
#define LAYER_ELBOW_SERVO_SPINDLE					26	

#define NUM_OF_IN_SILICO_NETWORK_LAYERS 				27	

#define NUM_OF_ANGULAR_SPINDLES						12
#define NUM_OF_FLEXOR_SPINDLES						1
#define NUM_OF_EXTENSOR_SPINDLES					NUM_OF_FLEXOR_SPINDLES


typedef struct __HybridNetRLBMIData HybridNetRLBMIData;

#include "Gui.h"
#include "../../../BlueSpike/SpikeTimeStamp.h"
#include "../../../BlueSpike/TemplateMatchingData.h"
#include "../../../BlueSpike/System/ShmSemNum/ShmSemNum.h"
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../Library/NeuronDynamicsData/NeuronDynamicsData.h"
#include "../../Library/STDPData/STDPData.h"
#include "../../Library/EligibilityData/EligibilityData.h"
#include "../../Library/EligibilityData/DepolEligibilityData.h"
#include "../../Library/SpikeData/SpikeData.h"
#include "../../Library/Stats/FiringStats.h"
#include "../../Library/InjectionCurrentData/InjectionCurrentData.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.h"
#include "../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.h"
#include "../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.h"
#include "../../../ExperimentHandlers/Library/Messages/NeuralNet2PostTrialHand.h"
#include "../../../ExperimentHandlers/Library/Status/TrialStatus.h"
#include "../../Library/Spindle/Spindle.h"

struct __HybridNetRLBMIData		
{
	SpikeTimeStamp 				*sorted_spike_time_stamp;    /// spike time stamps from biological neurons 
	RtTasksData					*rt_tasks_data;
	Network						*in_silico_network;
	Network						*blue_spike_network;
	NeuronDynamicsBufferLimited	*neuron_dynamics_limited_buffer;
	STDPBufferLimited			*stdp_limited_buffer;
	EligibilityBufferLimited			*eligibility_limited_buffer;
	DepolEligibilityBufferLimited		*depol_eligibility_limited_buffer;
	SpikeData					*blue_spike_spike_data;   // for visualization
	SpikeData					*in_silico_spike_data;  // for visualization
	TrialHand2NeuralNetMsg		*msgs_trial_hand_2_neural_net;
	NeuralNet2TrialHandMsg		*msgs_neural_net_2_trial_hand;
	MovObjHand2NeuralNetMsgMultiThread		*msgs_mov_obj_hand_2_neural_net_multi_thread;
	NeuralNet2MovObjHandMsgMultiThread		*msgs_neural_net_2_mov_obj_hand_multi_thread;
	NeuralNet2PostTrialHandMsg	*msgs_neural_net_2_post_trial_hand;
	TrialStatusEvents				*trial_status_events;   // to show status changed in graphs
	unsigned int					num_of_dedicated_cpu_threads;
	bool						simulation_in_progress;    // not to allow offline simulations while rt simulation is in progress
	ServoAngularLimit				servo_angle_min_max[THREE_DOF_ROBOT_NUM_OF_SERVOS];
	ExponentialAngularSpindleGroup	*angle_sensitive_spindles[THREE_DOF_ROBOT_NUM_OF_SERVOS];
	ExponentialPrimaryExtensorFlexorSpindleGroup *extensor_flexor_spindles[THREE_DOF_ROBOT_NUM_OF_SERVOS];
};

HybridNetRLBMIData * get_hybrid_net_rl_bmi_data(void);


#endif
