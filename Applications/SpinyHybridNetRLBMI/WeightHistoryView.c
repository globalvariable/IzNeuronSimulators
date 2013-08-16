#include "WeightHistoryView.h"



static GtkWidget *btn_refresh;
static GtkWidget **btn_select_arr;
static GtkWidget **lbl_layer_arr;
static GtkWidget **lbl_neuron_group_arr;
static GtkWidget **lbl_neuron_arr;
static LayerNrnGrpNrnSynapseCombo **combos_select_synapse_arr;
static SynapticWeightHistGraph **weight_hist_graph_arr;

static void refresh_button_func (void);
static void select_button_func (GtkWidget *btn_select);
static void combos_select_synapse_func(GtkWidget *changed_combo);


bool create_synaptic_wegiht_history_view_gui(void)
{
	unsigned int i;
	GtkWidget *frame, *frame_label, *vbox, *vbox1, *hbox, *lbl;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Synaptic Weights     ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (get_gui_tabs()), frame, frame_label);  

 	vbox = gtk_vbox_new(FALSE, 0);
        gtk_container_add (GTK_CONTAINER (frame), vbox);

	btn_refresh = gtk_button_new_with_label("Refresh");
	gtk_box_pack_start (GTK_BOX (vbox), btn_refresh, FALSE, FALSE, 0);

	combos_select_synapse_arr = g_new0(LayerNrnGrpNrnSynapseCombo*, NUM_OF_WEIGHT_HISTORY_GRAPHS);
	weight_hist_graph_arr = g_new0(SynapticWeightHistGraph*, NUM_OF_WEIGHT_HISTORY_GRAPHS);
	btn_select_arr = g_new0(GtkWidget*, NUM_OF_WEIGHT_HISTORY_GRAPHS);
	lbl_layer_arr = g_new0(GtkWidget*, NUM_OF_WEIGHT_HISTORY_GRAPHS);
	lbl_neuron_group_arr = g_new0(GtkWidget*, NUM_OF_WEIGHT_HISTORY_GRAPHS);
	lbl_neuron_arr = g_new0(GtkWidget*, NUM_OF_WEIGHT_HISTORY_GRAPHS);
	for (i = 0; i < NUM_OF_WEIGHT_HISTORY_GRAPHS; i++)
	{
  		hbox = gtk_hbox_new(FALSE, 0);
     		gtk_box_pack_start(GTK_BOX(vbox),hbox, TRUE,TRUE,0);

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
			return print_message(ERROR_MSG ,"HybridNetRLBMI", "WeightHistoryView", "create_synaptic_wegiht_history_view_gui", "! update_texts_of_combos_when_add_remove().");	
		btn_select_arr[i] = gtk_button_new_with_label("Select");
		gtk_box_pack_start (GTK_BOX (hbox), btn_select_arr[i], FALSE, FALSE, 0);
		lbl = gtk_label_new("      ");
		gtk_box_pack_start (GTK_BOX (hbox), lbl, FALSE, FALSE, 0);
		lbl_layer_arr[i] = gtk_label_new("N/A");
		gtk_box_pack_start (GTK_BOX (hbox), lbl_layer_arr[i], FALSE, FALSE, 0);
		lbl = gtk_label_new(":");
		gtk_box_pack_start (GTK_BOX (hbox), lbl, FALSE, FALSE, 0);
		lbl_neuron_group_arr[i] = gtk_label_new("N/A");
		gtk_box_pack_start (GTK_BOX (hbox), lbl_neuron_group_arr[i], FALSE, FALSE, 0);
		lbl = gtk_label_new(":");
		gtk_box_pack_start (GTK_BOX (hbox), lbl, FALSE, FALSE, 0);
		lbl_neuron_arr[i] = gtk_label_new("N/A");
		gtk_box_pack_start (GTK_BOX (hbox), lbl_neuron_arr[i], FALSE, FALSE, 0);

  		hbox = gtk_hbox_new(TRUE, 0);
    		gtk_box_pack_start(GTK_BOX(vbox1),hbox, TRUE,TRUE,0);
		weight_hist_graph_arr[i] = allocate_synaptic_weight_history_graph(hbox, weight_hist_graph_arr[i]); 

		g_signal_connect(G_OBJECT(combos_select_synapse_arr[i]->combo_layer), "changed", G_CALLBACK(combos_select_synapse_func), combos_select_synapse_arr[i]->combo_layer);
		g_signal_connect(G_OBJECT(combos_select_synapse_arr[i]->combo_neuron_group), "changed", G_CALLBACK(combos_select_synapse_func), combos_select_synapse_arr[i]->combo_neuron_group);	
		g_signal_connect(G_OBJECT(combos_select_synapse_arr[i]->combo_neuron), "changed", G_CALLBACK(combos_select_synapse_func), combos_select_synapse_arr[i]->combo_neuron);
		g_signal_connect(G_OBJECT(combos_select_synapse_arr[i]->combo_synapse), "changed", G_CALLBACK(combos_select_synapse_func), combos_select_synapse_arr[i]->combo_synapse);

		g_signal_connect(G_OBJECT(btn_select_arr[i]), "clicked", G_CALLBACK(select_button_func), btn_select_arr[i]);
	}

	g_signal_connect(G_OBJECT(btn_refresh), "clicked", G_CALLBACK(refresh_button_func), NULL);

	gtk_widget_show_all(get_gui_tabs());
	return TRUE;
}

