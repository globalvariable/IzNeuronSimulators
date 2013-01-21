#ifndef HYBRID_NET_RL_BMI_H
#define HYBRID_NET_RL_BMI_H

#define LAYER_BASE_SERVO_EXTENSOR_MOTOR 			0		//   decreasing pulse width for servo)   ///  TURN TO RIGHT
#define LAYER_BASE_SERVO_EXTENSOR_INHI				1
#define LAYER_BASE_SERVO_EXTENSOR_INTER			2
#define LAYER_BASE_SERVO_EXTENSOR_SPINDLE_II		3
#define LAYER_BASE_SERVO_EXTENSOR_SPINDLE_IA		4
#define LAYER_BASE_SERVO_FLEXOR_MOTOR				5		//   incresing pulse width for servo)   ///  TURN TO LEFT
#define LAYER_BASE_SERVO_FLEXOR_INHI					6		
#define LAYER_BASE_SERVO_FLEXOR_INTER				7
#define LAYER_BASE_SERVO_FLEXOR_SPINDLE_II			8
#define LAYER_BASE_SERVO_FLEXOR_SPINDLE_IA			9
#define LAYER_BASE_SERVO_ANGLE_SENS_SPINDLE		10

#define LAYER_SHOULDER_SERVO_EXTENSOR_MOTOR 		11		//   decreasing pulse width for servo)
#define LAYER_SHOULDER_SERVO_EXTENSOR_INHI			12
#define LAYER_SHOULDER_SERVO_EXTENSOR_INTER		13	
#define LAYER_SHOULDER_SERVO_EXTENSOR_SPINDLE_II	14	
#define LAYER_SHOULDER_SERVO_EXTENSOR_SPINDLE_IA	15
#define LAYER_SHOULDER_SERVO_FLEXOR_MOTOR			16		//   incresing pulse width for servo)
#define LAYER_SHOULDER_SERVO_FLEXOR_INHI			17		
#define LAYER_SHOULDER_SERVO_FLEXOR_INTER			18
#define LAYER_SHOULDER_SERVO_FLEXOR_SPINDLE_II		19	
#define LAYER_SHOULDER_SERVO_FLEXOR_SPINDLE_IA		20
#define LAYER_SHOULDER_SERVO_ANGLE_SENS_SPINDLE	21	

#define LAYER_ELBOW_SERVO_EXTENSOR_MOTOR 			22		//   decreasing pulse width for servo)
#define LAYER_ELBOW_SERVO_EXTENSOR_INHI			23
#define LAYER_ELBOW_SERVO_EXTENSOR_INTER			24	
#define LAYER_ELBOW_SERVO_EXTENSOR_SPINDLE_II		25
#define LAYER_ELBOW_SERVO_EXTENSOR_SPINDLE_IA		26		
#define LAYER_ELBOW_SERVO_FLEXOR_MOTOR			27		//   incresing pulse width for servo)
#define LAYER_ELBOW_SERVO_FLEXOR_INHI				28		
#define LAYER_ELBOW_SERVO_FLEXOR_INTER				29
#define LAYER_ELBOW_SERVO_FLEXOR_SPINDLE_II			30	
#define LAYER_ELBOW_SERVO_FLEXOR_SPINDLE_IA		31		
#define LAYER_ELBOW_SERVO_ANGLE_SENS_SPINDLE		32	

#define NUM_OF_IN_SILICO_NETWORK_LAYERS 				33	

#define NUM_OF_ANGULAR_SPINDLES						12
#define NUM_OF_FLEXOR_II_SPINDLES						1
#define NUM_OF_EXTENSOR_II_SPINDLES					NUM_OF_FLEXOR_II_SPINDLES

#define NUM_OF_FLEXOR_IA_SPINDLES						1
#define NUM_OF_EXTENSOR_IA_SPINDLES					NUM_OF_FLEXOR_IA_SPINDLES

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
	SpikeData					**blue_spike_spike_data_for_graph;   // for visualization
	SpikeData					**in_silico_spike_data_for_graph;  // for visualization  // each thread writes to one buffer.
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
	double						learning_rate;
};

HybridNetRLBMIData * get_hybrid_net_rl_bmi_data(void);


#endif
