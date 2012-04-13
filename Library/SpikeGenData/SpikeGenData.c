#include "SpikeGenData.h"


static unsigned int spike_time_stamp_buff_size = SPIKE_TIME_STAMP_BUFF_SIZE;

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
	data->trials_data = trials_data;
	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeGenData", "allocate_spike_generator_data", "Created spike_generator_data.");
	return data;
}

SpikeGenData* deallocate_spike_generator_data(SpikeGenData *data, TrialsData *trials_data)
{
	if (data == NULL)
		return (SpikeGenData*)print_message(BUG_MSG ,"IzNeuronSimulators", "SpikeGenData", "deallocate_spike_generator_data", "spike_generator_data == NULL.");	
	data->network = deallocate_network(data->network);
	data->current_templates = deallocate_current_templates(data->network, trials_data, data->current_templates);
	data->limited_current_pattern_buffer = deallocate_current_pattern_buffer_limited(data->network, data->limited_current_pattern_buffer);
	data->limited_neuron_dynamics_buffer = deallocate_neuron_dynamics_buffer_limited(data->network, data->limited_neuron_dynamics_buffer);
	data->spike_data = deallocate_spike_data(data->spike_data);
	g_free(data);
	return NULL;
}
bool write_generated_spike_to_blue_spike_buffer(unsigned int layer_num, unsigned int neuron_grp_num, unsigned int neuron_num, TimeStamp spike_time)
{
	SpikeTimeStamp *spike_time_stamp;
	unsigned int spike_time_stamp_buff_idx_write;

	spike_time_stamp = &shared_memory->spike_time_stamp;

	spike_time_stamp_buff_idx_write = spike_time_stamp->buff_idx_write;
	spike_time_stamp->spike_time_stamp_buff[spike_time_stamp_buff_idx_write].peak_time = spike_time;
	spike_time_stamp->spike_time_stamp_buff[spike_time_stamp_buff_idx_write].mwa_or_layer = layer_num;
	spike_time_stamp->spike_time_stamp_buff[spike_time_stamp_buff_idx_write].channel_or_neuron_group = neuron_grp_num;
	spike_time_stamp->spike_time_stamp_buff[spike_time_stamp_buff_idx_write].unit_or_neuron = neuron_num;	
	if ((spike_time_stamp_buff_idx_write +1) ==  spike_time_stamp_buff_size )	   // first check then increment. if first increment and check end of buffer might lead to problem during reading.
		spike_time_stamp->buff_idx_write = 0;
	else
		spike_time_stamp->buff_idx_write++;		
	return TRUE;			
}