static void combos_select_synapse_func(GtkWidget *changed_combo)
{
	unsigned int i;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	for (i = 0; i < NUM_OF_WEIGHT_HISTORY_GRAPHS; i++)
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
	if (i == NUM_OF_WEIGHT_HISTORY_GRAPHS)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "WeightHistoryView", "combos_select_neuron_func", "i == NUM_OF_GRAPHS.");			
	if(!update_texts_of_synapse_combos_when_change(combos_select_synapse_arr[i], bmi_data->in_silico_network, changed_combo))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "WeightHistoryView", "combos_select_neuron_func", "! update_texts_of_combos_when_change().");			
}

static void select_button_func (GtkWidget *btn_select)
{
	Network *in_silico_network = get_hybrid_net_rl_bmi_data()->in_silico_network;
	unsigned int layer_num, nrn_grp_num, nrn_num, syn_num;
	unsigned int i;
	Neuron *neuron;
	char temp[10];
	for (i = 0; i < NUM_OF_WEIGHT_HISTORY_GRAPHS; i++)
	{
		if (btn_select_arr[i] == btn_select)
			break;
	}
	if (i == NUM_OF_WEIGHT_HISTORY_GRAPHS)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "WeightHistoryView", "select_button_func", "i == NUM_OF_WEIGHT_HISTORY_GRAPHS.");
	for (i = 0; i < NUM_OF_WEIGHT_HISTORY_GRAPHS; i++)
	{
		if (! layer_neuron_group_neuron_synapse_get_selected(combos_select_synapse_arr[i], &layer_num, &nrn_grp_num, &nrn_num, &syn_num))
			return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "WeightHistoryView", "select_button_func", "! layer_neuron_group_neuron_synapse_get_selected().");	
		update_synapse_history_graph(weight_hist_graph_arr[i], in_silico_network, layer_num, nrn_grp_num, nrn_num, syn_num);

		neuron = get_neuron_address(in_silico_network, layer_num, nrn_grp_num, nrn_num);					

		sprintf (temp, "%u", neuron->syn_list->synapses[syn_num].axon_from_layer);
		gtk_label_set_text (GTK_LABEL (lbl_layer_arr[i]), temp);
		sprintf (temp, "%u", neuron->syn_list->synapses[syn_num].axon_from_neuron_group);
		gtk_label_set_text (GTK_LABEL (lbl_neuron_group_arr[i]), temp);
		sprintf (temp, "%u", neuron->syn_list->synapses[syn_num].axon_from_neuron);
		gtk_label_set_text (GTK_LABEL (lbl_neuron_arr[i]), temp);
	}
	return;
}

static void refresh_button_func (void)
{
	Network *in_silico_network = get_hybrid_net_rl_bmi_data()->in_silico_network;
	unsigned int layer_num, nrn_grp_num, nrn_num, syn_num;
	unsigned int i;
	Neuron *neuron;
	char temp[10];
	for (i = 0; i < NUM_OF_WEIGHT_HISTORY_GRAPHS; i++)
	{
		if (! layer_neuron_group_neuron_synapse_get_selected(combos_select_synapse_arr[i], &layer_num, &nrn_grp_num, &nrn_num, &syn_num))
			return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "WeightHistoryView", "select_button_func", "! layer_neuron_group_neuron_synapse_get_selected().");	
		update_synapse_history_graph(weight_hist_graph_arr[i], in_silico_network, layer_num, nrn_grp_num, nrn_num, syn_num);

		neuron = get_neuron_address(in_silico_network, layer_num, nrn_grp_num, nrn_num);					

		sprintf (temp, "%u", neuron->syn_list->synapses[syn_num].axon_from_layer);
		gtk_label_set_text (GTK_LABEL (lbl_layer_arr[i]), temp);
		sprintf (temp, "%u", neuron->syn_list->synapses[syn_num].axon_from_neuron_group);
		gtk_label_set_text (GTK_LABEL (lbl_neuron_group_arr[i]), temp);
		sprintf (temp, "%u", neuron->syn_list->synapses[syn_num].axon_from_neuron);
		gtk_label_set_text (GTK_LABEL (lbl_neuron_arr[i]), temp);
	}
	return;
}

