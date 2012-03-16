#include "CurrentPatternGraph.h"



CurrentPatternGraph* allocate_current_pattern_graph(GtkWidget *hbox, CurrentPatternGraph *graph, unsigned int num_of_data_points, TimeStamp sampling_interval)
{
	unsigned int i;
	if (graph != NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "allocate_current_pattern_graph", "graph != NULL");	
		return graph;
	}
	
	graph = g_new0(CurrentPatternGraph,1);
	graph->num_of_data_points = num_of_data_points;
	graph->sampling_interval = sampling_interval;
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

