#ifndef HYBRID_NET_RL_BMI_H
#define HYBRID_NET_RL_BMI_H

#define LAYER_BASE_SERVO_EXTENSOR_MOTOR 			0		//   decreasing pulse width for servo)
#define LAYER_BASE_SERVO_EXTENSOR_INHI				1
#define LAYER_BASE_SERVO_EXTENSOR_SPINDLE			2		// an extension decreases servo pulse width, decreases joint angle, decreases extension spindle firing rate, increases flexion spindle firing rate . 
#define LAYER_BASE_SERVO_EXTENSOR_INTER			3		
#define LAYER_BASE_SERVO_FLEXOR_MOTOR				4		//   incresing pulse width for servo)
#define LAYER_BASE_SERVO_FLEXOR_INHI					5		
#define LAYER_BASE_SERVO_FLEXOR_SPINDLE				6		//  a flexion increases servo pulse width, increases joint angle, decreases flexion spindle firing rate, increases extension spindle firing rate  . 
#define LAYER_BASE_SERVO_FLEXOR_INTER				7		

#define LAYER_SHOULDER_SERVO_EXTENSOR_MOTOR 		8		//   decreasing pulse width for servo)
#define LAYER_SHOULDER_SERVO_EXTENSOR_INHI			9
#define LAYER_SHOULDER_SERVO_EXTENSOR_SPINDLE	10		// decreases firing rate for an extension, increases for flexion
#define LAYER_SHOULDER_SERVO_EXTENSOR_INTER		11		
#define LAYER_SHOULDER_SERVO_FLEXOR_MOTOR			12		//   incresing pulse width for servo)
#define LAYER_SHOULDER_SERVO_FLEXOR_INHI			13		
#define LAYER_SHOULDER_SERVO_FLEXOR_SPINDLE		14		// decreases firing rate for an flexion, increases for extension
#define LAYER_SHOULDER_SERVO_FLEXOR_INTER			15		

#define LAYER_ELBOW_SERVO_EXTENSOR_MOTOR 			16		//   decreasing pulse width for servo)
#define LAYER_ELBOW_SERVO_EXTENSOR_INHI			17
#define LAYER_ELBOW_SERVO_EXTENSOR_SPINDLE		18		// decreases firing rate for an extension, increases for flexion
#define LAYER_ELBOW_SERVO_EXTENSOR_INTER			19		
#define LAYER_ELBOW_SERVO_FLEXOR_MOTOR			20		//   incresing pulse width for servo)
#define LAYER_ELBOW_SERVO_FLEXOR_INHI				21		
#define LAYER_ELBOW_SERVO_FLEXOR_SPINDLE			22		// decreases firing rate for an flexion, increases for extension
#define LAYER_ELBOW_SERVO_FLEXOR_INTER				23		

#define NUM_OF_IN_SILICO_NETWORK_LAYERS 				24	

#define NUM_OF_EXTENSOR_SPINDLES					4		
#define NUM_OF_FLEXOR_SPINDLES						NUM_OF_EXTENSOR_SPINDLES	

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
	ExponentialPrimaryFlexorSpindle	flexor_spindles[THREE_DOF_ROBOT_NUM_OF_SERVOS][NUM_OF_FLEXOR_SPINDLES];
	ExponentialPrimaryExtensorSpindle	extensor_spindles[THREE_DOF_ROBOT_NUM_OF_SERVOS][NUM_OF_EXTENSOR_SPINDLES];
};

HybridNetRLBMIData * get_hybrid_net_rl_bmi_data(void);


#endif
