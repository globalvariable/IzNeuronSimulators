#include "NetworkView.h"

// FIRST COLUMN
static GtkWidget *combo_neuron_type;
static GtkWidget *entry_a;
static GtkWidget *entry_b;
static GtkWidget *entry_c;
static GtkWidget *entry_d;
static GtkWidget *entry_k;
static GtkWidget *entry_C;
static GtkWidget *entry_v_threshold;
static GtkWidget *entry_v_peak;	
static GtkWidget *entry_v_resting;
static GtkWidget *entry_inhibitory;
static GtkWidget *entry_E_excitatory;
static GtkWidget *entry_E_inhibitory;
static GtkWidget *entry_tau_excitatory;  
static GtkWidget *entry_tau_inhibitory;
static GtkWidget *entry_num_of_neuron_for_group;
static GtkWidget *btn_add_neurons_to_layer;
static GtkWidget *entry_add_neurons_to_layer;

static GtkWidget *btn_interrogate_network;
static GtkWidget *btn_interrogate_neuron;

static GtkWidget *entry_parker_sochacki_err_tol;
static GtkWidget *entry_parker_sochacki_max_order;
static GtkWidget *btn_submit_parker_sochacki_params;

static GtkWidget *entry_internal_neurons_excitatory_connection_probability;
static GtkWidget *entry_internal_neurons_weight_excitatory_min;
static GtkWidget *entry_internal_neurons_weight_excitatory_max;
static GtkWidget *entry_internal_neurons_excitatory_delay_min;
static GtkWidget *entry_internal_neurons_excitatory_delay_max;
static GtkWidget *entry_internal_neurons_inhibitory_connection_probability;
static GtkWidget *entry_internal_neurons_weight_inhibitory_min;
static GtkWidget *entry_internal_neurons_weight_inhibitory_max;
static GtkWidget *entry_internal_neurons_inhibitory_delay_min;
static GtkWidget *entry_internal_neurons_inhibitory_delay_max;

static GtkWidget *btn_connect_internal_layer_to_internal_layer;
static GtkWidget *entry_internal_layer_num_to_connect;
static GtkWidget *entry_internal_layer_num_to_connect_internal_layer_to;

static GtkWidget *entry_external_neurons_excitatory_connection_probability;
static GtkWidget *entry_external_neurons_weight_excitatory_min;
static GtkWidget *entry_external_neurons_weight_excitatory_max;
static GtkWidget *entry_external_neurons_excitatory_delay_min;
static GtkWidget *entry_external_neurons_excitatory_delay_max;
static GtkWidget *entry_external_neurons_inhibitory_connection_probability;
static GtkWidget *entry_external_neurons_weight_inhibitory_min;
static GtkWidget *entry_external_neurons_weight_inhibitory_max;
static GtkWidget *entry_external_neurons_inhibitory_delay_min;
static GtkWidget *entry_external_neurons_inhibitory_delay_max;

static GtkWidget *btn_connect_external_layer_to_internal_layer;
static GtkWidget *entry_external_layer_num_to_connect;
static GtkWidget *entry_internal_layer_num_to_connect_external_layer_to;

// SECOND COLUMN
static LayerNrnGrpNeuronCombo *combos_select_neuron;
static NeuronDynamicsCombo *combo_neuron_dynamics = NULL;
static GtkWidget *entry_I_inject;
static GtkWidget *btn_submit_injection_current;

static GtkWidget *entry_simulation_length;

static GtkWidget *btn_simulate_with_no_reward;
static GtkWidget *btn_simulate_with_reward;
static GtkWidget *btn_simulate_with_punishment;

// GRAPHS
static NeuronDynamicsGraph *neuron_dynamics_graph = NULL;

// FIRST COLUMN
static void combo_neuron_type_func (void);
static void add_neurons_to_layer_button_func(void);
static void interrogate_network_button_func(void); 		
static void interrogate_neuron_button_func(void);
static void set_neuron_param_entries(int neuron_type);
static void submit_parker_sochacki_params_button_func(void);

