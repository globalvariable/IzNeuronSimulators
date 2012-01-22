#include "SimulationTab.h"

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
static GtkWidget *entry_parker_sochacki_err_tol;
static GtkWidget *entry_parker_sochacki_max_order;
static GtkWidget *btn_submit_parker_sochacki_params;

static GtkWidget *combo_layer_num_upper;
static GtkWidget *combo_neuron_group_num_upper;
static GtkWidget *combo_neuron_num_upper;
static GtkWidget *combo_layer_num_lower;
static GtkWidget *combo_neuron_group_num_lower;
static GtkWidget *combo_neuron_num_lower;
static GtkWidget *btn_interrogate_network;
static GtkWidget *btn_interrogate_neuron;

static GtkWidget *entry_num_of_trials_to_simulate;
static GtkWidget *btn_simulate;

/////// LAST COLUMN
static GtkWidget *	txv_notes;
static GtkWidget *	btn_select_neurosim_data_directory;
static GtkWidget *	btn_create_neurosim_data_directory;
static GtkWidget *	btn_save_neurosim_data;
static GtkWidget *	btn_load_neurosim_data;

static GtkWidget *btn_select_directory_load_spike_pattern_generator_data;
static GtkWidget *btn_load_spike_pattern_generator_data; 

static int upper_graph_idx;
static int lower_graph_idx;

static int upper_graph_combo_idx;
static int lower_graph_combo_idx;


void load_spike_pattern_generator_data_button_func(void);


