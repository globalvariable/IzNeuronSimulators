#include "Event.h"

bool schedule_synaptic_event(Neuron *nrn, TimeStamp event_time)	
{
	unsigned int i;
	NeuronAxonList 	*neuron_axon_list;
	Neuron 			**axon_to;
	AxonalDelay		*axon_delay;
	SynapseIndex		*syn_idx;
	unsigned int		num_of_axons;
	
	neuron_axon_list = nrn->axon_list;
	axon_to = neuron_axon_list->to;
	axon_delay = neuron_axon_list->delay;
	syn_idx = neuron_axon_list->syn_idx;
	num_of_axons = neuron_axon_list->num_of_axons;
	
	for (i = 0; i < num_of_axons; i++)
	{
		if (! write_to_neuron_synaptic_event_buffer(axon_to[i], event_time + axon_delay[i], syn_idx[i]))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "Event", "schedule_event_from_internal_neuron_to_internal_neuron", "! write_to_neuron_synaptic_event_buffer().");
	}
	return TRUE;	
}

bool insert_synaptic_event(Neuron *neuron, TimeStamp scheduled_event_time, SynapseIndex syn_idx)
{
/*	NeuronEventBuffer	*neuron_event_buffer;
	unsigned int		*ptr_event_buffer_write_idx;   	
	TimeStamp		*event_times;
	SynapseIndex		*syn_indexes;
	unsigned int 		event_buff_size;
	unsigned int 		idx, end_idx, buff_last_idx; 	
	
	neuron_event_buffer = neuron->event_buff;
	event_buff_size = neuron_event_buffer->buff_size;
	event_times = neuron_event_buffer->time;
	syn_indexes = neuron_event_buffer->syn_idx;
//	printf("new_event %llu\n", scheduled_event_time);

	pthread_mutex_lock(&(neuron_event_buffer->mutex));
	
	ptr_event_buffer_write_idx = &(neuron_event_buffer->write_idx);

	idx = *ptr_event_buffer_write_idx;
	end_idx = neuron_event_buffer->read_idx; 

	if ((((idx + 1) == event_buff_size) && (end_idx == 0)) || ((idx + 1) == end_idx))		// if write idx catches read idx (buffer full)
	/// SHOULD CHECK THIS BEFORE SHIFTING THE LAST INDEX: OTHERWISE READER CAN READ CORRUPTED LAST INDEX DATA.  // READER DOES NOT USE MUTEX (TO BE FASTER)
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
				syn_indexes[0] = syn_indexes[buff_last_idx];
				idx = buff_last_idx;
			}
			else
			{
				event_times[0] = scheduled_event_time;	// insert here & break;
				syn_indexes[0] = syn_idx;	
				break;			
			}
		}
		else
		{
			buff_last_idx = idx-1;
			if (scheduled_event_time < event_times[buff_last_idx])		// push buffered item to the next index
			{
				event_times[idx] = event_times[buff_last_idx];
				syn_indexes[idx] = syn_indexes[buff_last_idx];
				idx--;
			}
			else
			{
				event_times[idx] = scheduled_event_time;	// insert here & break;
				syn_indexes[idx] = syn_idx;	
				break;			
			}			
		}
	} while (1);
	
	if ((*ptr_event_buffer_write_idx + 1) == event_buff_size)
		*ptr_event_buffer_write_idx = 0;
	else
		(*ptr_event_buffer_write_idx)++;

	pthread_mutex_unlock(&(neuron_event_buffer->mutex));
*/
//	for (idx = 0; idx <event_buff_size; idx++ )
//		printf("%llu\n", neuron_event_buffer->time[idx]);
//	printf("---------------\n");
//	printf("%u\n", neuron_event_buffer->write_idx);
	return TRUE;
}

