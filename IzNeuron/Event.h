#ifndef EVENT_H
#define EVENT_H

#include "Neuron.h"
#include "Synapse.h"
#include "../../BlueSpike/TimeStamp.h"

#define EVENT_BUFF_SIZE_PER_NEURON 	120

typedef struct __EventData 	EventData;
typedef struct __EventItem 	EventItem;

struct __EventItem
{
	TimeStamp	time;
	Neuron 		*from;   
	SynapticWeight	weight;
};

struct __EventData
{
	EventItem		event_buff[EVENT_BUFF_SIZE_PER_NEURON];
	int 			buff_write_idx;
	int 			num_of_buffered_event;
};

#endif
