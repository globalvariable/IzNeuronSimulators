#ifndef SPIKE_DATA_H
#define SPIKE_DATA_H



typedef struct __SpikeData SpikeData;

#include <stdbool.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include "../../../BlueSpike/SpikeTimeStamp.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"

struct __SpikeData
{
	pthread_mutex_t 			mutex;   // required for multi_threaded simulation through the network
	SpikeTimeStampItem		*buff;
	unsigned int				buff_idx_write;
	unsigned int				buff_idx_read;  
	unsigned int				buffer_size;
} ;


SpikeData* allocate_spike_data(SpikeData *spike_data, unsigned int buffer_size);
SpikeData* deallocate_spike_data(SpikeData *spike_data);
bool write_to_spike_data(SpikeData *spike_data, unsigned int mwa_or_layer, unsigned int channel_or_neuron_group, unsigned int unit_or_neuron, TimeStamp spike_time);
bool write_to_spike_data_with_sorting(SpikeData *spike_data, unsigned int mwa_or_layer, unsigned int channel_or_neuron_group, unsigned int unit_or_neuron, TimeStamp spike_time);
bool get_next_spike_data_item(SpikeData* spike_data, SpikeTimeStampItem **data_item);


#endif
