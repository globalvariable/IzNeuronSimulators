#include "NetworkView.h"

static HybridNetRLBMIData *static_bmi_data = NULL;
static NeuralNet2GuiMsg *static_msgs_neural_net_2_gui = NULL;
static RtTasksData *rt_tasks_data = NULL;
static GtkWidget *btn_select_directory_to_save = NULL;
static SpikeData **in_silico_spike_data_for_recording = NULL;
static unsigned int num_of_dedicated_cpu_threads = 0;
static Neuron **static_all_neurons = NULL;
static unsigned int static_num_of_neurons = 0;

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

static GtkWidget *btn_make_output;
static GtkWidget *entry_layer_to_make_output;

static GtkWidget *entry_internal_neurons_excitatory_connection_probability;
static GtkWidget *entry_internal_neurons_excitatory_plastic_synapse;
static GtkWidget *entry_internal_neurons_weight_excitatory_min;
static GtkWidget *entry_internal_neurons_weight_excitatory_max;
static GtkWidget *entry_internal_neurons_excitatory_delay_min;
static GtkWidget *entry_internal_neurons_excitatory_delay_max;
static GtkWidget *entry_internal_neurons_inhibitory_connection_probability;
static GtkWidget *entry_internal_neurons_inhibitory_plastic_synapse;
static GtkWidget *entry_internal_neurons_weight_inhibitory_min;
static GtkWidget *entry_internal_neurons_weight_inhibitory_max;
static GtkWidget *entry_internal_neurons_inhibitory_delay_min;
static GtkWidget *entry_internal_neurons_inhibitory_delay_max;

static GtkWidget *btn_connect_internal_layer_to_internal_layer;
static GtkWidget *entry_internal_layer_num_to_connect;
static GtkWidget *entry_internal_layer_num_to_connect_internal_layer_to;

static GtkWidget *entry_external_neurons_excitatory_connection_probability;
static GtkWidget *entry_external_neurons_excitatory_plastic_synapse;
static GtkWidget *entry_external_neurons_weight_excitatory_min;
static GtkWidget *entry_external_neurons_weight_excitatory_max;
static GtkWidget *entry_external_neurons_excitatory_delay_min;
static GtkWidget *entry_external_neurons_excitatory_delay_max;
static GtkWidget *entry_external_neurons_inhibitory_connection_probability;
static GtkWidget *entry_external_neurons_inhibitory_plastic_synapse;
static GtkWidget *entry_external_neurons_weight_inhibitory_min;
static GtkWidget *entry_external_neurons_weight_inhibitory_max;
static GtkWidget *entry_external_neurons_inhibitory_delay_min;
static GtkWidget *entry_external_neurons_inhibitory_delay_max;


static GtkWidget *btn_connect_external_layer_to_internal_layer;
static GtkWidget *entry_external_layer_num_to_connect;
static GtkWidget *entry_internal_layer_num_to_connect_external_layer_to;

// SECOND COLUMN
static GtkWidget *entry_STDP_layer;
static GtkWidget *entry_STDP_neuron_group;
static GtkWidget *entry_STDP_pre_post_change_min;
static GtkWidget *entry_STDP_pre_post_change_max;
static GtkWidget *entry_STDP_pre_post_tau_min;
static GtkWidget *entry_STDP_pre_post_tau_max;

static GtkWidget *entry_eligibility_tau_min;
static GtkWidget *entry_eligibility_tau_max;
static GtkWidget *entry_max_eligibility_min;
static GtkWidget *entry_max_eligibility_max;

static GtkWidget *btn_submit_stdp_and_eligibility;

static GtkWidget *entry_learning_rate;
static GtkWidget *btn_submit_learning_rate;


static LayerNrnGrpNeuronCombo *combos_select_neuron;
static NeuronDynamicsCombo *combo_neuron_dynamics = NULL;

static GtkWidget *btn_submit_synaptic_weight_for_neuron;
static GtkWidget *btn_submit_excitatory_synaptic_weight_for_neuron;
static GtkWidget *entry_weight_lower_limit;
static GtkWidget *entry_weight_upper_limit;
static GtkWidget *btn_submit_new_stdp_and_eligibility_for_neuron;

static LayerNrnGrpNrnSynapseCombo *combos_select_synapse;
static GtkWidget *btn_submit_synaptic_weight_for_synapse;
static GtkWidget *btn_submit_new_stdp_and_eligibility_for_synapse;

static GtkWidget *btn_ready_for_simulation;

static GtkWidget *btn_start_hybrid_network;

static GtkWidget *btn_select_directory_to_save;
static GtkWidget *btn_create_recording_folder;

static GtkWidget *btn_select_directory_to_load;
static GtkWidget *btn_load_data;
static GtkWidget *entry_load_data_folder_num;


// FIRST COLUMN
static void combo_neuron_type_func (void);
static void add_neurons_to_layer_button_func(void);
static void interrogate_network_button_func(void); 		
static void interrogate_neuron_button_func(void);
static void set_neuron_param_entries(int neuron_type);
static void submit_parker_sochacki_params_button_func(void);
static void make_output_button_func(void);

static void connect_internal_layer_to_internal_layer_button_func(void);
static void connect_external_layer_to_internal_layer_button_func(void);
// SECOND COLUMN
static void submit_stdp_and_eligibility_button_func(void);
static void submit_learning_rate_button_func (void);

static void combos_select_neuron_func(GtkWidget *changed_combo);
static void combos_select_synapse_func(GtkWidget *changed_combo);

static void submit_synaptic_weight_for_neuron_button_func(void);
static void submit_excitatory_synaptic_weight_for_neuron_button_func(void);

static void submit_new_stdp_and_eligibility_for_neuron_button_func(void);
static void submit_synaptic_weight_for_synapse_button_func(void);
static void submit_new_stdp_and_eligibility_for_synapse_button_func(void);

static void ready_for_simulation_button_func(void);

