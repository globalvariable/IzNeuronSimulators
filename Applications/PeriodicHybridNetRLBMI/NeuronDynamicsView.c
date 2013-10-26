#include "NeuronDynamicsView.h"


static GtkWidget *btn_global_pause;
static GtkWidget **btn_pause_arr;
static GtkWidget **btn_select_arr;
static LayerNrnGrpNeuronCombo **combos_select_neuron_arr;
static NeuronDynamicsCombo **combo_neuron_dynamics_arr;
static NeuronDynamicsGraphScrollLimited **neuron_dynamics_graph_arr;

static void global_pause_button_func (void);
static void pause_button_func (GtkWidget *btn_pause);
static void select_button_func (GtkWidget *btn_select);
static void combos_select_neuron_func(GtkWidget *changed_combo);

bool create_neuron_dynamics_view_gui(void)
{
	unsigned int i;
	GtkWidget *frame, *frame_label, *vbox, *vbox1, *hbox, *hbox1;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Neuron Dynamics     ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (get_gui_tabs()), frame, frame_label);  

 	vbox = gtk_vbox_new(FALSE, 0);
        gtk_container_add (GTK_CONTAINER (frame), vbox);

	btn_pause_arr = g_new0(GtkWidget*, NUM_OF_NEURON_DYNAMICS_GRAPHS);
	btn_select_arr = g_new0(GtkWidget*, NUM_OF_NEURON_DYNAMICS_GRAPHS);
	combos_select_neuron_arr = g_new0(LayerNrnGrpNeuronCombo*, NUM_OF_NEURON_DYNAMICS_GRAPHS);
	combo_neuron_dynamics_arr= g_new0(NeuronDynamicsCombo*, NUM_OF_NEURON_DYNAMICS_GRAPHS);
	neuron_dynamics_graph_arr = g_new0(NeuronDynamicsGraphScrollLimited*, NUM_OF_NEURON_DYNAMICS_GRAPHS);

	btn_global_pause = gtk_button_new_with_label("Paused");
	gtk_box_pack_start (GTK_BOX (vbox), btn_global_pause, FALSE, FALSE, 0);
	gtk_widget_set_sensitive(btn_global_pause, FALSE);			

	for (i = 0; i < NUM_OF_NEURON_DYNAMICS_GRAPHS; i++)
	{
  		hbox = gtk_hbox_new(FALSE, 0);
     		gtk_box_pack_start(GTK_BOX(vbox),hbox, TRUE,TRUE,0);

		btn_pause_arr[i] = gtk_button_new_with_label("P");
		gtk_box_pack_start (GTK_BOX (hbox), btn_pause_arr[i], FALSE, FALSE, 0);

 		vbox1 = gtk_vbox_new(FALSE, 0);
		gtk_box_pack_start (GTK_BOX (hbox), vbox1, TRUE, TRUE, 0);

  		hbox = gtk_hbox_new(FALSE, 0);
     		gtk_box_pack_start(GTK_BOX(vbox1),hbox, FALSE,FALSE,0);
		combos_select_neuron_arr[i] = allocate_layer_neuron_group_neuron_combos();
        	gtk_box_pack_start(GTK_BOX(hbox), combos_select_neuron_arr[i]->combo_layer , FALSE,FALSE,0);
		gtk_widget_set_size_request(combos_select_neuron_arr[i]->combo_layer, 60, 25);	
        	gtk_box_pack_start(GTK_BOX(hbox), combos_select_neuron_arr[i]->combo_neuron_group, FALSE,FALSE,0);
		gtk_widget_set_size_request(combos_select_neuron_arr[i]->combo_neuron_group, 60, 25);	
        	gtk_box_pack_start(GTK_BOX(hbox), combos_select_neuron_arr[i]->combo_neuron , FALSE,FALSE,0);
		gtk_widget_set_size_request(combos_select_neuron_arr[i]->combo_neuron, 60, 25);	
		if(!update_texts_of_combos_when_add_remove(combos_select_neuron_arr[i], bmi_data->in_silico_network))
			return print_message(ERROR_MSG ,"HybridNetRLBMI", "NeuronDynamicsView", "create_neuron_dynamics_view_gui", "! update_texts_of_combos_when_add_remove().");	
  		hbox1 = gtk_hbox_new(FALSE, 0);
     		gtk_box_pack_start(GTK_BOX(hbox),hbox1, FALSE,FALSE,0);
		combo_neuron_dynamics_arr[i] = allocate_neuron_dynamics_combo(hbox1, combo_neuron_dynamics_arr[i]);
		btn_select_arr[i] = gtk_button_new_with_label("Select");
		gtk_box_pack_start (GTK_BOX (hbox), btn_select_arr[i], FALSE, FALSE, 0);
  		hbox = gtk_hbox_new(TRUE, 0);
    		gtk_box_pack_start(GTK_BOX(vbox1),hbox, TRUE,TRUE,0);
		neuron_dynamics_graph_arr[i] = allocate_neuron_dynamics_graph_scroll_limited(hbox, neuron_dynamics_graph_arr[i], GRAPH_LENGTHS/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, GRAPH_SCROLL_LENGTHS/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, BUFFER_FOLLOWUP_LATENCY, NUM_OF_STATUS_MARKERS, bmi_data->trial_status_events, bmi_data->neuron_dynamics_limited_buffer, i);  // 100 ms latency
		g_signal_connect(G_OBJECT(combos_select_neuron_arr[i]->combo_layer), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron_arr[i]->combo_layer);
		g_signal_connect(G_OBJECT(combos_select_neuron_arr[i]->combo_neuron_group), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron_arr[i]->combo_neuron_group);	
		g_signal_connect(G_OBJECT(combos_select_neuron_arr[i]->combo_neuron), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron_arr[i]->combo_neuron);

		g_signal_connect(G_OBJECT(btn_pause_arr[i]), "clicked", G_CALLBACK(pause_button_func), btn_pause_arr[i]);
		g_signal_connect(G_OBJECT(btn_select_arr[i]), "clicked", G_CALLBACK(select_button_func), btn_select_arr[i]);
	}

	g_signal_connect(G_OBJECT(btn_global_pause), "clicked", G_CALLBACK(global_pause_button_func), NULL);
	gtk_widget_show_all(get_gui_tabs());
	return TRUE;
}

