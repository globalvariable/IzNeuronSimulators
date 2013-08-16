#ifndef HYBRID_NET_RL_BMI_H
#define HYBRID_NET_RL_BMI_H

#define _GNU_SOURCE	// for sched_setaffinity(0, sizeof(mask), &mask))

typedef struct __HybridNetRLBMIData HybridNetRLBMIData;
typedef struct __HybridNetRLBMISynapseData HybridNetRLBMISynapseData;

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


struct __HybridNetRLBMISynapseData		
{
	double						blue_spike_2_in_silico_excitatory_connection_probability;
	double						blue_spike_2_in_silico_excitatory_connection_weight_max;
	double						blue_spike_2_in_silico_excitatory_connection_weight_min;
	double						blue_spike_2_in_silico_excitatory_connection_delay_max;
	double						blue_spike_2_in_silico_excitatory_connection_delay_min;

	double						blue_spike_2_in_silico_inhibitory_connection_probability;
	double						blue_spike_2_in_silico_inhibitory_connection_weight_max;
	double						blue_spike_2_in_silico_inhibitory_connection_weight_min;
	double						blue_spike_2_in_silico_inhibitory_connection_delay_max;
	double						blue_spike_2_in_silico_inhibitory_connection_delay_min;

	double						motor_2_inter_neuron_excitatory_connection_probability;
	double						motor_2_inter_neuron_excitatory_connection_weight_max;
	double						motor_2_inter_neuron_excitatory_connection_weight_min;
	double						motor_2_inter_neuron_excitatory_connection_delay_max;
	double						motor_2_inter_neuron_excitatory_connection_delay_min;

	double						motor_2_inter_neuron_inhibitory_connection_probability;
	double						motor_2_inter_neuron_inhibitory_connection_weight_max;
	double						motor_2_inter_neuron_inhibitory_connection_weight_min;
	double						motor_2_inter_neuron_inhibitory_connection_delay_max;
	double						motor_2_inter_neuron_inhibitory_connection_delay_min;

	double						inter_2_motor_neuron_excitatory_connection_probability;
	double						inter_2_motor_neuron_excitatory_connection_weight_max;
	double						inter_2_motor_neuron_excitatory_connection_weight_min;
	double						inter_2_motor_neuron_excitatory_connection_delay_max;
	double						inter_2_motor_neuron_excitatory_connection_delay_min;

	double						inter_2_motor_neuron_inhibitory_connection_probability;
	double						inter_2_motor_neuron_inhibitory_connection_weight_max;
	double						inter_2_motor_neuron_inhibitory_connection_weight_min;
	double						inter_2_motor_neuron_inhibitory_connection_delay_max;
	double						inter_2_motor_neuron_inhibitory_connection_delay_min;

	double						motor_2_exci_neuron_excitatory_connection_probability;
	double						motor_2_exci_neuron_excitatory_connection_weight_max;
	double						motor_2_exci_neuron_excitatory_connection_weight_min;
	double						motor_2_exci_neuron_excitatory_connection_delay_max;
	double						motor_2_exci_neuron_excitatory_connection_delay_min;

	double						motor_2_exci_neuron_inhibitory_connection_probability;
	double						motor_2_exci_neuron_inhibitory_connection_weight_max;
	double						motor_2_exci_neuron_inhibitory_connection_weight_min;
	double						motor_2_exci_neuron_inhibitory_connection_delay_max;
	double						motor_2_exci_neuron_inhibitory_connection_delay_min;

	double						exci_2_motor_neuron_excitatory_connection_probability;
	double						exci_2_motor_neuron_excitatory_connection_weight_max;
	double						exci_2_motor_neuron_excitatory_connection_weight_min;
	double						exci_2_motor_neuron_excitatory_connection_delay_max;
	double						exci_2_motor_neuron_excitatory_connection_delay_min;

	double						exci_2_motor_neuron_inhibitory_connection_probability;
	double						exci_2_motor_neuron_inhibitory_connection_weight_max;
	double						exci_2_motor_neuron_inhibitory_connection_weight_min;
	double						exci_2_motor_neuron_inhibitory_connection_delay_max;
	double						exci_2_motor_neuron_inhibitory_connection_delay_min;

};

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
	double						learning_rate;
	HybridNetRLBMISynapseData	synapse_data;
};



HybridNetRLBMIData * get_hybrid_net_rl_bmi_data(void);


#endif
