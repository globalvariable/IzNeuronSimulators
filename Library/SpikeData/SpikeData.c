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
	if ((*buff_idx_write) == spike_data->buff_idx_read)
	{
		pthread_mutex_unlock(&(spike_data->mutex));	
		print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeData", "write_to_spike_data", "SpikeData buffer is FULL!!!.");
	}
	pthread_mutex_unlock(&(spike_data->mutex));	
	return TRUE;
}

bool write_to_spike_data_with_sorting(SpikeData *spike_data, unsigned int mwa_or_layer, unsigned int channel_or_neuron_group, unsigned int unit_or_neuron, TimeStamp spike_time)
{
	SpikeTimeStampItem		*buff;
	SpikeTimeStampItem		*item, *item_insert;
	unsigned int		*ptr_buffer_write_idx;   	
	unsigned int 		buff_size;
	unsigned int 		idx, end_idx, buff_last_idx; 	

	buff = spike_data->buff;
	buff_size = spike_data->buffer_size;

	pthread_mutex_lock(&(spike_data->mutex));
	
	ptr_buffer_write_idx = &(spike_data->buff_idx_write);

	idx = *ptr_buffer_write_idx;
	end_idx = spike_data->buff_idx_read; 

	if ((((idx + 1) == buff_size) && (end_idx == 0)) || ((idx + 1) == end_idx))		// if write idx catches read idx (buffer full)
	/// SHOULD CHECK THIS BEFORE SHIFTING THE LAST INDEX: OTHERWISE READER CAN READ CORRUPTED LAST INDEX DATA.  // READER DOES NOT USE MUTEX (TO BE FASTER)
	{
		pthread_mutex_unlock(&(spike_data->mutex));
		print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeData", "write_to_spike_data_with_sorting", "SpikeData buffer is FULL!!!.");
		return FALSE;
	}

	do {
		if (idx == 0)
		{	
			buff_last_idx = buff_size-1;
			item = &(buff[buff_last_idx]);
			item_insert = &(buff[0]);
			if (spike_time < item->peak_time)		// push buffered item to the next index
			{
				item_insert->peak_time = item->peak_time;
				item_insert->mwa_or_layer = item->mwa_or_layer;
				item_insert->channel_or_neuron_group = item->channel_or_neuron_group;
				item_insert->unit_or_neuron = item->unit_or_neuron;
				idx = buff_last_idx;
			}
			else
			{
				item_insert->peak_time = spike_time;		// insert here & break;
				item_insert->mwa_or_layer = mwa_or_layer;
				item_insert->channel_or_neuron_group = channel_or_neuron_group;
				item_insert->unit_or_neuron = unit_or_neuron;
				break;			
			}
		}
		else
		{
			item = &(buff[idx-1]);
			item_insert = &(buff[idx]);
			if (spike_time < item->peak_time)		// push buffered item to the next index
			{
				item_insert->peak_time = item->peak_time;
				item_insert->mwa_or_layer = item->mwa_or_layer;
				item_insert->channel_or_neuron_group = item->channel_or_neuron_group;
				item_insert->unit_or_neuron = item->unit_or_neuron;
				idx--;
			}
			else
			{
				item_insert->peak_time = spike_time;		// insert here & break;
				item_insert->mwa_or_layer = mwa_or_layer;
				item_insert->channel_or_neuron_group = channel_or_neuron_group;
				item_insert->unit_or_neuron = unit_or_neuron;
				break;			
			}			
		}
	} while (1);
	
	if ((*ptr_buffer_write_idx + 1) == buff_size)
		*ptr_buffer_write_idx = 0;
	else
		(*ptr_buffer_write_idx)++;
	pthread_mutex_unlock(&(spike_data->mutex));
	return TRUE;
}

bool get_next_spike_data_item(SpikeData* spike_data, SpikeTimeStampItem *data_item)
{
	unsigned int *idx;
	SpikeTimeStampItem *buff_data_item;
	idx = &(spike_data->buff_idx_read);
	if (*idx == spike_data->buff_idx_write)
		return FALSE;
	buff_data_item = &(spike_data->buff[*idx]);	
	data_item->peak_time = buff_data_item->peak_time;
	data_item->mwa_or_layer = buff_data_item->mwa_or_layer;
	data_item->channel_or_neuron_group = buff_data_item->channel_or_neuron_group;
	data_item->unit_or_neuron = buff_data_item->unit_or_neuron;
	if ((*idx + 1) == spike_data->buffer_size)
		*idx = 0;
	else
		(*idx)++;
	return TRUE;
}
