#include "Event.h"

bool schedule_event(Neuron *nrn, TimeStamp event_time)
{
	unsigned int i;
	NeuronSynapseList *neuron_synapse_list;
	Neuron 			**synapse_to;
	SynapticDelay		*synapse_delay;
	SynapticWeight	*synapse_weight;
	unsigned int		num_of_synapses;
	TimeStamp		scheduled_event_time;
	
	neuron_synapse_list = nrn->syn_list;
	synapse_to = neuron_synapse_list->to;
	synapse_delay = neuron_synapse_list->delay;
	synapse_weight = neuron_synapse_list->weight;
	num_of_synapses = neuron_synapse_list->num_of_synapses;
	
	for (i = 0; i < num_of_synapses; i++)
	{
		scheduled_event_time = synapse_delay[i]+event_time;
		if (! insert_synaptic_event(synapse_to[i], scheduled_event_time, synapse_weight[i], nrn))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "Event", "schedule_event_from_internal_neuron_to_internal_neuron", "! insert_synaptic_event().");
	}
	return TRUE;	
}

bool insert_synaptic_event(Neuron *neuron, TimeStamp scheduled_event_time, double weight, Neuron *event_from)
{
	NeuronEventBuffer	*neuron_event_buffer;
	unsigned int		*ptr_event_buffer_write_idx;   	
	unsigned int		*ptr_event_buffer_read_idx;
	TimeStamp		*event_times;
	Neuron 			**events_from;
	SynapticWeight	*event_weights;	
	unsigned int 		event_buff_size;
	unsigned int 		idx, end_idx, buff_last_idx; 	
	
	neuron_event_buffer = neuron->event_buff;
	event_buff_size = neuron_event_buffer->buff_size;
	event_times = neuron_event_buffer->time;
	event_weights = neuron_event_buffer->weight;	
	events_from = neuron_event_buffer->from;
	printf("new_event %llu\n", scheduled_event_time);

	pthread_mutex_lock(&(neuron_event_buffer->mutex));
	
	ptr_event_buffer_write_idx = &(neuron_event_buffer->write_idx);
	ptr_event_buffer_read_idx = &(neuron_event_buffer->read_idx);

	idx = *ptr_event_buffer_write_idx;
	end_idx = *ptr_event_buffer_read_idx; 

	if ((((idx + 1) == event_buff_size) && (end_idx == 0)) || ((idx + 1) == end_idx))		// if write idx catches read idx (buffer full)
//	if ((((*ptr_event_buffer_write_idx + 1) == event_buff_size) && (*ptr_event_buffer_read_idx == 0)) || ((*ptr_event_buffer_write_idx + 1) == *ptr_event_buffer_read_idx))	
	{
		printf("ERROR: Event.c: insert_synaptic_event().\n");
		printf("ERROR: Event.c: Event buffer is full for this neuron:\n");
		printf("ERROR: Event.c: Layer %d:\n", neuron->layer);
		printf("ERROR: Event.c: Neuron Group %d:\n", neuron->neuron_group);
		printf("ERROR: Event.c: Neuron Number %d:\n", neuron->neuron_num);	
		pthread_mutex_unlock(&(neuron_event_buffer->mutex));
		return FALSE;
	}
	
	do {
		if (idx == 0)
		{	
			buff_last_idx = event_buff_size-1;
			if (scheduled_event_time < event_times[buff_last_idx])		// push buffered item to the next index
			{
				event_times[0] = event_times[buff_last_idx];
				event_weights[0] = event_weights[buff_last_idx];
				events_from[0] = events_from[buff_last_idx];
				idx = buff_last_idx;
			}
			else
			{
				event_times[0] = scheduled_event_time;	// insert here & break;
				event_weights[0] = weight;
				events_from[0] = event_from;	
				break;			
			}
		}
		else
		{
			if (scheduled_event_time < event_times[idx-1])		// push buffered item to the next index
			{
				event_times[idx] = event_times[idx-1];
				event_weights[idx] = event_weights[idx-1];
				events_from[idx] = events_from[idx-1];
				idx--;
			}
			else
			{
				event_times[idx] = scheduled_event_time;	// insert here & break;
				event_weights[idx] = weight;
				events_from[idx] = event_from;	
				break;			
			}			
		}
	} while (1);
	
	if ((*ptr_event_buffer_write_idx + 1) == event_buff_size)
		*ptr_event_buffer_write_idx = 0;
	else
		(*ptr_event_buffer_write_idx)++;

	pthread_mutex_unlock(&(neuron_event_buffer->mutex));

	for (idx = 0; idx <event_buff_size; idx++ )
		printf("%llu\n", neuron_event_buffer->time[idx]);
	printf("---------------\n");
//	printf("%u\n", neuron_event_buffer->write_idx);
	return TRUE;
}

bool increase_neuron_event_buffer_size(Neuron *neuron, unsigned int amount)
{
	NeuronEventBuffer	*ptr_neuron_event_buffer;
	TimeStamp		*time = NULL;		
	Neuron 			**from = NULL;			
	SynapticWeight		*weight = NULL;		

	ptr_neuron_event_buffer = neuron->event_buff;
	
	time = g_new0(TimeStamp, ptr_neuron_event_buffer->buff_size+amount);		// assume synaptic delay is 2 ms and neuron can fire every 1 ms.
	from = g_new0(Neuron*, ptr_neuron_event_buffer->buff_size+amount);
	weight = g_new0(SynapticWeight, ptr_neuron_event_buffer->buff_size+amount);
	ptr_neuron_event_buffer->buff_size = ptr_neuron_event_buffer->buff_size+amount;
	
	g_free(ptr_neuron_event_buffer->time);
	g_free(ptr_neuron_event_buffer->from);		
	g_free(ptr_neuron_event_buffer->weight);	
	
	ptr_neuron_event_buffer->time = time;	// point to new event buffer
	ptr_neuron_event_buffer->from = from;		
	ptr_neuron_event_buffer->weight = weight;	

	ptr_neuron_event_buffer->write_idx = 0;   // for spike time insetion sorting.
	ptr_neuron_event_buffer->read_idx = 0;	// reset event buffer, clear all data it has had.	

	return TRUE;
}

void clear_neuron_event_buffer(Neuron *neuron)
{
	int i;
	NeuronEventBuffer	*ptr_neuron_event_buffer;
	ptr_neuron_event_buffer = neuron->event_buff;

	pthread_mutex_lock(&(ptr_neuron_event_buffer->mutex));	
	for (i=0; i< ptr_neuron_event_buffer->buff_size; i++)
	{
		ptr_neuron_event_buffer->time[i] = 0;
		ptr_neuron_event_buffer->from[i] = 0;
		ptr_neuron_event_buffer->weight[i] = 0;
	}
	ptr_neuron_event_buffer->write_idx = 0;
	ptr_neuron_event_buffer->read_idx = 0;
	pthread_mutex_unlock(&(ptr_neuron_event_buffer->mutex));
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