bool increase_neuron_event_buffer_size(Neuron *neuron, unsigned int amount)
{
/*	NeuronEventBuffer	*ptr_neuron_event_buffer;
	TimeStamp		*time = NULL;		
	SynapseIndex		*syn_idx = NULL;			

	ptr_neuron_event_buffer = neuron->event_buff;
	
	time = g_new0(TimeStamp, ptr_neuron_event_buffer->buff_size+amount);		// assume synaptic delay is 2 ms and neuron can fire every 1 ms.
	syn_idx = g_new0(SynapseIndex, ptr_neuron_event_buffer->buff_size+amount);
	ptr_neuron_event_buffer->buff_size = ptr_neuron_event_buffer->buff_size+amount;
	
	g_free(ptr_neuron_event_buffer->time);
	g_free(ptr_neuron_event_buffer->syn_idx);		
	
	ptr_neuron_event_buffer->time = time;	// point to new event buffer
	ptr_neuron_event_buffer->syn_idx = syn_idx;		

	ptr_neuron_event_buffer->write_idx = 0;   // for spike time insetion sorting.
	ptr_neuron_event_buffer->read_idx = 0;	// reset event buffer, clear all data it has had.	
*/
	return TRUE;
}

void clear_neuron_event_buffer(Neuron *neuron)
{
/*	int i;
	NeuronEventBuffer	*ptr_neuron_event_buffer;
	ptr_neuron_event_buffer = neuron->event_buff;

	pthread_mutex_lock(&(ptr_neuron_event_buffer->mutex));	
	for (i=0; i< ptr_neuron_event_buffer->buff_size; i++)
	{
		ptr_neuron_event_buffer->time[i] = 0;
		ptr_neuron_event_buffer->syn_idx[i] = 0;
	}
	ptr_neuron_event_buffer->write_idx = 0;
	ptr_neuron_event_buffer->read_idx = 0;
	pthread_mutex_unlock(&(ptr_neuron_event_buffer->mutex));*/
}		

void destroy_neuron_event_buffer(Neuron *neuron)
{
/*	NeuronEventBuffer *ptr_neuron_event_buffer;
	
	ptr_neuron_event_buffer = neuron->event_buff;

	g_free(ptr_neuron_event_buffer->time);
	g_free(ptr_neuron_event_buffer->syn_idx);	

	g_free(ptr_neuron_event_buffer);
	
	return;*/
}

NeuronSynapticEventBuffer* allocate_neuron_synaptic_event_buffer(TimeStamp synaptic_delay)
{
	NeuronSynapticEventBuffer *event_buffer;
	unsigned int buff_size = ((unsigned int)(((double)synaptic_delay)/500000.0)) + 1;   // assumes presynaptic neuron cannot fire more than 2000 Hz. 
	event_buffer = g_new0(NeuronSynapticEventBuffer, 1);
	event_buffer->buff = g_new(NeuronSynapticEventBufferItem, buff_size);
	event_buffer->buff_size = buff_size;
	return event_buffer;
}

bool update_neuron_sorted_event_buffer_size(Neuron *neuron)
{
	unsigned int i, total_num_of_synaptic_event_buffers = 0;

	for (i = 0; i < neuron->syn_list->num_of_synapses; i++)
	{
		neuron->syn_list->synapses[i].event_buffer->write_idx = 0;   // clear previously received events
		neuron->syn_list->synapses[i].event_buffer->read_idx = 0; 
		total_num_of_synaptic_event_buffers += neuron->syn_list->synapses[i].event_buffer->buff_size;
	}

	g_free(neuron->sorted_event_buffer->buff);

	neuron->sorted_event_buffer->buff = g_new0(NeuronSortedEventBufferItem, total_num_of_synaptic_event_buffers);
	neuron->sorted_event_buffer->buff_size = total_num_of_synaptic_event_buffers;

	neuron->sorted_event_buffer->write_idx = 0;   // for spike time insetion sorting.
	neuron->sorted_event_buffer->read_idx = 0;	// reset event buffer, clear all data it has had.	

	return TRUE;
}

bool write_to_neuron_synaptic_event_buffer(Neuron *neuron, TimeStamp scheduled_event_time, SynapseIndex syn_idx)
{
	NeuronSynapticEventBuffer *event_buffer = neuron->syn_list->synapses[syn_idx].event_buffer;
	unsigned int idx = event_buffer->write_idx;
	
	event_buffer->buff[idx] = scheduled_event_time;		

	if ((idx + 1) == event_buffer->buff_size)
		idx = 0;
	else
		idx++;
	if (idx == event_buffer->read_idx)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Event", "write_to_neuron_synaptic_event_buffer", "BUFFER IS FULL!!!.");    		
	event_buffer->write_idx = idx;
	return TRUE;
}