static void connect_internal_layer_to_internal_layer_button_func(void);
static void connect_external_layer_to_internal_layer_button_func(void);
// SECOND COLUMN
static void combos_select_neuron_func(GtkWidget *changed_combo);
static void submit_injection_current_button_func(void);
static void simulate_with_no_reward_button_func(void);

bool create_network_view_gui(void)
{
	GtkWidget *frame, *frame_label, *table, *vbox, *hbox, *lbl;

        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Network & Reward     ");      
   
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

	combo_neuron_type = gtk_combo_box_new_text();
        gtk_box_pack_start(GTK_BOX(hbox),combo_neuron_type, TRUE,TRUE,0);
	gtk_widget_set_size_request(combo_neuron_type, 180, 30) ;
	
	int i; char neuron_type_str[50];
	for (i = 0; i < MAX_NUM_OF_NEURON_TYPES; i++)
	{
		gtk_combo_box_append_text(GTK_COMBO_BOX(combo_neuron_type), get_neuron_type_string(i, neuron_type_str));
	}

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("a: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_a = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_a, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_a, 50, 25) ;

	lbl = gtk_label_new(" b: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_b = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_b, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_b, 50, 25) ;

	lbl = gtk_label_new(" c: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_c = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_c, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_c, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("d: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_d = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_d, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_d, 50, 25) ;

	lbl = gtk_label_new(" k: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_k = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_k, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_k, 50, 25) ;

	lbl = gtk_label_new(" C: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_C = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_C, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_C, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("v_thres: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_v_threshold = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_v_threshold, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_v_threshold, 50, 25) ;

	lbl = gtk_label_new(" v_peak: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_v_peak = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_v_peak, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_v_peak, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("v_rest  : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_v_resting = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_v_resting, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_v_resting, 50, 25) ;

	lbl = gtk_label_new(" inhibit : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_inhibitory = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_inhibitory, FALSE,FALSE,0);
	gtk_entry_set_max_length (GTK_ENTRY(entry_inhibitory), 1);
	gtk_widget_set_size_request(entry_inhibitory, 50, 25) ;
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("E_Excite: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_E_excitatory = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_E_excitatory, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_E_excitatory, 50, 25) ;	
	
	lbl = gtk_label_new(" E_Inhibit: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_E_inhibitory = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_E_inhibitory, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_E_inhibitory, 50, 25) ;	
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Tau_Exc : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_tau_excitatory = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_tau_excitatory, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_tau_excitatory, 50, 25) ;	
	
	lbl = gtk_label_new(" Tau_Inh  : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_tau_inhibitory = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_tau_inhibitory, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_tau_inhibitory, 50, 25) ;	

 	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_neuron_type), 1);	
	set_neuron_param_entries(1);			

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Num of Neurons to Add:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
       	entry_num_of_neuron_for_group = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_num_of_neuron_for_group, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_num_of_neuron_for_group), "1");
	gtk_widget_set_size_request(entry_num_of_neuron_for_group, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_add_neurons_to_layer = gtk_button_new_with_label("Add Neurons to Layer");
	gtk_box_pack_start (GTK_BOX (hbox), btn_add_neurons_to_layer, TRUE, TRUE, 0);

        entry_add_neurons_to_layer = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_add_neurons_to_layer, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_add_neurons_to_layer), "0");
	gtk_widget_set_size_request(entry_add_neurons_to_layer, 50, 25) ;	

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);        
  
	lbl = gtk_label_new("Error. Tol:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
                
        entry_parker_sochacki_err_tol= gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_parker_sochacki_err_tol, FALSE,FALSE,0);
	char temp_str[40];
      	sprintf(temp_str, "%.1E", 0.0);	
	gtk_entry_set_text(GTK_ENTRY(entry_parker_sochacki_err_tol), temp_str);
	gtk_widget_set_size_request(entry_parker_sochacki_err_tol, 65, 25) ;
	lbl = gtk_label_new("Max Order:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_parker_sochacki_max_order= gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_parker_sochacki_max_order, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_parker_sochacki_max_order), "40");
	gtk_widget_set_size_request(entry_parker_sochacki_max_order, 30, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);        
  
	btn_submit_parker_sochacki_params = gtk_button_new_with_label("Submit Parker-Sochacki Params");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_parker_sochacki_params, TRUE, TRUE, 0);
	
        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);	

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_interrogate_network = gtk_button_new_with_label("Interrogate Network");
	gtk_box_pack_start (GTK_BOX (hbox), btn_interrogate_network, TRUE, TRUE, 0);	

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_interrogate_neuron = gtk_button_new_with_label("Interrogate Neuron");
	gtk_box_pack_start (GTK_BOX (hbox), btn_interrogate_neuron, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);	

	///////////////  CONNECT INTERNAL NEURONS

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
 	lbl = gtk_label_new("---------- Internal Network ----------");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);  

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

        lbl = gtk_label_new(" ----- EPSP -----");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Connection Probability:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_internal_neurons_excitatory_connection_probability = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_excitatory_connection_probability, FALSE,FALSE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_excitatory_connection_probability), "1.0");
	gtk_widget_set_size_request(entry_internal_neurons_excitatory_connection_probability, 50, 25) ;	
	
	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
        lbl = gtk_label_new("Weight:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        lbl = gtk_label_new("Min:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_weight_excitatory_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_weight_excitatory_min, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_weight_excitatory_min), "0.001");
	gtk_widget_set_size_request(entry_internal_neurons_weight_excitatory_min, 50, 25) ;	
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_weight_excitatory_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_weight_excitatory_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_weight_excitatory_max), "1.0");
	gtk_widget_set_size_request(entry_internal_neurons_weight_excitatory_max, 50, 25) ;	
	
 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Delay(ms):");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	lbl = gtk_label_new("Min:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_excitatory_delay_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_excitatory_delay_min , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_excitatory_delay_min), "1.0");
	gtk_widget_set_size_request(entry_internal_neurons_excitatory_delay_min, 50, 25) ;
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_excitatory_delay_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_excitatory_delay_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_excitatory_delay_max), "5.0");	
	gtk_widget_set_size_request(entry_internal_neurons_excitatory_delay_max, 50, 25) ;
		
 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
 	lbl = gtk_label_new("----- IPSP -----");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);       

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Connection Probability:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);	
        entry_internal_neurons_inhibitory_connection_probability = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_inhibitory_connection_probability, FALSE,FALSE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_inhibitory_connection_probability), "1.0");
	gtk_widget_set_size_request(entry_internal_neurons_inhibitory_connection_probability, 50, 25) ;	

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

        lbl = gtk_label_new("Weight:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        lbl = gtk_label_new("Min:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_weight_inhibitory_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_weight_inhibitory_min, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_weight_inhibitory_min), "0.001");
	gtk_widget_set_size_request(entry_internal_neurons_weight_inhibitory_min, 50, 25) ;	
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_weight_inhibitory_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_weight_inhibitory_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_weight_inhibitory_max), "1.0");
	gtk_widget_set_size_request(entry_internal_neurons_weight_inhibitory_max, 50, 25) ;	
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Delay(ms):");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	lbl = gtk_label_new("Min:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_inhibitory_delay_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_inhibitory_delay_min , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_inhibitory_delay_min), "1.0");
	gtk_widget_set_size_request(entry_internal_neurons_inhibitory_delay_min, 50, 25) ;
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_inhibitory_delay_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_inhibitory_delay_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_inhibitory_delay_max), "5.0");	
	gtk_widget_set_size_request(entry_internal_neurons_inhibitory_delay_max, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_connect_internal_layer_to_internal_layer = gtk_button_new_with_label("Connect Internal Layer");
	gtk_box_pack_start (GTK_BOX (hbox), btn_connect_internal_layer_to_internal_layer, FALSE, FALSE, 0);
        entry_internal_layer_num_to_connect= gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_layer_num_to_connect, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_layer_num_to_connect), "0");
	gtk_widget_set_size_request(entry_internal_layer_num_to_connect, 30, 25);		
	lbl = gtk_label_new("to");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);	
        entry_internal_layer_num_to_connect_internal_layer_to = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_layer_num_to_connect_internal_layer_to, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_layer_num_to_connect_internal_layer_to), "0");	
	gtk_widget_set_size_request(entry_internal_layer_num_to_connect_internal_layer_to, 30, 25);		
	
	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);  	

	///////////////  CONNECT EXTERNAL NEURONS

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
 	lbl = gtk_label_new("---------- External Network ----------");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);  

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

        lbl = gtk_label_new(" ----- EPSP -----");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Connection Probability:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_external_neurons_excitatory_connection_probability = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_excitatory_connection_probability, FALSE,FALSE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_excitatory_connection_probability), "1.0");
	gtk_widget_set_size_request(entry_external_neurons_excitatory_connection_probability, 50, 25) ;	
	
	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
        lbl = gtk_label_new("Weight:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        lbl = gtk_label_new("Min:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_weight_excitatory_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_weight_excitatory_min, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_weight_excitatory_min), "0.001");
	gtk_widget_set_size_request(entry_external_neurons_weight_excitatory_min, 50, 25) ;	
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_weight_excitatory_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_weight_excitatory_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_weight_excitatory_max), "1.0");
	gtk_widget_set_size_request(entry_external_neurons_weight_excitatory_max, 50, 25) ;	
	
 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Delay(ms):");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	lbl = gtk_label_new("Min:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_excitatory_delay_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_excitatory_delay_min , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_excitatory_delay_min), "1.0");
	gtk_widget_set_size_request(entry_external_neurons_excitatory_delay_min, 50, 25) ;
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_excitatory_delay_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_excitatory_delay_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_excitatory_delay_max), "5.0");	
	gtk_widget_set_size_request(entry_external_neurons_excitatory_delay_max, 50, 25) ;
		
 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
 	lbl = gtk_label_new("----- IPSP -----");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);       

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Connection Probability:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);	
        entry_external_neurons_inhibitory_connection_probability = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_inhibitory_connection_probability, FALSE,FALSE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_inhibitory_connection_probability), "1.0");
	gtk_widget_set_size_request(entry_external_neurons_inhibitory_connection_probability, 50, 25) ;	

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

        lbl = gtk_label_new("Weight:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        lbl = gtk_label_new("Min:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_weight_inhibitory_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_weight_inhibitory_min, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_weight_inhibitory_min), "0.001");
	gtk_widget_set_size_request(entry_external_neurons_weight_inhibitory_min, 50, 25) ;	
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_weight_inhibitory_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_weight_inhibitory_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_weight_inhibitory_max), "1.0");
	gtk_widget_set_size_request(entry_external_neurons_weight_inhibitory_max, 50, 25) ;	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Delay(ms):");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	lbl = gtk_label_new("Min:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_inhibitory_delay_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_inhibitory_delay_min , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_inhibitory_delay_min), "1.0");
	gtk_widget_set_size_request(entry_external_neurons_inhibitory_delay_min, 50, 25) ;
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_inhibitory_delay_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_inhibitory_delay_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_inhibitory_delay_max), "5.0");	
	gtk_widget_set_size_request(entry_external_neurons_inhibitory_delay_max, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_connect_external_layer_to_internal_layer = gtk_button_new_with_label("Connect External Layer");
	gtk_box_pack_start (GTK_BOX (hbox), btn_connect_external_layer_to_internal_layer, FALSE, FALSE, 0);
        entry_external_layer_num_to_connect= gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_layer_num_to_connect, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_layer_num_to_connect), "0");
	gtk_widget_set_size_request(entry_external_layer_num_to_connect, 30, 25);		
	lbl = gtk_label_new("to");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);	
        entry_internal_layer_num_to_connect_external_layer_to = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_layer_num_to_connect_external_layer_to, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_layer_num_to_connect_external_layer_to), "0");	
	gtk_widget_set_size_request(entry_internal_layer_num_to_connect_external_layer_to, 30, 25);

