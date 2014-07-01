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

bool write_to_spike_data(SpikeData *spike_data, unsigned int layer, unsigned int neuron_group, unsigned int neuron, TimeStamp spike_time)
{
	unsigned int *buff_idx_write;
	SpikeTimeStampItem *item;

	buff_idx_write = &(spike_data->buff_idx_write);
	item = &(spike_data->buff[*buff_idx_write]);
	item->peak_time = spike_time;
	item->layer = layer;
	item->neuron_group = neuron_group;
	item->neuron = neuron;	
	if ((*buff_idx_write +1) ==  spike_data->buffer_size )	   // first check then increment. if first increment and check end of buffer might lead to problem during reading.
		*buff_idx_write = 0;
	else
		(*buff_idx_write)++;
	if ((*buff_idx_write) == spike_data->buff_idx_read)
	{
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeData", "write_to_spike_data", "SpikeData buffer is FULL!!!.");
	}
	return TRUE;
}

bool write_to_spike_data_multi_threaded(SpikeData *spike_data, unsigned int layer, unsigned int neuron_group, unsigned int neuron, TimeStamp spike_time)
{
	unsigned int *buff_idx_write;
	SpikeTimeStampItem *item;

	pthread_mutex_lock(&(spike_data->mutex));
	buff_idx_write = &(spike_data->buff_idx_write);
	item = &(spike_data->buff[*buff_idx_write]);
	item->peak_time = spike_time;
	item->layer = layer;
	item->neuron_group = neuron_group;
	item->neuron = neuron;	
	if ((*buff_idx_write +1) ==  spike_data->buffer_size )	   // first check then increment. if first increment and check end of buffer might lead to problem during reading.
		*buff_idx_write = 0;
	else
		(*buff_idx_write)++;
	if ((*buff_idx_write) == spike_data->buff_idx_read)
	{
		pthread_mutex_unlock(&(spike_data->mutex));	
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeData", "write_to_spike_data", "SpikeData buffer is FULL!!!.");
	}
	pthread_mutex_unlock(&(spike_data->mutex));	
	return TRUE;
}

bool write_to_spike_data_with_sorting_single_threaded(SpikeData *spike_data, unsigned int layer, unsigned int neuron_group, unsigned int neuron, TimeStamp spike_time)
{
	SpikeTimeStampItem		*buff;
	SpikeTimeStampItem		*item, *item_insert;
	unsigned int		*ptr_buffer_write_idx;   	
	unsigned int 		buff_size;
	unsigned int 		idx, buff_last_idx; 	

	buff = spike_data->buff;
	buff_size = spike_data->buffer_size;

//	pthread_mutex_lock(&(spike_data->mutex));   // no need to have it. no parallel writing to this data should be realized.
	
	ptr_buffer_write_idx = &(spike_data->buff_idx_write);

	idx = *ptr_buffer_write_idx;

	do {
		if (idx == 0)
		{	
			buff_last_idx = buff_size-1;
			item = &(buff[buff_last_idx]);
			item_insert = &(buff[0]);
			if (spike_time < item->peak_time)		// push buffered item to the next index
			{
				item_insert->peak_time = item->peak_time;
				item_insert->layer = item->layer;
				item_insert->neuron_group = item->neuron_group;
				item_insert->neuron = item->neuron;
				idx = buff_last_idx;
			}
			else
			{
				item_insert->peak_time = spike_time;		// insert here & break;
				item_insert->layer = layer;
				item_insert->neuron_group = neuron_group;
				item_insert->neuron = neuron;
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
				item_insert->layer = item->layer;
				item_insert->neuron_group = item->neuron_group;
				item_insert->neuron = item->neuron;
				idx--;
			}
			else
			{
				item_insert->peak_time = spike_time;		// insert here & break;
				item_insert->layer = layer;
				item_insert->neuron_group = neuron_group;
				item_insert->neuron = neuron;
				break;			
			}			
		}
	} while (1);
	
	if (((*ptr_buffer_write_idx) + 1) == buff_size)
		(*ptr_buffer_write_idx) = 0;
	else
		(*ptr_buffer_write_idx)++;
	if ((*ptr_buffer_write_idx) == spike_data->buff_idx_read)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeData", "write_to_spike_data_with_sorting", "SpikeData buffer is FULL!!!.");
//	pthread_mutex_unlock(&(spike_data->mutex));
	return TRUE;
}

bool get_next_spike_data_item(SpikeData* spike_data, SpikeTimeStampItem *data_item)
{
	unsigned int *idx;
	SpikeTimeStampItem *buff_data_item;
	idx = &(spike_data->buff_idx_read);
	if ((*idx) == spike_data->buff_idx_write)
		return FALSE;
	buff_data_item = &(spike_data->buff[*idx]);	
	data_item->peak_time = buff_data_item->peak_time;
	data_item->layer = buff_data_item->layer;
	data_item->neuron_group = buff_data_item->neuron_group;
	data_item->neuron = buff_data_item->neuron;
	if (((*idx) + 1) == spike_data->buffer_size)
		(*idx) = 0;
	else
		(*idx)++;
	return TRUE;
}

void get_spike_data_item_by_idx(SpikeData* spike_data, unsigned int idx, SpikeTimeStampItem *data_item)
{
	SpikeTimeStampItem *buff_data_item;
	buff_data_item = &(spike_data->buff[idx]);	
	data_item->peak_time = buff_data_item->peak_time;
	data_item->layer = buff_data_item->layer;
	data_item->neuron_group = buff_data_item->neuron_group;
	data_item->neuron = buff_data_item->neuron;
}

void reset_spike_data_read_idx(SpikeData* spike_data)
{
	spike_data->buff_idx_read = spike_data->buff_idx_write;
}
