#include "CurrentPatternGraph.h"



CurrentPatternGraph* allocate_current_pattern_graph(GtkWidget *hbox, CurrentPatternGraph *graph)
{
	if (graph != NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "allocate_current_pattern_graph", "graph != NULL");	
		return graph;
	}
	
	graph = g_new0(CurrentPatternGraph,1);
		
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

	graph->x = g_new0(float, 1000);  // create tentatively
	graph->y = g_new0(float, 1000);
	
	graph->graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (1000, graph->x, graph->y, &color_line, 0));
	gtk_databox_graph_add (GTK_DATABOX (graph->box), graph->graph);	
	gtk_databox_set_total_limits (GTK_DATABOX (graph->box), 0, 1000, 200, -200);	
	
	return graph;			
}

bool update_current_pattern_graph(CurrentPatternGraph *graph, CurrentPatterns* current_patterns, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, bool display_template, bool display_current_pattern_buffer, unsigned int trial_num, unsigned int trial_type_num)
{
	CurrentPattern 			*current_pattern;
	TrialData 				*trial_data;
	unsigned int i;
	unsigned int num_of_samples;
	double *current;
	float *x, *y;	
	float min_y = 50000000, max_y = -50000000;
	TimeStamp *sampling_times;
	TimeStamp trial_start_time, trial_end_time;
	GdkColor color_line;
	color_line.red = 0;
	color_line.green = 0;
	color_line.blue = 0;
	
	if (graph == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "update_current_pattern_graph", "graph == NULL");
	if (current_patterns == NULL)
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "update_current_pattern_graph", "current_patterns == NULL");	
	if (!is_neuron(current_patterns->network, layer, neuron_group, neuron_num))	
		return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "update_current_pattern_graph", "! is_neuron().");

	g_free(graph->x);
	g_free(graph->y);	
	if (display_template)
	{
		if (trial_type_num >= current_patterns->trial_stats->num_of_trial_types)
			return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "update_current_pattern_graph", "trial_type_num >= current_patterns->trial_stats->num_of_trial_types.");
		num_of_samples = current_patterns->current_pattern_templates[trial_type_num].num_of_samples;
		sampling_times = current_patterns->current_pattern_templates[trial_type_num].sampling_times;
		current = current_patterns->current_pattern_templates[trial_type_num].neuron_current_templates[layer][neuron_group][neuron_num].pattern_template.pattern;
		graph->x = g_new0(float, num_of_samples); 
		graph->y = g_new0(float, num_of_samples);
		gtk_databox_graph_remove (GTK_DATABOX (graph->box), graph->graph);
		g_object_unref(graph->graph);
		graph->graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (num_of_samples, graph->x, graph->y, &color_line, 0));
		gtk_databox_graph_add (GTK_DATABOX (graph->box), graph->graph);	
		x = graph->x;;
		y = graph->y;	
		trial_start_time = 0;
		trial_end_time = current_patterns->trial_stats->trial_type_stats[trial_type_num].max_trial_type_length_assigned;
		for ( i=0; i < num_of_samples; i++)
		{
			x[i] = sampling_times[i]/ 1000000.0;    // milliseconds scale by shifting
			y[i] = current[i];
			if (y[i] > max_y)
				max_y = y[i];
			if (y[i] < min_y)
				min_y = y[i];		
			gtk_databox_set_total_limits (GTK_DATABOX (graph->box), trial_start_time - 5, (trial_end_time/1000000) + 5, max_y + 5, min_y - 5);					
		}
	}
	else if (display_current_pattern_buffer)
	{
		trial_type_num = current_patterns->trial_stats->buffer.trial_type_idx_of_trial_type_stats;
		if (trial_type_num >= current_patterns->trial_stats->num_of_trial_types)
			return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "update_current_pattern_graph", "trial_or_pattern_type_num >= current_patterns->trial_stats->num_of_trial_types.");
		num_of_samples = current_patterns->buffer[trial_type_num].write_idx;
		sampling_times = current_patterns->buffer[trial_type_num].sampling_times;
		current = current_patterns->buffer[trial_type_num].neuron_current_pattern[layer][neuron_group][neuron_num].pattern;		
		graph->x = g_new0(float, num_of_samples); 
		graph->y = g_new0(float, num_of_samples);
		gtk_databox_graph_remove (GTK_DATABOX (graph->box), graph->graph);
		g_object_unref(graph->graph);
		graph->graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (num_of_samples, graph->x, graph->y, &color_line, 0));
		gtk_databox_graph_add (GTK_DATABOX (graph->box), graph->graph);	
		x = graph->x;
		y = graph->y;	
		trial_start_time = current_patterns->trial_stats->buffer.trial_start_time;
		trial_end_time = trial_start_time + current_patterns->trial_stats->buffer.trial_length;
		for ( i=0; i < num_of_samples; i++)
		{
			x[i] = (sampling_times[i] - trial_start_time)/ 1000000.0;    // milliseconds scale by shifting
			y[i] = current[i];
			if (y[i] > max_y)
				max_y = y[i];
			if (y[i] < min_y)
				min_y = y[i];		
			if (sampling_times[i] < trial_start_time)
				return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "update_current_pattern_graph", "sampling_times[i] < trial_start_time");
			if (sampling_times[i] > trial_end_time)
				return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "update_current_pattern_graph", "sampling_times[i] > trial_end_time");
		} 
		gtk_databox_set_total_limits (GTK_DATABOX (graph->box), 0 - 5, (current_patterns->trial_stats->trial_type_stats[trial_type_num].max_trial_type_length_assigned/1000000) + 5, max_y + 5, min_y - 5);							
	}
	else 
	{
		if (trial_num >= current_patterns->trial_stats->total_num_of_trials)
			return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "update_current_pattern_graph", "trial_or_pattern_type_num >= current_patterns->trial_stats->num_of_trial_types.");
		if (!get_current_pattern(current_patterns, trial_num, &current_pattern))
			return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "update_current_pattern_graph", "! get_current_pattern().");
		if (!get_trial_data(current_patterns->trial_stats, trial_num, &trial_data))
			return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "update_current_pattern_graph", "! get_current_pattern().");
		num_of_samples = current_pattern->num_of_samples;
		sampling_times = current_pattern->sampling_times;
		trial_type_num = trial_data->trial_type_idx_of_trial_type_stats;
		if (trial_type_num >= current_patterns->trial_stats->num_of_trial_types)
			return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "update_current_pattern_graph", "trial_or_pattern_type_num >= current_patterns->trial_stats->num_of_trial_types.");
		current = current_pattern->neuron_current_pattern[layer][neuron_group][neuron_num].pattern;			
		graph->x = g_new0(float, num_of_samples); 
		graph->y = g_new0(float, num_of_samples);
		gtk_databox_graph_remove (GTK_DATABOX (graph->box), graph->graph);
		g_object_unref(graph->graph);
		graph->graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (num_of_samples, graph->x, graph->y, &color_line, 0));
		gtk_databox_graph_add (GTK_DATABOX (graph->box), graph->graph);	
		x = graph->x;
		y = graph->y;	
		trial_start_time = trial_data->trial_start_time;
		trial_end_time = trial_start_time + trial_data->trial_length;
		for ( i=0; i < num_of_samples; i++)
		{
			x[i] = (sampling_times[i] - trial_start_time)/ 1000000.0;    // milliseconds scale by shifting
			y[i] = current[i];
			if (y[i] > max_y)
				max_y = y[i];
			if (y[i] < min_y)
				min_y = y[i];		
			if (sampling_times[i] < trial_start_time)
				return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "update_current_pattern_graph", "sampling_times[i] < trial_start_time");
			if (sampling_times[i] > trial_end_time)
				return print_message(ERROR_MSG ,"NeuroSim", "CurrentPatternGraph", "update_current_pattern_graph", "sampling_times[i] > trial_end_time");
		} 
		gtk_databox_set_total_limits (GTK_DATABOX (graph->box), 0 - 5, (current_patterns->trial_stats->trial_type_stats[trial_type_num].max_trial_type_length_assigned/1000000) + 5, max_y + 5, min_y - 5);			
	}	
	
	return TRUE;
}


