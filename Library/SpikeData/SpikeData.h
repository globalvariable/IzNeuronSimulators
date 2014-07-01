#ifndef SPIKE_DATA_H
#define SPIKE_DATA_H



typedef struct __SpikeData SpikeData;
typedef struct __SpikeTimeStampItem SpikeTimeStampItem;

#include <stdbool.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../BlueSpike/System/TimeStamp/TimeStamp.h"

struct __SpikeTimeStampItem
{
	TimeStamp 		peak_time;				// spike peak time
	unsigned int		layer;				
	unsigned int		neuron_group;		
	unsigned int		neuron;				
};

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
bool write_to_spike_data(SpikeData *spike_data, unsigned int layer, unsigned int neuron_group, unsigned int neuron, TimeStamp spike_time);
bool write_to_spike_data_multi_threaded(SpikeData *spike_data, unsigned int layer, unsigned int neuron_group, unsigned int neuron, TimeStamp spike_time);
bool write_to_spike_data_with_sorting_single_threaded(SpikeData *spike_data, unsigned int layer, unsigned int neuron_group, unsigned int neuron, TimeStamp spike_time);
bool get_next_spike_data_item(SpikeData* spike_data, SpikeTimeStampItem *data_item);
void get_spike_data_item_by_idx(SpikeData* spike_data, unsigned int idx, SpikeTimeStampItem *data_item);
void reset_spike_data_read_idx(SpikeData* spike_data);

#endif
