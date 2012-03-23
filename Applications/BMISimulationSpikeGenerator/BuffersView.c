#include "BuffersView.h"

static LayerNrnGrpNeuronCombo *combos_select_neuron;
static NeuronDynamicsCombo *combo_neuron_dynamics = NULL;

static GtkWidget *btn_enable_current_buffer_visualization;
static GtkWidget *btn_enable_neuron_dynamics_buffer_visualization;

static CurrentPatternGraph *current_pattern_buffer_graph = NULL;
static NeuronDynamicsGraph *neuron_dynamics_graph = NULL;
static unsigned int num_of_data_points_of_buffer_graphs;
static unsigned int num_of_samples_of_buffers;
static unsigned int previous_start_idx_to_plot;



static void combos_select_neuron_func(GtkWidget *changed_combo);
static gboolean timeout_callback(gpointer user_data) ;

bool create_buffers_view_gui(void)
{
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();
	GtkWidget *frame, *frame_label, *table, *vbox, *hbox, *lbl;

        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Buffers     ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (get_gui_tabs()), frame, frame_label);  


 	hbox = gtk_hbox_new(TRUE, 0);
        gtk_container_add (GTK_CONTAINER (frame), hbox);

	table = gtk_table_new(6,8,TRUE);
	gtk_box_pack_start(GTK_BOX(hbox),table, TRUE,TRUE,0);
	


