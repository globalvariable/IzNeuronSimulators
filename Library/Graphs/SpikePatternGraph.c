#include "SpikePatternGraph.h"



NetworkSpikePatternGraphScroll* allocate_network_spike_pattern_graph_scroll(Network* network, GtkWidget *hbox, NetworkSpikePatternGraphScroll *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, int graph_height, unsigned int num_of_data_points_to_scroll, TimeStamp spike_buffer_followup_latency, SpikeData **source_spike_data_to_plot, unsigned int num_of_markers, TrialStatusEvents *trial_status_events, unsigned int	num_of_spike_data_buffer)
{
	GdkColor color_bg;
	GdkColor color_line;
	GdkColor color_status_marker;
	GtkWidget *vbox, *vbox1;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	unsigned int i, j, k, m;
	unsigned int num_of_all_neurons_in_network;
	if (graph != NULL)
	{
		print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikePatternGraph", "allocate_network_spike_pattern_graph", "graph != NULL");	
		return graph;
	}
	if (spike_buffer_followup_latency < 100000000)
		return (NetworkSpikePatternGraphScroll*)print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikePatternGraph", "allocate_network_spike_pattern_graph", "spike_buffer_followup_latency < 100 ms.");	
	if (source_spike_data_to_plot == NULL)
		return (NetworkSpikePatternGraphScroll*)print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikePatternGraph", "allocate_network_spike_pattern_graph", "source_spike_data_to_plot == NULL.");	
	graph = g_new0(NetworkSpikePatternGraphScroll,1);	
	graph->locally_paused = TRUE;
	graph->num_of_data_points = num_of_data_points;
 	graph->sampling_interval = sampling_interval;
	graph->graph_len = sampling_interval*num_of_data_points;
	graph->graph_len_ms = graph->graph_len/1000000;
	graph->num_of_data_points_to_scroll = num_of_data_points_to_scroll;
	graph->data_point_placement_start_idx = num_of_data_points - num_of_data_points_to_scroll;
	graph->graph_len_to_scroll = sampling_interval*num_of_data_points_to_scroll;
	graph->spike_buffer_followup_latency = spike_buffer_followup_latency;
	graph->source_spike_data_to_plot = source_spike_data_to_plot;
	graph->num_of_source_spike_data_to_plot = num_of_spike_data_buffer;
	graph->spike_handling_buffer = allocate_spike_data(graph->spike_handling_buffer, (unsigned int)(get_num_of_neurons_in_network(network)*(spike_buffer_followup_latency/1000000000.0)*500) ); // to buffer spikes appear during spike_buffer_followup_latency, assuming neuron firing rate cannot be more than 500 Hz 

	color_bg.red = 0;
	color_bg.green = 0;
	color_bg.blue = 0;
	
	color_line.red = 65535;
	color_line.green = 65535;
	color_line.blue = 65535;


	vbox = gtk_vbox_new(FALSE, 0);
     	gtk_box_pack_start(GTK_BOX(hbox),vbox, TRUE,TRUE,0);

	graph->status_markers = g_new0(StatusMarkers,1);
	graph->status_markers->markers = g_new0(StatusMarker, num_of_markers);
	graph->status_markers->num_of_markers = num_of_markers;
	for (i = 0; i < num_of_markers; i++)
	{
		graph->status_markers->markers[i].y[0] = 0;
		graph->status_markers->markers[i].y[1] = 1;
		graph->status_markers->markers[i].x[0] = -100;
		graph->status_markers->markers[i].x[1] = -100;
	}

	num_of_all_neurons_in_network = get_num_of_neurons_in_network(network);
	get_num_of_layers_in_network(network, &num_of_layers);
	graph->neuron_graphs = g_new0(NeuronSpikePatternGraphScroll **,num_of_layers);	
	for (i = 0; i < num_of_layers; i++)
	{
		get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
		graph->neuron_graphs[i] = g_new0(NeuronSpikePatternGraphScroll *,num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{		
			get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
			vbox1 = gtk_vbox_new(FALSE, 0);
     			gtk_box_pack_start(GTK_BOX(vbox),vbox1, FALSE,FALSE,1);

			gtk_widget_set_size_request(vbox1, 50, (int) ((((double)num_of_neurons_in_neuron_group) / num_of_all_neurons_in_network)* graph_height) - 5) ;// scale graph height  // minus 5 (thickness of gap between nrn_group graph)

			graph->neuron_graphs[i][j] = g_new0(NeuronSpikePatternGraphScroll , num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				graph->neuron_graphs[i][j][k].databox = gtk_databox_new ();
				gtk_box_pack_start (GTK_BOX (vbox1), graph->neuron_graphs[i][j][k].databox, TRUE, TRUE, 0);
  				gtk_widget_modify_bg (graph->neuron_graphs[i][j][k].databox, GTK_STATE_NORMAL, &color_bg);
				graph->neuron_graphs[i][j][k].x = g_new0(float, num_of_data_points);  
				graph->neuron_graphs[i][j][k].y = g_new0(float, num_of_data_points);
				for (m = 0; m < num_of_data_points; m++)
					graph->neuron_graphs[i][j][k].x[m] = (m*sampling_interval)/1000000;
				graph->neuron_graphs[i][j][k].graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (num_of_data_points, graph->neuron_graphs[i][j][k].x, graph->neuron_graphs[i][j][k].y, &color_line, 0));
				gtk_databox_graph_add (GTK_DATABOX (graph->neuron_graphs[i][j][k].databox), graph->neuron_graphs[i][j][k].graph);	
				for (m = 0; m < num_of_markers; m++)
				{
					if (! get_status_marker_color(&color_status_marker, m))
						return (NetworkSpikePatternGraphScroll*)print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikePatternGraph", "allocate_network_spike_pattern_graph_scroll", "! get_status_marker_color().");
			 		gtk_databox_graph_add (GTK_DATABOX (graph->neuron_graphs[i][j][k].databox), gtk_databox_lines_new (2, graph->status_markers->markers[m].x, graph->status_markers->markers[m].y, &color_status_marker, 1)); 			
				}
			}
		}
	}
	graph->trial_status_events = trial_status_events;
	gtk_widget_show_all(hbox);	
	set_total_limits_network_spike_pattern_graph(network, graph);
	return graph;						
}