static void start_hybrid_network_button_func(void);

static void create_recording_folder_button_func (void);

static void load_data_button_func (void);

static void set_directory_btn_select_directory_to_save(void);

static gboolean timeout_callback(gpointer user_data) ;

bool create_network_view_gui(void)
{
	static_msgs_neural_net_2_gui = get_hybrid_net_rl_bmi_data()->msgs_neural_net_2_gui;
	rt_tasks_data = get_hybrid_net_rl_bmi_data()->rt_tasks_data;
	in_silico_spike_data_for_recording = get_hybrid_net_rl_bmi_data()->in_silico_spike_data_for_recording;
	num_of_dedicated_cpu_threads = get_hybrid_net_rl_bmi_data()->num_of_dedicated_cpu_threads;


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

 	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_neuron_type), 2);	
	set_neuron_param_entries(2);			

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
      	sprintf(temp_str, "%.1E", 1.0e-3);	
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

	btn_make_output = gtk_button_new_with_label("   Make Output   ");
        gtk_box_pack_start(GTK_BOX(hbox),btn_make_output, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	lbl = gtk_label_new("Layer :");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_layer_to_make_output = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry_layer_to_make_output), "0");
	gtk_box_pack_start(GTK_BOX(hbox), entry_layer_to_make_output, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_layer_to_make_output, 40, 25) ;	

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

	lbl = gtk_label_new("Probability:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_excitatory_connection_probability = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_excitatory_connection_probability, FALSE,FALSE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_excitatory_connection_probability), "1.0");
	gtk_widget_set_size_request(entry_internal_neurons_excitatory_connection_probability, 50, 25) ;	
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);	
	lbl = gtk_label_new("Plastic:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_excitatory_plastic_synapse = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_excitatory_plastic_synapse , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_excitatory_plastic_synapse), "0");
	gtk_widget_set_size_request(entry_internal_neurons_excitatory_plastic_synapse, 20, 25) ;	
	
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
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_excitatory_delay_min), "7.0");
	gtk_widget_set_size_request(entry_internal_neurons_excitatory_delay_min, 50, 25) ;
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_excitatory_delay_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_excitatory_delay_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_excitatory_delay_max), "7.0");	
	gtk_widget_set_size_request(entry_internal_neurons_excitatory_delay_max, 50, 25) ;
		
 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
 	lbl = gtk_label_new("----- IPSP -----");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);       

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Probability:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_inhibitory_connection_probability = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_inhibitory_connection_probability, FALSE,FALSE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_inhibitory_connection_probability), "1.0");
	gtk_widget_set_size_request(entry_internal_neurons_inhibitory_connection_probability, 50, 25) ;	
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);	
	lbl = gtk_label_new("Plastic:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_inhibitory_plastic_synapse = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_inhibitory_plastic_synapse , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_inhibitory_plastic_synapse), "0");
	gtk_widget_set_size_request(entry_internal_neurons_inhibitory_plastic_synapse, 20, 25) ;	


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
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_inhibitory_delay_min), "7.0");
	gtk_widget_set_size_request(entry_internal_neurons_inhibitory_delay_min, 50, 25) ;
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_internal_neurons_inhibitory_delay_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_neurons_inhibitory_delay_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_neurons_inhibitory_delay_max), "7.0");	
	gtk_widget_set_size_request(entry_internal_neurons_inhibitory_delay_max, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_connect_internal_layer_to_internal_layer = gtk_button_new_with_label("Connect Internal Layer");
	gtk_box_pack_start (GTK_BOX (hbox), btn_connect_internal_layer_to_internal_layer, FALSE, FALSE, 0);
        entry_internal_layer_num_to_connect= gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_internal_layer_num_to_connect, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_internal_layer_num_to_connect), "2");
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

	lbl = gtk_label_new("Probability:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_excitatory_connection_probability = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_excitatory_connection_probability, FALSE,FALSE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_excitatory_connection_probability), "1.0");
	gtk_widget_set_size_request(entry_external_neurons_excitatory_connection_probability, 50, 25) ;	
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);	
	lbl = gtk_label_new("Plastic:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_excitatory_plastic_synapse = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_excitatory_plastic_synapse , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_excitatory_plastic_synapse), "0");
	gtk_widget_set_size_request(entry_external_neurons_excitatory_plastic_synapse, 20, 25) ;

	
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
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_excitatory_delay_min), "7.0");
	gtk_widget_set_size_request(entry_external_neurons_excitatory_delay_min, 50, 25) ;
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_excitatory_delay_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_excitatory_delay_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_excitatory_delay_max), "7.0");	
	gtk_widget_set_size_request(entry_external_neurons_excitatory_delay_max, 50, 25) ;
		
 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
 	lbl = gtk_label_new("----- IPSP -----");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);       

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Probability:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_inhibitory_connection_probability = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_inhibitory_connection_probability, FALSE,FALSE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_inhibitory_connection_probability), "1.0");
	gtk_widget_set_size_request(entry_external_neurons_inhibitory_connection_probability, 50, 25) ;	
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);	
	lbl = gtk_label_new("Plastic:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_inhibitory_plastic_synapse = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_inhibitory_plastic_synapse , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_inhibitory_plastic_synapse), "0");
	gtk_widget_set_size_request(entry_external_neurons_inhibitory_plastic_synapse, 20, 25) ;

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
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_inhibitory_delay_min), "7.0");
	gtk_widget_set_size_request(entry_external_neurons_inhibitory_delay_min, 50, 25) ;
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_external_neurons_inhibitory_delay_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_external_neurons_inhibitory_delay_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_external_neurons_inhibitory_delay_max), "7.0");	
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

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

 	lbl = gtk_label_new("---------- STDP ----------");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);  

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

 	lbl = gtk_label_new("Pre - Post ----------");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);  

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Change:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	lbl = gtk_label_new("Min:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_STDP_pre_post_change_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_STDP_pre_post_change_min , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_STDP_pre_post_change_min), "0.3");
	gtk_widget_set_size_request(entry_STDP_pre_post_change_min, 50, 25) ;
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_STDP_pre_post_change_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_STDP_pre_post_change_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_STDP_pre_post_change_max), "0.3");	
	gtk_widget_set_size_request(entry_STDP_pre_post_change_max, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Tau(ms):");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	lbl = gtk_label_new("Min:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_STDP_pre_post_tau_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_STDP_pre_post_tau_min , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_STDP_pre_post_tau_min), "30");
	gtk_widget_set_size_request(entry_STDP_pre_post_tau_min, 50, 25) ;
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_STDP_pre_post_tau_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_STDP_pre_post_tau_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_STDP_pre_post_tau_max), "30");	
	gtk_widget_set_size_request(entry_STDP_pre_post_tau_max, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

 	lbl = gtk_label_new("Eligibility ---------");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);  

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Tau(ms):");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	lbl = gtk_label_new("Min:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_eligibility_tau_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_eligibility_tau_min , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_eligibility_tau_min), "150");
	gtk_widget_set_size_request(entry_eligibility_tau_min, 50, 25) ;
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_eligibility_tau_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_eligibility_tau_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_eligibility_tau_max), "150");	
	gtk_widget_set_size_request(entry_eligibility_tau_max, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Max Elig:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	lbl = gtk_label_new("Min:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_max_eligibility_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_max_eligibility_min , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_max_eligibility_min), "1.8");
	gtk_widget_set_size_request(entry_max_eligibility_min, 50, 25) ;
	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_max_eligibility_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_max_eligibility_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_max_eligibility_max), "1.8");	
	gtk_widget_set_size_request(entry_max_eligibility_max, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_stdp_and_eligibility = gtk_button_new_with_label("Submit");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_stdp_and_eligibility, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        lbl = gtk_label_new("Layer:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_STDP_layer = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_STDP_layer, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_STDP_layer), "0");
	gtk_widget_set_size_request(entry_STDP_layer, 25, 25) ;	
        lbl = gtk_label_new("NrnGrp:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	entry_STDP_neuron_group = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_STDP_neuron_group, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_STDP_neuron_group), "0");
	gtk_widget_set_size_request(entry_STDP_neuron_group, 25, 25) ;	

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);  	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_learning_rate = gtk_button_new_with_label("Submit Learning Rate");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_learning_rate, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
        entry_learning_rate = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_learning_rate, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_learning_rate), "1.0");	
	gtk_widget_set_size_request(entry_learning_rate, 50, 25) ;

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);  

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	
	btn_ready_for_simulation = gtk_button_new_with_label("Ready");
	gtk_box_pack_start (GTK_BOX (hbox), btn_ready_for_simulation, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);  	

  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	combos_select_neuron = allocate_layer_neuron_group_neuron_combos();
        gtk_box_pack_start(GTK_BOX(hbox),combos_select_neuron->combo_layer , TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_neuron->combo_neuron_group, TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox),combos_select_neuron->combo_neuron , TRUE,TRUE,0);
	combo_neuron_dynamics = allocate_neuron_dynamics_combo(hbox, combo_neuron_dynamics);

