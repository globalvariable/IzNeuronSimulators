#include "NeuronDynamicsGraph.h"


NeuronDynamicsGraph* allocate_neuron_dynamics_graph(GtkWidget *hbox, NeuronDynamicsGraph *graph, unsigned int num_of_data_points, TimeStamp sampling_interval)
{
	unsigned int i;
	if (graph != NULL)
	{
		print_message(ERROR_MSG ,"IzNeuronSimulators", "NeuronDynamicsGraph", "allocate_neuron_dynamics_box_and_graph", "graph != NULL");	
		return graph;
	}
	
	graph = g_new0(NeuronDynamicsGraph,1);
	graph->num_of_data_points = num_of_data_points;
	graph->sampling_interval = sampling_interval;
	graph->graph_len = sampling_interval*num_of_data_points;
	GdkColor color_bg;
	GdkColor color_line;
	GdkColor color_grid;

	color_bg.red = 65535;
	color_bg.green = 65535;
	color_bg.blue = 65535;
	
	color_grid.red = 50000;
	color_grid.green = 50000;
	color_grid.blue = 50000;
	
	color_line.red = 0;
	color_line.green = 0;
	color_line.blue = 0;

	gtk_databox_create_box_with_scrollbars_and_rulers (&(graph->box), &(graph->databox),TRUE, TRUE, TRUE, TRUE);
  	gtk_widget_modify_bg (graph->box, GTK_STATE_NORMAL, &color_bg);
 	gtk_databox_graph_add (GTK_DATABOX (graph->box), gtk_databox_grid_new (9, 9, &color_grid, 0)); 	
	gtk_box_pack_start (GTK_BOX (hbox), graph->databox, TRUE, TRUE, 0);

	graph->x = g_new0(float, num_of_data_points);  
	graph->y = g_new0(float, num_of_data_points);

	for (i = 0; i < num_of_data_points; i++)
		graph->x[i] = i*(sampling_interval/1000000.0);
	graph->graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (num_of_data_points, graph->x, graph->y, &color_line, 0));
	gtk_databox_graph_add (GTK_DATABOX (graph->box), graph->graph);	
	gtk_databox_set_total_limits (GTK_DATABOX (graph->box), 0, (graph->graph_len/1000000), 200, -200);	

	gtk_widget_show_all(hbox);	

	return graph;						
}

bool update_neuron_dynamics_graph(NeuronDynamicsGraph *graph)
{
	float max_y = 0, min_y = 0;
	float *y = graph->y;
	unsigned int i;
	unsigned int num_of_data_points = graph->num_of_data_points;

	for (i = 0; i < num_of_data_points; i++)
	{
		if (y[i] > max_y)
			max_y = y[i];
		if (y[i] < min_y)
			min_y = y[i];
	}
	if (max_y < 100)
		max_y = 200;
	else if (max_y < 200)
		max_y = 200;
	else if (max_y < 300)
		max_y = 300;
	else if (max_y < 400)
		max_y = 400;
	else 
		max_y = max_y+100;

	if (min_y > 0)
		min_y = 0;
	else if (min_y > -100)
		min_y = -100;
	else if (min_y > -200)
		min_y = -200;
	else if (min_y > -300)
		min_y = -300;
	else
		min_y = min_y-100;

	gtk_databox_set_total_limits (GTK_DATABOX (graph->box), 0 , (graph->graph_len/1000000) , max_y, min_y);

	return TRUE;	
}

bool clear_neuron_dynamics_graph(NeuronDynamicsGraph *graph)
{
	unsigned int i;
	unsigned int num_of_data_points = graph->num_of_data_points;
	float *y = graph->y;
	for (i = 0; i < num_of_data_points; i++)
		y[i] = 0;
	return TRUE;
}


bool change_length_of_neuron_dynamics_graph(NeuronDynamicsGraph *graph, TimeStamp new_length, bool log_request)
{
	unsigned int num_of_data_points;
	if (new_length == graph->graph_len)
		return TRUE;
	
	if (log_request)
		print_message(INFO_MSG ,"IzNeuronSimulators", "NeuronDynamicsGraph", "change_length_of_neuron_dynamics_graph", "Changing graph length");		

	gtk_databox_graph_remove (GTK_DATABOX (graph->databox), graph->graph);
	g_object_unref(graph->graph);
	g_free(graph->x);  
	g_free(graph->y);  
	num_of_data_points = new_length/graph->sampling_interval;
	graph->x = g_new0(float, num_of_data_points);  
	graph->y = g_new0(float, num_of_data_points);	
	graph->num_of_data_points = num_of_data_points;
	graph->graph_len =  new_length;
	return TRUE;
}