bool determine_spike_pattern_graph_scroll_start_time_and_read_indexes(NetworkSpikePatternGraphScroll *graph, TimeStamp current_system_time)
{
	unsigned int i;
	graph->new_part_start_time = current_system_time;
	for (i = 0; i < graph->num_of_source_spike_data_to_plot; i++)
		reset_spike_data_read_idx(graph->source_spike_data_to_plot[i]);
	reset_spike_data_read_idx(graph->spike_handling_buffer);
	graph->trial_status_event_buffer_read_idx = graph->trial_status_events->buff_write_idx;
	return TRUE;
}

bool handle_spike_pattern_graph_scrolling_and_plotting(NetworkSpikePatternGraphScroll *graph, Network *network, TimeStamp current_system_time)
{
	TimeStamp 				new_part_start_time;
	TimeStamp 				new_part_end_time;
	TimeStamp 				spike_time;
	TimeStamp				sampling_interval;
	TimeStamp				graph_len_to_scroll;
	TimeStamp				graph_len;
	unsigned int				data_point_placement_start_idx;
	SpikeData				**source_spike_data_to_plot;
	SpikeData				*spike_handling_buffer;
	SpikeTimeStampItem		*spike_handling_buffer_buff;	
	unsigned int				spike_handling_buffer_size;
	SpikeTimeStampItem		spike_item;
	NeuronSpikePatternGraphScroll	***neuron_graphs;
	StatusMarker 				*markers;
	TrialStatusEventItem		*status_event_item;
	float					status_marker_x;
	unsigned int				idx, end_idx, i;
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
		new_part_start_time = graph->new_part_start_time;
		new_part_end_time = new_part_start_time + graph->graph_len_to_scroll;
		sampling_interval =  graph->sampling_interval;
		data_point_placement_start_idx = graph->data_point_placement_start_idx;
		spike_handling_buffer = graph->spike_handling_buffer;
		spike_handling_buffer_buff = spike_handling_buffer->buff;
		spike_handling_buffer_size = spike_handling_buffer->buffer_size;
		source_spike_data_to_plot = graph->source_spike_data_to_plot;
		neuron_graphs =  graph->neuron_graphs;
		if (graph->scroll_request)   // it is necessary otherwise set_total_limits cannot display slided and clear graph part. 
			scroll_network_spike_pattern_graph(network, graph);
		if (current_system_time > (new_part_end_time + graph->spike_buffer_followup_latency) )
		{
			// first handle graph' s spike buffer
			idx = spike_handling_buffer->buff_idx_read;
			end_idx = spike_handling_buffer->buff_idx_write;
			while (idx != end_idx)   // do not use get_next_spike_data_item(spike_handling_buffer, &spike_item) since write_to_spike_data(spike_handling_buffer) in while loop writes in this buffer as well. So there will be an infinite loop.
			{
				get_spike_data_item_by_idx(spike_handling_buffer, idx, &spike_item);
				spike_time = spike_item.peak_time;
				if (spike_time < new_part_start_time)
				{
					printf("1 sp time: %llu\n", spike_time);
					print_message(WARNING_MSG ,"IzNeuronSimulators", "SpikePatternGraph", "handle_spike_pattern_graph_scrolling_and_plotting", "spike_time < new_part_start_time (Only normal at plotting resumed).");
					idx++;
					if (idx == spike_handling_buffer_size)
						idx = 0;
					continue;				
				}
				if (spike_time < new_part_end_time)
					neuron_graphs[spike_item.mwa_or_layer][spike_item.channel_or_neuron_group][spike_item.unit_or_neuron].y[((spike_time - new_part_start_time) / sampling_interval) + data_point_placement_start_idx] = 1;
				else
					write_to_spike_data(spike_handling_buffer, spike_item.mwa_or_layer, spike_item.channel_or_neuron_group, spike_item.unit_or_neuron, spike_time);
				idx++;
				if (idx == spike_handling_buffer_size)
					idx = 0;
			}
			spike_handling_buffer->buff_idx_read = end_idx;
			// second handle graph' s source buffer	
			for (i = 0; i < graph->num_of_source_spike_data_to_plot; i++)
			{
				while (get_next_spike_data_item(source_spike_data_to_plot[i], &spike_item))
				{
					spike_time = spike_item.peak_time;
					if (spike_time < new_part_start_time)
					{
						printf("2 sp time: %llu\n", spike_time);
						print_message(WARNING_MSG ,"IzNeuronSimulators", "SpikePatternGraph", "handle_spike_pattern_graph_scrolling_and_plotting", "spike_time < new_part_start_time (Only normal at plotting resumed).");
						continue;		
					}
					if (spike_time < new_part_end_time)
						neuron_graphs[spike_item.mwa_or_layer][spike_item.channel_or_neuron_group][spike_item.unit_or_neuron].y[((spike_time - new_part_start_time) / sampling_interval) + data_point_placement_start_idx] = 1;
					else
						write_to_spike_data(spike_handling_buffer, spike_item.mwa_or_layer, spike_item.channel_or_neuron_group, spike_item.unit_or_neuron, spike_time);	
				}
			}
			while (graph->trial_status_event_buffer_read_idx != graph->trial_status_events->buff_write_idx)
			{
				status_event_item = &(graph->trial_status_events->buff[graph->trial_status_event_buffer_read_idx]);
				graph_len_to_scroll = graph->graph_len_to_scroll;
				graph_len = graph->graph_len;
				status_marker_x = (status_event_item->status_start_time - (new_part_start_time + graph_len_to_scroll - graph_len)) / 1000000.0;   // find beginning of graph and put marker
				markers = graph->status_markers->markers;
				switch (status_event_item->trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:    // Do nothing
						break;  		
					case TRIAL_STATUS_IN_TRIAL:	
						markers[STATUS_MARKER_GREEN].x[0] = status_marker_x;   
						markers[STATUS_MARKER_GREEN].x[1] = status_marker_x;
						break;
					case TRIAL_STATUS_IN_REFRACTORY:	
						markers[STATUS_MARKER_RED].x[0] = status_marker_x;   
						markers[STATUS_MARKER_RED].x[1] = status_marker_x;
						break;
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:  
						markers[STATUS_MARKER_YELLOW].x[0] = status_marker_x;   
						markers[STATUS_MARKER_YELLOW].x[1] = status_marker_x;
						break;
					default:
						return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikePatternGraph", "handle_spike_pattern_graph_scrolling_and_plotting", "Unknown trial_status.");
				}
				graph->trial_status_event_buffer_read_idx++;
				if (graph->trial_status_event_buffer_read_idx == graph->trial_status_events->buffer_size)
					graph->trial_status_event_buffer_read_idx = 0;
			}
			// plot and send scroll request to get prepared for next plotting.
			set_total_limits_network_spike_pattern_graph(network, graph);
			graph->scroll_request = TRUE;
			graph->new_part_start_time = new_part_end_time; // prepare for next acquisition
		}
		else
		{
			// first handle graph' s spike buffer
			idx = spike_handling_buffer->buff_idx_read;
			end_idx = spike_handling_buffer->buff_idx_write;
			while (idx != end_idx)   // do not use get_next_spike_data_item(spike_handling_buffer, &spike_item) since write_to_spike_data(spike_handling_buffer) in while loop writes in this buffer as well. So there will be an infinite loop.
			{
				get_spike_data_item_by_idx(spike_handling_buffer, idx, &spike_item);
				spike_time = spike_item.peak_time;
				if (spike_time < new_part_start_time)
				{
					printf("3 sp time: %llu\n", spike_time);
					print_message(WARNING_MSG ,"IzNeuronSimulators", "SpikePatternGraph", "handle_spike_pattern_graph_scrolling_and_plotting", "spike_time < new_part_start_time (Only normal at plotting resumed).");
					idx++;
					if (idx == spike_handling_buffer_size)
						idx = 0;
					continue;		
				}
				if (spike_time < new_part_end_time)
					neuron_graphs[spike_item.mwa_or_layer][spike_item.channel_or_neuron_group][spike_item.unit_or_neuron].y[((spike_time - new_part_start_time) / sampling_interval) + data_point_placement_start_idx] = 1;
				else
					write_to_spike_data(spike_handling_buffer, spike_item.mwa_or_layer, spike_item.channel_or_neuron_group, spike_item.unit_or_neuron, spike_time);
				idx++;
				if (idx == spike_handling_buffer_size)
					idx = 0;
			}
			spike_handling_buffer->buff_idx_read = end_idx;
			// second handle graph' s source buffer	
			for (i = 0; i < graph->num_of_source_spike_data_to_plot; i++)
			{			
				while (get_next_spike_data_item(source_spike_data_to_plot[i], &spike_item))
				{
					spike_time = spike_item.peak_time;
					if (spike_time < new_part_start_time)
					{
						printf("4 sp time: %llu\n", spike_time);
						print_message(WARNING_MSG ,"IzNeuronSimulators", "SpikePatternGraph", "handle_spike_pattern_graph_scrolling_and_plotting", "spike_time < new_part_start_time (Only normal at plotting resumed).");
						continue;				
					}
					if (spike_time < new_part_end_time)
						neuron_graphs[spike_item.mwa_or_layer][spike_item.channel_or_neuron_group][spike_item.unit_or_neuron].y[((spike_time - new_part_start_time) / sampling_interval) + data_point_placement_start_idx] = 1;
					else
						write_to_spike_data(spike_handling_buffer, spike_item.mwa_or_layer, spike_item.channel_or_neuron_group, spike_item.unit_or_neuron, spike_time);	
				}
			}
		}	
			
	}
	return TRUE;
}

