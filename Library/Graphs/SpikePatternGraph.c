#include "SpikePatternGraph.h"



NetworkSpikePatternGraphScroll* allocate_network_spike_pattern_graph_scroll(Network* network, GtkWidget *hbox, NetworkSpikePatternGraphScroll *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, int graph_height, unsigned int num_of_data_points_to_scroll, TimeStamp spike_buffer_followup_latency, SpikeData *source_spike_data_to_plot)
{
	GdkColor color_bg;
	GdkColor color_line;
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
	graph->paused = TRUE;
	graph->num_of_data_points = num_of_data_points;
 	graph->sampling_interval = sampling_interval;
	graph->graph_len = sampling_interval*num_of_data_points;
	graph->graph_len_ms = graph->graph_len/1000000;
	graph->num_of_data_points_to_scroll = num_of_data_points_to_scroll;
	graph->data_point_placement_start_idx = num_of_data_points - num_of_data_points_to_scroll;
	graph->graph_len_to_scroll = sampling_interval*num_of_data_points_to_scroll;
	graph->spike_buffer_followup_latency = spike_buffer_followup_latency;
	graph->source_spike_data_to_plot = source_spike_data_to_plot;
	graph->spike_handling_buffer = allocate_spike_data(graph->spike_handling_buffer, (unsigned int)(get_num_of_neurons_in_network(network)*(spike_buffer_followup_latency/1000000000.0)*500) ); // to buffer spikes appear during spike_buffer_followup_latency, assuming neuron firing rate cannot be more than 500 Hz 

	color_bg.red = 0;
	color_bg.green = 0;
	color_bg.blue = 0;
	
	color_line.red = 65535;
	color_line.green = 65535;
	color_line.blue = 65535;


	vbox = gtk_vbox_new(FALSE, 0);
     	gtk_box_pack_start(GTK_BOX(hbox),vbox, TRUE,TRUE,0);

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
				gtk_databox_set_total_limits (GTK_DATABOX (graph->neuron_graphs[i][j][k].databox), 0.0, (graph->graph_len_ms), 1.0, 0.9);	
			}
		}
	}
	gtk_widget_show_all(hbox);	

	return graph;						
}

bool determine_spike_pattern_graph_scroll_start_time_and_read_indexes(NetworkSpikePatternGraphScroll *graph, TimeStamp current_system_time)
{
	graph->new_part_start_time = current_system_time;
	graph->source_spike_data_buffer_read_idx = graph->source_spike_data_to_plot->buff_idx_write;
	graph->spike_handling_buffer_read_idx = graph->spike_handling_buffer->buff_idx_write;
	return TRUE;
}

