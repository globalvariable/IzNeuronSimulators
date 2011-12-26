#include "Event.h"

static MainEventBuffer *main_event_buffer;    // Use dynamic allocation for each neuron.    Layer ----> Neuron Group ---> Neuron   3D array. 

bool initialize_main_event_buffer(void)
{
	main_event_buffer = NULL;
	main_event_buffer = g_new0(MainEventBuffer, 1);
	if (main_event_buffer == NULL)
	{
		printf("Event: ERROR: Couldn' t create main event buffer\n");
		return FALSE;
	}	
	return TRUE;

}

bool add_layer_event_buffer_to_main_event_buffer(int layer)
{
	LayerEventBuffer		*ptr_layer_event_buffer = NULL;
	
	if ((layer > main_event_buffer->layer_event_buffer_count) || (layer < 0) )
	{
		printf ("Event: ERROR: Couldn't add layer  event buffer %d.\n", layer);
		printf ("Event: ERROR: Layer number shouldn't be larger than %d and smaller than 0\n", main_event_buffer->layer_event_buffer_count);
		return FALSE;
	}
	
	if (layer == main_event_buffer->layer_event_buffer_count)		// New & valid event buffer layer request from user
	{
		ptr_layer_event_buffer = g_new0(LayerEventBuffer, 1);
		if (ptr_layer_event_buffer == NULL)
		{
			printf("Event: ERROR: Couldn' t create layer event buffer %d\n", layer);
			return FALSE;
		}
		main_event_buffer->layer_event_buffer[layer] = ptr_layer_event_buffer;
		main_event_buffer->layer_event_buffer_count++;		
	}
	
	// if it is an existing layer, do not do anything
	return TRUE;
}	

bool add_neuron_group_event_buffer_to_layer_event_buffer(int layer, int num_of_neuron)
{
	int i;
	LayerEventBuffer		*ptr_layer_event_buffer = NULL;
	NeuronGroupEventBuffer	*ptr_neuron_group_event_buffer = NULL;
	NeuronEventBuffer		*ptr_neuron_event_buffer ;
	Neuron				*neuron;			
	
	ptr_neuron_group_event_buffer = g_new0(NeuronGroupEventBuffer, 1);
	if (ptr_neuron_group_event_buffer == NULL)
	{
		printf("Event: ERROR: Couldn' t create neuron group  event buffer\n");
		return FALSE;
	}
	ptr_layer_event_buffer = main_event_buffer->layer_event_buffer[layer];			
	ptr_layer_event_buffer->neuron_group_event_buffer[ptr_layer_event_buffer->neuron_group_event_buffer_count] = ptr_neuron_group_event_buffer;
		
	ptr_neuron_group_event_buffer->neuron_event_buffer = g_new0(NeuronEventBuffer, num_of_neuron);
	if (ptr_neuron_group_event_buffer->neuron_event_buffer == NULL)
	{
		printf("Event: ERROR: Couldn' t create %d neuron  event buffer\n", num_of_neuron);
		return FALSE;
	}
	ptr_neuron_group_event_buffer->neuron_event_buffer_count = num_of_neuron;
	
	for (i = 0; i < num_of_neuron; i++)
	{
		neuron = &(all_network->layers[layer]->neuron_groups[ptr_layer_event_buffer->neuron_group_event_buffer_count]->neurons[i]);			// For faster data reach in event scheduling and insertion
		ptr_neuron_event_buffer = &(ptr_neuron_group_event_buffer->neuron_event_buffer[i]);	
		neuron->event_buff = ptr_neuron_event_buffer;	
	}	
	
	ptr_layer_event_buffer->neuron_group_event_buffer_count++;	
	return TRUE;
}


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
	pthread_mutex_t 	*mutex_event_buff;

	int 				idx, end_idx; 	
	
	neuron_event_buffer = neuron->event_buff;
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
		printf("ERROR: Event.c: Layer %d:\n", neuron->layer);
		printf("ERROR: Event.c: Neuron Group %d:\n", neuron->neuron_group);
		printf("ERROR: Event.c: Neuron Number %d:\n", neuron->neuron_num);	
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
	pthread_mutex_unlock(mutex_event_buff);	
	return 1;
}

bool increment_neuron_event_buffer_size(Neuron *neuron)
{
	int i;
	int layer = neuron->layer;
	int neuron_group = neuron->neuron_group;
	int neuron_num = neuron->neuron_num;

	NeuronEventBuffer	*ptr_neuron_event_buffer;
	TimeStamp		*time = NULL;		
	Neuron 			**from = NULL;			
	SynapticWeight		*weight = NULL;		

	ptr_neuron_event_buffer = &(main_event_buffer->layer_event_buffer[layer]->neuron_group_event_buffer[neuron_group]->neuron_event_buffer[neuron_num]);
	
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

