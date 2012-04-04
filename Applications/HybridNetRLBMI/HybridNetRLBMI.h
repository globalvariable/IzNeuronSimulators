#ifndef HYBRID_NET_RL_BMI_H
#define HYBRID_NET_RL_BMI_H


typedef struct __HybridNetRLBMIData HybridNetRLBMIData;

#include "Gui.h"
#include "../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialsData.h"
#include "../../Library/NeuronDynamicsData/NeuronDynamicsData.h"
#include "../../Library/SpikeData/SpikeData.h"

struct __HybridNetRLBMIData		
{
	TrialsData					*trials_data;
	Network						*in_silico_network;
	Network						*blue_spike_network;
	NeuronDynamicsBuffer			*neuron_dynamics_pattern_buffer;
	SpikeData					*blue_spike_spike_data;
	SpikeData					*in_silico_spike_data;
	bool						simulation_in_progress;
};

HybridNetRLBMIData * get_hybrid_net_rl_bmi_data(void);


#endif