///////////////////////////////////////////// SECOND COLUMN  ///////////////////////////////////////////////////////////////

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 1,2, 0, 6);  

  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	combos_select_neuron = allocate_layer_neuron_group_neuron_combos();
        gtk_box_pack_start(GTK_BOX(hbox),combos_select_neuron->combo_layer , TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_neuron->combo_neuron_group, TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox),combos_select_neuron->combo_neuron , TRUE,TRUE,0);
	combo_neuron_dynamics = allocate_neuron_dynamics_combo(hbox, combo_neuron_dynamics);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_injection_current = gtk_button_new_with_label("Submit Injection Current");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_injection_current, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	entry_I_inject =  gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_I_inject, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_I_inject, 40, 25);	
	gtk_entry_set_text(GTK_ENTRY(entry_I_inject), "0");	

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);  	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Simulation Length(ms):");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);	
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	entry_simulation_length = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_simulation_length, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_simulation_length, 50, 25);	
	gtk_entry_set_text(GTK_ENTRY(entry_simulation_length), "1000");	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	
	btn_simulate_with_no_reward = gtk_button_new_with_label("No Reward");
	gtk_box_pack_start (GTK_BOX (hbox), btn_simulate_with_no_reward, TRUE, TRUE, 0);
	btn_simulate_with_reward = gtk_button_new_with_label("Reward");
	gtk_box_pack_start (GTK_BOX (hbox), btn_simulate_with_reward, TRUE, TRUE, 0);
	btn_simulate_with_punishment = gtk_button_new_with_label("Punish");
	gtk_box_pack_start (GTK_BOX (hbox), btn_simulate_with_punishment, TRUE, TRUE, 0);

