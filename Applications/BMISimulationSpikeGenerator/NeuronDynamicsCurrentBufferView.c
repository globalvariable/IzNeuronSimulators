#include "NeuronDynamicsCurrentBufferView.h"

static GtkWidget *btn_global_pause;
static GtkWidget *btn_pause_current_pattern_graph;
static GtkWidget *btn_pause_neuron_dynamics_graph;
static GtkWidget *btn_select_current_pattern_graph;
static GtkWidget *btn_select_neuron_dynamics_graph;
static LayerNrnGrpNeuronCombo *combos_select_neuron_for_current_pattern;
static LayerNrnGrpNeuronCombo *combos_select_neuron_for_neuron_dynamics;

static NeuronDynamicsCombo *combo_neuron_dynamics = NULL;

static CurrentPatternGraphScroll *current_pattern_graph = NULL;
static NeuronDynamicsGraphScroll *neuron_dynamics_graph = NULL;

static void global_pause_button_func (void);
static void pause_current_pattern_graph_func(void);
static void select_current_pattern_graph_func(void);
static void pause_neuron_dynamics_graph_func(void);
static void select_neuron_dynamics_graph_func(void);

static void combos_select_neuron_for_current_pattern_func(GtkWidget *changed_combo);
static void combos_select_neuron_for_neuron_dynamics_func(GtkWidget *changed_combo);