NeuronDynamicsGraphScroll* allocate_neuron_dynamics_graph_scroll(GtkWidget *hbox, NeuronDynamicsGraphScroll *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, unsigned int num_of_data_points_to_scroll, TimeStamp buffer_followup_latency, unsigned int num_of_markers, TrialStatusEvents *trial_status_events)
{
	unsigned int i;
	if (graph != NULL)
	{
		print_message(ERROR_MSG ,"IzNeuronSimulators", "NeuronDynamicsGraph", "allocate_neuron_dynamics_box_and_graph", "graph != NULL");	
		return graph;
	}
	
	graph = g_new0(NeuronDynamicsGraphScroll,1);
	pthread_mutex_init(&(graph->mutex), NULL);
	graph->paused = TRUE;
	graph->num_of_data_points = num_of_data_points;
	graph->sampling_interval = sampling_interval;
	graph->graph_len = sampling_interval*num_of_data_points;
	GdkColor color_bg;
	GdkColor color_line;
	GdkColor color_grid;
	GdkColor color_status_marker;

	color_bg.red = 0;
	color_bg.green = 0;
	color_bg.blue = 0;
	
	color_grid.red = 25000;
	color_grid.green = 25000;
	color_grid.blue = 25000;
	
	color_line.red = 65535;
	color_line.green = 65535;
	color_line.blue = 65535;

	graph->databox = gtk_databox_new ();	
	gtk_box_pack_start (GTK_BOX (hbox), graph->databox, TRUE, TRUE, 0);
  	gtk_widget_modify_bg (graph->databox, GTK_STATE_NORMAL, &color_bg);
 	gtk_databox_graph_add (GTK_DATABOX (graph->databox), gtk_databox_grid_new (9, 9, &color_grid, 0)); 	

	graph->x = g_new0(float, num_of_data_points);  
	graph->y = g_new0(float, num_of_data_points);

	for (i = 0; i < num_of_data_points; i++)
		graph->x[i] = i*(sampling_interval/1000000.0);
	graph->graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (num_of_data_points, graph->x, graph->y, &color_line, 0));
	gtk_databox_graph_add (GTK_DATABOX (graph->databox), graph->graph);	
	gtk_databox_set_total_limits (GTK_DATABOX (graph->databox), 0, (graph->graph_len/1000000), 200, -200);	

	graph->num_of_data_points_to_scroll = num_of_data_points_to_scroll;
	graph->graph_len_to_scroll = sampling_interval*num_of_data_points_to_scroll;
	graph->buffer_followup_latency = buffer_followup_latency;

	graph->status_markers = g_new0(StatusMarkers,1);
	graph->status_markers->markers = g_new0(StatusMarker, num_of_markers);
	graph->status_markers->num_of_markers = num_of_markers;

	for (i = 0; i < num_of_markers; i++)
	{
		if (! get_status_marker_color(&color_status_marker, i))
			return (NeuronDynamicsGraphScroll*)print_message(ERROR_MSG ,"IzNeuronSimulators", "NeuronDynamicsGraph", "allocate_neuron_dynamics_graph_scroll", "! get_status_marker_color().");
 		gtk_databox_graph_add (GTK_DATABOX (graph->databox), gtk_databox_lines_new (2, graph->status_markers->markers[i].x, graph->status_markers->markers[i].y, &color_status_marker, 1)); 			
	}
	graph->trial_status_events = trial_status_events;
	gtk_widget_show_all(hbox);	

	return graph;				
}

