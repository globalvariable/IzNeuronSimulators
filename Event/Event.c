#include "Event.h"


int schedule_events(Neuron *nrn, int layer, int neuron_group, int neuron_num, double dt_part, TimeStamp integration_start_time);
{
	int i;
	NeuronSynapseList *neuron_synapse_list;

	Neuron 			**synapse_to;
	SynapticDelay		*synapse_delay;
	SynapticWeight		*synapse_weight;
	int 				num_of_synapses;
	TimeStamp		scheduled_event_time;
	
	neuron_synapse_list = &(synapse_lists[layer][neuron_group][neuron_num]);
	synapse_to = neuron_synapse_list->to;
	synapse_delay = neuron_synapse_list->delay;
	synapse_weight = neuron_synapse_list->weight;
	num_of_synapses = neuron_synapse_list->num_of_synapses;
	
	for (i = 0; i < num_of_synapses, i++)
	{
		scheduled_event_time = synapse_delay[i]+integration_start_time+ (dt_part*PARKER_SOCHACKI_EMBEDDED_STEP_SIZE);
		if (!insert_synaptic_event(synapse_to[i]->layer, synapse_to[i]->neuron_group, synapse_to[i]->neuron_num, scheduled_event_time, synapse_weight[i], nrn))
			return 0;
	}
	return 1;	
}

int insert_synaptic_event(int layer, int neuron_group, int neuron_num, TimeStamp scheduled_event_time, double weight, Neuron *event_from)
{
	NeuronEventBuffer	*neuron_event_buffer;
	int				*ptr_event_buffer_write_idx;   	
	int				*ptr_event_buffer_read_idx;
	TimeStamp		*event_times;
	Neuron 			**events_from;
	SynapticWeight		*event_weights;	
	int 				event_buff_size;
	pthread_mutex_t 	*mutex_event_buff;

	int 				idx, end_idx; 	

	neuron_event_buffer = &(event_buffers[layer][neuron_group][neuron_num]);
	event_buff_size = neuron_event_buffer->buff_size;
	event_times = neuron_event_buffer->time;
	event_weights = neuron_event_buffer->weight;	
	events_from = neuron_event_buffer->from;
	
	mutex_event_buff = &(neuron_event_buffer->mutex);	
	pthread_mutex_lock(mutex_event_buff);
	
	ptr_event_buffer_write_idx = &(neuron_event_buffer->write_idx);
	ptr_event_buffer_read_idx = &(neuron_event_buffer->read_idx);

	idx = *ptr_event_buffer_write_idx;
	end_idx = *ptr_event_buffer_read_idx; 

	if ((idx + 1) == end_idx)
	{
		printf("ERROR: Event.c: insert_synaptic_event().\n");
		printf("ERROR: Event.c: Event buffer is full for this neuron:\n");
		printf("ERROR: Event.c: Layer %d:\n", layer);
		printf("ERROR: Event.c: Neuron Group %d:\n", neuron_group);
		printf("ERROR: Event.c: Neuron Number %d:\n", neuron_num);	
		return 0;			
	}
	
	do {
		if (scheduled_event_time < event_times[idx])		// push buffed item to the next index
		{
			if (idx + 1 == event_buff_size)	// reached end of buffer	
			{
				event_times[0] = event_times[idx];
				event_weights[0] = event_weights[idx];
				events_from[0] = events_from[idx];
			}
			else
			{
				event_times[idx+1] = event_times[idx];
				event_weights[idx+1] = event_weights[idx];
				events_from[idx+1] = events_from[idx];			
			}
		}
		else
		{
			event_times[idx+1] = scheduled_event_time;	// insert here & break;
			event_weights[idx+1] = weight;
			events_from[idx+1] = event_from;
			break;
		}
		idx --;
		if (idx == 0)
			idx = event_buff_size -1;
	} while (idx != end_idx);
	
	(*ptr_event_buffer_write_idx)++
	pthread_mutex_unlock(lock);	
}
