#ifndef HYBRID_NET_RL_BMI_H
#define HYBRID_NET_RL_BMI_H

typedef struct __HybridNetRLBMIData HybridNetRLBMIData;

#include "Gui.h"
#include "HybridNetRLBMIConfig.h"
#include "../../../BlueSpike/SpikeTimeStamp.h"
#include "../../../BlueSpike/System/ShmSemNum/ShmSemNum.h"
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../Library/NeuronDynamicsData/NeuronDynamicsData.h"
#include "../../Library/STDPData/STDPData.h"
#include "../../Library/EligibilityData/EligibilityData.h"
#include "../../Library/SpikeData/SpikeData.h"
#include "../../Library/Stats/FiringStats.h"
#include "../../Library/InjectionCurrentData/InjectionCurrentData.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.h"
#include "../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.h"
#include "../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.h"
#include "../../../ExperimentHandlers/Library/Messages/NeuralNet2Gui.h"
#include "../../../ExperimentHandlers/Library/Status/TrialStatus.h"
#include "../../Library/Spindle/Spindle.h"

struct __HybridNetRLBMIData		
{
	SpikeTimeStamp 				*sorted_spike_time_stamp;    /// spike time stamps from biological neurons // sorted according to unit, not according to spike time.
	RtTasksData					*rt_tasks_data;
	Network						*in_silico_network;
	Network						*blue_spike_network;
	NeuronDynamicsBufferLimited	*neuron_dynamics_limited_buffer;
	STDPBufferLimited			*stdp_limited_buffer;
	EligibilityBufferLimited			*eligibility_limited_buffer;
	SpikeData					**blue_spike_spike_data_for_graph;   // for visualization
	SpikeData					**in_silico_spike_data_for_graph;  // for visualization  // each thread writes to one buffer.
	SpikeData					**in_silico_spike_data_for_recording;  // for recording to hdd // each thread writes to one buffer.
	TrialHand2NeuralNetMsg		*msgs_trial_hand_2_neural_net;
	NeuralNet2TrialHandMsg		*msgs_neural_net_2_trial_hand;
	MovObjHand2NeuralNetMsgMultiThread		*msgs_mov_obj_hand_2_neural_net_multi_thread;
	NeuralNet2MovObjHandMsgMultiThread		*msgs_neural_net_2_mov_obj_hand_multi_thread;
	NeuralNet2GuiMsg				*msgs_neural_net_2_gui;
	TrialStatusEvents				*trial_status_events;   // to show status changed in graphs
	unsigned int					num_of_dedicated_cpu_threads;
	bool						simulation_in_progress;    // not to allow offline simulations while rt simulation is in progress
	ServoAngularLimit				servo_angle_min_max[THREE_DOF_ROBOT_NUM_OF_SERVOS];
	ExponentialAngularSpindleGroup	*angle_sensitive_spindles[THREE_DOF_ROBOT_NUM_OF_SERVOS];
	double						learning_rate;
};

HybridNetRLBMIData * get_hybrid_net_rl_bmi_data(void);


#endif