bool determine_neuron_dynamics_graph_scroll_start_indexes(NeuronDynamicsGraphScroll *graph, TimeStamp current_system_time, TimeStamp last_sample_time, unsigned int neuron_dynamics_buffer_write_idx, unsigned int neuron_dynamics_buffer_size)
{
	unsigned int last_sample_times_idx;
	unsigned int current_system_times_idx;
	if (neuron_dynamics_buffer_write_idx == 0)
		last_sample_times_idx = neuron_dynamics_buffer_size - 1;
	else
		last_sample_times_idx = neuron_dynamics_buffer_write_idx -1;

	if (current_system_time < last_sample_time)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "NeuronDynamicsGraph", "determine_neuron_dynamics_graph_scroll_start_indexes", "current_system_time < last_sample_time)");
	if ((((current_system_time-last_sample_time)/graph->sampling_interval) + last_sample_times_idx) > neuron_dynamics_buffer_size)
		current_system_times_idx = ((current_system_time-last_sample_time)/graph->sampling_interval)  + last_sample_times_idx - 	neuron_dynamics_buffer_size;
	else
		current_system_times_idx = ((current_system_time-last_sample_time)/graph->sampling_interval)  + last_sample_times_idx;
	graph->new_part_start_time = current_system_time;
	graph->new_part_start_idx = current_system_times_idx;
	graph->trial_status_event_buffer_read_idx = graph->trial_status_events->buff_write_idx;
	return TRUE;
}

bool handle_neuron_dynamics_graph_scrolling_and_plotting(NeuronDynamicsGraphScroll *graph, NeuronDynamicsBuffer *neuron_dynamics_buffer, TimeStamp current_system_time)
{
	TimeStamp new_part_start_time;
	TimeStamp new_part_end_time;
	TimeStamp graph_len_to_scroll;
	TimeStamp graph_len;
	unsigned int idx, end_idx, graph_old_part_end_idx, graph_sample_idx = 0; 
	unsigned int layer, neuron_grp, neuron_num;
	int dynamics_type;	
	unsigned int buffer_size;
	StatusMarker *markers;
	TrialStatusEventItem *status_event_item;	
	if (!graph->paused)
	{
		if (graph->scroll_request)   // it is necessary otherwise set_total_limits cannot display slided and clear graph part. 
			scroll_neuron_dynamics_graph(graph);
		new_part_start_time = graph->new_part_start_time;
		new_part_end_time = new_part_start_time + graph->graph_len_to_scroll;
		buffer_size = neuron_dynamics_buffer->buffer_size;

		if (current_system_time > (new_part_end_time + graph->buffer_followup_latency) )
		{
			pthread_mutex_lock(&(graph->mutex));
			layer = graph->active_layer;
			neuron_grp = graph->active_neuron_group;
			neuron_num = graph->active_neuron;
			dynamics_type = graph->active_dynamics_type;
			pthread_mutex_unlock(&(graph->mutex));
			idx =  graph->new_part_start_idx;
			if ((idx + graph->num_of_data_points_to_scroll) > buffer_size)
				end_idx = idx + graph->num_of_data_points_to_scroll - buffer_size;
			else
				end_idx = idx + graph->num_of_data_points_to_scroll;
			graph_old_part_end_idx = graph->num_of_data_points - graph->num_of_data_points_to_scroll;
			while (idx != end_idx)
			{
				graph->y[graph_old_part_end_idx + graph_sample_idx] = neuron_dynamics_buffer->buffer[idx].network_neuron_dyn[layer][neuron_grp][neuron_num].dyn[dynamics_type];
				graph_sample_idx++;
				if ((idx + 1) == buffer_size)
					idx = 0;
				else
					idx++;
			}
			while (graph->trial_status_event_buffer_read_idx != graph->trial_status_events->buff_write_idx)
			{
				markers = graph->status_markers->markers;
				status_event_item = &(graph->trial_status_events->buff[graph->trial_status_event_buffer_read_idx]);
				graph_len_to_scroll = graph->graph_len_to_scroll;
				graph_len = graph->graph_len;
				switch (status_event_item->trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:    // Do nothing
						break;  		
					case TRIAL_STATUS_IN_TRIAL:	
						markers[STATUS_MARKER_GREEN].x[0] = (status_event_item->status_start_time - (new_part_start_time + graph_len_to_scroll - graph_len)) / 1000000.0;   // find beginning of graph and put marker
						markers[STATUS_MARKER_GREEN].x[1] = markers[STATUS_MARKER_GREEN].x[0];
						break;
					case TRIAL_STATUS_IN_REFRACTORY:	
						markers[STATUS_MARKER_RED].x[0] = (status_event_item->status_start_time - (new_part_start_time + graph_len_to_scroll - graph_len)) / 1000000.0;   // find beginning of graph and put marker
						markers[STATUS_MARKER_RED].x[1] = markers[STATUS_MARKER_RED].x[0];
						break;
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:  
						markers[STATUS_MARKER_YELLOW].x[0] = (status_event_item->status_start_time - (new_part_start_time + graph_len_to_scroll - graph_len)) / 1000000.0;   // find beginning of graph and put marker
						markers[STATUS_MARKER_YELLOW].x[1] = markers[STATUS_MARKER_YELLOW].x[0];
						break;		
					default:
						return print_message(ERROR_MSG ,"IzNeuronSimulators", "CurrentPatternGraph", "handle_current_pattern_graph_scrolling_and_plotting", "Unknown trial_status.");
				}
				graph->trial_status_event_buffer_read_idx++;
				if (graph->trial_status_event_buffer_read_idx == graph->trial_status_events->buffer_size)
					graph->trial_status_event_buffer_read_idx = 0;
			}
			set_total_limits_neuron_dynamics_graph_scroll(graph);
			graph->scroll_request = TRUE;
			graph->new_part_start_time = new_part_end_time; // prepare for next acquisition
			graph->new_part_start_idx = end_idx; // prepare for next acquisition
		}

	}
	return TRUE;	
}