bool handle_spike_pattern_graph_scrolling_and_plotting(NetworkSpikePatternGraphScroll *graph, Network *network, TimeStamp current_system_time)
{
	TimeStamp 				new_part_start_time;
	TimeStamp 				new_part_end_time;
	TimeStamp 				spike_time;
	TimeStamp				sampling_interval;
	unsigned int				idx, end_idx;
	unsigned int				data_point_placement_start_idx;
	SpikeData				*source_spike_data_to_plot;
	SpikeData				*spike_handling_buffer;
	SpikeTimeStampItem		*spike_handling_buffer_buff;	
	unsigned int				spike_handling_buffer_size;
	SpikeTimeStampItem		*source_spike_data_buffer_buff;	
	unsigned int				source_spike_data_buffer_size;
	SpikeTimeStampItem		*spike_item;
	NeuronSpikePatternGraphScroll	***neuron_graphs;
	if (!graph->paused)
	{
		new_part_start_time = graph->new_part_start_time;
		new_part_end_time = new_part_start_time + graph->graph_len_to_scroll;
		sampling_interval =  graph->sampling_interval;
		data_point_placement_start_idx = graph->data_point_placement_start_idx;
		spike_handling_buffer = graph->spike_handling_buffer;
		spike_handling_buffer_buff = spike_handling_buffer->buff;
		spike_handling_buffer_size = spike_handling_buffer->buffer_size;
		source_spike_data_to_plot = graph->source_spike_data_to_plot;
		source_spike_data_buffer_buff = source_spike_data_to_plot->buff;
		source_spike_data_buffer_size = source_spike_data_to_plot->buffer_size;
		neuron_graphs =  graph->neuron_graphs;
		if (graph->scroll_request)   // it is necessary otherwise set_total_limits cannot display slided and clear graph part. 
			scroll_network_spike_pattern_graph(network, graph);
		if (current_system_time > (new_part_end_time + graph->spike_buffer_followup_latency) )
		{
			// first handle graph' s spike buffer
			idx = graph->spike_handling_buffer_read_idx;		
			end_idx = spike_handling_buffer->buff_idx_write;
			while (idx != end_idx)
			{
				spike_item = &(spike_handling_buffer_buff[idx]);
				spike_time = spike_item->peak_time;
				if (spike_time < new_part_start_time)
					print_message(WARNING_MSG ,"IzNeuronSimulators", "SpikePatternGraph", "handle_spike_pattern_graph_scrolling_and_plotting", "spike_time < new_part_start_time (Only normal at plotting resumed).");
				if (spike_time < new_part_end_time)
					neuron_graphs[spike_item->mwa_or_layer][spike_item->channel_or_neuron_group][spike_item->unit_or_neuron].y[((spike_time - new_part_start_time) / sampling_interval) + data_point_placement_start_idx] = 1;
				else
					write_to_spike_data(spike_handling_buffer, spike_item->mwa_or_layer, spike_item->channel_or_neuron_group, spike_item->unit_or_neuron, spike_time);
				idx++;
				if (idx == spike_handling_buffer_size)
					idx = 0;
			}
			graph->spike_handling_buffer_read_idx = end_idx;
			// second handle graph' s source buffer	
			idx = graph->source_spike_data_buffer_read_idx;
			end_idx = source_spike_data_to_plot->buff_idx_write;	
			while (idx != end_idx)
			{
				spike_item = &(source_spike_data_buffer_buff[idx]);
				spike_time = spike_item->peak_time;
				if (spike_time < new_part_start_time)
					print_message(WARNING_MSG ,"IzNeuronSimulators", "SpikePatternGraph", "handle_spike_pattern_graph_scrolling_and_plotting", "spike_time < new_part_start_time (Only normal at plotting resumed).");
				if (spike_time < new_part_end_time)
					neuron_graphs[spike_item->mwa_or_layer][spike_item->channel_or_neuron_group][spike_item->unit_or_neuron].y[((spike_time - new_part_start_time) / sampling_interval) + data_point_placement_start_idx] = 1;
				else
					write_to_spike_data(spike_handling_buffer, spike_item->mwa_or_layer, spike_item->channel_or_neuron_group, spike_item->unit_or_neuron, spike_time);	
				idx++;
				if (idx == source_spike_data_buffer_size)
					idx = 0;			
			}
			graph->source_spike_data_buffer_read_idx = end_idx;	
			// plot and send scroll request to get prepared for next plotting.
			set_total_limits_network_spike_pattern_graph(network, graph);
			graph->scroll_request = TRUE;
			graph->new_part_start_time = new_part_end_time; // prepare for next acquisition
		}
		else
		{
			// first handle graph' s spike buffer
			idx = graph->spike_handling_buffer_read_idx;		
			end_idx = spike_handling_buffer->buff_idx_write;
			while (idx != end_idx)
			{
				spike_item = &(spike_handling_buffer_buff[idx]);
				spike_time = spike_item->peak_time;
				if (spike_time < new_part_start_time)
					print_message(WARNING_MSG ,"IzNeuronSimulators", "SpikePatternGraph", "handle_spike_pattern_graph_scrolling_and_plotting", "spike_time < new_part_start_time (Only normal at plotting resumed).");
				if (spike_time < new_part_end_time)
					neuron_graphs[spike_item->mwa_or_layer][spike_item->channel_or_neuron_group][spike_item->unit_or_neuron].y[((spike_time - new_part_start_time) / sampling_interval) + data_point_placement_start_idx] = 1;
				else
					write_to_spike_data(spike_handling_buffer, spike_item->mwa_or_layer, spike_item->channel_or_neuron_group, spike_item->unit_or_neuron, spike_time);
				idx++;
				if (idx == spike_handling_buffer_size)
					idx = 0;
			}
			graph->spike_handling_buffer_read_idx = end_idx;
			// second handle graph' s source buffer	
			idx = graph->source_spike_data_buffer_read_idx;
			end_idx = source_spike_data_to_plot->buff_idx_write;	
			while (idx != end_idx)
			{
				spike_item = &(source_spike_data_buffer_buff[idx]);
				spike_time = spike_item->peak_time;
				if (spike_time < new_part_start_time)
					print_message(WARNING_MSG ,"IzNeuronSimulators", "SpikePatternGraph", "handle_spike_pattern_graph_scrolling_and_plotting", "spike_time < new_part_start_time (Only normal at plotting resumed).");
				if (spike_time < new_part_end_time)
					neuron_graphs[spike_item->mwa_or_layer][spike_item->channel_or_neuron_group][spike_item->unit_or_neuron].y[((spike_time - new_part_start_time) / sampling_interval) + data_point_placement_start_idx] = 1;
				else
					write_to_spike_data(spike_handling_buffer, spike_item->mwa_or_layer, spike_item->channel_or_neuron_group, spike_item->unit_or_neuron, spike_time);	

				idx++;
				if (idx == source_spike_data_buffer_size)
					idx = 0;			
			}
			graph->source_spike_data_buffer_read_idx = end_idx;
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

	get_num_of_layers_in_network(network, &num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{
		get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{		
			get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				for (m = scroll; m < end_idx; m++)
					graph->neuron_graphs[i][j][k].y[m-scroll] = graph->neuron_graphs[i][j][k].y[m];	// instead of sliding in a straigthforward manner, think of making a list which holds the data points having spikes. the size of the list would be num_of_data_points and num_of_spikes would determine the used part of the list by spikes.
				for (m = clear_start_idx ; m < end_idx; m++)
				{
					graph->neuron_graphs[i][j][k].y[m] = 0;		
				}				
			}
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
