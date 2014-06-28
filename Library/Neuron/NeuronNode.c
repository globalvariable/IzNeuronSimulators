#include "NeuronNode.h"


bool evaluate_node_inputs (Neuron *nrn, unsigned int *num_of_rxed_spikes, TimeStamp start_time, TimeStamp end_time)  // for brain machine interfaces which compares spike rates of neurons to control robot.
{
	NeuronSortedEventBuffer		*sorted_event_buffer = nrn->sorted_event_buffer;
	NeuronSortedEventBufferItem	*event_item;	
	TimeStamp 			event_time;
	unsigned int			*read_idx, write_idx;

	*num_of_rxed_spikes = 0;

	if (! sort_neuron_events(nrn))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "NeuronNode", "evaluate_node_inputs", "! sort_neuron_events().");

	read_idx = &(sorted_event_buffer->read_idx);
	write_idx = sorted_event_buffer->write_idx;
	while ((*read_idx) != write_idx)			
	{
		event_item = &(sorted_event_buffer->buff[*read_idx]);
		event_time = event_item->time;
		if (event_time >= end_time)	// Do not handle if it is at start of next step or later than that
			break;
		if (event_time < start_time)	// this function handles sorted synaptic events. the events are sorted during buffering
		{
			printf ("Simulate: BUG: evaluate_neuron_dyn - Simulate.c\n");
			printf ("Simulate: BUG: event_time is %llu but the step start time is %llu\n", event_time, start_time);	
			printf ("Simulate: BUG: event_time:%llu event_type: %u synapse idx: %u.\n", event_time, event_item->event_type,  event_item->syn_idx);	
			printf ("Simulate: BUG: There must be a problem at event sorting\n");	
			return 0;
		}
//		printf("event: %llu syn: %u weight: %f type: %u\n", event_time, event_item->syn_idx, synapses[event_item->syn_idx].weight, event_item->event_type); 	
		if (event_item->event_type == NEURON_EVENT_TYPE_SYNAPTIC_EVENT)
		{
			(*num_of_rxed_spikes)++;
		}

		if ((*read_idx + 1) == sorted_event_buffer->buff_size)
			*read_idx = 0;
		else
			(*read_idx)++;
	}


	return  TRUE;
}
