#ifndef SPIKE_DATA_H
#define SPIKE_DATA_H



typedef struct __SpikeData SpikeData;

#include "../../../BlueSpike/SpikeTimeStamp.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include <stdbool.h>
#include <gtk/gtk.h>

struct __SpikeData
{
	SpikeTimeStampItem		*buff;
	unsigned int				buff_idx_write;
	unsigned int				buffer_size;
} ;


SpikeData* allocate_spike_data(SpikeData *spike_data, unsigned int buffer_size);
SpikeData* deallocate_spike_data(SpikeData *spike_data);
bool write_to_spike_data(SpikeData *spike_data, unsigned int mwa_or_layer, unsigned int channel_or_neuron_group, unsigned int unit_or_neuron, TimeStamp spike_time);




#endif