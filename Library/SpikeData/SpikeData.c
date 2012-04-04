#include "SpikeData.h"


SpikeData* allocate_spike_data(SpikeData *spike_data, unsigned int buffer_size)
{
	if (spike_data != NULL)
	{
		spike_data = deallocate_spike_data(spike_data);
		spike_data = allocate_spike_data(spike_data, buffer_size);
		return spike_data;
	}
	spike_data = g_new0(SpikeData,1);
	spike_data->buff = g_new0(SpikeTimeStampItem, buffer_size);
	spike_data->buffer_size = buffer_size;
	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeData", "allocate_spike_data", "Created spike_data.");
	return spike_data;
}

SpikeData* deallocate_spike_data(SpikeData *spike_data)
{
	if (spike_data == NULL)
		return (SpikeData*)print_message(WARNING_MSG ,"IzNeuronSimulators", "SpikeData", "deallocate_spike_data", "spike_data == NULL.");
	g_free(spike_data->buff); 
	g_free(spike_data); 
	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeData", "deallocate_spike_data", "Destroyed spike_data.");
	return NULL;
}

bool write_to_spike_data(SpikeData *spike_data, unsigned int mwa_or_layer, unsigned int channel_or_neuron_group, unsigned int unit_or_neuron, TimeStamp spike_time)
{
	unsigned int buff_idx_write;

	buff_idx_write = spike_data->buff_idx_write;
	spike_data->buff[buff_idx_write].peak_time = spike_time;
	spike_data->buff[buff_idx_write].mwa_or_layer = mwa_or_layer;
	spike_data->buff[buff_idx_write].channel_or_neuron_group = channel_or_neuron_group;
	spike_data->buff[buff_idx_write].unit_or_neuron = unit_or_neuron;	
	if ((buff_idx_write +1) ==  spike_data->buffer_size )	   // first check then increment. if first increment and check end of buffer might lead to problem during reading.
		spike_data->buff_idx_write = 0;
	else
		spike_data->buff_idx_write++;		
	return TRUE;
}
