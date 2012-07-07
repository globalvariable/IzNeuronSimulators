#include "EligibilityView.h"


static GtkWidget **btn_pause_arr;
static GtkWidget **btn_select_arr;
static LayerNrnGrpNrnSynapseCombo **combos_select_synapse_arr;
static EligibilityGraphScrollLimited **eligibility_graph_arr;

static void pause_button_func (GtkWidget *btn_pause);
static void select_button_func (GtkWidget *btn_select);
static void combos_select_synapse_func(GtkWidget *changed_combo);

bool create_eligibility_view_gui(void)
{
	unsigned int i;
	GtkWidget *frame, *frame_label, *vbox, *vbox1, *hbox;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Eligibility     ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (get_gui_tabs()), frame, frame_label);  

 	vbox = gtk_vbox_new(FALSE, 0);
        gtk_container_add (GTK_CONTAINER (frame), vbox);

	btn_pause_arr = g_new0(GtkWidget*, NUM_OF_ELIGIBILITY_GRAPHS);
	btn_select_arr = g_new0(GtkWidget*, NUM_OF_ELIGIBILITY_GRAPHS);
	combos_select_synapse_arr = g_new0(LayerNrnGrpNrnSynapseCombo*, NUM_OF_ELIGIBILITY_GRAPHS);
	eligibility_graph_arr = g_new0(EligibilityGraphScrollLimited*, NUM_OF_ELIGIBILITY_GRAPHS);

	for (i = 0; i < NUM_OF_ELIGIBILITY_GRAPHS; i++)
	{
  		hbox = gtk_hbox_new(FALSE, 0);
     		gtk_box_pack_start(GTK_BOX(vbox),hbox, TRUE,TRUE,0);

		btn_pause_arr[i] = gtk_button_new_with_label("P");
		gtk_box_pack_start (GTK_BOX (hbox), btn_pause_arr[i], FALSE, FALSE, 0);

 		vbox1 = gtk_vbox_new(FALSE, 0);
		gtk_box_pack_start (GTK_BOX (hbox), vbox1, TRUE, TRUE, 0);

  		hbox = gtk_hbox_new(FALSE, 0);
     		gtk_box_pack_start(GTK_BOX(vbox1),hbox, FALSE,FALSE,0);
		combos_select_synapse_arr[i] = allocate_layer_neuron_group_neuron_synapse_combos();
        	gtk_box_pack_start(GTK_BOX(hbox), combos_select_synapse_arr[i]->combo_layer , FALSE,FALSE,0);
		gtk_widget_set_size_request(combos_select_synapse_arr[i]->combo_layer, 60, 25);	
        	gtk_box_pack_start(GTK_BOX(hbox), combos_select_synapse_arr[i]->combo_neuron_group, FALSE,FALSE,0);
		gtk_widget_set_size_request(combos_select_synapse_arr[i]->combo_neuron_group, 60, 25);	
        	gtk_box_pack_start(GTK_BOX(hbox), combos_select_synapse_arr[i]->combo_neuron , FALSE,FALSE,0);
		gtk_widget_set_size_request(combos_select_synapse_arr[i]->combo_neuron, 60, 25);	
        	gtk_box_pack_start(GTK_BOX(hbox), combos_select_synapse_arr[i]->combo_synapse , FALSE,FALSE,0);
		gtk_widget_set_size_request(combos_select_synapse_arr[i]->combo_synapse, 60, 25);	
		if(!update_texts_of_synapse_combos_when_add_remove(combos_select_synapse_arr[i], bmi_data->in_silico_network))
			return print_message(ERROR_MSG ,"HybridNetRLBMI", "EligibilityView", "create_eligibility_view_gui", "! update_texts_of_combos_when_add_remove().");	
		btn_select_arr[i] = gtk_button_new_with_label("Select");
		gtk_box_pack_start (GTK_BOX (hbox), btn_select_arr[i], FALSE, FALSE, 0);
  		hbox = gtk_hbox_new(TRUE, 0);
    		gtk_box_pack_start(GTK_BOX(vbox1),hbox, TRUE,TRUE,0);
		eligibility_graph_arr[i] = allocate_eligibility_graph_scroll_limited(hbox, eligibility_graph_arr[i], GRAPH_LENGTHS/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, GRAPH_SCROLL_LENGTHS/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, BUFFER_FOLLOWUP_LATENCY, NUM_OF_STATUS_MARKERS, bmi_data->trial_status_events, bmi_data->eligibility_limited_buffer, i);  // 100 ms latency
		g_signal_connect(G_OBJECT(combos_select_synapse_arr[i]->combo_layer), "changed", G_CALLBACK(combos_select_synapse_func), combos_select_synapse_arr[i]->combo_layer);
		g_signal_connect(G_OBJECT(combos_select_synapse_arr[i]->combo_neuron_group), "changed", G_CALLBACK(combos_select_synapse_func), combos_select_synapse_arr[i]->combo_neuron_group);	
		g_signal_connect(G_OBJECT(combos_select_synapse_arr[i]->combo_neuron), "changed", G_CALLBACK(combos_select_synapse_func), combos_select_synapse_arr[i]->combo_neuron);
		g_signal_connect(G_OBJECT(combos_select_synapse_arr[i]->combo_synapse), "changed", G_CALLBACK(combos_select_synapse_func), combos_select_synapse_arr[i]->combo_synapse);

		g_signal_connect(G_OBJECT(btn_pause_arr[i]), "clicked", G_CALLBACK(pause_button_func), btn_pause_arr[i]);
		g_signal_connect(G_OBJECT(btn_select_arr[i]), "clicked", G_CALLBACK(select_button_func), btn_select_arr[i]);
	}

	gtk_widget_show_all(get_gui_tabs());
	return TRUE;
}

