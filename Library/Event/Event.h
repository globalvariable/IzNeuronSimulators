#ifndef EVENT_H
#define EVENT_H

typedef struct 	__NeuronEventBuffer		NeuronEventBuffer;

#include "../../../BlueSpike/TimeStamp.h"
#include "../Neuron/Neuron.h"
#include "../Network/Network.h"
#include "../Synapse/Synapse.h"
#include "../ParkerSochacki/ParkerSochacki.h"
#include <pthread.h>

// Lock this struct while multicore cpu is utilized. Multiple thread can write in to this buffer.
struct __NeuronEventBuffer
{
	pthread_mutex_t 	mutex;
	TimeStamp		*time;			// Dynamically this item and increment event_buff_size according to the number of neuron making connection with this neuron.
	Neuron			**from;			// Dynamically this item and increment event_buff_size according to the number of neuron making connection with this neuron.
	SynapticWeight	*weight;			// Dynamically this item and increment event_buff_size according to the number of neuron making connection with this neuron.
	unsigned int		buff_size;		// Determine event_buff_size by incrementing with each connection. Finally allocate the event buffers (time, *from, weight)
	unsigned int		write_idx;   		// Only writers into this buffer shoud lock it and increment write_idx after writing is complete.  
	unsigned int		read_idx;   		// No need to lock this struct while this neuron reading its buffer to evaluate its dynamics. It should handle the events from buffer_prev_idx (including) to buffer_write_idx (excluding). Only one proc shoud read it!!!
};

/// Functions
bool schedule_event(Neuron *nrn, TimeStamp event_time);
bool insert_synaptic_event(Neuron *neuron, TimeStamp scheduled_event_time, double weight, Neuron *event_from);
bool increase_neuron_event_buffer_size(Neuron *neuron, unsigned int amount);
void clear_neuron_event_buffer(Neuron *neuron);
void destroy_neuron_event_buffer(Neuron *neuron);

#endif