/*	if(!update_texts_of_combos_when_add_remove(combos_select_neuron, get_hybrid_net_rl_bmi_data()->in_silico_network))  // it is put here since main() adds neurons to the layers previously
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "create_network_view_gui(", "! update_texts_of_combos_when_add_remove().");	
*/
   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_synaptic_weight_for_neuron = gtk_button_new_with_label("Submit Weight");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_synaptic_weight_for_neuron, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	entry_weight_lower_limit=  gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_weight_lower_limit, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_weight_lower_limit, 50, 25);	
	gtk_entry_set_text(GTK_ENTRY(entry_weight_lower_limit), "0.001");	
	entry_weight_upper_limit=  gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_weight_upper_limit, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_weight_upper_limit, 50, 25);	
	gtk_entry_set_text(GTK_ENTRY(entry_weight_upper_limit), "1");

   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_excitatory_synaptic_weight_for_neuron = gtk_button_new_with_label("Submit Excita. Weight for Neuron");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_excitatory_synaptic_weight_for_neuron, FALSE, FALSE, 0);	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_new_stdp_and_eligibility_for_neuron = gtk_button_new_with_label("Submit New STDP-Elig for Neuron");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_new_stdp_and_eligibility_for_neuron, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);  	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	combos_select_synapse = allocate_layer_neuron_group_neuron_synapse_combos();
       	gtk_box_pack_start(GTK_BOX(hbox), combos_select_synapse->combo_layer , TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_synapse->combo_neuron_group, TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_synapse->combo_neuron , TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_synapse->combo_synapse , TRUE,TRUE,0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_synaptic_weight_for_synapse = gtk_button_new_with_label("Submit New Weight for Synap");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_synaptic_weight_for_synapse, TRUE, TRUE, 0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_new_stdp_and_eligibility_for_synapse = gtk_button_new_with_label("Submit New STDP-Elig for Synap");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_new_stdp_and_eligibility_for_synapse, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);  	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	
	btn_start_hybrid_network = gtk_button_new_with_label("Start Hybrid Network");
	gtk_box_pack_start (GTK_BOX (hbox), btn_start_hybrid_network, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);  	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
  	btn_select_directory_to_save = gtk_file_chooser_button_new ("Select Directory", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
	gtk_box_pack_start (GTK_BOX (hbox), btn_select_directory_to_save, TRUE, TRUE, 0);

   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	     

	btn_create_recording_folder = gtk_button_new_with_label("Create Recording Folder");
	gtk_box_pack_start (GTK_BOX (hbox), btn_create_recording_folder, TRUE, TRUE, 0);
	set_directory_btn_select_directory_to_save();

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);  	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
  	btn_select_directory_to_load = gtk_file_chooser_button_new ("Select Directory", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
	gtk_box_pack_start (GTK_BOX (hbox), btn_select_directory_to_load, TRUE, TRUE, 0);

   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0); 

	btn_load_data = gtk_button_new_with_label("Load");
	gtk_box_pack_start (GTK_BOX (hbox), btn_load_data, TRUE, TRUE, 0);

	entry_load_data_folder_num =  gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_load_data_folder_num, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_load_data_folder_num, 50, 25);	
	gtk_entry_set_text(GTK_ENTRY(entry_load_data_folder_num), "0");


