#include "SpikeGenData.h"


SpikeGenData* allocate_spike_generator_data(SpikeGenData *data, TrialsData *trials_data)
{
	if (data != NULL)
	{
		data = deallocate_spike_generator_data(data, trials_data);
		data = allocate_spike_generator_data(data, trials_data);
		return data;
	}	
	data = g_new0(SpikeGenData, 1);
	data->network = allocate_network(data->network);
	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeGenData", "allocate_spike_generator_data", "Created spike_generator_data.");
	return data;
}

SpikeGenData* deallocate_spike_generator_data(SpikeGenData *data, TrialsData *trials_data)
{
	if (data == NULL)
		return (SpikeGenData*)print_message(BUG_MSG ,"IzNeuronSimulators", "SpikeGenData", "deallocate_spike_generator_data", "spike_generator_data == NULL.");	
	data->network = deallocate_network(data->network);
	data->injection_current = deallocate_injection_current_data(data->network, trials_data, data->injection_current);
	data->neuron_dynamics_pattern_buffer = deallocate_neuron_dynamics_buffer(data->network, data->neuron_dynamics_pattern_buffer);
	g_free(data);
	return NULL;
}

bool get_shm_spike_generator_data(SpikeGenData **data)
{
	if (shared_memory->additional_data[SHM_ADDITIONAL_DATA_SPIKE_GENERATOR_DATA] == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenData", "get_spike_generator_data", "spike_generator_data == NULL.");
	*data = shared_memory->additional_data[SHM_ADDITIONAL_DATA_SPIKE_GENERATOR_DATA];
	return TRUE;
}

bool set_shm_spike_generator_data(SpikeGenData *data)
{
	if (shared_memory->additional_data[SHM_ADDITIONAL_DATA_SPIKE_GENERATOR_DATA] != NULL)
		print_message(WARNING_MSG ,"IzNeuronSimulators", "SpikeGenData", "get_spike_generator_data", "spike_generator_data != NULL.");
	shared_memory->additional_data[SHM_ADDITIONAL_DATA_SPIKE_GENERATOR_DATA] = data;
	return TRUE;
}