bool create_neuron_dynamics_and_current_buffer_view_gui(void)
{
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();
	GtkWidget *frame, *frame_label, *vbox, *vbox1, *hbox, *hbox1;

        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Neuron Dynamics & Current     ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (get_gui_tabs()), frame, frame_label);  

 	vbox = gtk_vbox_new(FALSE, 0);
        gtk_container_add (GTK_CONTAINER (frame), vbox);

	btn_global_pause = gtk_button_new_with_label("Paused");
	gtk_box_pack_start (GTK_BOX (vbox), btn_global_pause, FALSE, FALSE, 0);
	gtk_widget_set_sensitive(btn_global_pause, FALSE);	

  	hbox = gtk_hbox_new(FALSE, 0);
     	gtk_box_pack_start(GTK_BOX(vbox),hbox, TRUE,TRUE,0);

	btn_pause_current_pattern_graph = gtk_button_new_with_label("P");
	gtk_box_pack_start (GTK_BOX (hbox), btn_pause_current_pattern_graph, FALSE, FALSE, 0);

 	vbox1 = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), vbox1, TRUE, TRUE, 0);

  	hbox = gtk_hbox_new(FALSE, 0);
     	gtk_box_pack_start(GTK_BOX(vbox1),hbox, FALSE,FALSE,0);
	combos_select_neuron_for_current_pattern = allocate_layer_neuron_group_neuron_combos();
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_neuron_for_current_pattern->combo_layer , FALSE,FALSE,0);
	gtk_widget_set_size_request(combos_select_neuron_for_current_pattern->combo_layer, 60, 25);	
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_neuron_for_current_pattern->combo_neuron_group, FALSE,FALSE,0);
	gtk_widget_set_size_request(combos_select_neuron_for_current_pattern->combo_neuron_group, 60, 25);	
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_neuron_for_current_pattern->combo_neuron , FALSE,FALSE,0);
	gtk_widget_set_size_request(combos_select_neuron_for_current_pattern->combo_neuron, 60, 25);	
	if(!update_texts_of_combos_when_add_remove(combos_select_neuron_for_current_pattern, spike_gen_data->network))
		return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "NeuronDynamicsCurrentBufferView", "create_neuron_dynamics_and_current_buffer_view_gui", "! update_texts_of_combos_when_add_remove().");

	btn_select_current_pattern_graph = gtk_button_new_with_label("Select");
	gtk_box_pack_start (GTK_BOX (hbox), btn_select_current_pattern_graph, FALSE, FALSE, 0);

  	hbox = gtk_hbox_new(TRUE, 0);
    	gtk_box_pack_start(GTK_BOX(vbox1),hbox, TRUE,TRUE,0);
	current_pattern_graph = allocate_current_pattern_graph_scroll(hbox, current_pattern_graph, GRAPH_LENGTHS/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, GRAPH_SCROLL_LENGTHS/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, BUFFER_FOLLOWUP_LATENCY, NUM_OF_STATUS_MARKERS, get_bmi_simulation_spike_generator_trials_data()); // 100 ms latency


	g_signal_connect(G_OBJECT(btn_pause_current_pattern_graph), "clicked", G_CALLBACK(pause_current_pattern_graph_func), NULL);
	g_signal_connect(G_OBJECT(btn_select_current_pattern_graph), "clicked", G_CALLBACK(select_current_pattern_graph_func), NULL);
	g_signal_connect(G_OBJECT(combos_select_neuron_for_current_pattern->combo_layer), "changed", G_CALLBACK(combos_select_neuron_for_current_pattern_func), combos_select_neuron_for_current_pattern->combo_layer);
	g_signal_connect(G_OBJECT(combos_select_neuron_for_current_pattern->combo_neuron_group), "changed", G_CALLBACK(combos_select_neuron_for_current_pattern_func), combos_select_neuron_for_current_pattern->combo_neuron_group);	
	g_signal_connect(G_OBJECT(combos_select_neuron_for_current_pattern->combo_neuron), "changed", G_CALLBACK(combos_select_neuron_for_current_pattern_func), combos_select_neuron_for_current_pattern->combo_neuron);

  	hbox = gtk_hbox_new(FALSE, 0);
     	gtk_box_pack_start(GTK_BOX(vbox),hbox, TRUE,TRUE,0);

	btn_pause_neuron_dynamics_graph = gtk_button_new_with_label("P");
	gtk_box_pack_start (GTK_BOX (hbox), btn_pause_neuron_dynamics_graph, FALSE, FALSE, 0);

 	vbox1 = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), vbox1, TRUE, TRUE, 0);

  	hbox = gtk_hbox_new(FALSE, 0);
     	gtk_box_pack_start(GTK_BOX(vbox1),hbox, FALSE,FALSE,0);
	combos_select_neuron_for_neuron_dynamics = allocate_layer_neuron_group_neuron_combos();
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_neuron_for_neuron_dynamics->combo_layer , FALSE,FALSE,0);
	gtk_widget_set_size_request(combos_select_neuron_for_neuron_dynamics->combo_layer, 60, 25);	
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_neuron_for_neuron_dynamics->combo_neuron_group, FALSE,FALSE,0);
	gtk_widget_set_size_request(combos_select_neuron_for_neuron_dynamics->combo_neuron_group, 60, 25);	
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_neuron_for_neuron_dynamics->combo_neuron , FALSE,FALSE,0);
	gtk_widget_set_size_request(combos_select_neuron_for_neuron_dynamics->combo_neuron, 60, 25);	
	if(!update_texts_of_combos_when_add_remove(combos_select_neuron_for_neuron_dynamics, spike_gen_data->network))
		return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "NeuronDynamicsCurrentBufferView", "create_neuron_dynamics_and_current_buffer_view_gui", "! update_texts_of_combos_when_add_remove().");

  	hbox1 = gtk_hbox_new(FALSE, 0);
     	gtk_box_pack_start(GTK_BOX(hbox),hbox1, FALSE,FALSE,0);
	combo_neuron_dynamics = allocate_neuron_dynamics_combo(hbox1, combo_neuron_dynamics);

	btn_select_neuron_dynamics_graph = gtk_button_new_with_label("Select");
	gtk_box_pack_start (GTK_BOX (hbox), btn_select_neuron_dynamics_graph, FALSE, FALSE, 0);

  	hbox = gtk_hbox_new(TRUE, 0);
    	gtk_box_pack_start(GTK_BOX(vbox1),hbox, TRUE,TRUE,0);
	neuron_dynamics_graph = allocate_neuron_dynamics_graph_scroll(hbox, neuron_dynamics_graph, 3000000000/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, 1000000000/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, 100000000);  // 100 ms latency

	g_signal_connect(G_OBJECT(btn_pause_neuron_dynamics_graph), "clicked", G_CALLBACK(pause_neuron_dynamics_graph_func), NULL);
	g_signal_connect(G_OBJECT(btn_select_neuron_dynamics_graph), "clicked", G_CALLBACK(select_neuron_dynamics_graph_func), NULL);
	g_signal_connect(G_OBJECT(combos_select_neuron_for_neuron_dynamics->combo_layer), "changed", G_CALLBACK(combos_select_neuron_for_neuron_dynamics_func), combos_select_neuron_for_current_pattern->combo_layer);
	g_signal_connect(G_OBJECT(combos_select_neuron_for_neuron_dynamics->combo_neuron_group), "changed", G_CALLBACK(combos_select_neuron_for_neuron_dynamics_func), combos_select_neuron_for_neuron_dynamics->combo_neuron_group);	
	g_signal_connect(G_OBJECT(combos_select_neuron_for_neuron_dynamics->combo_neuron), "changed", G_CALLBACK(combos_select_neuron_for_neuron_dynamics_func), combos_select_neuron_for_neuron_dynamics->combo_neuron);


	g_signal_connect(G_OBJECT(btn_global_pause), "clicked", G_CALLBACK(global_pause_button_func), NULL);

	gtk_widget_show_all(get_gui_tabs());

	return TRUE;
}