/////////  GRAPHS  ////////////////////////////////

	vbox = gtk_vbox_new(TRUE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 2,7, 0, 6);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 2,7, 3, 6);
 	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, TRUE,TRUE,0);

	g_signal_connect(G_OBJECT(combo_neuron_type), "changed", G_CALLBACK(combo_neuron_type_func), NULL);
    	g_signal_connect(G_OBJECT(btn_add_neurons_to_layer), "clicked", G_CALLBACK(add_neurons_to_layer_button_func), NULL);
    	g_signal_connect(G_OBJECT(btn_interrogate_network), "clicked", G_CALLBACK(interrogate_network_button_func), NULL);		
      	g_signal_connect(G_OBJECT(btn_interrogate_neuron), "clicked", G_CALLBACK(interrogate_neuron_button_func), NULL); 
      	g_signal_connect(G_OBJECT(btn_submit_parker_sochacki_params), "clicked", G_CALLBACK(submit_parker_sochacki_params_button_func), NULL);
      	g_signal_connect(G_OBJECT(btn_make_output), "clicked", G_CALLBACK(make_output_button_func), NULL);
      	g_signal_connect(G_OBJECT(btn_connect_internal_layer_to_internal_layer), "clicked", G_CALLBACK(connect_internal_layer_to_internal_layer_button_func), NULL);
      	g_signal_connect(G_OBJECT(btn_connect_external_layer_to_internal_layer), "clicked", G_CALLBACK(connect_external_layer_to_internal_layer_button_func), NULL);

      	g_signal_connect(G_OBJECT(btn_submit_stdp_and_eligibility), "clicked", G_CALLBACK(submit_stdp_and_eligibility_button_func), NULL);

      	g_signal_connect(G_OBJECT(btn_submit_learning_rate), "clicked", G_CALLBACK(submit_learning_rate_button_func), NULL);

	g_signal_connect(G_OBJECT(combos_select_neuron->combo_layer), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_layer);
	g_signal_connect(G_OBJECT(combos_select_neuron->combo_neuron_group), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_neuron_group);	
	g_signal_connect(G_OBJECT(combos_select_neuron->combo_neuron), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_neuron);

      	g_signal_connect(G_OBJECT(btn_submit_synaptic_weight_for_neuron), "clicked", G_CALLBACK(submit_synaptic_weight_for_neuron_button_func), NULL);
      	g_signal_connect(G_OBJECT(btn_submit_excitatory_synaptic_weight_for_neuron), "clicked", G_CALLBACK(submit_excitatory_synaptic_weight_for_neuron_button_func), NULL);
      	g_signal_connect(G_OBJECT(btn_submit_synaptic_weight_for_synapse), "clicked", G_CALLBACK(submit_synaptic_weight_for_synapse_button_func), NULL);


      	g_signal_connect(G_OBJECT(btn_submit_new_stdp_and_eligibility_for_neuron), "clicked", G_CALLBACK(submit_new_stdp_and_eligibility_for_neuron_button_func), NULL);
      	g_signal_connect(G_OBJECT(btn_submit_new_stdp_and_eligibility_for_synapse), "clicked", G_CALLBACK(submit_new_stdp_and_eligibility_for_synapse_button_func), NULL);

      	g_signal_connect(G_OBJECT(btn_ready_for_simulation), "clicked", G_CALLBACK(ready_for_simulation_button_func), NULL);

      	g_signal_connect(G_OBJECT(btn_start_hybrid_network), "clicked", G_CALLBACK(start_hybrid_network_button_func), NULL);

	g_signal_connect(G_OBJECT(combos_select_synapse->combo_layer), "changed", G_CALLBACK(combos_select_synapse_func), combos_select_synapse->combo_layer);
	g_signal_connect(G_OBJECT(combos_select_synapse->combo_neuron_group), "changed", G_CALLBACK(combos_select_synapse_func), combos_select_synapse->combo_neuron_group);	
	g_signal_connect(G_OBJECT(combos_select_synapse->combo_neuron), "changed", G_CALLBACK(combos_select_synapse_func), combos_select_synapse->combo_neuron);
	g_signal_connect(G_OBJECT(combos_select_synapse->combo_synapse), "changed", G_CALLBACK(combos_select_synapse_func), combos_select_synapse->combo_synapse);

	g_signal_connect(G_OBJECT(btn_create_recording_folder), "clicked", G_CALLBACK(create_recording_folder_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_load_data), "clicked", G_CALLBACK(load_data_button_func), NULL);

//	gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, FALSE);
	gtk_widget_set_sensitive(btn_make_output, FALSE);	
	gtk_widget_set_sensitive(btn_connect_internal_layer_to_internal_layer, FALSE);	
	gtk_widget_set_sensitive(btn_connect_external_layer_to_internal_layer, FALSE);
	gtk_widget_set_sensitive(btn_submit_synaptic_weight_for_neuron, FALSE);	
	gtk_widget_set_sensitive(btn_submit_excitatory_synaptic_weight_for_neuron, FALSE);	
	gtk_widget_set_sensitive(btn_submit_synaptic_weight_for_synapse, FALSE);	
	gtk_widget_set_sensitive(btn_submit_new_stdp_and_eligibility_for_neuron, FALSE);
	gtk_widget_set_sensitive(btn_submit_new_stdp_and_eligibility_for_synapse, FALSE);
	gtk_widget_set_sensitive(btn_start_hybrid_network, FALSE);	
	gtk_widget_set_sensitive(btn_submit_stdp_and_eligibility, TRUE);	
	gtk_widget_set_sensitive(btn_ready_for_simulation, FALSE);	
	gtk_widget_set_sensitive(btn_create_recording_folder, FALSE);	
	gtk_widget_set_sensitive(btn_submit_learning_rate, FALSE);	

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

	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, num_of_neuron, layer, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "add_neurons_to_layer_button_func", "! add_iz_neurons_to_layer().");	
	if(!update_texts_of_combos_when_add_remove(combos_select_neuron, bmi_data->in_silico_network))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "add_neurons_to_layer_button_func", "! update_texts_of_combos_when_add_remove().");	
	gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, TRUE);		
	return;
}

