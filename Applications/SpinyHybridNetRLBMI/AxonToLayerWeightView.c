#include "AxonToLayerWeightView.h"



static GtkWidget *btn_refresh;
static GtkWidget **btn_select_arr;
static LayerNrnGrpNeuronCombo **combos_select_neuron_arr;
static SynapticWeightHistGraph **weight_hist_graph_arr;
static GtkWidget **entry_target_layer_arr;
static SynapticWeight	**mean_target_layer_weight_history_arr;


static void refresh_button_func (void);
static void select_button_func (GtkWidget *btn_select);
static void combos_select_neuron_func(GtkWidget *changed_combo);


bool create_axon_to_layer_weight_view_gui(void)
{
	unsigned int i;
	GtkWidget *frame, *frame_label, *vbox, *vbox1, *hbox, *lbl;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Axon to Layer Weights     ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (get_gui_tabs()), frame, frame_label);  

 	vbox = gtk_vbox_new(FALSE, 0);
        gtk_container_add (GTK_CONTAINER (frame), vbox);

	btn_refresh = gtk_button_new_with_label("Refresh");
	gtk_box_pack_start (GTK_BOX (vbox), btn_refresh, FALSE, FALSE, 0);

	combos_select_neuron_arr = g_new0(LayerNrnGrpNeuronCombo*, NUM_OF_AXON_TO_LAYER_GRAPHS);
	weight_hist_graph_arr = g_new0(SynapticWeightHistGraph*, NUM_OF_AXON_TO_LAYER_GRAPHS);
	btn_select_arr = g_new0(GtkWidget*, NUM_OF_AXON_TO_LAYER_GRAPHS);
	entry_target_layer_arr = g_new0(GtkWidget*, NUM_OF_AXON_TO_LAYER_GRAPHS);
	mean_target_layer_weight_history_arr = g_new0(SynapticWeight*, NUM_OF_AXON_TO_LAYER_GRAPHS);

	for (i = 0; i < NUM_OF_AXON_TO_LAYER_GRAPHS; i++)
	{
  		hbox = gtk_hbox_new(FALSE, 0);
     		gtk_box_pack_start(GTK_BOX(vbox),hbox, TRUE,TRUE,0);

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
		if(!update_texts_of_combos_when_add_remove(combos_select_neuron_arr[i], bmi_data->blue_spike_network))
			return print_message(ERROR_MSG ,"HybridNetRLBMI", "AxonToLayerView", "create_axon_to_layer_weight_view_gui", "! update_texts_of_combos_when_add_remove().");	

		lbl = gtk_label_new("  Target Layer: ");
		gtk_box_pack_start (GTK_BOX (hbox), lbl, FALSE, FALSE, 0);

		entry_target_layer_arr[i] =  gtk_entry_new();
       		gtk_box_pack_start(GTK_BOX(hbox), entry_target_layer_arr[i], FALSE,FALSE,0);
		gtk_widget_set_size_request(entry_target_layer_arr[i], 50, 25);	
		gtk_entry_set_text(GTK_ENTRY(entry_target_layer_arr[i]), "0");	

		mean_target_layer_weight_history_arr[i] = g_new0(SynapticWeight, SYNAPTIC_WEIGHT_HISTORY_BUFFER_SIZE);

		btn_select_arr[i] = gtk_button_new_with_label("Select");
		gtk_box_pack_start (GTK_BOX (hbox), btn_select_arr[i], FALSE, FALSE, 0);

  		hbox = gtk_hbox_new(TRUE, 0);
    		gtk_box_pack_start(GTK_BOX(vbox1),hbox, TRUE,TRUE,0);
		weight_hist_graph_arr[i] = allocate_synaptic_weight_history_graph(hbox, weight_hist_graph_arr[i]); 

		g_signal_connect(G_OBJECT(combos_select_neuron_arr[i]->combo_layer), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron_arr[i]->combo_layer);
		g_signal_connect(G_OBJECT(combos_select_neuron_arr[i]->combo_neuron_group), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron_arr[i]->combo_neuron_group);	
		g_signal_connect(G_OBJECT(combos_select_neuron_arr[i]->combo_neuron), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron_arr[i]->combo_neuron);

		g_signal_connect(G_OBJECT(btn_select_arr[i]), "clicked", G_CALLBACK(select_button_func), btn_select_arr[i]);
	}

	g_signal_connect(G_OBJECT(btn_refresh), "clicked", G_CALLBACK(refresh_button_func), NULL);

	gtk_widget_show_all(get_gui_tabs());
	return TRUE;
}

static void combos_select_neuron_func(GtkWidget *changed_combo)
{
	unsigned int i;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	for (i = 0; i < NUM_OF_AXON_TO_LAYER_GRAPHS; i++)
	{
		if (combos_select_neuron_arr[i]->combo_layer == changed_combo)
			break;
		else if (combos_select_neuron_arr[i]->combo_neuron_group == changed_combo)
			break;
		else if (combos_select_neuron_arr[i]->combo_neuron == changed_combo)
			break;
	}
	if (i == NUM_OF_AXON_TO_LAYER_GRAPHS)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "AxonToLayerView", "combos_select_neuron_func", "i == NUM_OF_GRAPHS.");			
	if(!update_texts_of_combos_when_change(combos_select_neuron_arr[i], bmi_data->blue_spike_network, changed_combo))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "AxonToLayerView", "combos_select_neuron_func", "! update_texts_of_combos_when_change().");			
}

static void select_button_func (GtkWidget *btn_select)
{
	Network *blue_spike_network = get_hybrid_net_rl_bmi_data()->blue_spike_network;
	unsigned int layer_num, nrn_grp_num, nrn_num, target_layer_num;
	unsigned int i;

	for (i = 0; i < NUM_OF_AXON_TO_LAYER_GRAPHS; i++)
	{
		if (btn_select_arr[i] == btn_select)
			break;
	}
	if (i == NUM_OF_AXON_TO_LAYER_GRAPHS)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "AxonToLayerView", "select_button_func", "i == NUM_OF_AXON_TO_LAYER_GRAPHS.");
	for (i = 0; i < NUM_OF_AXON_TO_LAYER_GRAPHS; i++)
	{
		if (! layer_neuron_group_neuron_get_selected(combos_select_neuron_arr[i], &layer_num, &nrn_grp_num, &nrn_num))
			return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "AxonToLayerView", "select_button_func", "! layer_neuron_group_neuron_get_selected().");	
		target_layer_num = (unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_target_layer_arr[i])));
		update_axon_to_layer_wieght_history_graph(weight_hist_graph_arr[i], blue_spike_network, layer_num, nrn_grp_num, nrn_num, target_layer_num, mean_target_layer_weight_history_arr[i]);

	}
	return;
}

static void refresh_button_func (void)
{
	Network *blue_spike_network = get_hybrid_net_rl_bmi_data()->blue_spike_network;
	unsigned int layer_num, nrn_grp_num, nrn_num, target_layer_num;
	unsigned int i;

	for (i = 0; i < NUM_OF_AXON_TO_LAYER_GRAPHS; i++)
	{
		if (! layer_neuron_group_neuron_get_selected(combos_select_neuron_arr[i], &layer_num, &nrn_grp_num, &nrn_num))
			return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "AxonToLayerView", "select_button_func", "! layer_neuron_group_neuron_get_selected().");	
		target_layer_num = (unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_target_layer_arr[i])));
		update_axon_to_layer_wieght_history_graph(weight_hist_graph_arr[i], blue_spike_network, layer_num, nrn_grp_num, nrn_num, target_layer_num, mean_target_layer_weight_history_arr[i]);

	}
	return;
}