bool create_simulation_tab(GtkWidget * tabs)
{
	GtkWidget *frame, *frame_label, *table, *vbox, *hbox, *lbl, *hbox1, *vbox1;

        frame = gtk_frame_new (NULL);
        frame_label = gtk_label_new ("     SIMULATION     ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (tabs), frame, frame_label);  


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
	gtk_widget_set_size_request(combo_neuron_type, 200, 25) ;
	
	int i; char neuron_type_str[50];
	for (i = 0; i < MAX_NUM_OF_NEURON_TYPES; i++)
	{
		gtk_combo_box_append_text(GTK_COMBO_BOX(combo_neuron_type), get_neuron_type_string(i, neuron_type_str));
	}
 	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_neuron_type), 1);
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("	a:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_a = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_a, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_a, 50, 25) ;

	lbl = gtk_label_new("	b:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_b = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_b, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_b, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("	c:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_c = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_c, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_c, 50, 25) ;

	lbl = gtk_label_new("	d:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_d = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_d, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_d, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("	k:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_k = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_k, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_k, 50, 25) ;

	lbl = gtk_label_new("	C:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_C = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_C, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_C, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("	v thres	:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_v_threshold = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_v_threshold, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_v_threshold, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("	v peak	:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_v_peak = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_v_peak, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_v_peak, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("	v resting	:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_v_resting = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_v_resting, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_v_resting, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("	inhibit	:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_inhibitory = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_inhibitory, FALSE,FALSE,0);
	gtk_entry_set_max_length (GTK_ENTRY(entry_inhibitory), 1);
	gtk_widget_set_size_request(entry_inhibitory, 50, 25) ;
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("	E Excite	:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_E_excitatory = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_E_excitatory, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_E_excitatory, 50, 25) ;	
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("	E Inhibit	:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_E_inhibitory = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_E_inhibitory, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_E_inhibitory, 50, 25) ;	
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("	Tau Excite:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_tau_excitatory = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_tau_excitatory, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_tau_excitatory, 50, 25) ;	
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("	Tau Inhibit:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_tau_inhibitory = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_tau_inhibitory, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_tau_inhibitory, 50, 25) ;	

	set_neuron_param_entries(1);
	
	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);  	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Num of Neuron : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
       	entry_num_of_neuron_for_group = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_num_of_neuron_for_group, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_num_of_neuron_for_group), "1");
	gtk_widget_set_size_request(entry_num_of_neuron_for_group, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_add_neurons_to_layer = gtk_button_new_with_label("Add Neurons to Layer");
	gtk_box_pack_start (GTK_BOX (hbox), btn_add_neurons_to_layer, FALSE, FALSE, 0);

        entry_add_neurons_to_layer = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_add_neurons_to_layer, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_add_neurons_to_layer), "0");
	gtk_widget_set_size_request(entry_add_neurons_to_layer, 50, 25) ;	

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 10);  	

	///////////////  CONNECT INTERNAL NEURONS

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
 	lbl = gtk_label_new("Internal Network: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);  

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

        lbl = gtk_label_new("EPSP:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Connection Probability(0-1) : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_excitatory_connection_probability = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_excitatory_connection_probability, FALSE,FALSE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_excitatory_connection_probability), "1");
	gtk_widget_set_size_request(entry_internal_neurons_excitatory_connection_probability, 50, 25) ;	
	
	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
        lbl = gtk_label_new("Weight Min:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_weight_excitatory_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_weight_excitatory_min, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_weight_excitatory_min), "0.001");
	gtk_widget_set_size_request(entry_internal_neurons_weight_excitatory_min, 50, 25) ;	
	
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_weight_excitatory_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_weight_excitatory_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_weight_excitatory_max), "1");
	gtk_widget_set_size_request(entry_internal_neurons_weight_excitatory_max, 50, 25) ;	
	
 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Delay Min(ns): ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_excitatory_delay_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_excitatory_delay_min , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_excitatory_delay_min), "1000000");
	gtk_widget_set_size_request(entry_internal_neurons_excitatory_delay_min, 55, 25) ;
	
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_excitatory_delay_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_excitatory_delay_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_excitatory_delay_max), "5000000");	
	gtk_widget_set_size_request(entry_internal_neurons_excitatory_delay_max, 55, 25) ;

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);  	
		
 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
 	lbl = gtk_label_new("IPSP: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);       

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Connection Probability(0-1) : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_inhibitory_connection_probability = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_inhibitory_connection_probability, FALSE,FALSE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_inhibitory_connection_probability), "1");
	gtk_widget_set_size_request(entry_internal_neurons_inhibitory_connection_probability, 50, 25) ;	

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Weight Min: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_weight_inhibitory_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_weight_inhibitory_min, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_weight_inhibitory_min), "0.001");
	gtk_widget_set_size_request(entry_internal_neurons_weight_inhibitory_min, 50, 25) ;	
	
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_weight_inhibitory_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_weight_inhibitory_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_weight_inhibitory_max), "1");
	gtk_widget_set_size_request(entry_internal_neurons_weight_inhibitory_max, 50, 25) ;	
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Delay Min(ns): ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_inhibitory_delay_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_inhibitory_delay_min , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_inhibitory_delay_min), "1000000");
	gtk_widget_set_size_request(entry_internal_neurons_inhibitory_delay_min, 55, 25) ;
	
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_inhibitory_delay_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_inhibitory_delay_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_inhibitory_delay_max), "5000000");	
	gtk_widget_set_size_request(entry_internal_neurons_inhibitory_delay_max, 55, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_connect_internal_layer_to_internal_layer = gtk_button_new_with_label("Connect Internal Layer");
	gtk_box_pack_start (GTK_BOX (hbox), btn_connect_internal_layer_to_internal_layer, FALSE, FALSE, 0);
        entry_internal_layer_num_to_connect= gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_layer_num_to_connect, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_layer_num_to_connect), "0");
	gtk_widget_set_size_request(entry_internal_layer_num_to_connect, 30, 25);		
	lbl = gtk_label_new(" to ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);	
        entry_internal_layer_num_to_connect_internal_layer_to = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_layer_num_to_connect_internal_layer_to, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_layer_num_to_connect_internal_layer_to), "0");	
	gtk_widget_set_size_request(entry_internal_layer_num_to_connect_internal_layer_to, 30, 25);		
	
	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 15);  	

	///////////////  CONNECT EXTERNAL NEURONS

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
 	lbl = gtk_label_new("External Network: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);      

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

        lbl = gtk_label_new("EPSP:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Connection Probability(0-1) : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_excitatory_connection_probability= gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_excitatory_connection_probability, FALSE,FALSE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_excitatory_connection_probability), "1");
	gtk_widget_set_size_request(entry_external_neurons_excitatory_connection_probability, 50, 25) ;	
	
	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
        lbl = gtk_label_new("Weight Min:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_weight_excitatory_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_weight_excitatory_min, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_weight_excitatory_min), "0.001");
	gtk_widget_set_size_request(entry_external_neurons_weight_excitatory_min, 50, 25) ;	
	
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_weight_excitatory_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_weight_excitatory_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_weight_excitatory_max), "1");
	gtk_widget_set_size_request(entry_external_neurons_weight_excitatory_max, 50, 25) ;	
	
 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Delay Min(ns): ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_excitatory_delay_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_excitatory_delay_min , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_excitatory_delay_min), "1000000");
	gtk_widget_set_size_request(entry_external_neurons_excitatory_delay_min, 55, 25) ;
	
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_excitatory_delay_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_excitatory_delay_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_excitatory_delay_max), "5000000");	
	gtk_widget_set_size_request(entry_external_neurons_excitatory_delay_max, 55, 25) ;

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);  	
		
 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
 	lbl = gtk_label_new("IPSP: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);       

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Connection Probability(0-1) : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_inhibitory_connection_probability = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_inhibitory_connection_probability, FALSE,FALSE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_inhibitory_connection_probability), "1");
	gtk_widget_set_size_request(entry_external_neurons_inhibitory_connection_probability, 50, 25) ;	

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Weight Min: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_weight_inhibitory_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_weight_inhibitory_min, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_weight_inhibitory_min), "0.001");
	gtk_widget_set_size_request(entry_external_neurons_weight_inhibitory_min, 50, 25) ;	
	
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_weight_inhibitory_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_weight_inhibitory_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_weight_inhibitory_max), "1");
	gtk_widget_set_size_request(entry_external_neurons_weight_inhibitory_max, 50, 25) ;	
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Delay Min(ns): ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_inhibitory_delay_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_inhibitory_delay_min , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_inhibitory_delay_min), "1000000");
	gtk_widget_set_size_request(entry_external_neurons_inhibitory_delay_min, 55, 25) ;
	
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_inhibitory_delay_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_inhibitory_delay_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_inhibitory_delay_max), "5000000");	
	gtk_widget_set_size_request(entry_external_neurons_inhibitory_delay_max, 55, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_connect_external_layer_to_internal_layer = gtk_button_new_with_label("Connect Ext Layer");
	gtk_box_pack_start (GTK_BOX (hbox), btn_connect_external_layer_to_internal_layer, FALSE, FALSE, 0);
        entry_external_layer_num_to_connect= gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_layer_num_to_connect, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_layer_num_to_connect), "0");
	gtk_widget_set_size_request(entry_external_layer_num_to_connect, 30, 25);		
	lbl = gtk_label_new(" to ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);	
        entry_internal_layer_num_to_connect_external_layer_to = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_layer_num_to_connect_external_layer_to, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_layer_num_to_connect_external_layer_to), "0");	
	gtk_widget_set_size_request(entry_internal_layer_num_to_connect_external_layer_to, 30, 25);	
	
		
		
