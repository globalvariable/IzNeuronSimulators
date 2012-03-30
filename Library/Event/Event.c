#include "Event.h"

int schedule_events(Neuron *nrn, double dt_part, TimeStamp integration_start_time)
{
	int i;
	NeuronSynapseList *neuron_synapse_list;

	Neuron 			**synapse_to;
	SynapticDelay		*synapse_delay;
	SynapticWeight		*synapse_weight;
	int 				num_of_synapses;
	TimeStamp		scheduled_event_time;
	
	neuron_synapse_list = nrn->syn_list;
	synapse_to = neuron_synapse_list->to;
	synapse_delay = neuron_synapse_list->delay;
	synapse_weight = neuron_synapse_list->weight;
	num_of_synapses = neuron_synapse_list->num_of_synapses;
	
	for (i = 0; i < num_of_synapses; i++)
	{
		scheduled_event_time = synapse_delay[i]+integration_start_time+ (dt_part*PARKER_SOCHACKI_EMBEDDED_STEP_SIZE);
		if (!insert_synaptic_event(synapse_to[i], scheduled_event_time, synapse_weight[i], nrn))
			return 0;
	}
	return 1;	
}

int insert_synaptic_event(Neuron *neuron, TimeStamp scheduled_event_time, double weight, Neuron *event_from)
{
	NeuronEventBuffer	*neuron_event_buffer;
	int				*ptr_event_buffer_write_idx;   	
	int				*ptr_event_buffer_read_idx;
	TimeStamp		*event_times;
	Neuron 			**events_from;
	SynapticWeight		*event_weights;	
	int 				event_buff_size;
	int 				idx, end_idx; 	
	
	neuron_event_buffer = neuron->event_buff;
	event_buff_size = neuron_event_buffer->buff_size;
	event_times = neuron_event_buffer->time;
	event_weights = neuron_event_buffer->weight;	
	events_from = neuron_event_buffer->from;
	
	pthread_mutex_lock(&(neuron_event_buffer->mutex));
	
	ptr_event_buffer_write_idx = &(neuron_event_buffer->write_idx);
	ptr_event_buffer_read_idx = &(neuron_event_buffer->read_idx);

	idx = *ptr_event_buffer_write_idx;
	end_idx = *ptr_event_buffer_read_idx; 

	if ((idx + 1) == end_idx)
	{
		printf("ERROR: Event.c: insert_synaptic_event().\n");
		printf("ERROR: Event.c: Event buffer is full for this neuron:\n");
		printf("ERROR: Event.c: Layer %d:\n", neuron->layer);
		printf("ERROR: Event.c: Neuron Group %d:\n", neuron->neuron_group);
		printf("ERROR: Event.c: Neuron Number %d:\n", neuron->neuron_num);	
		pthread_mutex_unlock(&(neuron_event_buffer->mutex));
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
			if (idx + 1 == event_buff_size)	// reached end of buffer	
			{		
				event_times[0] = scheduled_event_time;	// insert here & break;
				event_weights[0] = weight;
				events_from[0] = event_from;
				break;
			}
			else
			{
				event_times[idx+1] = scheduled_event_time;	// insert here & break;
				event_weights[idx+1] = weight;
				events_from[idx+1] = event_from;	
				break;		
			}
		}
		idx --;
		if (idx == 0)
			idx = event_buff_size -1;
	} while (idx != end_idx);
	
	if ((*ptr_event_buffer_write_idx + 1) == event_buff_size)
		*ptr_event_buffer_write_idx = 0;
	else
		(*ptr_event_buffer_write_idx)++;
	pthread_mutex_unlock(&(neuron_event_buffer->mutex));
	printf("%llu\n", neuron_event_buffer->time[0]);
	printf("%llu\n", neuron_event_buffer->time[1]);
	return 1;
}

bool increment_neuron_event_buffer_size(Neuron *neuron)
{
	int layer = neuron->layer;
	int neuron_group = neuron->neuron_group;
	int neuron_num = neuron->neuron_num;

	NeuronEventBuffer	*ptr_neuron_event_buffer;
	TimeStamp		*time = NULL;		
	Neuron 			**from = NULL;			
	SynapticWeight		*weight = NULL;		

	ptr_neuron_event_buffer = neuron->event_buff;
	
	time = g_new0(TimeStamp, ptr_neuron_event_buffer->buff_size+1);
	from = g_new0(Neuron*, ptr_neuron_event_buffer->buff_size+1);
	weight = g_new0(SynapticWeight, ptr_neuron_event_buffer->buff_size+1);
		
	if ((time == NULL) || (from == NULL) || (weight == NULL))
	{
		printf("Event: ERROR: Couldn' t allocate event buffer for neuron (Layer: %d, Neuron Group: %d, Neuron Num: %d)\n", layer, neuron_group, neuron_num);
		return FALSE;
	}	
	
	ptr_neuron_event_buffer->write_idx = 0;
	ptr_neuron_event_buffer->read_idx = 0;	// reset event buffer, clear all data it has had.
	
	g_free(ptr_neuron_event_buffer->time);
	g_free(ptr_neuron_event_buffer->from);		
	g_free(ptr_neuron_event_buffer->weight);	
	
	ptr_neuron_event_buffer->time = time;	// point to new event buffer
	ptr_neuron_event_buffer->from = from;		
	ptr_neuron_event_buffer->weight = weight;	
	
	ptr_neuron_event_buffer->buff_size++;
	
	return TRUE;
}

void clear_neuron_event_buffer(Neuron *neuron)
{
	int i;
	NeuronEventBuffer	*ptr_neuron_event_buffer;
	ptr_neuron_event_buffer = neuron->event_buff;
	
	for (i=0; i< ptr_neuron_event_buffer->buff_size; i++)
	{
		ptr_neuron_event_buffer->time[i] = 0;
		ptr_neuron_event_buffer->from[i] = 0;
		ptr_neuron_event_buffer->weight[i] = 0;
	}

}		

void destroy_neuron_event_buffer(Neuron *neuron)
{
	NeuronEventBuffer *ptr_neuron_event_buffer;
	
	ptr_neuron_event_buffer = neuron->event_buff;

	g_free(ptr_neuron_event_buffer->time);
	g_free(ptr_neuron_event_buffer->from);	
 	g_free(ptr_neuron_event_buffer->weight);

	g_free(ptr_neuron_event_buffer);
	
	return;
}

void destroy_ext_neuron_event_buffer(ExtNeuron *neuron)
{
	NeuronEventBuffer *ptr_neuron_event_buffer;
	
	ptr_neuron_event_buffer = neuron->event_buff;

	g_free(ptr_neuron_event_buffer->time);
	g_free(ptr_neuron_event_buffer->from);	
 	g_free(ptr_neuron_event_buffer->weight);

	g_free(ptr_neuron_event_buffer);
}