bool scroll_neuron_dynamics_graph(NeuronDynamicsGraphScroll *graph)
{
	unsigned int i;
	unsigned int scroll = graph->num_of_data_points_to_scroll; 
	unsigned int end_idx = graph->num_of_data_points;
	StatusMarker *markers = graph->status_markers->markers;
	unsigned int num_of_markers = graph->status_markers->num_of_markers;
	float graph_len_to_scroll = graph->graph_len_to_scroll / 1000000.0;

	for (i = scroll; i < end_idx; i++)
		graph->y[i-scroll] = graph->y[i];

	for (i = 0; i < num_of_markers; i ++)
	{
		if (markers[i].x[0] >= -100) // to ensure push to out of graph
		{
			markers[i].x[0] = markers[i].x[0] - graph_len_to_scroll;
			markers[i].x[1] = markers[i].x[0];
		}
	}	
	graph->scroll_request = FALSE;
	return TRUE;
}

bool submit_neuron_dynamics_graph_neuron_and_dynamics_type(NeuronDynamicsGraphScroll *graph, unsigned int layer, unsigned int neuron_grp, unsigned int neuron_num, int dynamics_type)
{
	pthread_mutex_lock(&(graph->mutex));
	graph->active_layer = layer;
	graph->active_neuron_group = neuron_grp;
	graph->active_neuron = neuron_num;
	graph->active_dynamics_type = dynamics_type;
	pthread_mutex_unlock(&(graph->mutex));
	return TRUE;
}

bool set_total_limits_neuron_dynamics_graph_scroll(NeuronDynamicsGraphScroll *graph)
{
	float max_y = 0, min_y = 0;
	float *y = graph->y;
	unsigned int i;
	unsigned int num_of_data_points = graph->num_of_data_points;
	StatusMarker *markers = graph->status_markers->markers;
	unsigned int num_of_markers = graph->status_markers->num_of_markers;

	for (i = 0; i < num_of_data_points; i++)
	{
		if (y[i] > max_y)
			max_y = y[i];
		if (y[i] < min_y)
			min_y = y[i];
	}
	if (max_y < 100)
		max_y = 100;
	else if (max_y < 200)
		max_y = 200;
	else if (max_y < 300)
		max_y = 300;
	else if (max_y < 400)
		max_y = 400;
	else 
		max_y = max_y+100;

	if (min_y > 0)
		min_y = 0;
	else if (min_y > -100)
		min_y = -100;
	else if (min_y > -200)
		min_y = -200;
	else if (min_y > -300)
		min_y = -300;
	else
		min_y = min_y-100;

	for (i = 0; i < num_of_markers; i ++)
	{
		markers[i].y[0] = min_y;
		markers[i].y[1] = max_y;
	}

	gtk_databox_set_total_limits (GTK_DATABOX (graph->databox), 0 , (graph->graph_len/1000000) , max_y, min_y);

	return TRUE;	
}

