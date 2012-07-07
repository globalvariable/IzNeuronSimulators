#include "Event.h"

bool schedule_event(Neuron *nrn, TimeStamp event_time)
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
		if (! insert_synaptic_event(axon_to[i], event_time + axon_delay[i], syn_idx[i]))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "Event", "schedule_event_from_internal_neuron_to_internal_neuron", "! insert_synaptic_event().");
	}
	return TRUE;	
}

bool insert_synaptic_event(Neuron *neuron, TimeStamp scheduled_event_time, SynapseIndex syn_idx)
{
	NeuronEventBuffer	*neuron_event_buffer;
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

//	for (idx = 0; idx <event_buff_size; idx++ )
//		printf("%llu\n", neuron_event_buffer->time[idx]);
//	printf("---------------\n");
//	printf("%u\n", neuron_event_buffer->write_idx);
	return TRUE;
}

bool increase_neuron_event_buffer_size(Neuron *neuron, unsigned int amount)
{
	NeuronEventBuffer	*ptr_neuron_event_buffer;
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
		ptr_neuron_event_buffer->syn_idx[i] = 0;
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
	g_free(ptr_neuron_event_buffer->syn_idx);	

	g_free(ptr_neuron_event_buffer);
	
	return;
}

