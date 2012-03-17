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
		graph->x[i] = i*sampling_interval/1000000;
	graph->graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (num_of_data_points, graph->x, graph->y, &color_line, 0));
	gtk_databox_graph_add (GTK_DATABOX (graph->box), graph->graph);	
	gtk_databox_set_total_limits (GTK_DATABOX (graph->box), 0, num_of_data_points*sampling_interval/1000000, 200, -200);	

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






