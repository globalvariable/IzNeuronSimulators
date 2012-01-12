#include "NeuronDynamicsGraph.h"


static GtkWidget **boxes = NULL;
static GtkWidget **databoxes = NULL;
static GtkDataboxGraph **graphs = NULL;
static GdkColor color_bg;
static GdkColor color_line;
static GdkColor color_grid;
static float **y_axis = NULL;    
static float **x_axis = NULL;  
static int num_of_graphs = 0;


bool add_neuron_dynamics_box_and_graph(GtkWidget *hbox, TimeStamp pattern_length, int *graph_idx)
{
	int i;
	GtkWidget **lcl_boxes = NULL;
	GtkWidget **lcl_databoxes = NULL;	
	float **lcl_y_axis = NULL;    
	float **lcl_x_axis = NULL;  
	GtkDataboxGraph **lcl_graphs = NULL;	
	
	TimeStampMs pattern_length_to_disp = (TimeStampMs)(pattern_length/1000000);

	color_bg.red = 65535;
	color_bg.green = 65535;
	color_bg.blue = 65535;
	
	color_grid.red = 50000;
	color_grid.green = 50000;
	color_grid.blue = 50000;
	
	color_line.red = 0;
	color_line.green = 0;
	color_line.blue = 0;


	is_allocated(boxes, TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_WARNING, "This is the first time for allocation of neuron dynamics boxes.", EXPECTS_ALLOCATED);	
	is_allocated(databoxes, TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_WARNING, "This is the first time for allocation of neuron dynamics databoxes.", EXPECTS_ALLOCATED);	

	lcl_boxes = g_new0(GtkWidget*, num_of_graphs + 1);	
	lcl_databoxes = g_new0(GtkWidget*, num_of_graphs + 1);
	if (!is_allocated(lcl_boxes, TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_FAILED, "lcl_boxes", EXPECTS_ALLOCATED))	
		return FALSE;
	if (!is_allocated(lcl_boxes, TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_BUG, "lcl_databoxes", EXPECTS_ALLOCATED))	
		return FALSE;	
					
	for (i = 0; i< num_of_graphs; i++)
	{
		lcl_boxes[i] = boxes[i];
		lcl_databoxes[i] = databoxes[i];		
	}		
	g_free(boxes);
	g_free(databoxes);
	boxes = lcl_boxes;
	databoxes = lcl_databoxes;

	if (is_allocated(boxes[num_of_graphs], TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_BUG, "boxes[num_of_graphs]", EXPECTS_NOT_ALLOCATED))	
		return FALSE;
	if (is_allocated(databoxes[num_of_graphs], TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_BUG, "databoxes[num_of_graphs]", EXPECTS_NOT_ALLOCATED))	
		return FALSE;
	gtk_databox_create_box_with_scrollbars_and_rulers (&boxes[num_of_graphs], &databoxes[num_of_graphs],TRUE, TRUE, TRUE, TRUE);
	if (!is_allocated(boxes[num_of_graphs], TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_FAILED, "boxes[num_of_graphs]", EXPECTS_ALLOCATED))	
		return FALSE;
	if (!is_allocated(databoxes[num_of_graphs], TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph",  ALLOCATION_FAILED, "databoxes[num_of_graphs]", EXPECTS_ALLOCATED))	
		return FALSE;

  	gtk_widget_modify_bg (boxes[num_of_graphs], GTK_STATE_NORMAL, &color_bg);
 	gtk_databox_graph_add (GTK_DATABOX (boxes[num_of_graphs]), gtk_databox_grid_new (9, 9, &color_grid, 0)); 	
	gtk_box_pack_start (GTK_BOX (hbox), databoxes[num_of_graphs], TRUE, TRUE, 0);

	is_allocated(x_axis, TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_WARNING, "This is the first time for allocation of neuron dynamics x axes.", EXPECTS_ALLOCATED);	
	is_allocated(y_axis, TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_WARNING, "This is the first time for allocation of neuron dynamics y axes.", EXPECTS_ALLOCATED);	

	lcl_x_axis = g_new0(float*, num_of_graphs + 1);
	lcl_y_axis = g_new0(float*, num_of_graphs + 1);
	if (!is_allocated(lcl_x_axis, TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_FAILED, "lcl_x_axis", EXPECTS_ALLOCATED))	
		return FALSE;	
	if (!is_allocated(lcl_y_axis, TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_FAILED, "lcl_y_axis", EXPECTS_ALLOCATED))	
		return FALSE;		
	
	for (i = 0; i< num_of_graphs; i++)
	{
		lcl_x_axis[i] = x_axis[i];
		lcl_y_axis[i] = y_axis[i];		
	}		
	g_free(x_axis);
	g_free(y_axis);	
	x_axis = lcl_x_axis;
	y_axis = lcl_y_axis;			
		
		
	if (is_allocated(x_axis[num_of_graphs], TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_BUG, "x_axis[num_of_graphs]", EXPECTS_NOT_ALLOCATED))	
		return FALSE;
	if (is_allocated(y_axis[num_of_graphs], TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_BUG, "y_axis[num_of_graphs]", EXPECTS_NOT_ALLOCATED))	
		return FALSE;
	x_axis[num_of_graphs] = g_new0(float,  pattern_length_to_disp);
	y_axis[num_of_graphs] = g_new0(float, pattern_length_to_disp);
	if (!is_allocated(x_axis[num_of_graphs], TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_FAILED, "x_axis[num_of_graphs]", EXPECTS_ALLOCATED))	
		return FALSE;
	if (!is_allocated(y_axis[num_of_graphs], TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph",  ALLOCATION_FAILED, "y_axis[num_of_graphs]", EXPECTS_ALLOCATED))	
		return FALSE;		

	for (i = 0; i< pattern_length_to_disp; i++)
	{
		x_axis[num_of_graphs][i] = i;
	}

	is_allocated(graphs, TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_WARNING, "This is the first time for allocation of neuron dynamics graphs.", EXPECTS_ALLOCATED);	
	
	lcl_graphs = g_new0(GtkDataboxGraph *, num_of_graphs + 1);	
	if (!is_allocated(lcl_graphs, TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_FAILED, "lcl_graphs", EXPECTS_ALLOCATED))	
		return FALSE;
	for (i = 0; i< num_of_graphs; i++)
	{
		lcl_graphs[i] = graphs[i];
	}	
	g_free(graphs);
	graphs = lcl_graphs; 

	if (is_allocated(graphs[num_of_graphs], TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_BUG, "graphs[num_of_graphs]", EXPECTS_NOT_ALLOCATED))	
		return FALSE;	
	graphs[num_of_graphs] = GTK_DATABOX_GRAPH(gtk_databox_lines_new (pattern_length_to_disp, x_axis[num_of_graphs], y_axis[num_of_graphs], &color_line, 0));
	if (!is_allocated(graphs[num_of_graphs], TRUE, "NeuronDynamicsGraph", "add_neuron_dynamics_box_and_graph", ALLOCATION_FAILED, "graphs[num_of_graphs]", EXPECTS_ALLOCATED))	
		return FALSE;	
	gtk_databox_graph_add (GTK_DATABOX (boxes[num_of_graphs]), graphs[num_of_graphs]);	
	gtk_databox_set_total_limits (GTK_DATABOX (boxes[num_of_graphs]), 0, pattern_length_to_disp, MAX_V_VALUE, MIN_V_VALUE);	
	
	*graph_idx = num_of_graphs;	
	num_of_graphs ++;
	
	printf ("NeuronDynamicsGraph: INFO: Number of total graphs in neuron dynamics graphs inventory is %d\n", num_of_graphs);

	return TRUE;			
}











