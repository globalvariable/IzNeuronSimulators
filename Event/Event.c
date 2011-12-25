#include "Event.h"

static MainEventBuffer *main_event_buffer;    // Use dynamic allocation for each neuron.    Layer ----> Neuron Group ---> Neuron   3D array. 

bool initialize_main_event_buffer(void)
{
	main_event_buffer == NULL;
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
		printf ("Event: ERROR: Layer number shouldn't be larger than %d and smaller than 0\n", all_network->count);
		return FALSE;
	}
	
	if (layer == main_event_buffer->layer_event_buffer_count)		// New & valid synapse list layer request from user
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

bool add_neuron_group_event_buffer_to_layer_event_buffer(int layer, int num_of _neuron)
{
	LayerEventBuffer		*ptr_layer_event_buffer = NULL;
	NeuroGroupEventBuffer	*ptr_neuron_group_event_buffer = NULL;
	NeuronEventBuffer		*ptr_neuron_event_buffer =NULL;
	
	ptr_neuron_group_event_buffer = g_new0(NeuroGroupEventBuffer, 1);
	if (ptr_neuron_group_syn_list == NULL)
	{
		printf("Synapse: ERROR: Couldn' t create neuron group  event buffer\n");
		return FALSE;
	}
	ptr_layer_event_buffer = main_event_buffer->layer_event_buffer[layer];			
	ptr_layer_event_buffer->neuron_groups_event_buffer[ptr_layer_event_buffer->neuron_group_event_buffer_count] = ptr_neuron_group_event_buffer;
	ptr_layer_event_buffer->neuron_group_event_buffer_count++;
		
	ptr_neuron_group_event_buffer->neuron_event_buffer = g_new0(NeuronEventBuffer, num_of_neuron);
	if (ptr_neuron_group_event_buffer->neuron_event_buffer == NULL)
	{
		printf("Synapse: ERROR: Couldn' t create %d neuron  event buffer\n", num_of_neuron);
		return FALSE;
	}
	ptr_neuron_group_event_buffer->neuron_event_buffer_count = num_of_neuron;
	
	return TRUE;
}


int schedule_events(Neuron *nrn, int layer, int neuron_group, int neuron_num, double dt_part, TimeStamp integration_start_time)
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
	
	for (i = 0; i < num_of_synapses; i++)
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