bool clear_neuron_dynamics_graph_w_scroll(NeuronDynamicsGraphScroll *graph)
{
	unsigned int i;
	unsigned int num_of_data_points = graph->num_of_data_points;
	StatusMarker *markers = graph->status_markers->markers;
	unsigned int num_of_markers = graph->status_markers->num_of_markers;

	float *y = graph->y;
	for (i = 0; i < num_of_data_points; i++)
		y[i] = 0;
	for (i = 0; i < num_of_markers; i ++)
	{
		markers[i].x[0] = -100;   // to ensure push to out of graph
		markers[i].x[1] = -100;
	}
	set_total_limits_neuron_dynamics_graph_scroll(graph);
	return TRUE;
}


NeuronDynamicsGraphScrollLimited* allocate_neuron_dynamics_graph_scroll_limited(GtkWidget *hbox, NeuronDynamicsGraphScrollLimited *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, unsigned int num_of_data_points_to_scroll, TimeStamp buffer_followup_latency, unsigned int num_of_markers, TrialStatusEvents *trial_status_events, NeuronDynamicsBufferLimited* limited_dynamics_buffer, unsigned int limited_dynamics_buffer_list_idx)
{
	unsigned int i;
	if (graph != NULL)
	{
		print_message(ERROR_MSG ,"IzNeuronSimulators", "NeuronDynamicsGraph", "allocate_neuron_dynamics_graph_scroll_limited", "graph != NULL");	
		return graph;
	}
	
	graph = g_new0(NeuronDynamicsGraphScrollLimited,1);
	graph->locally_paused = TRUE;
	graph->num_of_data_points = num_of_data_points;
	graph->sampling_interval = sampling_interval;
	graph->graph_len = sampling_interval*num_of_data_points;
	GdkColor color_bg;
	GdkColor color_line;
	GdkColor color_grid;
	GdkColor color_status_marker;

	color_bg.red = 0;
	color_bg.green = 0;
	color_bg.blue = 0;
	
	color_grid.red = 25000;
	color_grid.green = 25000;
	color_grid.blue = 25000;
	
	color_line.red = 65535;
	color_line.green = 65535;
	color_line.blue = 65535;

	graph->databox = gtk_databox_new ();	
	gtk_box_pack_start (GTK_BOX (hbox), graph->databox, TRUE, TRUE, 0);
  	gtk_widget_modify_bg (graph->databox, GTK_STATE_NORMAL, &color_bg);
 	gtk_databox_graph_add (GTK_DATABOX (graph->databox), gtk_databox_grid_new (9, 9, &color_grid, 0)); 	

	graph->x = g_new0(float, num_of_data_points);  
	graph->y = g_new0(float, num_of_data_points);

	for (i = 0; i < num_of_data_points; i++)
		graph->x[i] = i*(sampling_interval/1000000.0);
	graph->graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (num_of_data_points, graph->x, graph->y, &color_line, 0));
	gtk_databox_graph_add (GTK_DATABOX (graph->databox), graph->graph);	

	graph->num_of_data_points_to_scroll = num_of_data_points_to_scroll;
	graph->graph_len_to_scroll = sampling_interval*num_of_data_points_to_scroll;
	graph->buffer_followup_latency = buffer_followup_latency;

	graph->status_markers = g_new0(StatusMarkers,1);
	graph->status_markers->markers = g_new0(StatusMarker, num_of_markers);
	graph->status_markers->num_of_markers = num_of_markers;

	for (i = 0; i < num_of_markers; i++)
	{
		if (! get_status_marker_color(&color_status_marker, i))
			return (NeuronDynamicsGraphScrollLimited*)print_message(ERROR_MSG ,"IzNeuronSimulators", "NeuronDynamicsGraph", "allocate_neuron_dynamics_graph_scroll_limited", "! get_status_marker_color().");
 		gtk_databox_graph_add (GTK_DATABOX (graph->databox), gtk_databox_lines_new (2, graph->status_markers->markers[i].x, graph->status_markers->markers[i].y, &color_status_marker, 1)); 			
	}
	graph->trial_status_events = trial_status_events;
	graph->limited_dynamics_buffer = limited_dynamics_buffer;
	graph->limited_dynamics_buffer_list_idx = limited_dynamics_buffer_list_idx;
	gtk_widget_show_all(hbox);	
	gtk_databox_set_total_limits (GTK_DATABOX (graph->databox), 0, (graph->graph_len/1000000), 200, -200);	
	return graph;				
}

