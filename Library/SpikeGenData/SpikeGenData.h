#ifndef SPIKE_GEN_DATA
#define SPIKE_GEN_DATA



typedef struct __SpikeGenData SpikeGenData;


struct __SpikeGenData		// get gloabal trial stats by searching though TrialTypeData
{
	Network						*network;
	CurrentPatternTemplate			*current_patten_template;
	CurrentPatternBuffer			*current_patten_buffer;
	NeuronDynamicsPatternBuffer	*neuron_dynamics_pattern_buffer;
};

SpikeGenData* allocate_spike_generator_data(TrialsData *trials_data, SpikeGenData *data);
SpikeGenData* deallocate_spike_generator_data(TrialsData *trials_data, SpikeGenData *data);
bool get_shm_spike_generator_data(SpikeGenData **data);

#endif