bool scroll_network_spike_pattern_graph(Network* network, NetworkSpikePatternGraphScroll *graph)
{
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	unsigned int i, j, k, m;
	unsigned int scroll = graph->num_of_data_points_to_scroll; 
	unsigned int end_idx = graph->num_of_data_points;
	unsigned int clear_start_idx = graph->data_point_placement_start_idx;
	float *y;
	StatusMarker *markers = graph->status_markers->markers;
	unsigned int num_of_markers = graph->status_markers->num_of_markers;
	float graph_len_to_scroll = graph->graph_len_to_scroll / 1000000.0;
	get_num_of_layers_in_network(network, &num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{
		get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{		
			get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				y = graph->neuron_graphs[i][j][k].y;
				for (m = scroll; m < end_idx; m++)
					y[m-scroll] = y[m];	// instead of sliding in a straigthforward manner, think of making a list which holds the data points having spikes. the size of the list would be num_of_data_points and num_of_spikes would determine the used part of the list by spikes.
				for (m = clear_start_idx ; m < end_idx; m++)
					y[m] = 0;		
			}
		}
	}
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

bool set_total_limits_network_spike_pattern_graph(Network* network, NetworkSpikePatternGraphScroll *graph)
{
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	unsigned int i, j, k;
	NeuronSpikePatternGraphScroll ***neuron_graphs = graph->neuron_graphs;
	unsigned int 	graph_len_ms = graph->graph_len_ms;
	get_num_of_layers_in_network(network, &num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{
		get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{		
			get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				gtk_databox_set_total_limits (GTK_DATABOX (neuron_graphs[i][j][k].databox), 0.0, graph_len_ms, 1.0, 0.9);	
			}
		}
	}
	return TRUE;
}

bool clear_network_spike_pattern_graph_w_scroll(Network* network, NetworkSpikePatternGraphScroll *graph)
{
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	unsigned int i, j, k, m;
	unsigned int end_idx = graph->num_of_data_points;
	unsigned int 	graph_len_ms = graph->graph_len_ms;
	NeuronSpikePatternGraphScroll	***neuron_graphs = graph->neuron_graphs;
	StatusMarker *markers = graph->status_markers->markers;
	unsigned int num_of_markers = graph->status_markers->num_of_markers;
	for (i = 0; i < num_of_markers; i ++)
	{
		markers[i].x[0] = -100; // to ensure push to out of graph
		markers[i].x[1] = -100;
	}
	get_num_of_layers_in_network(network, &num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{
		get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{		
			get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				for (m = 0; m < end_idx; m++)
					neuron_graphs[i][j][k].y[m] = 0;	// instead of sliding in a straigthforward manner, think of making a list which holds the data points having spikes. the size of the list would be num_of_data_points and num_of_spikes would determine the used part of the list by spikes.
				gtk_databox_set_total_limits (GTK_DATABOX (neuron_graphs[i][j][k].databox), 0.0, graph_len_ms, 1.0, 0.9);	
			}
		}
	}

	return TRUE;
}