void interrogate_network_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	interrogate_network(bmi_data->in_silico_network);
	printf ("learning_rate: %f\n", bmi_data->reward_data.learning_rate);
} 		

void interrogate_neuron_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	interrogate_neuron	(bmi_data->in_silico_network, gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_layer)), gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron_group)), gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron)) );
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
	if (! parker_sochacki_set_order_tolerance(bmi_data->in_silico_network, (unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_parker_sochacki_max_order))), atof(gtk_entry_get_text(GTK_ENTRY(entry_parker_sochacki_err_tol)))))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_parker_sochacki_params_button_func", "! parker_sochacki_set_order_tolerance().");
	
	gtk_widget_set_sensitive(btn_add_neurons_to_layer, FALSE);			
	gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, FALSE);	
	gtk_widget_set_sensitive(btn_make_output, TRUE);	
}

static void make_output_button_func(void)
{
	unsigned int layer_num;
	char *end_ptr;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	layer_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_layer_to_make_output)), &end_ptr, 10);	
	set_layer_type_of_the_neurons_in_layer(bmi_data->in_silico_network, layer_num, NEURON_LAYER_TYPE_OUTPUT);
	gtk_widget_set_sensitive(btn_connect_internal_layer_to_internal_layer, TRUE);	
	gtk_widget_set_sensitive(btn_connect_external_layer_to_internal_layer, TRUE);
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
	AxonalDelay EPSP_delay_max = (AxonalDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_excitatory_delay_max))));
	AxonalDelay EPSP_delay_min = (AxonalDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_excitatory_delay_min))));
	AxonalDelay IPSP_delay_max = (AxonalDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_inhibitory_delay_max))));
	AxonalDelay IPSP_delay_min = (AxonalDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_inhibitory_delay_min))));
	double excitatory_connection_probability = atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_excitatory_connection_probability)));
	double inhibitory_connection_probability = atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_inhibitory_connection_probability)));
	bool excitatory_plastic = (bool)atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_excitatory_plastic_synapse)));
	bool inhibitory_plastic = (bool)atof(gtk_entry_get_text(GTK_ENTRY(entry_internal_neurons_inhibitory_plastic_synapse)));

	if (! connect_layers(bmi_data->in_silico_network, this_layer, bmi_data->in_silico_network, target_layer, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_max, EPSP_delay_min, IPSP_delay_max, IPSP_delay_min, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, excitatory_connection_probability, inhibitory_connection_probability, excitatory_plastic, inhibitory_plastic))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "connect_internal_layer_to_internal_layer_button_func", "! connect_network_layer_to_network_layer().");	
	gtk_widget_set_sensitive(btn_submit_stdp_and_eligibility, TRUE);	
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
	AxonalDelay EPSP_delay_max = (AxonalDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_excitatory_delay_max))));
	AxonalDelay EPSP_delay_min = (AxonalDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_excitatory_delay_min))));
	AxonalDelay IPSP_delay_max = (AxonalDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_inhibitory_delay_max))));
	AxonalDelay IPSP_delay_min = (AxonalDelay)(1000000*atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_inhibitory_delay_min))));
	double excitatory_connection_probability = atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_excitatory_connection_probability)));
	double inhibitory_connection_probability = atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_inhibitory_connection_probability)));
	bool excitatory_plastic = (bool)atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_excitatory_plastic_synapse)));
	bool inhibitory_plastic = (bool)atof(gtk_entry_get_text(GTK_ENTRY(entry_external_neurons_inhibitory_plastic_synapse)));

	if (! connect_layers(bmi_data->blue_spike_network, this_layer, bmi_data->in_silico_network, target_layer, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_max, EPSP_delay_min, IPSP_delay_max, IPSP_delay_min, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, excitatory_connection_probability, inhibitory_connection_probability, excitatory_plastic, inhibitory_plastic))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "connect_external_layer_to_internal_layer_button_func", "! connect_ext_network_layer_to_int_network_layer().");
	gtk_widget_set_sensitive(btn_submit_stdp_and_eligibility, TRUE);			
}

static void submit_stdp_and_eligibility_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	unsigned int layer = (unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_STDP_layer)));
	unsigned int neuron_group = (unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_STDP_neuron_group)));
	double STDP_pre_post_change_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_STDP_pre_post_change_min)));
	double STDP_pre_post_change_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_STDP_pre_post_change_max)));
	double STDP_pre_post_tau_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_STDP_pre_post_tau_min)));
	double STDP_pre_post_tau_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_STDP_pre_post_tau_max)));
	double eligibility_tau_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_eligibility_tau_min)));
	double eligibility_tau_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_eligibility_tau_max)));
	double max_eligibility_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_max_eligibility_min)));
	double max_eligibility_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_max_eligibility_max)));

	if (! create_ps_pre_post_stdp_for_neuron_group(bmi_data->in_silico_network, layer, neuron_group, STDP_pre_post_change_min, STDP_pre_post_change_max, STDP_pre_post_tau_min, STDP_pre_post_tau_max))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_stdp_and_eligibility_button_func", "! create_ps_stdp_for_neuron_group().");	

	if (! create_ps_eligibility_for_neuron_group(bmi_data->in_silico_network, layer, neuron_group, eligibility_tau_min, eligibility_tau_max, max_eligibility_min, max_eligibility_max))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_stdp_and_eligibility_button_func", "! create_ps_eligibility_for_neuron_group().");	

	gtk_widget_set_sensitive(btn_submit_learning_rate, TRUE);	
	gtk_widget_set_sensitive(btn_ready_for_simulation, TRUE);	
	return;
}