bool determine_neuron_dynamics_graph_scroll_limited_start_indexes(NeuronDynamicsGraphScrollLimited *graph, TimeStamp last_sample_time, unsigned int neuron_dynamics_limited_buffer_write_idx, unsigned int neuron_dynamics_limited_buffer_size)
{
	unsigned int last_sample_times_idx;

	if (neuron_dynamics_limited_buffer_write_idx == 0)
		last_sample_times_idx = neuron_dynamics_limited_buffer_size - 1;
	else
		last_sample_times_idx = neuron_dynamics_limited_buffer_write_idx -1;

	graph->new_part_start_time = last_sample_time;	// last sample time (of the snn simulation) might be higher than current periodic system time (rt_tasks_data->current_system_time). It might lead to a little jitter in plot in orders of a few millisecond 
	graph->new_part_start_idx = last_sample_times_idx;
	graph->trial_status_event_buffer_read_idx = graph->trial_status_events->buff_write_idx;
	return TRUE;
}

bool handle_limited_neuron_dynamics_graph_scrolling_and_plotting(NeuronDynamicsGraphScrollLimited *graph, TimeStamp current_system_time)
{
	TimeStamp new_part_start_time;
	TimeStamp new_part_end_time;
	TimeStamp graph_len_to_scroll;
	TimeStamp graph_len;
	unsigned int idx, end_idx, graph_old_part_end_idx, graph_sample_idx = 0; 
	unsigned int selection_list_idx;
	unsigned int buffer_size;
	StatusMarker *markers;
	TrialStatusEventItem *status_event_item;	
	NeuronDynamicsBufferLimited* limited_dynamics_buffer = graph->limited_dynamics_buffer;
	if ((graph->global_pause_request) && (graph->scroll_request))
	{
		graph->global_pause_request = FALSE;
		graph->globally_paused = TRUE;
	}
	if ((graph->local_pause_request) && (graph->scroll_request))
	{
		graph->local_pause_request = FALSE;
		graph->locally_paused = TRUE;
	}
	if ((!graph->locally_paused) && (!graph->globally_paused))
	{
		if (graph->scroll_request)   // it is necessary otherwise set_total_limits cannot display slided and clear graph part. 
			scroll_limited_neuron_dynamics_graph(graph);
		new_part_start_time = graph->new_part_start_time;
		new_part_end_time = new_part_start_time + graph->graph_len_to_scroll;


		if (current_system_time > (new_part_end_time + graph->buffer_followup_latency) )
		{
			selection_list_idx = graph->limited_dynamics_buffer_list_idx;
			buffer_size = limited_dynamics_buffer->selected_dyns[selection_list_idx].buffer_size;
			idx =  graph->new_part_start_idx;
			if ((idx + graph->num_of_data_points_to_scroll) > buffer_size)
				end_idx = idx + graph->num_of_data_points_to_scroll - buffer_size;
			else
				end_idx = idx + graph->num_of_data_points_to_scroll;
			graph_old_part_end_idx = graph->num_of_data_points - graph->num_of_data_points_to_scroll;
			while (idx != end_idx)
			{
				graph->y[graph_old_part_end_idx + graph_sample_idx] = limited_dynamics_buffer->selected_dyns[selection_list_idx].neuron_dynamic[idx];
				graph_sample_idx++;
				if ((idx + 1) == buffer_size)
					idx = 0;
				else
					idx++;
			}
			while (graph->trial_status_event_buffer_read_idx != graph->trial_status_events->buff_write_idx)
			{
				markers = graph->status_markers->markers;
				status_event_item = &(graph->trial_status_events->buff[graph->trial_status_event_buffer_read_idx]);
				graph_len_to_scroll = graph->graph_len_to_scroll;
				graph_len = graph->graph_len;
				switch (status_event_item->trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:    // Do nothing
						break;  		
					case TRIAL_STATUS_IN_TRIAL:	
						markers[STATUS_MARKER_GREEN].x[0] = (status_event_item->status_start_time - (new_part_start_time + graph_len_to_scroll - graph_len)) / 1000000.0;   // find beginning of graph and put marker
						markers[STATUS_MARKER_GREEN].x[1] = markers[STATUS_MARKER_GREEN].x[0];
						break;
					case TRIAL_STATUS_IN_REFRACTORY:	
						markers[STATUS_MARKER_RED].x[0] = (status_event_item->status_start_time - (new_part_start_time + graph_len_to_scroll - graph_len)) / 1000000.0;   // find beginning of graph and put marker
						markers[STATUS_MARKER_RED].x[1] = markers[STATUS_MARKER_RED].x[0];
						break;
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:  
						markers[STATUS_MARKER_YELLOW].x[0] = (status_event_item->status_start_time - (new_part_start_time + graph_len_to_scroll - graph_len)) / 1000000.0;   // find beginning of graph and put marker
						markers[STATUS_MARKER_YELLOW].x[1] = markers[STATUS_MARKER_YELLOW].x[0];
						break;		
					default:
						return print_message(ERROR_MSG ,"IzNeuronSimulators", "CurrentPatternGraph", "handle_current_pattern_graph_scrolling_and_plotting", "Unknown trial_status.");
				}
				graph->trial_status_event_buffer_read_idx++;
				if (graph->trial_status_event_buffer_read_idx == graph->trial_status_events->buffer_size)
					graph->trial_status_event_buffer_read_idx = 0;
			}
			set_total_limits_limited_neuron_dynamics_graph_scroll(graph);
			graph->scroll_request = TRUE;
			graph->new_part_start_time = new_part_end_time; // prepare for next acquisition
			graph->new_part_start_idx = end_idx; // prepare for next acquisition
		}

	}
	return TRUE;	
}