///////////////////////////////////////////// SECOND COLUMN  ///////////////////////////////////////////////////////////////

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 1,2,0,6);     ///  Stimulus graph control

  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Parker-Sochacki Integration:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        
  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);        
  
	lbl = gtk_label_new("Err. Tolerance:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
                
        entry_parker_sochacki_err_tol= gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_parker_sochacki_err_tol, FALSE,FALSE,0);
	char temp_str[40];
      	sprintf(temp_str, "%.16E", 0.0);	
	gtk_entry_set_text(GTK_ENTRY(entry_parker_sochacki_err_tol), temp_str);
	gtk_widget_set_size_request(entry_parker_sochacki_err_tol, 50, 25) ;

  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);        
  
	lbl = gtk_label_new("Max Order:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
                
        entry_parker_sochacki_max_order= gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_parker_sochacki_max_order, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_parker_sochacki_max_order), "40");
	gtk_widget_set_size_request(entry_parker_sochacki_max_order, 50, 25) ;
	
  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);        
  
	btn_submit_parker_sochacki_params = gtk_button_new_with_label("Submit Parameters");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_parker_sochacki_params, TRUE, TRUE, 0);
	
         gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);
         
  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Layer : NeuronGroup : Neuron");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        
	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
	combo_layer_num_upper = gtk_combo_box_new_text();
        gtk_box_pack_start(GTK_BOX(hbox),combo_layer_num_upper, TRUE,TRUE,0);

	combo_neuron_group_num_upper = gtk_combo_box_new_text();
        gtk_box_pack_start(GTK_BOX(hbox),combo_neuron_group_num_upper, TRUE,TRUE,0);
	        
	combo_neuron_num_upper = gtk_combo_box_new_text();
        gtk_box_pack_start(GTK_BOX(hbox),combo_neuron_num_upper, TRUE,TRUE,0);

	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
	combo_layer_num_lower = gtk_combo_box_new_text();
        gtk_box_pack_start(GTK_BOX(hbox),combo_layer_num_lower, TRUE,TRUE,0);

	combo_neuron_group_num_lower = gtk_combo_box_new_text();
        gtk_box_pack_start(GTK_BOX(hbox),combo_neuron_group_num_lower, TRUE,TRUE,0);
	        
	combo_neuron_num_lower = gtk_combo_box_new_text();
        gtk_box_pack_start(GTK_BOX(hbox),combo_neuron_num_lower, TRUE,TRUE,0);
        
 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_interrogate_network = gtk_button_new_with_label("Interrogate Network");
	gtk_box_pack_start (GTK_BOX (hbox), btn_interrogate_network, TRUE, TRUE, 0);	

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_interrogate_neuron = gtk_button_new_with_label("Interrogate Neuron");
	gtk_box_pack_start (GTK_BOX (hbox), btn_interrogate_neuron, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);	
        
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Num of Trials to Simulate: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
 
 	entry_num_of_trials_to_simulate = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_num_of_trials_to_simulate, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_num_of_trials_to_simulate), "1");
	gtk_widget_set_size_request(entry_num_of_trials_to_simulate, 50, 25) ;        
        
 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_simulate = gtk_button_new_with_label("Simulate");
	gtk_box_pack_start (GTK_BOX (hbox), btn_simulate, TRUE, TRUE, 0);        
        
