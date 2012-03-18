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
	data->current_templates = deallocate_current_templates(data->network, trials_data, data->current_templates);
	data->current_pattern_buffer = deallocate_current_pattern_buffer(data->network, data->current_pattern_buffer);
	data->neuron_dynamics_pattern_buffer = deallocate_neuron_dynamics_buffer(data->network, data->neuron_dynamics_pattern_buffer);
	g_free(data);
	return NULL;
}