/////////  GRAPHS  ////////////////////////////////

 	hbox = gtk_hbox_new(TRUE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 2,7, 0, 3);
	neuron_dynamics_graph = allocate_neuron_dynamics_graph(hbox, neuron_dynamics_graph, (1000000*(unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_simulation_length))))/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);


	g_signal_connect(G_OBJECT(combo_neuron_type), "changed", G_CALLBACK(combo_neuron_type_func), NULL);
    	g_signal_connect(G_OBJECT(btn_add_neurons_to_layer), "clicked", G_CALLBACK(add_neurons_to_layer_button_func), NULL);
    	g_signal_connect(G_OBJECT(btn_interrogate_network), "clicked", G_CALLBACK(interrogate_network_button_func), NULL);		
      	g_signal_connect(G_OBJECT(btn_interrogate_neuron), "clicked", G_CALLBACK(interrogate_neuron_button_func), NULL); 
      	g_signal_connect(G_OBJECT(btn_submit_parker_sochacki_params), "clicked", G_CALLBACK(submit_parker_sochacki_params_button_func), NULL);
      	g_signal_connect(G_OBJECT(btn_connect_internal_layer_to_internal_layer), "clicked", G_CALLBACK(connect_internal_layer_to_internal_layer_button_func), NULL);
      	g_signal_connect(G_OBJECT(btn_connect_external_layer_to_internal_layer), "clicked", G_CALLBACK(connect_external_layer_to_internal_layer_button_func), NULL);

	g_signal_connect(G_OBJECT(combos_select_neuron->combo_layer), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_layer);
	g_signal_connect(G_OBJECT(combos_select_neuron->combo_neuron_group), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_neuron_group);	
	g_signal_connect(G_OBJECT(combos_select_neuron->combo_neuron), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_neuron);

      	g_signal_connect(G_OBJECT(btn_submit_injection_current), "clicked", G_CALLBACK(submit_injection_current_button_func), NULL);
      	g_signal_connect(G_OBJECT(btn_simulate_with_no_reward), "clicked", G_CALLBACK(simulate_with_no_reward_button_func), NULL);

	gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, FALSE);

	return TRUE;
}