static void combos_select_neuron_func(GtkWidget *changed_combo)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	if (bmi_data == NULL)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "combos_select_neuron_func", "bmi_data == NULL.");
	if(!update_texts_of_combos_when_change(combos_select_neuron, bmi_data->in_silico_network, changed_combo))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "combos_select_neuron_func", "!update_texts_of_combos_when_change().");
}

static void submit_synaptic_weight_for_neuron_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	Neuron *neuron;
	unsigned int layer_num, nrn_grp_num, nrn_num;
	double lower_limit, upper_limit;
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_synaptic_weight_for_neuron_button_func", "! layer_neuron_group_neuron_get_selected().");
	neuron = get_neuron_address(bmi_data->in_silico_network, layer_num, nrn_grp_num, nrn_num);					
	lower_limit = atof(gtk_entry_get_text(GTK_ENTRY(entry_weight_lower_limit)));
	upper_limit = atof(gtk_entry_get_text(GTK_ENTRY(entry_weight_upper_limit)));
	if (! set_neuron_synaptic_weights(neuron, lower_limit, upper_limit))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_synaptic_weight_for_neuron_button_func", "! set_neuron_synaptic_weights().");
}

static void submit_excitatory_synaptic_weight_for_neuron_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	Neuron *neuron;
	unsigned int layer_num, nrn_grp_num, nrn_num;
	double lower_limit, upper_limit;
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_excitatory_synaptic_weight_for_neuron_button_func", "! layer_neuron_group_neuron_get_selected().");
	neuron = get_neuron_address(bmi_data->in_silico_network, layer_num, nrn_grp_num, nrn_num);					
	lower_limit = atof(gtk_entry_get_text(GTK_ENTRY(entry_weight_lower_limit)));
	upper_limit = atof(gtk_entry_get_text(GTK_ENTRY(entry_weight_upper_limit)));
	if (! set_neuron_excitatory_synaptic_weights(neuron, lower_limit, upper_limit))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_excitatory_synaptic_weight_for_neuron_button_func", "! set_neuron_excitatory_synaptic_weights().");
}

static void submit_new_stdp_and_eligibility_for_neuron_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	Neuron *neuron;
	unsigned int layer_num, nrn_grp_num, nrn_num;
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_new_stdp_and_eligibility_for_neuron_button_func", "! layer_neuron_group_neuron_get_selected().");
	neuron = get_neuron_address(bmi_data->in_silico_network, layer_num, nrn_grp_num, nrn_num);		
	double STDP_pre_post_change_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_STDP_pre_post_change_min)));
	double STDP_pre_post_change_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_STDP_pre_post_change_max)));
	double STDP_pre_post_tau_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_STDP_pre_post_tau_min)));
	double STDP_pre_post_tau_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_STDP_pre_post_tau_max)));

	double eligibility_tau_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_eligibility_tau_min)));
	double eligibility_tau_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_eligibility_tau_max)));
	double max_eligibility_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_max_eligibility_min)));
	double max_eligibility_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_max_eligibility_max)));

	if (! submit_new_ps_eligibility_vals_for_neuron(neuron, eligibility_tau_min, eligibility_tau_max, max_eligibility_min, max_eligibility_max))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_new_stdp_and_eligibility_for_neuron_button_func", "! submit_new_ps_eligibility_vals_for_neuron().");	
	if (! submit_new_ps_pre_post_stdp_vals_for_neuron(neuron, STDP_pre_post_change_min, STDP_pre_post_change_max, STDP_pre_post_tau_min, STDP_pre_post_tau_max))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_new_stdp_and_eligibility_for_neuron_button_func", "! submit_new_ps_pre_post_stdp_vals_for_neuron().");	

	gtk_widget_set_sensitive(btn_ready_for_simulation, TRUE);	
	return;
}

static void submit_synaptic_weight_for_synapse_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	Neuron *neuron;
	unsigned int layer_num, nrn_grp_num, nrn_num, syn_num;
	double lower_limit, upper_limit;

	if (! layer_neuron_group_neuron_synapse_get_selected(combos_select_synapse, &layer_num, &nrn_grp_num, &nrn_num, &syn_num))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_synaptic_weight_for_synapse_button_func", "! layer_neuron_group_neuron_synapse_get_selected().");	

	neuron = get_neuron_address(bmi_data->in_silico_network, layer_num, nrn_grp_num, nrn_num);					
	lower_limit = atof(gtk_entry_get_text(GTK_ENTRY(entry_weight_lower_limit)));
	upper_limit = atof(gtk_entry_get_text(GTK_ENTRY(entry_weight_upper_limit)));
	if (! set_neuron_synapse_synaptic_weight(neuron, lower_limit, upper_limit, syn_num))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_synaptic_weight_for_synapse_button_func", "! set_neuron_synapse_synaptic_weight().");

}

