#include "SynapticWeightHistGraph.h"


SynapticWeightHistGraph* allocate_synaptic_weight_history_graph(GtkWidget *hbox, SynapticWeightHistGraph *graph)
{
	unsigned int i;
	if (graph != NULL)
	{
		print_message(ERROR_MSG ,"IzNeuronSimulators", "SynapticWeightHistGraph", "allocate_synaptic_weight_history_graph", "graph != NULL");	
		return graph;
	}
	graph = g_new0(SynapticWeightHistGraph,1);

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

	graph->x = g_new0(float, SYNAPTIC_WEIGHT_HISTORY_BUFFER_SIZE);  
	graph->y = g_new0(float, SYNAPTIC_WEIGHT_HISTORY_BUFFER_SIZE);

	for (i = 0; i < SYNAPTIC_WEIGHT_HISTORY_BUFFER_SIZE; i++)
		graph->x[i] = (float)i;
	graph->graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (SYNAPTIC_WEIGHT_HISTORY_BUFFER_SIZE, graph->x, graph->y, &color_line, 0));
	gtk_databox_graph_add (GTK_DATABOX (graph->box), graph->graph);	
	gtk_databox_set_total_limits (GTK_DATABOX (graph->box), 0, SYNAPTIC_WEIGHT_HISTORY_BUFFER_SIZE, 15, -15);	

	gtk_widget_show_all(hbox);	

	return graph;						

}

bool update_synapse_history_graph(SynapticWeightHistGraph *graph, Network *network, unsigned int layer_num, unsigned int nrn_grp_num, unsigned int nrn_num, unsigned int syn_num)
{
	int read_idx, i;   // should be int, not unsigned int 
	Synapse *synapse;
	synapse = &(get_neuron_address(network, layer_num, nrn_grp_num, nrn_num)->syn_list->synapses[syn_num]);	
	SynapticWeight	*weight_history = synapse->weight_history;

	read_idx = synapse->weight_history_write_idx -1;
	if (read_idx < 0)
		read_idx = SYNAPTIC_WEIGHT_HISTORY_BUFFER_SIZE -1;  

	for (i = SYNAPTIC_WEIGHT_HISTORY_BUFFER_SIZE-1; i >= 0; i--)
	{
		graph->y[i] = weight_history[read_idx];
		read_idx--;
		if (read_idx < 0)
			read_idx = SYNAPTIC_WEIGHT_HISTORY_BUFFER_SIZE -1;  
	}
	gtk_databox_set_total_limits (GTK_DATABOX (graph->box), 0, SYNAPTIC_WEIGHT_HISTORY_BUFFER_SIZE, 15, -15);	
	return TRUE;				
}
