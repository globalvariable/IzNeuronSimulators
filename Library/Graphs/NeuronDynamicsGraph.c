#include "NeuronDynamicsGraph.h"


NeuronDynamicsGraph* allocate_neuron_dynamics_graph(GtkWidget *hbox, NeuronDynamicsGraph *graph, unsigned int num_of_data_points, TimeStamp sampling_interval)
{
	unsigned int i;
	if (graph != NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamicsGraph", "allocate_neuron_dynamics_box_and_graph", "graph != NULL");	
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
	gtk_databox_set_total_limits (GTK_DATABOX (graph->box), -100, (graph->graph_len/1000000)+ 100, 200, -200);	

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

	gtk_databox_set_total_limits (GTK_DATABOX (graph->box), -100 , (graph->graph_len/1000000) + 100, max_y, min_y);

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
		print_message(INFO_MSG ,"NeuroSim", "NeuronDynamicsGraph", "change_length_of_neuron_dynamics_graph", "Changing graph length");		

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

NeuronDynamicsGraphScroll* allocate_neuron_dynamics_graph_scroll(GtkWidget *hbox, NeuronDynamicsGraphScroll *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, unsigned int num_of_data_points_to_scroll, TimeStamp buffer_followup_latency)
{
	unsigned int i;
	if (graph != NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamicsGraph", "allocate_neuron_dynamics_box_and_graph", "graph != NULL");	
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
	gtk_databox_set_total_limits (GTK_DATABOX (graph->box), -100, (graph->graph_len/1000000)+ 100, 200, -200);	

	graph->num_of_data_points_to_scroll = num_of_data_points_to_scroll;
	graph->graph_len_to_scroll = sampling_interval*num_of_data_points_to_scroll;
	graph->buffer_followup_latency = buffer_followup_latency;
	gtk_widget_show_all(hbox);	

	return graph;				
}

bool handle_neuron_dynamics_graph_scrolling_and_plotting(NeuronDynamicsGraphScroll *graph, NeuronDynamicsBuffer *neuron_dynamics_buffer)
{
	TimeStamp current_time;
	TimeStamp new_part_start_time;
	TimeStamp new_part_end_time;
	unsigned int new_part_start_idx, end_idx; 
	unsigned int layer, neuron_grp, neuron_num;
	int dynamics_type;	
	unsigned int buffer_size;
	
	if (!graph->paused)
	{
		if (graph->scroll_request)   // it is necessary otherwise set_total_limits cannot display slided and clear graph part. 
		{
			scroll_neuron_dynamics_graph(graph);
			graph->scroll_request = FALSE;
		}	
		current_time = neuron_dynamics_buffer->last_sample_time;
		new_part_start_time = graph->new_part_start_time;
		new_part_end_time = new_part_start_time + graph->graph_len_to_scroll;
		buffer_size = neuron_dynamics_buffer->buffer_size;

		if (current_time > (new_part_end_time + graph->buffer_followup_latency) )
		{
			pthread_mutex_lock(&(graph->mutex));
			layer = graph->active_layer;
			neuron_grp = graph->active_neuron_group;
			neuron_num = graph->active_neuron;
			dynamics_type = graph->dynamics_type;
			pthread_mutex_unlock(&(graph->mutex));
			new_part_start_idx =  graph->new_part_start_idx;
			(current_time - new_part_start_time)/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE
			if ((new_part_start_idx + graph->num_of_data_points_to_scroll) > buffer_size)
				new_part_start_idx = new_part_start_idx + graph->num_of_data_points_to_scroll - buffer_size;
			else
				new_part_start_idx = new_part_start_idx + graph->num_of_data_points_to_scroll;
			while (idx != end_idx)
			{
				graph->y[idx] = neuron_dynamics_buffer->buffer[idx].network_neuron_dyn[layer][neuron_grp][neuron_num].dyn[dynamics_type];
				if ((new_part_start_idx + 1) == buffer_size)
					new_part_start_idx = 0;
				else
					new_part_start_idx++;
			}
		}
		update_neuron_dynamics_graph_scroll(graph);
		graph->scroll_request = TRUE;
		graph->new_part_start_time = new_part_end_time; // prepare for next acquisition
	}
	return TRUE;	
}

bool scroll_neuron_dynamics_graph(NeuronDynamicsGraphScroll *graph)
{
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

bool update_neuron_dynamics_graph_scroll(NeuronDynamicsGraphScroll *graph)
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

	gtk_databox_set_total_limits (GTK_DATABOX (graph->box), -100 , (graph->graph_len/1000000) + 100, max_y, min_y);

	return TRUE;	
}
