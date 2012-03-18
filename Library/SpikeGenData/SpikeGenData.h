#ifndef SPIKE_GEN_DATA
#define SPIKE_GEN_DATA



typedef struct __SpikeGenData SpikeGenData;

#include "../Network/Network.h"
#include "../InjectionCurrentData/InjectionCurrentData.h"
#include "../NeuronDynamicsData/NeuronDynamicsData.h"

struct __SpikeGenData		// get gloabal trial stats by searching though TrialTypeData
{
	Network						*network;
	CurrentTemplate				*current_templates;
	CurrentPatternBuffer			*current_pattern_buffer;
	NeuronDynamicsBuffer			*neuron_dynamics_pattern_buffer;
};

SpikeGenData* allocate_spike_generator_data(SpikeGenData *data, TrialsData *trials_data);
SpikeGenData* deallocate_spike_generator_data(SpikeGenData *data, TrialsData *trials_data);
#endif