static void combo_neuron_type_func (void)
{
	int neuron_type;
	neuron_type = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_neuron_type));
	set_neuron_param_entries(neuron_type);	
}

static void add_neurons_to_layer_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	int num_of_neuron;
	int layer; 
	double a;
	double b;
	double c;
	double d;
	double k;
	double C;
	double v_resting;
	double v_threshold;
	double v_peak;
	bool inhibitory;
	double E_excitatory;
	double E_inhibitory;
	double tau_excitatory;
	double tau_inhibitory;
	int randomize_params;

 	num_of_neuron = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_num_of_neuron_for_group)));
	layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_add_neurons_to_layer))); 
	a = atof(gtk_entry_get_text(GTK_ENTRY(entry_a)));
	b = atof(gtk_entry_get_text(GTK_ENTRY(entry_b)));
	c = atof(gtk_entry_get_text(GTK_ENTRY(entry_c)));
	d = atof(gtk_entry_get_text(GTK_ENTRY(entry_d)));
	k = atof(gtk_entry_get_text(GTK_ENTRY(entry_k)));
	C = atof(gtk_entry_get_text(GTK_ENTRY(entry_C)));
	v_resting = atof(gtk_entry_get_text(GTK_ENTRY(entry_v_resting)));
	v_threshold = atof(gtk_entry_get_text(GTK_ENTRY(entry_v_threshold)));
	v_peak = atof(gtk_entry_get_text(GTK_ENTRY(entry_v_peak)));
	inhibitory = (bool)atof(gtk_entry_get_text(GTK_ENTRY(entry_inhibitory)));
	E_excitatory = atof(gtk_entry_get_text(GTK_ENTRY(entry_E_excitatory)));
	E_inhibitory = atof(gtk_entry_get_text(GTK_ENTRY(entry_E_inhibitory)));
	tau_excitatory = atof(gtk_entry_get_text(GTK_ENTRY(entry_tau_excitatory)));
	tau_inhibitory = atof(gtk_entry_get_text(GTK_ENTRY(entry_tau_inhibitory)));
	randomize_params = 0;

	if (!add_neurons_to_layer(bmi_data->int_network, num_of_neuron, layer, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return;
	if(!update_texts_of_combos_when_add_remove(combos_select_neuron, bmi_data->int_network))
		return;
	gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, TRUE);		
	return;
}