//////	GRAPHS
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 2,7, 0,3);  
 	vbox1 = gtk_vbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(hbox),vbox1, FALSE,FALSE,0);		
	hbox1 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, FALSE,FALSE,0);	
	if(!add_neuron_dynamics_combo(hbox1, &upper_graph_combo_idx))
		return FALSE;	
	if(!add_neuron_dynamics_box_and_graph(hbox, 1000*1000000, &upper_graph_idx))	// 1000 ms
		return FALSE;

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 2,7, 3,6); 
 	vbox1 = gtk_vbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(hbox),vbox1, FALSE,FALSE,0);		
	hbox1 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, FALSE,FALSE,0);		 
	if(!add_neuron_dynamics_combo(hbox1, &lower_graph_combo_idx))
		return FALSE;		
	if(!add_neuron_dynamics_box_and_graph(hbox, 1000*1000000, &lower_graph_idx))	// 1000 ms
		return FALSE;	
 
/////// LAST COLUMN

 	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 7,8, 0,6);  
	
 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);       	
     	  	
  	lbl = gtk_label_new("Notes:");
	gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE, 0);
	 	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	      
 	gtk_widget_set_size_request(hbox, 50, 700);
 	
	txv_notes = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(hbox),txv_notes, TRUE,TRUE, 0);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);  	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Folder : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);

	btn_select_neurosim_data_directory = gtk_file_chooser_button_new ("Select Folder", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox),btn_select_neurosim_data_directory, TRUE,TRUE,0);
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_create_neurosim_data_directory = gtk_button_new_with_label("Create NeuroSimData Directory");
	gtk_box_pack_start (GTK_BOX (hbox), btn_create_neurosim_data_directory, TRUE, TRUE, 0);
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_save_neurosim_data = gtk_button_new_with_label("Save NeuroSimData");
	gtk_box_pack_start (GTK_BOX (hbox), btn_save_neurosim_data, TRUE, TRUE, 0);	
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_load_neurosim_data = gtk_button_new_with_label("Load NeuroSimData");
	gtk_box_pack_start (GTK_BOX (hbox), btn_load_neurosim_data, TRUE, TRUE, 0);   

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);	
        
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);              

	lbl = gtk_label_new("Folder : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);

	btn_select_directory_load_spike_pattern_generator_data = gtk_file_chooser_button_new ("Select Folder", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox),btn_select_directory_load_spike_pattern_generator_data, TRUE,TRUE,0);
	set_path_for_btn_select_directory_load_spike_pattern_generator_data();
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_load_spike_pattern_generator_data = gtk_button_new_with_label("Load SpikePatternGenerator Data");
	gtk_box_pack_start (GTK_BOX (hbox), btn_load_spike_pattern_generator_data, TRUE, TRUE, 0);   

	g_signal_connect(G_OBJECT(btn_load_spike_pattern_generator_data), "clicked", G_CALLBACK(load_spike_pattern_generator_data_button_func), NULL);	
	
	
		        		
	return TRUE;

}


