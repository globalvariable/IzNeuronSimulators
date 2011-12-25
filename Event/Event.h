#ifndef EVENT_H
#define EVENT_H

typedef struct 	__MainEventBuffer			MainEventBuffer;
typedef struct 	__LayerEventBuffer 		LayerEventBuffer;
typedef struct 	__NeuroGroupEventBuffer 		NeuroGroupEventBuffer;
typedef struct 	__NeuronEventBuffer		NeuronEventBuffer;

#include "../../BlueSpike/TimeStamp.h"
#include "../IzNeuron/Neuron.h"
#include "../Synapse/Synapse.h"
#include "../ParkerSochacki/ParkerSochacki.h"
#include <pthread.h>

// Lock this struct while multicore cpu is utilized. Multiple thread can write in to this buffer.

struct __MainEventBuffer
{
	LayerEventBuffer	**layer_event_buffer;
	int 				layer_event_buffer_count;
};

struct __LayerEventBuffer
{
	NeuroGroupEventBuffer	**neuron_group_event_buffer;
	int 					neuron_group_event_buffer_count;
};

struct __NeuroGroupEventBuffer
{
	NeuronEventBuffer	*neuron_event_buffer;
	int 				neuron_event_buffer_count;
};

struct __NeuronEventBuffer
{
	pthread_mutex_t 	mutex;
	TimeStamp		*time;			// Dynamically this item and increment event_buff_size according to the number of neuron making connection with this neuron.
	Neuron 			**from;			// Dynamically this item and increment event_buff_size according to the number of neuron making connection with this neuron.
	SynapticWeight		*weight;			// Dynamically this item and increment event_buff_size according to the number of neuron making connection with this neuron.
	int 				buff_size;			// Determine event_buff_size by incrementing with each connection. Finally allocate the event buffers (time, *from, weight)
	int				write_idx;   		// Only writers into this buffer shoud lock it and increment write_idx after writing is complete.  
	int				read_idx;   		// No need to lock this struct while this neuron reading its buffer to evaluate its dynamics. It should handle the events from buffer_prev_idx (including) to buffer_write_idx (excluding)
};



/// Functions
bool initialize_main_event_buffer(void);
bool add_layer_event_buffer_to_main_event_buffer(int layer);
bool add_neuron_group_event_buffer_to_layer_event_buffer(int layer, int num_of _neuron);
int schedule_events(Neuron *nrn, int layer, int neuron_group, int neuron_num, double dt_part, TimeStamp integration_start_time);
int insert_synaptic_event(int layer, int neuron_group, int neuron_num, TimeStamp scheduled_event_time, double weight, Neuron *event_from);


#endif