static void submit_new_stdp_and_eligibility_for_synapse_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	Neuron *neuron;
	unsigned int layer_num, nrn_grp_num, nrn_num, syn_num;
	if (! layer_neuron_group_neuron_synapse_get_selected(combos_select_synapse, &layer_num, &nrn_grp_num, &nrn_num, &syn_num))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_new_stdp_and_eligibility_for_synapse_button_func", "! layer_neuron_group_neuron_synapse_get_selected().");	

	neuron = get_neuron_address(bmi_data->in_silico_network, layer_num, nrn_grp_num, nrn_num);		
	double STDP_pre_post_change_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_STDP_pre_post_change_min)));
	double STDP_pre_post_change_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_STDP_pre_post_change_max)));
	double STDP_pre_post_tau_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_STDP_pre_post_tau_min)));
	double STDP_pre_post_tau_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_STDP_pre_post_tau_max)));

	double eligibility_tau_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_eligibility_tau_min)));
	double eligibility_tau_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_eligibility_tau_max)));
	double max_eligibility_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_max_eligibility_min)));
	double max_eligibility_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_max_eligibility_max)));

	if (! submit_new_ps_eligibility_vals_for_synapse(neuron, eligibility_tau_min, eligibility_tau_max, syn_num, max_eligibility_min, max_eligibility_max))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_new_stdp_and_eligibility_for_neuron_button_func", "! submit_new_ps_eligibility_vals_for_neuron().");
	if (! submit_new_ps_pre_post_stdp_vals_for_synapse(neuron, STDP_pre_post_change_min, STDP_pre_post_change_max, STDP_pre_post_tau_min, STDP_pre_post_tau_max, syn_num))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_new_stdp_and_eligibility_for_neuron_button_func", "! submit_new_ps_stdp_vals_for_neuron().");	
	
	gtk_widget_set_sensitive(btn_ready_for_simulation, TRUE);	
	return;	
}

static void start_hybrid_network_button_func(void)
{
	gtk_widget_set_sensitive(btn_start_hybrid_network, FALSE);	
	hybrid_net_rl_bmi_create_rt_threads();	
	send_global_pause_button_sensitive_request();	 // enable pause/resume button to resume buffer visualization. resume adjusts buffer read indexes and start times. 
}

static void ready_for_simulation_button_func(void)
{
	unsigned int i;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	SpikeData *spike_data;
	bmi_data->neuron_dynamics_limited_buffer = allocate_neuron_dynamics_buffer_limited(bmi_data->in_silico_network, bmi_data->neuron_dynamics_limited_buffer, 3000000000/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, NUM_OF_NEURON_DYNAMICS_GRAPHS);  // 3 second buffer for 1 second graph refresh rate. 
	bmi_data->stdp_limited_buffer = allocate_stdp_buffer_limited(bmi_data->in_silico_network, bmi_data->stdp_limited_buffer, 3000000000/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, NUM_OF_STDP_GRAPHS);  // 3 second buffer for 1 second graph refresh rate. 
	bmi_data->eligibility_limited_buffer = allocate_eligibility_buffer_limited(bmi_data->in_silico_network, bmi_data->eligibility_limited_buffer, 3000000000/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, NUM_OF_ELIGIBILITY_GRAPHS);  // 3 second buffer for 1 second graph refresh rate. 
	bmi_data->blue_spike_spike_data_for_graph = g_new0(SpikeData*, 1);
	bmi_data->blue_spike_spike_data_for_graph[0] = allocate_spike_data(bmi_data->blue_spike_spike_data_for_graph[0], get_num_of_neurons_in_network(bmi_data->blue_spike_network)*3*500 ); // 3 seconds buffer assuming a neuron firing rate cannot be more than 500 Hz 
	for (i = 0; i < bmi_data->num_of_dedicated_cpu_threads; i ++)
		bmi_data->in_silico_spike_data_for_graph[i] = allocate_spike_data(bmi_data->in_silico_spike_data_for_graph[i], get_num_of_neurons_in_network(bmi_data->in_silico_network)*3*500 ); // 3 seconds buffer assuming a neuron firing rate cannot be more than 500 Hz 
	for (i = 0; i < bmi_data->num_of_dedicated_cpu_threads; i ++)
		bmi_data->in_silico_spike_data_for_recording[i] = allocate_spike_data(bmi_data->in_silico_spike_data_for_recording[i], get_num_of_neurons_in_network(bmi_data->in_silico_network)*3*500 ); // 3 seconds buffer assuming a neuron firing rate cannot be more than 500 Hz 

	if(!update_texts_of_synapse_combos_when_add_remove(combos_select_synapse, bmi_data->in_silico_network))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "ready_for_simulation_button_func", "! update_texts_of_combos_when_add_remove().");	

	if(!update_texts_of_combos_when_add_remove(combos_select_neuron, bmi_data->in_silico_network))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "add_neurons_to_layer_button_func", "! update_texts_of_combos_when_add_remove().");	

	if (!buffer_view_handler())
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "ready_for_simulation_button_func", "! create_buffers_view_gui().");	
	gtk_widget_set_sensitive(btn_start_hybrid_network, TRUE);	
	gtk_widget_set_sensitive(btn_submit_synaptic_weight_for_neuron, TRUE);
	gtk_widget_set_sensitive(btn_submit_excitatory_synaptic_weight_for_neuron, TRUE);	
	gtk_widget_set_sensitive(btn_submit_synaptic_weight_for_synapse, TRUE);	
	gtk_widget_set_sensitive(btn_submit_new_stdp_and_eligibility_for_neuron, TRUE);	
	gtk_widget_set_sensitive(btn_submit_new_stdp_and_eligibility_for_synapse, TRUE);	
	gtk_widget_set_sensitive(btn_create_recording_folder, TRUE);	

	for (i = 0; i < bmi_data->num_of_dedicated_cpu_threads; i++)
	{
		spike_data = bmi_data->in_silico_spike_data_for_recording[i];
		spike_data->buff_idx_read = spike_data->buff_idx_write;
	}
	static_bmi_data = get_hybrid_net_rl_bmi_data();
	static_all_neurons = static_bmi_data->in_silico_network->all_neurons;
	static_num_of_neurons = static_bmi_data->in_silico_network->num_of_neurons;
  	g_timeout_add(50, timeout_callback, NULL);		
}