void set_neuron_param_entries(int neuron_type)
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



void load_spike_pattern_generator_data_button_func(void)
{
	int i, j;
	char *path_temp = NULL, *path = NULL;
	int version;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_load_spike_pattern_generator_data));
	path = &path_temp[7];   // since     uri returns file:///home/....

	char  *end_ptr;	
	
	int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	int num_of_patterns;
	TimeStampMs pattern_length_ms;
	int num_of_spikes;
	int max_num_of_spikes_in_patterns = 0;	

	TimeStamp trial_start_ns = 0;	
	// use below to utilize in program.
	SpikeTimeStampItem *unsorted_spike_time_stamps;
	
	if (!write_path_for_btn_select_directory_load_spike_pattern_generator_data(path))
		return;
	
	initialize_spike_pattern_generator_data_read_write_handlers();
		
	if (!get_spike_pattern_generator_data_format_version(&version, path))
	{
		return;
	}	

	/// Get EXT_NETWORK
	
	neurosim_set_ext_network(allocate_external_network(neurosim_get_ext_network()));   // deallocates previously allocated external network and brings a new one.

	if (!((*spike_pattern_generator_data_get_num_of_layers[version])(2, path, &num_of_layers)))
		return;
	for (i=0;i<num_of_layers; i++)
	{
		 if(!((*spike_pattern_generator_data_get_num_of_neuron_groups_in_layer[version])(3, path, i , &num_of_neuron_groups_in_layer)))
			return;
		for (j=0;j<num_of_neuron_groups_in_layer; j++)
		{
			if(!((*spike_pattern_generator_data_get_num_of_neurons_in_neuron_group[version])(4, path, i, j, &num_of_neurons_in_neuron_group)))
				return;
			add_neurons_to_external_network_layer(neurosim_get_ext_network(), num_of_neurons_in_neuron_group , i, 0);
		}		
	}	
	
	/// GET_NUM_OF_PATTERNS
	
	if (!((*spike_pattern_generator_data_get_num_of_patterns[version])(2, path, &num_of_patterns)))
		return;

	/// GET_MAX_NUM_OF_SPIKES_IN_ALL_PATTERNS	

	for (i=0;i< num_of_patterns; i++)
	{
		if(!((*spike_pattern_generator_data_get_num_of_spikes_in_pattern[version])(3, path, i , &num_of_spikes)))
			return;
		if (num_of_spikes > max_num_of_spikes_in_patterns)
			max_num_of_spikes_in_patterns = num_of_spikes;	
	}

	// ALLOCATE_ext_network_single_spike_pattern
	
	neurosim_set_ext_network_single_spike_pattern(allocate_single_spike_pattern(neurosim_get_ext_network_single_spike_pattern(), max_num_of_spikes_in_patterns));	 // deallocates previously allocated external network spike patterns and brings a new one.				
	neurosim_set_ext_network_unsorted_single_spike_pattern(allocate_single_spike_pattern(neurosim_get_ext_network_unsorted_single_spike_pattern(), max_num_of_spikes_in_patterns));	
	
	// LOAD ext_network_unsorted_single_spike_pattern & then ext_network_SORTED_single_spike_pattern
	
	if(!((*spike_pattern_generator_data_get_num_of_spikes_in_pattern[version])(3, path, i , &num_of_spikes)))
		return;	
	if(!get_spikes_ptr_of_single_spike_pattern(neurosim_get_ext_network_unsorted_single_spike_pattern(), &unsorted_spike_time_stamps))
		return;
	if(!((*spike_pattern_generator_data_get_all_spike_time_stamps_in_pattern[version])(4, path, i , num_of_spikes, unsorted_spike_time_stamps)))
		return;
	if(!set_used_num_of_spikes_of_single_spike_pattern(neurosim_get_ext_network_unsorted_single_spike_pattern(), num_of_spikes))
		return;						
	if(!reset_single_spike_pattern_write_idx(neurosim_get_ext_network_single_spike_pattern()))
		return (void)print_message(ERROR_MSG ,"NeuroSim", "SimulationTab", "load_spike_pattern_generator_data_button_func", "Couldn' t reset spike_pattern.");	
	for (i=0; i<num_of_spikes; i++)		
	{
		if(!write_spike_time_stamp_to_single_spike_pattern(neurosim_get_ext_network_single_spike_pattern(), &(unsorted_spike_time_stamps[i])))			
			return (void)print_message(ERROR_MSG ,"NeuroSim", "SimulationTab", "load_spike_pattern_generator_data_button_func", "Couldn' t  add spike timestamp of SpikePatternGeneratorData.");
		//	printf("%llu %d %d %d\n", spike_time_stamps[j].peak_time, spike_time_stamps[j].mwa_or_layer, spike_time_stamps[j].channel_or_neuron_group, spike_time_stamps[j].unit_or_neuron);
	}	
	

	// ALLOCATE main_single_trial_stats
	
	neurosim_set_main_single_trial_stats(allocate_main_single_trial_stats(neurosim_get_main_single_trial_stats()));	 // deallocates previously allocated external network spike patterns and brings a new	

	// LOAD main_single_trial_stats

	if (strtoul(gtk_entry_get_text(GTK_ENTRY(entry_num_of_trials_to_simulate)), &end_ptr, 10) < num_of_patterns)
		return (void)print_message(ERROR_MSG ,"NeuroSim", "SimulationTab", "load_spike_pattern_generator_data_button_func", "num_of_trials_allocated < num_of_patterns.");	
	
	if (strtoul(gtk_entry_get_text(GTK_ENTRY(entry_num_of_trials_to_simulate)), &end_ptr, 10) > num_of_patterns)
		print_message(WARNING_MSG ,"NeuroSim", "SimulationTab", "load_spike_pattern_generator_data_button_func", "Some patterns will be duplicate since num_of_trials_allocated > num_of_patterns.");										
					
	if(!((*spike_pattern_generator_data_get_pattern_length[version])(3, path, 0, &pattern_length_ms)))
		return;

	if(!reset_main_single_trial_stats(neurosim_get_main_single_trial_stats()))
		return;
		
	if (!write_to_main_single_trial_stats(neurosim_get_main_single_trial_stats(), trial_start_ns , pattern_length_ms*1000000))     // leave 1 second interval between trials. 
		return (void)print_message(ERROR_MSG ,"NeuroSim", "SimulationTab", "load_spike_pattern_generator_data_button_func", "Couldn' t write pattern length to single trial stats.");							

	return;
}

