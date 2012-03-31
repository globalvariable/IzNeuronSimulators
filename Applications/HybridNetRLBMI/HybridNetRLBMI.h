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
	Network						*int_network;
	ExtNetwork					*ext_network;
	NeuronDynamicsBuffer			*neuron_dynamics_pattern_buffer;
	SpikeData					*int_network_spike_data;
	SpikeData					*ext_network_spike_data;
};

HybridNetRLBMIData * get_hybrid_net_rl_bmi_data(void);


#endif