static void combos_select_neuron_for_current_pattern_func(GtkWidget *changed_combo)
{
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();	
	if(!update_texts_of_combos_when_change(combos_select_neuron_for_current_pattern, spike_gen_data->network, changed_combo))
		return;
}
static void combos_select_neuron_for_neuron_dynamics_func(GtkWidget *changed_combo)
{
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();	
	if(!update_texts_of_combos_when_change(combos_select_neuron_for_neuron_dynamics, spike_gen_data->network, changed_combo))
		return;
}

CurrentPatternGraphScroll* get_current_pattern_graph_w_scroll_ptr(void)
{
	return current_pattern_graph;
}

NeuronDynamicsGraphScroll* get_neuron_dynamics_graph_w_scroll_ptr(void)
{
	return neuron_dynamics_graph;
}

static void pause_current_pattern_graph_func(void)
{
	if (current_pattern_graph->paused)
	{
		if (send_current_pattern_graph_resume_request_to_buffer_view_handler())   // should resume all graphs at the same time to provide sync
			gtk_button_set_label (GTK_BUTTON(btn_pause_current_pattern_graph),"R");  
	}
	else
	{
		current_pattern_graph->paused = TRUE;
		gtk_button_set_label (GTK_BUTTON(btn_pause_current_pattern_graph),"P");		
	}
}
static void select_current_pattern_graph_func(void)
{
	unsigned int layer_num, nrn_grp_num, nrn_num;

	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron_for_current_pattern, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "NeuronDynamicsCurrentBufferView", "select_neuron_dynamics_graph_func", "! layer_neuron_group_neuron_get_selected().");	
	if (!submit_current_pattern_graph_neuron(current_pattern_graph, layer_num, nrn_grp_num, nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "NeuronDynamicsCurrentBufferView", "select_neuron_dynamics_graph_func", "! submit_current_pattern_graph_neuron().");
}


static void pause_neuron_dynamics_graph_func(void)
{
	if (neuron_dynamics_graph->paused)
	{
		if (send_neuron_dynamics_graph_resume_request_to_buffer_view_handler())   // should resume all graphs at the same time to provide sync
			gtk_button_set_label (GTK_BUTTON(btn_pause_neuron_dynamics_graph),"R");  
	}
	else
	{
		neuron_dynamics_graph->paused = TRUE;
		gtk_button_set_label (GTK_BUTTON(btn_pause_neuron_dynamics_graph),"P");		
	}
}

static void select_neuron_dynamics_graph_func(void)
{
	unsigned int layer_num, nrn_grp_num, nrn_num;
	int dynamics_type;

	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron_for_neuron_dynamics, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "NeuronDynamicsCurrentBufferView", "select_neuron_dynamics_graph_func", "! layer_neuron_group_neuron_get_selected().");	
	if (!neuron_dynamics_combo_get_selected(combo_neuron_dynamics, &dynamics_type))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "NeuronDynamicsCurrentBufferView", "select_neuron_dynamics_graph_func", "! neuron_dynamics_combo_get_selected().");	
	if (!submit_neuron_dynamics_graph_neuron_and_dynamics_type(neuron_dynamics_graph, layer_num, nrn_grp_num, nrn_num, dynamics_type))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "NeuronDynamicsCurrentBufferView", "select_neuron_dynamics_graph_func", "! submit_neuron_dynamics_graph_neuron_and_dynamics_type().");
}

void send_global_pause_button_sensitive_request(void)
{
	gtk_widget_set_sensitive(btn_global_pause, TRUE);			
}

static void global_pause_button_func (void)
{
	if (is_buffer_view_handler_paused())
	{
		send_global_resume_request_to_buffer_view_handler();
		gtk_button_set_label (GTK_BUTTON(btn_global_pause),"Resumed");
	}
	else
	{		
		send_global_pause_request_to_buffer_view_handler();
		gtk_button_set_label (GTK_BUTTON(btn_global_pause),"Paused");
	}
}