void set_path_for_btn_select_directory_load_spike_pattern_generator_data(void)
{
	char line[600];
	FILE *fp = NULL;
       	if ((fp = fopen("./initial_path_spike_pattern_generator_data", "r")) == NULL)  
       	{ 
       		printf ("ERROR: SimulationTab: Couldn't find file: ./initial_path_spike_pattern_generator_data\n"); 
       		printf ("ERROR: SimulationTab: /home is loaded as initial direcoty to create data folder\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_load_spike_pattern_generator_data),"/home");
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: SimulationTab: Couldn' t read ./initial_path_spike_pattern_generator_data\n"); 
       			printf ("ERROR: SimulationTab: /home is loaded as initial direcoty to create data folder\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_load_spike_pattern_generator_data),"/home");
		}
		else
		{
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_load_spike_pattern_generator_data),line);
		}
		fclose(fp); 		
	}  	 
}

bool write_path_for_btn_select_directory_load_spike_pattern_generator_data(char *path)
{
	FILE *fp = NULL;
       	if ((fp = fopen("./initial_path_spike_pattern_generator_data", "w")) == NULL)  
       	{ 
       		printf ("ERROR: SimulationTab: Couldn't write into file: ./initial_path_spike_pattern_generator_data\n"); 
		return FALSE;
       	}
       	else
       	{
		fprintf(fp,"%s", path);
		fclose(fp); 
		return TRUE;		
	}  	 
}
