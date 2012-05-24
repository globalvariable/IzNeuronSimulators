#ifndef HYBRID_NET_RL_BMI_H
#define HYBRID_NET_RL_BMI_H

#define MOTOR_OUTPUT_BIN_SIZE	50000000 // 50 ms


typedef struct __HybridNetRLBMIData HybridNetRLBMIData;

#include "Gui.h"
#include "../../../BlueSpike/BlueSpikeData.h"
#include "../../../BlueSpike/RtTasksData.h"
#include "../../Library/NeuronDynamicsData/NeuronDynamicsData.h"
#include "../../Library/SpikeData/SpikeData.h"
#include "../../Library/MotorOutput/MotorOutput.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.h"
#include "../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.h"
#include "../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.h"
#include "../../../ExperimentHandlers/Library/Status/TrialStatus.h"

struct __HybridNetRLBMIData		
{
	BlueSpikeData				*blue_spike_data;
	RtTasksData					*rt_tasks_data;
	Network						*in_silico_network;
	Network						*blue_spike_network;
	NeuronDynamicsBufferLimited	*neuron_dynamics_limited_buffer;
	SpikeData					*blue_spike_spike_data;
	SpikeData					*in_silico_spike_data;
	MotorOutputs					*motor_outputs;
	TrialHand2NeuralNetMsg		*msgs_trial_hand_2_neural_net;
	NeuralNet2TrialHandMsg		*msgs_neural_net_2_trial_hand;
	MovObjHand2NeuralNetMsg		*msgs_mov_obj_hand_2_neural_net;
	NeuralNet2MovObjHandMsg		*msgs_neural_net_2_mov_obj_hand;
	TrialStatusEvents				*trial_status_events;
};

HybridNetRLBMIData * get_hybrid_net_rl_bmi_data(void);


#endif