static void combos_select_synapse_func(GtkWidget *changed_combo)
{
	unsigned int i;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	for (i = 0; i < NUM_OF_ELIGIBILITY_GRAPHS; i++)
	{
		if (combos_select_synapse_arr[i]->combo_layer == changed_combo)
			break;
		else if (combos_select_synapse_arr[i]->combo_neuron_group == changed_combo)
			break;
		else if (combos_select_synapse_arr[i]->combo_neuron == changed_combo)
			break;
		else if (combos_select_synapse_arr[i]->combo_synapse == changed_combo)
			break;
	}
	if (i == NUM_OF_ELIGIBILITY_GRAPHS)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "EligibilityView", "combos_select_neuron_func", "i == NUM_OF_GRAPHS.");			
	if(!update_texts_of_synapse_combos_when_change(combos_select_synapse_arr[i], bmi_data->in_silico_network, changed_combo))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "EligibilityView", "combos_select_neuron_func", "! update_texts_of_combos_when_change().");			
}

static void pause_button_func (GtkWidget *btn_pause)
{
	unsigned int i;
	for (i = 0; i < NUM_OF_ELIGIBILITY_GRAPHS; i++)
	{
		if (btn_pause_arr[i] == btn_pause)
			break;
	}
	if (i == NUM_OF_ELIGIBILITY_GRAPHS)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "EligibilityView", "pause_button_func", "i == NUM_OF_GRAPHS.");	
	if (eligibility_graph_arr[i]->locally_paused)
	{
		if (send_eligibility_graph_resume_request_to_buffer_view_handler(i))   // should resume all graphs at the same time to provide sync
			gtk_button_set_label (GTK_BUTTON(btn_pause_arr[i]),"R");  
	}
	else
	{
		if (is_buffer_view_handler_paused())
			eligibility_graph_arr[i]->locally_paused = TRUE;
		else
			eligibility_graph_arr[i]->local_pause_request = TRUE;
		gtk_button_set_label (GTK_BUTTON(btn_pause_arr[i]),"P");		
	}
	return;
}

static void select_button_func (GtkWidget *btn_select)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	unsigned int layer_num, nrn_grp_num, nrn_num, syn_num;
	unsigned int i;
	for (i = 0; i < NUM_OF_ELIGIBILITY_GRAPHS; i++)
	{
		if (btn_select_arr[i] == btn_select)
			break;
	}
	if (i == NUM_OF_ELIGIBILITY_GRAPHS)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "EligibilityView", "select_button_func", "i == NUM_OF_ELIGIBILITY_GRAPHS.");
	if (! layer_neuron_group_neuron_synapse_get_selected(combos_select_synapse_arr[i], &layer_num, &nrn_grp_num, &nrn_num, &syn_num))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "EligibilityView", "select_button_func", "! layer_neuron_group_neuron_synapse_get_selected().");	
	if (!submit_selected_synapse_to_eligibility_buffer_limited(bmi_data->in_silico_network, bmi_data->eligibility_limited_buffer, layer_num, nrn_grp_num, nrn_num, syn_num, i))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "EligibilityView", "select_button_func", "! submit_selected_neuron_to_eligibility_buffer_limited().");	
	return;
}

unsigned int get_num_eligibility_graphs_w_scroll(void)
{
	return  NUM_OF_ELIGIBILITY_GRAPHS;
}

EligibilityGraphScrollLimited** get_eligibility_graphs_w_scroll_ptr(void)
{
	return eligibility_graph_arr;
}