void interrogate_network_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	interrogate_network(bmi_data->int_network);
} 		

void interrogate_neuron_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	interrogate_neuron	(bmi_data->int_network, gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_layer)), gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron_group)), gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron)) );
}

static void set_neuron_param_entries(int neuron_type)
{
	char str[20];
	
	double v;
	double a;
	double b;
	double c;
	double d;
	double k;
	double C;
	double v_resting;
	double v_threshold;
	double v_peak;
//	double I_inject;
	bool inhibitory;
	double E_excitatory;
	double E_inhibitory;
	double tau_excitatory;
	double tau_inhibitory;


	
	get_neuron_type_parameters(neuron_type, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);

//	sprintf(str, "%.2f", v);	
//	gtk_entry_set_text(GTK_ENTRY(entry_v), str);
	sprintf(str, "%.2f", a);	
	gtk_entry_set_text(GTK_ENTRY(entry_a), str);
	sprintf(str, "%.2f", b);		
	gtk_entry_set_text(GTK_ENTRY(entry_b), str);
	sprintf(str, "%.2f", c);	
	gtk_entry_set_text(GTK_ENTRY(entry_c), str);
	sprintf(str, "%.2f", d);
	gtk_entry_set_text(GTK_ENTRY(entry_d), str);
	sprintf(str, "%.2f", C);
	gtk_entry_set_text(GTK_ENTRY(entry_C), str);
	sprintf(str, "%.2f", k);
	gtk_entry_set_text(GTK_ENTRY(entry_k), str);
	sprintf(str, "%.2f", v_resting);
	gtk_entry_set_text(GTK_ENTRY(entry_v_resting), str);
	sprintf(str, "%.2f", v_threshold);
	gtk_entry_set_text(GTK_ENTRY(entry_v_threshold), str);
	sprintf(str, "%.2f", v_peak);
	gtk_entry_set_text(GTK_ENTRY(entry_v_peak), str);
	sprintf(str, "%d", inhibitory);
	gtk_entry_set_text(GTK_ENTRY(entry_inhibitory), str);
	sprintf(str, "%.2f", E_excitatory);
	gtk_entry_set_text(GTK_ENTRY(entry_E_excitatory), str);
	sprintf(str, "%.2f", E_inhibitory);
	gtk_entry_set_text(GTK_ENTRY(entry_E_inhibitory), str);
	sprintf(str, "%.2f", tau_excitatory);
	gtk_entry_set_text(GTK_ENTRY(entry_tau_excitatory), str);
	sprintf(str, "%.2f", tau_inhibitory);
	gtk_entry_set_text(GTK_ENTRY(entry_tau_inhibitory), str);	

}

