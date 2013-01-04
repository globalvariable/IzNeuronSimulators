#ifndef HYBRID_NET_RL_BMI_H
#define HYBRID_NET_RL_BMI_H

#define LAYER_BASE_SERVO_EXTENSOR_MOTOR 			0		//   decreasing pulse width for servo)
#define LAYER_BASE_SERVO_EXTENSOR_INHI				1		
#define LAYER_BASE_SERVO_FLEXOR_MOTOR				2		//   incresing pulse width for servo)
#define LAYER_BASE_SERVO_FLEXOR_INHI					3		
#define LAYER_BASE_JOINT_ANGLE						4

#define LAYER_SHOULDER_SERVO_EXTENSOR_MOTOR 		5		//   decreasing pulse width for servo)
#define LAYER_SHOULDER_SERVO_EXTENSOR_INHI			6		
#define LAYER_SHOULDER_SERVO_FLEXOR_MOTOR			7		//   incresing pulse width for servo)
#define LAYER_SHOULDER_SERVO_FLEXOR_INHI			8		
#define LAYER_SHOULDER_JOINT_ANGLE					9


#define LAYER_ELBOW_SERVO_EXTENSOR_MOTOR 			10		//   decreasing pulse width for servo)
#define LAYER_ELBOW_SERVO_EXTENSOR_INHI			11		
#define LAYER_ELBOW_SERVO_FLEXOR_MOTOR			12		//   incresing pulse width for servo)
#define LAYER_ELBOW_SERVO_FLEXOR_INHI				13		
#define LAYER_ELBOW_JOINT_ANGLE						14

#define NUM_OF_IN_SILICO_NETWORK_LAYERS 				15


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
	double						secondary_spindle_current_min;
	double						secondary_spindle_current_max;
};

HybridNetRLBMIData * get_hybrid_net_rl_bmi_data(void);


#endif