///////////////////////////////////////////// FIRST COLUMN  ///////////////////////////////////////////////////////////////
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 0,1, 0, 6);  

  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("--- Select Neuron ---");
        gtk_box_pack_start(GTK_BOX(hbox),lbl,TRUE,TRUE,0);

  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	combos_select_neuron = allocate_layer_neuron_group_neuron_combos();
        gtk_box_pack_start(GTK_BOX(hbox),combos_select_neuron->combo_layer , TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_neuron->combo_neuron_group, TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox),combos_select_neuron->combo_neuron , TRUE,TRUE,0);
	combo_neuron_dynamics = allocate_neuron_dynamics_combo(hbox, combo_neuron_dynamics);
	if(!update_texts_of_combos_when_add_remove(combos_select_neuron, spike_gen_data->network))
		return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BuffersView", "create_buffers_view_gui", "! lupdate_current_pattern_graph().");

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("--- Visualizations ---");
        gtk_box_pack_start(GTK_BOX(hbox),lbl,TRUE,TRUE,0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_enable_current_buffer_visualization = gtk_button_new_with_label("InjectionCurrent: Enabled");
	gtk_box_pack_start (GTK_BOX (hbox), btn_enable_current_buffer_visualization, TRUE, TRUE, 0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_enable_neuron_dynamics_buffer_visualization = gtk_button_new_with_label("Neuron Dynamics: Enabled ");
	gtk_box_pack_start (GTK_BOX (hbox), btn_enable_neuron_dynamics_buffer_visualization, TRUE, TRUE, 0);

///////////////////////////////////////////// GRAPHS  ///////////////////////////////////////////////////////////////
 	hbox = gtk_hbox_new(TRUE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 1, 8, 0, 3);

	current_pattern_buffer_graph = allocate_current_pattern_graph(hbox, current_pattern_buffer_graph, 1000000000/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);  // num_of_dots for the graph should be divided by current_buffer_size without remainder.

 	hbox = gtk_hbox_new(TRUE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 1, 8, 3, 6);

	neuron_dynamics_graph = allocate_neuron_dynamics_graph(hbox, neuron_dynamics_graph, 1000000000/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, PARKER_SOCHACKI_INTEGRATION_STEP_SIZE); // num_of_dots for the graph should be divided by current_buffer_size without remainder.

	gtk_widget_show_all(get_gui_tabs());

	g_signal_connect(G_OBJECT(combos_select_neuron->combo_layer), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_layer);
	g_signal_connect(G_OBJECT(combos_select_neuron->combo_neuron_group), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_neuron_group);	
	g_signal_connect(G_OBJECT(combos_select_neuron->combo_neuron), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_neuron);


	num_of_data_points_of_buffer_graphs = current_pattern_buffer_graph->num_of_data_points;   // same as neuron dynamics buffer
	num_of_samples_of_buffers = spike_gen_data->current_pattern_buffer->buffer_size;  // same as neuron dynamics buffer

	previous_start_idx_to_plot = spike_gen_data->current_pattern_buffer->buff_write_idx / num_of_data_points_of_buffer_graphs;
	if (previous_start_idx_to_plot == 0)
		 previous_start_idx_to_plot = num_of_samples_of_buffers - num_of_data_points_of_buffer_graphs;    // read previous section
	else
		previous_start_idx_to_plot = (previous_start_idx_to_plot-1) * num_of_data_points_of_buffer_graphs;  // read previous section

	g_timeout_add(500, timeout_callback, NULL);

	return TRUE;
}

static gboolean timeout_callback(gpointer user_data) 
{
	unsigned int start_idx;
	unsigned int i;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	int neuron_dynamics_type_idx;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();
	CurrentPatternBuffer *current_pattern_buffer = spike_gen_data->current_pattern_buffer;
	NeuronCurrentSample *current_buffer = current_pattern_buffer->current_buffer;
	NeuronDynamicsBuffer *neuron_dynamics_pattern_buffer = spike_gen_data->neuron_dynamics_pattern_buffer;
	NetworkNeuronDynamics *neuron_dynamics_buffer = neuron_dynamics_pattern_buffer->buffer;
	float *current_buffer_graph_y = current_pattern_buffer_graph->y;
	float *neuron_dynamics_buffer_graph_y = neuron_dynamics_graph->y;
	start_idx = current_pattern_buffer->buff_write_idx / num_of_data_points_of_buffer_graphs;   // Handle previous current_pattern_buffer_graph->num_of_data_points
	if (start_idx == 0)
		 start_idx = num_of_samples_of_buffers - num_of_data_points_of_buffer_graphs;    // read previous section
	else
		start_idx = (start_idx-1) * num_of_data_points_of_buffer_graphs;  // read previous section
			
	if (start_idx != previous_start_idx_to_plot)   // Do not plot the same section if it is the same as the previous one due to high refresh rate (high timeout_callback frequency)
	{	
		previous_start_idx_to_plot = start_idx;	
		layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num);
		neuron_dynamics_combo_get_selected(combo_neuron_dynamics, &neuron_dynamics_type_idx);
		switch (neuron_dynamics_type_idx)
		{
			case DYNAMICS_TYPE_V:
				for (i = 0; i < num_of_data_points_of_buffer_graphs; i++)
				{
					current_buffer_graph_y[i] = current_buffer[i+start_idx].current_sample[layer_num][nrn_grp_num][nrn_num];
					neuron_dynamics_buffer_graph_y[i] = neuron_dynamics_buffer[i+start_idx].network_neuron_dyn[layer_num][nrn_grp_num][nrn_num].v;
				}
				break;
			case DYNAMICS_TYPE_U:
				for (i = 0; i < num_of_data_points_of_buffer_graphs; i++)
				{
					current_buffer_graph_y[i] = current_buffer[i+start_idx].current_sample[layer_num][nrn_grp_num][nrn_num];
					neuron_dynamics_buffer_graph_y[i] = neuron_dynamics_buffer[i+start_idx].network_neuron_dyn[layer_num][nrn_grp_num][nrn_num].u;
				}
				break;
			case DYNAMICS_TYPE_E:
				for (i = 0; i < num_of_data_points_of_buffer_graphs; i++)
				{
					current_buffer_graph_y[i] = current_buffer[i+start_idx].current_sample[layer_num][nrn_grp_num][nrn_num];
					neuron_dynamics_buffer_graph_y[i] = neuron_dynamics_buffer[i+start_idx].network_neuron_dyn[layer_num][nrn_grp_num][nrn_num].e;
				}
				break;
			case DYNAMICS_TYPE_I:
				for (i = 0; i < num_of_data_points_of_buffer_graphs; i++)
				{
					current_buffer_graph_y[i] = current_buffer[i+start_idx].current_sample[layer_num][nrn_grp_num][nrn_num];
					neuron_dynamics_buffer_graph_y[i] = neuron_dynamics_buffer[i+start_idx].network_neuron_dyn[layer_num][nrn_grp_num][nrn_num].i;
				}
				break;	
		}		
		if (!update_current_pattern_graph(current_pattern_buffer_graph))
			return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BuffersView", "timeout_callback", "! lupdate_current_pattern_graph().");
		if (!update_neuron_dynamics_graph(neuron_dynamics_graph))
			return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BuffersView", "timeout_callback", "! update_neuron_dynamics_graph().");
	}

	return TRUE;  

}

static void combos_select_neuron_func(GtkWidget *changed_combo)
{
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();
	if(!update_texts_of_combos_when_change(combos_select_neuron, spike_gen_data->network, changed_combo))
		return;
}


