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
	pthread_mutex_init(&(spike_data->mutex), NULL);	
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
	unsigned int *buff_idx_write;
	SpikeTimeStampItem *item;

	buff_idx_write = &(spike_data->buff_idx_write);
	item = &(spike_data->buff[*buff_idx_write]);

	pthread_mutex_lock(&(spike_data->mutex));
	item->peak_time = spike_time;
	item->mwa_or_layer = mwa_or_layer;
	item->channel_or_neuron_group = channel_or_neuron_group;
	item->unit_or_neuron = unit_or_neuron;	
	if ((*buff_idx_write +1) ==  spike_data->buffer_size )	   // first check then increment. if first increment and check end of buffer might lead to problem during reading.
		*buff_idx_write = 0;
	else
		(*buff_idx_write)++;
	pthread_mutex_unlock(&(spike_data->mutex));	
	return TRUE;
}
