#include "Simulate.h"

int evaluate_neuron_dyn(Neuron *nrn, int layer, int neuron_group, int neuron_num, TimeStamp start_time, TimeStamp end_time)
{

	TimeStamp 		integration_start_ns;

	NeuronEventBuffer	*neuron_event_buffer;
	int				*ptr_event_buffer_write_idx;   	
	int				*ptr_event_buffer_read_idx;
	int 				idx, end_idx; 	
	TimeStamp		*event_times;
	SynapticWeight		*event_weights;	
	int 				event_buff_size;
	
	TimeStamp		event_time;	
				
	neuron_event_buffer = &(event_buffers[layer][neuron_group][neuron_num]);
	ptr_event_buffer_write_idx = &(neuron_event_buffer->write_idx);
	ptr_event_buffer_read_idx = &(neuron_event_buffer->read_idx);
	event_times = neuron_event_buffer->time;
	event_weights = neuron_event_buffer->weight;
	event_buff_size =neuron_ event_buffer->buff_size;
			
	integration_start_ns = start_time;   // required to know it to schedule events for outputs  (parker_sochacki_step_start_time+ dt_part)
	
	idx = *ptr_event_buffer_read_idx;
	end_idx = *ptr_event_buffer_write_idx;

	while (idx != end_idx)		// There is a check for event buffer overflow in schedule_events()
	{
		event_time = event_times[idx];
		if (event_time >= end_time)	// Do not handle if it is at start of next step or later than that
			break;
		if (event_time < start_time)
		{
			printf ("BUG: evaluate_neuron_dyn - Simulate.c\n");
			printf ("BUG: event_time is %llu but the step start time is % llu\n", event_time, start_time);		
			printf ("BUG: There must be a problem at event sorting\n");	
			return 0;
		}
		parker_sochacki_integration(nrn, layer, neuron_group, neuron_num, integration_start_ns, event_time);
		if (event_weight[idx]>0)
			nrn->conductance_excitatory += event_weights[idx];
		else if (event_weight[idx]<0)
			nrn->conductance_inhibitory -= event_weights[idx];	
		else
			printf ("BUG: evaluate_neuron_dyn - Simulate.c  -----> Weight cannot be zero\n");	
		integration_start_ns = event_time;
		idx++;
		if (idx == event_buff_size)
			idx = 0;
	}
	parker_sochacki_integration(nrn, layer, neuron_group, neuron_num, integration_start_ns, end_time);
	*ptr_event_buffer_read_idx = end_idx;			// Finally set event_buffer_read_idx,	event_buffer_write_idx will be set by  insert_synaptic_event at the end of function so that no corruption will appear even with multithreading
	return 1;
}