static void submit_parker_sochacki_params_button_func(void)
{	
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	if (! parker_sochacki_set_order_tolerance(bmi_data->int_network, (unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_parker_sochacki_max_order))), atof(gtk_entry_get_text(GTK_ENTRY(entry_parker_sochacki_err_tol)))))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_parker_sochacki_params_button_func", "! parker_sochacki_set_order_tolerance().");	
	gtk_widget_set_sensitive(btn_add_neurons_to_layer, FALSE);			
	gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, FALSE);	
}

static void connect_internal_layer_to_internal_layer_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	unsigned int this_layer = (unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_layer_num_to_connect)));
	unsigned int target_layer = (unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_layer_num_to_connect_internal_layer_to)));
	SynapticWeight weight_excitatory_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_weight_excitatory_max)));
	SynapticWeight weight_excitatory_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_weight_excitatory_min)));
	SynapticWeight weight_inhibitory_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_weight_inhibitory_max)));
	SynapticWeight weight_inhibitory_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_weight_inhibitory_max)));
	SynapticDelay EPSP_delay_max = (SynapticDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_excitatory_delay_max))));
	SynapticDelay EPSP_delay_min = (SynapticDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_excitatory_delay_min))));
	SynapticDelay IPSP_delay_max = (SynapticDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_inhibitory_delay_max))));
	SynapticDelay IPSP_delay_min = (SynapticDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_inhibitory_delay_min))));
	double excitatory_connection_probability = atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_excitatory_connection_probability)));
	double inhibitory_connection_probability = atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_inhibitory_connection_probability)));

	if (! connect_network_layer_to_network_layer(bmi_data->int_network, this_layer, target_layer, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_max, EPSP_delay_min, IPSP_delay_max, IPSP_delay_min, excitatory_connection_probability, inhibitory_connection_probability))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "connect_internal_layer_to_internal_layer_button_func", "! connect_network_layer_to_network_layer().");		
}

static void connect_external_layer_to_internal_layer_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	unsigned int this_layer = (unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_external_layer_num_to_connect)));
	unsigned int target_layer = (unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_layer_num_to_connect_external_layer_to)));
	SynapticWeight weight_excitatory_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_weight_excitatory_max)));
	SynapticWeight weight_excitatory_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_weight_excitatory_min)));
	SynapticWeight weight_inhibitory_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_weight_inhibitory_max)));
	SynapticWeight weight_inhibitory_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_weight_inhibitory_max)));
	SynapticDelay EPSP_delay_max = (SynapticDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_excitatory_delay_max))));
	SynapticDelay EPSP_delay_min = (SynapticDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_excitatory_delay_min))));
	SynapticDelay IPSP_delay_max = (SynapticDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_inhibitory_delay_max))));
	SynapticDelay IPSP_delay_min = (SynapticDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_inhibitory_delay_min))));
	double excitatory_connection_probability = atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_excitatory_connection_probability)));
	double inhibitory_connection_probability = atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_inhibitory_connection_probability)));

	if (! connect_ext_network_layer_to_int_network_layer(bmi_data->ext_network, this_layer, target_layer, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_max, EPSP_delay_min, IPSP_delay_max, IPSP_delay_min, excitatory_connection_probability, inhibitory_connection_probability))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "connect_external_layer_to_internal_layer_button_func", "! connect_ext_network_layer_to_int_network_layer().");		
}

