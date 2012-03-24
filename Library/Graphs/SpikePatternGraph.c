#include "SpikePatternGraph.h"



NetworkSpikePatternGraph* allocate_network_spike_pattern_graph(Network* network, GtkWidget *hbox, NetworkSpikePatternGraph *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, int graph_height)
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
	graph = g_new0(NetworkSpikePatternGraph,1);	
	graph->num_of_data_points = num_of_data_points;
	graph->sampling_interval = sampling_interval;
	graph->graph_len = sampling_interval*num_of_data_points;
	graph->graph_len_ms = graph->graph_len/1000000;
	color_bg.red = 0;
	color_bg.green = 0;
	color_bg.blue = 0;
	
	color_line.red = 65535;
	color_line.green = 65535;
	color_line.blue = 65535;


	vbox = gtk_vbox_new(FALSE, 0);
     	gtk_box_pack_start(GTK_BOX(hbox),vbox, TRUE,TRUE,0);

	get_num_of_neurons_in_network(network, &num_of_all_neurons_in_network);
	get_num_of_layers_in_network(network, &num_of_layers);
	graph->neuron_graphs = g_new0(NeuronSpikePatternGraph **,num_of_layers);	
	for (i = 0; i < num_of_layers; i++)
	{
		get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
		graph->neuron_graphs[i] = g_new0(NeuronSpikePatternGraph *,num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{		
			get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
			vbox1 = gtk_vbox_new(FALSE, 0);
     			gtk_box_pack_start(GTK_BOX(vbox),vbox1, FALSE,FALSE,1);

			gtk_widget_set_size_request(vbox1, 50, (int) ((((double)num_of_neurons_in_neuron_group) / num_of_all_neurons_in_network)* graph_height) - 5) ;// scale graph height  // minus 5 (thickness of gap between nrn_group graph)

			graph->neuron_graphs[i][j] = g_new0(NeuronSpikePatternGraph , num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				graph->neuron_graphs[i][j][k].databox = gtk_databox_new ();
				gtk_box_pack_start (GTK_BOX (vbox1), graph->neuron_graphs[i][j][k].databox, TRUE, TRUE, 0);
  				gtk_widget_modify_bg (graph->neuron_graphs[i][j][k].databox, GTK_STATE_NORMAL, &color_bg);
				graph->neuron_graphs[i][j][k].x = g_new0(float, num_of_data_points);  
				graph->neuron_graphs[i][j][k].y = g_new0(float, num_of_data_points);
				for (m = 0; m < num_of_data_points; m++)
					graph->neuron_graphs[i][j][k].x[m] = m*sampling_interval/1000000;
				graph->neuron_graphs[i][j][k].graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (num_of_data_points, graph->neuron_graphs[i][j][k].x, graph->neuron_graphs[i][j][k].y, &color_line, 0));
				gtk_databox_graph_add (GTK_DATABOX (graph->neuron_graphs[i][j][k].databox), graph->neuron_graphs[i][j][k].graph);	
				gtk_databox_set_total_limits (GTK_DATABOX (graph->neuron_graphs[i][j][k].databox), 0, (graph->graph_len_ms), 1.0, 0.9);	
			}
		}
	}
	gtk_widget_show_all(hbox);	

	return graph;						
}

bool clear_network_spike_pattern_graph_y_axes(Network* network, NetworkSpikePatternGraph *graph)
{
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	unsigned int i, j, k, m;
	get_num_of_layers_in_network(network, &num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{
		get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{		
			get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				for (m = 0; m < graph->num_of_data_points; m++)
					graph->neuron_graphs[i][j][k].y[m] =0;
			}
		}
	}
	return TRUE;
}

bool 

/*
bool update_network_spike_pattern_graph(Network* network, NetworkSpikePatternGraph *graph, SpikeData *spike_data, TimeStamp current_time)
{
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	unsigned int i, j, k, m;
	get_num_of_layers_in_network(network, &num_of_layers);
	for (i = 0; i < num_of_layers; i++)
	{
		get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{		
			get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group);
			for (k = 0; k < num_of_neurons_in_neuron_group; k++)
			{
				for (m = 0; m < num_of_data_points; m++)
					graph->neuron_graphs[i][j][k].y[m] =0;
			}
		}
	}
	return TRUE;
}*/