static void combos_select_synapse_func(GtkWidget *changed_combo)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();

	if (bmi_data == NULL)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "combos_select_synapse_func", "bmi_data == NULL.");

	if(!update_texts_of_synapse_combos_when_change(combos_select_synapse, bmi_data->in_silico_network, changed_combo))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "combos_select_synapse_func", "! update_texts_of_combos_when_change().");			
}

static void submit_learning_rate_button_func (void)
{
	double learning_rate = atof(gtk_entry_get_text(GTK_ENTRY(entry_learning_rate)));
	if (learning_rate < 0)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "submit_learning_rate_button_func", "learning_rate <= 0.");

	get_hybrid_net_rl_bmi_data()->reward_data.learning_rate = learning_rate;

	gtk_widget_set_sensitive(btn_ready_for_simulation, TRUE);	
}


static void create_recording_folder_button_func (void)
{
	unsigned int path_len;
	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
	path = &path_temp[7];   // since     uri returns file:///home/....	
	path_len = strlen(path_temp);
	if (strcmp(&(path_temp[path_len-8]),"EXP_DATA") == 0)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "create_recording_folder_button_func", "Selected folder is /EXP_DATA main folder. Select a folder inside this folder.");				
	if ((*create_main_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(1, path))		// record in last format version
	{
		
	}
	else
		print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "create_recording_folder_button_func", " *create_main_directory().");			
}


static void set_directory_btn_select_directory_to_save(void)
{
	char line[600];
	FILE *fp = NULL;
       	if ((fp = fopen("./path_initial_directory", "r")) == NULL)  
       	{ 
		print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "set_directory_btn_select_directory_to_save", "Couldn't find file: ./path_initial_directory.");
		print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "set_directory_btn_select_directory_to_save", "/home is loaded as initial direcoty to create data folder.");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),"/home");
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "set_directory_btn_select_directory_to_save", "Couldn' t read ./path_initial_directory.");
			print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "set_directory_btn_select_directory_to_save", "/home is loaded as initial direcoty to create data folder.");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),"/home");
		}
		else
		{
			line[strlen(line)-16] = 0;   
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),line);
		}
		fclose(fp); 		
	}  	 
}


static gboolean timeout_callback(gpointer user_data) 
{
	char *path_temp, *path;
	static bool recording = FALSE;
	unsigned int i, recording_number;
	NeuralNet2GuiMsgItem msg_item;
	double reward, learning_rate;



	while (get_next_neural_net_2_gui_msg_buffer_item(static_msgs_neural_net_2_gui, &msg_item))
	{
		switch (msg_item.msg_type)
		{
			case NEURAL_NET_2_GUI_MSG_START_RECORDING:
				path_temp = NULL; path = NULL;
				path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
				path = &path_temp[7];   // since     uri returns file:///home/....	
				recording_number = msg_item.additional_data.recording_number;
				if (!(*create_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(3, path, &rt_tasks_data->current_system_time, recording_number))	
				{
					print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "timeout_callback", " *create_data_directory().");		
					exit(1);
				}
				recording = TRUE;	
				break;
			case NEURAL_NET_2_GUI_MSG_STOP_RECORDING:
				if (! (*fclose_all_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(1, &rt_tasks_data->current_system_time))	
				{
					print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "timeout_callback", " *fclose_all_data_file().");		
					exit(1);
				}
				recording = FALSE;		
				break;
			case NEURAL_NET_2_GUI_MSG_CANCEL_RECORDING:
				path_temp = NULL; path = NULL;
				path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
				path = &path_temp[7];   // since     uri returns file:///home/....		

				recording_number = msg_item.additional_data.recording_number;
				if (! (*fclose_all_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(1, &rt_tasks_data->current_system_time))	
				{
					print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "timeout_callback", "! *fclose_all_data_files().");
					exit(1);
				}
				if (! (*delete_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, path, recording_number))
				{
					print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "timeout_callback", " *delete_all_data_files().");
					exit(1);
				}
				recording = FALSE;
				break;
			case NEURAL_NET_2_GUI_MSG_UPDATE_SYNAPTIC_WEIGHTS:
				usleep(100000);  /// sleep 100 ms so that rt neural net simulator can save eligibility traces which is handled by all neurons received from trial handler. 
				reward = msg_item.additional_data.reward;
				learning_rate = static_bmi_data->reward_data.learning_rate;
				for (i = 0; i < static_num_of_neurons; i++)
				{
					if (! write_neuron_synaptic_weights_to_history(static_all_neurons[i])) {
						print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "timeout_callback", "! update_neuron_synaptic_weights()"); exit(1); }
				}
				break;
			default:
				return print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "timeout_callback", "switch (msg_item.msg_type) - default");
		}
	}
	if (recording)
	{
		if (!(*write_to_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(0))	
		{
			print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "timeout_callback", " *write_to_data_files().");		
			exit(1);
		}	
	}
	else
	{
		for (i = 0; i < num_of_dedicated_cpu_threads; i++)
		{
			reset_spike_data_read_idx(in_silico_spike_data_for_recording[i]);
		}
	}
	return TRUE;
}

static void load_data_button_func (void)
{
	unsigned int path_len, data_folder_num;
	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_load));
	path = &path_temp[7];   // since     uri returns file:///home/....	
	path_len = strlen(path_temp);

	data_folder_num = (unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_load_data_folder_num)));
			
	if ((*load_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(5, path, get_hybrid_net_rl_bmi_data()->in_silico_network, get_hybrid_net_rl_bmi_data()->blue_spike_network, data_folder_num, get_hybrid_net_rl_bmi_data()))		// record in last format version
	{
		print_message(INFO_MSG ,"HybridNetRLBMI", "Gui", "load_data_button_func", "Successfully loaded data");	
		gtk_widget_set_sensitive(btn_ready_for_simulation, TRUE);		
		gtk_widget_set_sensitive(btn_submit_learning_rate, TRUE);				
	}
	else
	{
		print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "load_data_button_func", " *load_data_directory().");			
	}
}