static void combos_select_neuron_func(GtkWidget *changed_combo)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	if (bmi_data == NULL)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "combos_select_neuron_func", "bmi_data == NULL.");
	if(!update_texts_of_combos_when_change(combos_select_neuron, bmi_data->int_network, changed_combo))
		return;
}

static void submit_injection_current_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	unsigned int layer_num, nrn_grp_num, nrn_num;
	Neuron *neuron;
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_injection_current_button_func", "! layer_neuron_group_neuron_get_selected().");
	neuron = get_neuron_address(bmi_data->int_network, layer_num, nrn_grp_num, nrn_num);
	neuron->I_inject =  atof(gtk_entry_get_text(GTK_ENTRY(entry_I_inject)));
}


static void simulate_with_no_reward_button_func(void)
{
	char *end_ptr;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	Neuron *neuron;
	float *y_neuron_dynamics = neuron_dynamics_graph->y;
	TimeStamp sampling_interval = neuron_dynamics_graph->sampling_interval;
	unsigned int i, j, k;
	unsigned int layer_num, nrn_grp_num, nrn_num;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	TimeStamp time_ns;
	TimeStamp spike_time;
	TimeStamp simulation_length = 1000000*strtoull(gtk_entry_get_text(GTK_ENTRY(entry_simulation_length)), &end_ptr, 10);
	int neuron_dynamics_type_idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_neuron_dynamics->combo));
	if (! change_length_of_neuron_dynamics_graph(neuron_dynamics_graph,simulation_length , TRUE))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "simulate_with_no_reward_button_func", "! change_length_of_neuron_dynamics_graph().");	
	reset_all_network_neuron_dynamics (bmi_data->int_network);
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "simulate_with_no_reward_button_func", "! layer_neuron_group_neuron_get_selected().");
	get_num_of_layers_in_network(bmi_data->int_network, &num_of_layers);
	for (time_ns = 0; time_ns < simulation_length; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)   // integrate remaining part in the next task period
	{
		for (i = 0; i < num_of_layers; i++)
		{	
			get_num_of_neuron_groups_in_layer(bmi_data->int_network, i, &num_of_neuron_groups_in_layer);
			for (j=0; j<num_of_neuron_groups_in_layer; j++)
			{
				get_num_of_neurons_in_neuron_group(bmi_data->int_network, i, j, &num_of_neurons_in_neuron_group);
				for (k = 0; k < num_of_neurons_in_neuron_group; k++)
				{
					neuron = get_neuron_address(bmi_data->int_network, i, j, k);
					spike_time = evaluate_neuron_dyn(neuron, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
//					if (spike_time != MAX_TIME_STAMP)
					if ((i != layer_num) || (j != nrn_grp_num) || (k != nrn_num))
						continue;
					switch (neuron_dynamics_type_idx)
					{
						case DYNAMICS_TYPE_V:
							y_neuron_dynamics[time_ns/sampling_interval] = neuron->v;
							break; 
						case DYNAMICS_TYPE_U:
							y_neuron_dynamics[time_ns/sampling_interval] = neuron->u;
							break; 
						case DYNAMICS_TYPE_E:
							y_neuron_dynamics[time_ns/sampling_interval] = neuron->conductance_excitatory;
							break; 
						case DYNAMICS_TYPE_I:
							y_neuron_dynamics[time_ns/sampling_interval] = neuron->conductance_inhibitory;
							break; 
						default:
							return (void)print_message(BUG_MSG ,"HybridNetRLBMI", "NetworkView", "simulate_with_no_reward_button_func", "Invalid neuron_dynamics_type_idx.");
					}		
				}
			}
		}		
	}
	if (!update_neuron_dynamics_graph(neuron_dynamics_graph))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "simulate_with_no_reward_button_func", "!update_neuron_dynamics_graph().");	
}

