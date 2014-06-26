#ifndef EVENT_H
#define EVENT_H

typedef struct 	__NeuronEventBuffer		NeuronEventBuffer;
typedef struct 	__NeuronSortedEventBufferItem		NeuronSortedEventBufferItem;
typedef struct 	__NeuronSortedEventBuffer		NeuronSortedEventBuffer;
typedef struct 	__NeuronSynapticEventBuffer	NeuronSynapticEventBuffer;


#include "../../../BlueSpike/System/TimeStamp/TimeStamp.h"
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"

typedef unsigned int NeuronEventType;
typedef TimeStamp NeuronSynapticEventBufferItem;


#define NEURON_EVENT_TYPE_NULL_EVENT						0
#define NEURON_EVENT_TYPE_SYNAPTIC_EVENT					1


#include "../Neuron/Neuron.h"
#include "../Network/Network.h"
#include "../Axon/Axon.h"
#include "../ParkerSochacki/ParkerSochacki.h"
#include <pthread.h>

// Lock this struct while multicore cpu is utilized. Multiple thread can write in to this buffer.
struct __NeuronEventBuffer
{
	pthread_mutex_t 	mutex;
	TimeStamp		*time;			// buff_size
	SynapseIndex		*syn_idx;		// buff_size // connects to the post neuron & holds the synapse idx of the created synapse
	unsigned int		buff_size;		// Determine event_buff_size by incrementing with each connection. Finally allocate the event buffers (time, *from, weight)
	unsigned int		write_idx;   		// Only writers into this buffer shoud lock it and increment write_idx after writing is complete.  
	unsigned int		read_idx;   		// No need to lock this struct while this neuron reading its buffer to evaluate its dynamics. It should handle the events from buffer_prev_idx (including) to buffer_write_idx (excluding). Only one proc shoud read it!!!
};

struct __NeuronSortedEventBufferItem
{
	TimeStamp		time;			// buff_size
	NeuronEventType	event_type;		// buff_size
	SynapseIndex		syn_idx;		// buff_size // connects to the post neuron & holds the synapse idx of the created synapse
};

struct __NeuronSortedEventBuffer
{
	NeuronSortedEventBufferItem	*buff;		// buff_size // connects to the post neuron & holds the synapse idx of the created synapse
	unsigned int		write_idx;   		// Only neuron writes here before integration with event sorting by reading synaptic input 
	unsigned int		read_idx;   		// Only neuron reads it during integration. 
	unsigned int		buff_size;		// Determine event_buff_size by incrementing with each connection. Finally allocate the event buffers (time, *from, weight)
};

struct __NeuronSynapticEventBuffer
{
	NeuronSynapticEventBufferItem	*buff;	// no need to have syn_idx data since it is written to SYNAPTIC event buffer. 	
	unsigned int		write_idx;   		// Only presynaptic neuron writes here without sorting
	unsigned int		read_idx;   		// Only neuron reads it before integration. 
	unsigned int		buff_size;		// buff_size is determined acording to synaptic delay.
};


/// Functions
bool schedule_synaptic_event(Neuron *nrn, TimeStamp event_time);
/*bool insert_synaptic_event(Neuron *neuron, TimeStamp scheduled_event_time, SynapseIndex syn_idx);
bool increase_neuron_event_buffer_size(Neuron *neuron, unsigned int amount);
void clear_neuron_event_buffer(Neuron *neuron);
void destroy_neuron_event_buffer(Neuron *neuron);
*/

NeuronSynapticEventBuffer *allocate_neuron_synaptic_event_buffer(TimeStamp synaptic_delay);
bool update_neuron_sorted_event_buffer_size(Neuron *neuron);
bool write_to_neuron_synaptic_event_buffer(Neuron *neuron, TimeStamp scheduled_event_time, SynapseIndex syn_idx);
bool get_next_synaptic_event_buffer_item(NeuronSynapticEventBuffer *event_buffer, NeuronSynapticEventBufferItem	*item);   /// typedef TimeStamp NeuronSynapticEventBufferItem

bool sort_neuron_events(Neuron *neuron);
bool sort_synaptic_event_for_neuron(NeuronSortedEventBuffer	*sorted_event_buffer, TimeStamp scheduled_time, SynapseIndex syn_idx);

void reset_neuron_event_buffer(Neuron *neuron);

#endif