bool get_next_synaptic_event_buffer_item(NeuronSynapticEventBuffer *event_buffer, NeuronSynapticEventBufferItem	*item)   /// typedef TimeStamp NeuronSynapticEventBufferItem
{
	unsigned int *idx;
	if (event_buffer->read_idx == event_buffer->write_idx)
		return FALSE;
	idx = &(event_buffer->read_idx);
	*item = event_buffer->buff[*idx];
	if ((*idx + 1) == event_buffer->buff_size)
		*idx = 0;
	else
		(*idx)++;
	return TRUE;
}



bool sort_neuron_events(Neuron *neuron)
{
	Synapse			*synapses = neuron->syn_list->synapses;
	SynapseIndex		num_of_synapses = neuron->syn_list->num_of_synapses; 
	NeuronSortedEventBuffer	*sorted_event_buffer = neuron->sorted_event_buffer;
	NeuronSynapticEventBuffer	*synaptic_event_buffer;
	NeuronSynapticEventBufferItem	synaptic_event_item;

	SynapseIndex i;

	for (i = 0; i < num_of_synapses; i++)
	{
		synaptic_event_buffer = synapses[i].event_buffer;
		while (get_next_synaptic_event_buffer_item(synaptic_event_buffer, &synaptic_event_item))
		{
			if (! sort_synaptic_event_for_neuron(sorted_event_buffer, synaptic_event_item, i))
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "Event", "sort_neuron_events", "! sort_synaptic_event_for_neuron().");    		
		}
	}

	return TRUE;
}

bool sort_synaptic_event_for_neuron(NeuronSortedEventBuffer *sorted_event_buffer, TimeStamp scheduled_time, SynapseIndex syn_idx)
{
	unsigned int *write_idx = &(sorted_event_buffer->write_idx);
	unsigned int idx = *write_idx;
	unsigned int event_buff_size = sorted_event_buffer->buff_size;
	NeuronSortedEventBufferItem *item_hand, *item_ins, *buff = sorted_event_buffer->buff;		

	do {
		if (idx == 0)
		{	
			item_hand = &(buff[event_buff_size-1]);
			item_ins = &(buff[0]);
			if (scheduled_time < item_hand->time)		// push buffered item to the next index
			{
				item_ins->time = item_hand->time;
				item_ins->event_type = item_hand->event_type;
				item_ins->syn_idx = item_hand->syn_idx;
				idx = event_buff_size-1;
			}
			else
			{
				item_ins->time = scheduled_time;				// insert here & break;
				item_ins->event_type = NEURON_EVENT_TYPE_SYNAPTIC_EVENT;
				item_ins->syn_idx = syn_idx;
				break;			
			}
		}
		else
		{
			item_hand = &(buff[idx-1]);
			item_ins = &(buff[idx]);
			if (scheduled_time < item_hand->time)		// push buffered item to the next index
			{
				item_ins->time = item_hand->time;
				item_ins->event_type = item_hand->event_type;
				item_ins->syn_idx = item_hand->syn_idx;
				idx--;
			}
			else
			{
				item_ins->time = scheduled_time;				// insert here & break;
				item_ins->event_type = NEURON_EVENT_TYPE_SYNAPTIC_EVENT;
				item_ins->syn_idx = syn_idx;
				break;			
			}			
		}
	} while (1);
	
	if ((*write_idx + 1) == event_buff_size)
		*write_idx = 0;
	else
		(*write_idx)++;
	if (*write_idx == sorted_event_buffer->read_idx)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Event", "sort_synaptic_event_for_neuron", "BUFFER IS FULL!!!.");    		
	return TRUE;
}

void reset_neuron_event_buffer(Neuron *neuron)
{
	NeuronSortedEventBuffer	*sorted_event_buffer = neuron->sorted_event_buffer;
	NeuronSynapticEventBuffer	*synaptic_event_buffer;
	SynapseIndex		i, num_of_synapses = neuron->syn_list->num_of_synapses; 


	num_of_synapses = neuron->syn_list->num_of_synapses;

	sorted_event_buffer->read_idx = sorted_event_buffer->write_idx;
	for (i = 0; i < num_of_synapses; i++)
	{
		synaptic_event_buffer = neuron->syn_list->synapses[i].event_buffer;
		synaptic_event_buffer->read_idx = synaptic_event_buffer->write_idx;
	}
	return;
}