bool scroll_limited_neuron_dynamics_graph(NeuronDynamicsGraphScrollLimited *graph)
{
	unsigned int i;
	unsigned int scroll = graph->num_of_data_points_to_scroll; 
	unsigned int end_idx = graph->num_of_data_points;
	StatusMarker *markers = graph->status_markers->markers;
	unsigned int num_of_markers = graph->status_markers->num_of_markers;
	float graph_len_to_scroll = graph->graph_len_to_scroll / 1000000.0;

	for (i = scroll; i < end_idx; i++)
		graph->y[i-scroll] = graph->y[i];

	for (i = 0; i < num_of_markers; i ++)
	{
		if (markers[i].x[0] >= -100) // to ensure push to out of graph
		{
			markers[i].x[0] = markers[i].x[0] - graph_len_to_scroll;
			markers[i].x[1] = markers[i].x[0];
		}
	}	
	graph->scroll_request = FALSE;
	return TRUE;
}

bool set_total_limits_limited_neuron_dynamics_graph_scroll(NeuronDynamicsGraphScrollLimited *graph)
{
	float max_y = 0, min_y = 0;
	float *y = graph->y;
	unsigned int i;
	unsigned int num_of_data_points = graph->num_of_data_points;
	StatusMarker *markers = graph->status_markers->markers;
	unsigned int num_of_markers = graph->status_markers->num_of_markers;

	for (i = 0; i < num_of_data_points; i++)
	{
		if (y[i] > max_y)
			max_y = y[i];
		if (y[i] < min_y)
			min_y = y[i];
	}
	if (max_y < 100)
		max_y = 100;
	else if (max_y < 200)
		max_y = 200;
	else if (max_y < 300)
		max_y = 300;
	else if (max_y < 400)
		max_y = 400;
	else 
		max_y = max_y+100;

	if (min_y > 0)
		min_y = 0;
	else if (min_y > -100)
		min_y = -100;
	else if (min_y > -200)
		min_y = -200;
	else if (min_y > -300)
		min_y = -300;
	else
		min_y = min_y-100;

	for (i = 0; i < num_of_markers; i ++)
	{
		markers[i].y[0] = min_y;
		markers[i].y[1] = max_y;
	}

	gtk_databox_set_total_limits (GTK_DATABOX (graph->databox), 0 , (graph->graph_len/1000000) , max_y, min_y);

	return TRUE;
}

bool clear_limited_neuron_dynamics_graph_w_scroll(NeuronDynamicsGraphScrollLimited *graph)
{
	unsigned int i;
	unsigned int num_of_data_points = graph->num_of_data_points;
	StatusMarker *markers = graph->status_markers->markers;
	unsigned int num_of_markers = graph->status_markers->num_of_markers;

	float *y = graph->y;
	for (i = 0; i < num_of_data_points; i++)
		y[i] = 0;
	for (i = 0; i < num_of_markers; i ++)
	{
		markers[i].x[0] = -100;   // to ensure push to out of graph
		markers[i].x[1] = -100;
	}
	set_total_limits_limited_neuron_dynamics_graph_scroll(graph);
	return TRUE;
}
