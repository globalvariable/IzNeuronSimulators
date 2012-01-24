#include "NeuronDynamicsGraph.h"


NeuronDynamicsGraph* allocate_neuron_dynamics_graph(GtkWidget *hbox, NeuronDynamicsGraph *graph)
{
	if (graph != NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamicsGraph", "allocate_neuron_dynamics_box_and_graph", "graph != NULL");	
		return graph;
	}
	
	graph = g_new0(NeuronDynamicsGraph,1);
		
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

	graph->num_of_points_allocated = 1000;  // create tentatively
	graph->x = g_new0(float, graph->num_of_points_allocated);  
	graph->y = g_new0(float, graph->num_of_points_allocated);
	
	graph->graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (graph->num_of_points_allocated, graph->x, graph->y, &color_line, 0));
	gtk_databox_graph_add (GTK_DATABOX (graph->box), graph->graph);	
	gtk_databox_set_total_limits (GTK_DATABOX (graph->box), 0, graph->num_of_points_allocated, MAX_V_VALUE, MIN_V_VALUE);	
	
	return graph;			
}


bool update_neuron_dynamics_graph(NeuronDynamicsGraph *graph, AllNeuronsDynamicsSinglePattern* dynamics_pattern, int layer, int neuron_group, int neuron_num, int dynamics_type)
{
	int i;
	double *dynamics, initial_dynamics;
	float *x, *y;	
	float max_x = 0, min_y = 50000000, max_y = -50000000;
	GdkColor color_line;
	color_line.red = 0;
	color_line.green = 0;
	color_line.blue = 0;
	
	if (graph == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamicsGraph", "update_neuron_dynamics_graph", "graph == NULL");
		
	if (dynamics_pattern == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamicsGraph", "update_neuron_dynamics_graph", "dynamics_pattern == NULL");	
		
	if (!is_neuron(dynamics_pattern->network, layer, neuron_group, neuron_num))	
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamicsGraph", "update_neuron_dynamics_graph", "No such a neuron.");
		
	if ((dynamics_type < 0) || (dynamics_type > 3))
		return print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamicsGraph", "update_neuron_dynamics_graph", "Invalid neuron dynamics type submitted.");	
		
	if ((dynamics_pattern->num_of_allocated_samples + 1) != graph->num_of_points_allocated)   // reallocate graph with new samples
	{
		graph->num_of_points_allocated = dynamics_pattern->num_of_allocated_samples+1;    // to display initial v as well 
		g_free(graph->x);
		g_free(graph->y);		
		graph->x = g_new0(float, graph->num_of_points_allocated); // to display initial v as well  
		graph->y = g_new0(float, graph->num_of_points_allocated);	
		gtk_databox_graph_remove (GTK_DATABOX (graph->box), graph->graph);
		g_object_unref(graph->graph);
		graph->graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (graph->num_of_points_allocated, graph->x, graph->y, &color_line, 0));
		gtk_databox_graph_add (GTK_DATABOX (graph->box), graph->graph);	
	}	
	
	if (dynamics_type ==  DYNAMICS_TYPE_V)
	{
		initial_dynamics = dynamics_pattern->network_pattern[layer][neuron_group][neuron_num].initial_v;
		dynamics = dynamics_pattern->network_pattern[layer][neuron_group][neuron_num].v;
	}
	else if (dynamics_type ==  DYNAMICS_TYPE_U)
	{
		initial_dynamics = dynamics_pattern->network_pattern[layer][neuron_group][neuron_num].initial_u;
		dynamics = dynamics_pattern->network_pattern[layer][neuron_group][neuron_num].u;
	}
	else if (dynamics_type ==  DYNAMICS_TYPE_E)
	{
		initial_dynamics = dynamics_pattern->network_pattern[layer][neuron_group][neuron_num].initial_e;
		dynamics = dynamics_pattern->network_pattern[layer][neuron_group][neuron_num].e;	
	}
	else if (dynamics_type ==  DYNAMICS_TYPE_I)
	{
		initial_dynamics = dynamics_pattern->network_pattern[layer][neuron_group][neuron_num].initial_i;
		dynamics = dynamics_pattern->network_pattern[layer][neuron_group][neuron_num].i;	
	}
	else
	{
		return print_message(BUG_MSG ,"NeuroSim", "NeuronDynamicsGraph", "update_neuron_dynamics_graph", "Invalid dynamics_type.");			
	}	
	
	x = graph->x;
	y = graph->y;	
	x[0] = 0;
	y[0] = initial_dynamics;	

	for ( i=1; i < graph->num_of_points_allocated; i++)
	{
		if ( i <= dynamics_pattern->num_of_used_samples)
		{
			x[i] = (dynamics_pattern->sampling_times[i] - dynamics_pattern->initial_time) / 1000000.0;    // milliseconds scale by shifting
			y[i] = dynamics[i];
			if (x[i] > max_x)
				max_x = x[i];
			if (y[i] > max_y)
				max_y = y[i];
			if (y[i] < min_y)
				min_y = y[i];				
		}
		else
		{
			x[i] = (dynamics_pattern->sampling_times[dynamics_pattern->num_of_used_samples-1] - dynamics_pattern->initial_time) / 1000000.0;     // freeze graphing here. end point of line will be the last point of sample
			y[i] = dynamics[dynamics_pattern->num_of_used_samples-1];	
		} 
	}
	
	gtk_databox_set_total_limits (GTK_DATABOX (graph->box), 0-5, max_x+5, max_y+5, min_y-5);	
		
	return TRUE;
}