static void combos_select_neuron_func(GtkWidget *changed_combo)
{
	unsigned int i;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	for (i = 0; i < NUM_OF_NEURON_DYNAMICS_GRAPHS; i++)
	{
		if (combos_select_neuron_arr[i]->combo_layer == changed_combo)
			break;
		else if (combos_select_neuron_arr[i]->combo_neuron_group == changed_combo)
			break;
		else if (combos_select_neuron_arr[i]->combo_neuron == changed_combo)
			break;
	}
	if (i == NUM_OF_NEURON_DYNAMICS_GRAPHS)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NeuronDynamicsView", "combos_select_neuron_func", "i == NUM_OF_GRAPHS.");			
	if(!update_texts_of_combos_when_change(combos_select_neuron_arr[i], bmi_data->in_silico_network, changed_combo))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NeuronDynamicsView", "combos_select_neuron_func", "! update_texts_of_combos_when_change().");			
}

static void pause_button_func (GtkWidget *btn_pause)
{
	unsigned int i;
	for (i = 0; i < NUM_OF_NEURON_DYNAMICS_GRAPHS; i++)
	{
		if (btn_pause_arr[i] == btn_pause)
			break;
	}
	if (i == NUM_OF_NEURON_DYNAMICS_GRAPHS)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NeuronDynamicsView", "pause_button_func", "i == NUM_OF_GRAPHS.");	
	if (neuron_dynamics_graph_arr[i]->locally_paused)
	{
		if (send_neuron_dynamics_graph_resume_request_to_buffer_view_handler(i))   // should resume all graphs at the same time to provide sync
			gtk_button_set_label (GTK_BUTTON(btn_pause_arr[i]),"R");  
	}
	else
	{
		if (is_buffer_view_handler_paused())
			neuron_dynamics_graph_arr[i]->locally_paused = TRUE;
		else
			neuron_dynamics_graph_arr[i]->local_pause_request = TRUE;
		gtk_button_set_label (GTK_BUTTON(btn_pause_arr[i]),"P");		
	}
	return;
}

static void select_button_func (GtkWidget *btn_select)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	unsigned int layer_num, nrn_grp_num, nrn_num;
	int dynamics_type;
	unsigned int i;
	for (i = 0; i < NUM_OF_NEURON_DYNAMICS_GRAPHS; i++)
	{
		if (btn_select_arr[i] == btn_select)
			break;
	}
	if (i == NUM_OF_NEURON_DYNAMICS_GRAPHS)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NeuronDynamicsView", "select_button_func", "i == NUM_OF_NEURON_DYNAMICS_GRAPHS.");
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron_arr[i], &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "select_button_func", "! layer_neuron_group_neuron_get_selected().");	
	if (!neuron_dynamics_combo_get_selected(combo_neuron_dynamics_arr[i], &dynamics_type))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "select_button_func", "! neuron_dynamics_combo_get_selected().");	
	if (!submit_selected_neuron_to_neuron_dynamics_buffer_limited(bmi_data->in_silico_network, bmi_data->neuron_dynamics_limited_buffer, layer_num, nrn_grp_num, nrn_num, dynamics_type, i))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "select_button_func", "! submit_selected_neuron_to_neuron_dynamics_buffer_limited().");	
	return;
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

unsigned int get_num_neuron_dynamics_graphs_w_scroll(void)
{
	return  NUM_OF_NEURON_DYNAMICS_GRAPHS;
}

NeuronDynamicsGraphScrollLimited** get_neuron_dynamics_graphs_w_scroll_ptr(void)
{
	return neuron_dynamics_graph_arr;
}

void send_global_pause_button_sensitive_request(void)
{
	gtk_widget_set_sensitive(btn_global_pause, TRUE);			
}
