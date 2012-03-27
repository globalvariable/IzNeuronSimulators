#include "NetworkView.h"


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

static void set_neuron_param_entries(int neuron_type);

bool create_network_view_gui(void)
{
	GtkWidget *frame, *frame_label, *table, *vbox, *hbox, *lbl;

        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Network & Currents     ");      
   
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

	return TRUE;
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
