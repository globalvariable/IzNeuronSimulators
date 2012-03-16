#include "CurrentPatternView.h"



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

static GtkWidget *entry_num_of_trial_start_available_currents;
static GtkWidget *entry_num_of_in_refractory_currents;
static GtkWidget *entry_num_of_in_trial_currents;
static GtkWidget *btn_submit_num_of_currents;

static GtkWidget *entry_in_refractory_current_num;
static GtkWidget *entry_in_refractory_current_length;
static GtkWidget *entry_trial_start_available_current_num;
static GtkWidget *entry_trial_start_available_current_length;
static GtkWidget *btn_submit_current_lengths;
static GtkWidget *btn_generate_current_injection_graphs;

static GtkWidget *entry_parker_sochacki_err_tol;
static GtkWidget *entry_parker_sochacki_max_order;
static GtkWidget *btn_submit_parker_sochacki_params;

static GtkWidget *current_pattern_graph_hbox;
static CurrentPatternGraph *current_pattern_graph = NULL;
static GtkWidget *neuron_dynamics_graph_hbox;
static NeuronDynamicsGraph *neuron_dynamics_graph = NULL;



// SECOND COLUMN
static LayerNrnGrpNeuronCombo *combos_select_neuron;
static TrialTypesCombo *combo_trial_type;
static GtkWidget *entry_current_pattern_number;
static GtkWidget *btn_display_currents_and_dynamics_in_trial;
static GtkWidget *btn_display_currents_and_dynamics_trial_available;
static GtkWidget *btn_display_currents_and_dynamics_in_refractory;


static GtkWidget *btn_refresh_screen;

static GtkWidget *btn_display_current_template;
static GtkWidget *btn_display_patterns_current;

static GtkWidget *btn_interrogate_network;
static GtkWidget *btn_interrogate_neuron;
static GtkWidget *btn_interrogate_neuron_current_params_in_pattern;

static GtkWidget *entry_init_current_max;
static GtkWidget *entry_init_current_duration;
static GtkWidget *btn_submit_initial_current_params;

static GtkWidget *combo_signal_type;
static GtkWidget *entry_amplifier;
static GtkWidget *entry_freq;
static GtkWidget *entry_start_time;
static GtkWidget *entry_end_time;
static GtkWidget *entry_initial_current;
static GtkWidget *btn_draw_template;
static GtkWidget *btn_clear_template;
static GtkWidget *btn_copy_drawn_to_raw_stimuli;
static GtkWidget *btn_add_noise;
static GtkWidget *entry_noise_variance;
static GtkWidget *entry_noise_period;
static GtkWidget *btn_submit_initial_neuron_voltage;
static GtkWidget *entry_initial_neuron_voltage;
static GtkWidget *entry_initial_neuron_voltage_variance;
static GtkWidget *btn_simulate;
static GtkWidget *btn_display_neuron_dynamics;
static GtkWidget *combo_dynamics_type;


/// LAST COLUMN
static GtkWidget *txv_notes;
static GtkWidget *btn_select_directory;
static GtkWidget *btn_create_directory;
static GtkWidget *btn_save;
static GtkWidget *btn_load;

// THIRD COLUMN
///   at graphs.h


static void combo_neuron_type_func (void);
static void add_neurons_to_layer_button_func(void);
static void submit_num_of_currents_button_func(void);
static void submit_current_lengths_button_func(void);
static void generate_current_injection_graphs_button_func(void);
static void submit_parker_sochacki_params_button_func(void);



static void submit_initial_current_params_button_func(void);
static void combos_select_neuron_func(GtkWidget *changed_combo);
static void refresh_screen_button_func(void);
static void display_current_template_button_func(void);
static void display_patterns_current_button_func(void);
static void draw_template_button_func(void);
static void clear_template_button_func(void);

bool create_current_pattern_view_gui(GtkWidget *tabs)
{
	GtkWidget *frame, *frame_label, *table, *vbox, *hbox, *lbl;

        frame = gtk_frame_new ("Network & Currents");
        frame_label = gtk_label_new ("Network & Currents");      
   
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
	gtk_widget_set_size_request(combo_neuron_type, 180, 30) ;
	
	int i; char neuron_type_str[50];
	for (i = 0; i < MAX_NUM_OF_NEURON_TYPES; i++)
	{
		gtk_combo_box_append_text(GTK_COMBO_BOX(combo_neuron_type), get_neuron_type_string(i, neuron_type_str));
	}
 	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_neuron_type), 1);


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
	
        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,10);	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("# of Trial Availab. Currents: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
       	entry_num_of_trial_start_available_currents = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_num_of_trial_start_available_currents, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_num_of_trial_start_available_currents), "1");
	gtk_widget_set_size_request(entry_num_of_trial_start_available_currents, 30, 25) ; 

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("# of In Refractory Currents: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
       	entry_num_of_in_refractory_currents = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_num_of_in_refractory_currents, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_num_of_in_refractory_currents), "1");
	gtk_widget_set_size_request(entry_num_of_in_refractory_currents, 30, 25) ; 

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("# In Trial Current: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
       	entry_num_of_in_trial_currents = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_num_of_in_trial_currents, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_num_of_in_trial_currents), "1");
	gtk_widget_set_size_request(entry_num_of_in_trial_currents, 30, 25) ; 

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_num_of_currents = gtk_button_new_with_label("Submit Num of Trial Currents");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_num_of_currents, TRUE, TRUE, 0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("TrialAvailCurr #: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
       	entry_trial_start_available_current_num = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_trial_start_available_current_num, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_trial_start_available_current_num), "0");
	gtk_widget_set_size_request(entry_trial_start_available_current_num, 25, 25) ; 

	lbl = gtk_label_new("Len(ms): ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
       	entry_trial_start_available_current_length = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_trial_start_available_current_length, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_trial_start_available_current_length), "100");
	gtk_widget_set_size_request(entry_trial_start_available_current_length, 40, 25) ; 

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("InRefracCurr # : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
       	entry_in_refractory_current_num = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_in_refractory_current_num, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_in_refractory_current_num), "0");
	gtk_widget_set_size_request(entry_in_refractory_current_num, 25, 25) ; 

	lbl = gtk_label_new("Len(ms): ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
       	entry_in_refractory_current_length = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_in_refractory_current_length, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_in_refractory_current_length), "100");
	gtk_widget_set_size_request(entry_in_refractory_current_length, 40, 25) ; 

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_current_lengths = gtk_button_new_with_label("Submit Current Lengths");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_current_lengths, TRUE, TRUE, 0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_generate_current_injection_graphs = gtk_button_new_with_label("Generate Graphs");
	gtk_box_pack_start (GTK_BOX (hbox), btn_generate_current_injection_graphs, TRUE, TRUE, 0);

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
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
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
///////////////////////////////////////////// SECOND COLUMN  ///////////////////////////////////////////////////////////////

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 1,2,0,6);     ///  Stimulus graph control

	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
 
	combos_select_neuron = allocate_layer_neuron_group_neuron_combos();
        gtk_box_pack_start(GTK_BOX(hbox),combos_select_neuron->combo_layer , TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_neuron->combo_neuron_group, TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox),combos_select_neuron->combo_neuron , TRUE,TRUE,0);

  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	combo_trial_type = allocate_trial_types_combo();
        gtk_box_pack_start(GTK_BOX(hbox), combo_trial_type->combo, TRUE,TRUE,0);
	if (! update_trial_types_combo(get_bmi_simulation_spike_generator_trials_data(), combo_trial_type))
		return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternView", "create_current_pattern_view_gui", "! update_trial_types_combo().");

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Trial Status Current Number: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	entry_current_pattern_number = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_current_pattern_number, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_current_pattern_number), "0");
	gtk_widget_set_size_request(entry_current_pattern_number, 30, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Display Neuron Current & Dynamics:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_display_currents_and_dynamics_in_trial = gtk_button_new_with_label("InTrial");
	gtk_box_pack_start (GTK_BOX (hbox), btn_display_currents_and_dynamics_in_trial, TRUE, TRUE, 0);
	btn_display_currents_and_dynamics_trial_available = gtk_button_new_with_label("TrialAvail");
	gtk_box_pack_start (GTK_BOX (hbox), btn_display_currents_and_dynamics_trial_available, TRUE, TRUE, 0);
	btn_display_currents_and_dynamics_in_refractory = gtk_button_new_with_label("InRefrac");
	gtk_box_pack_start (GTK_BOX (hbox), btn_display_currents_and_dynamics_in_refractory, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_refresh_screen = gtk_button_new_with_label("Refresh Screen");
	gtk_box_pack_start (GTK_BOX (hbox), btn_refresh_screen, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_display_current_template = gtk_button_new_with_label("Display Current Template");
	gtk_box_pack_start (GTK_BOX (hbox), btn_display_current_template, TRUE, TRUE, 0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_display_patterns_current = gtk_button_new_with_label("Display Pattern's Current");
	gtk_box_pack_start (GTK_BOX (hbox), btn_display_patterns_current, TRUE, TRUE, 0);
	
	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_interrogate_network = gtk_button_new_with_label("Interrogate Network");
	gtk_box_pack_start (GTK_BOX (hbox), btn_interrogate_network, TRUE, TRUE, 0);	

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_interrogate_neuron = gtk_button_new_with_label("Interrogate Neuron");
	gtk_box_pack_start (GTK_BOX (hbox), btn_interrogate_neuron, TRUE, TRUE, 0);

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_interrogate_neuron_current_params_in_pattern = gtk_button_new_with_label("Interrogate Current Params");
	gtk_box_pack_start (GTK_BOX (hbox), btn_interrogate_neuron_current_params_in_pattern, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("InitCurMax:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_init_current_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_init_current_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_init_current_max), "0");
	gtk_widget_set_size_request(entry_init_current_max, 50, 25) ;

	lbl = gtk_label_new("Dur(ms):");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_init_current_duration = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_init_current_duration, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_init_current_duration), "0");
	gtk_widget_set_size_request(entry_init_current_duration, 50, 25) ;

 	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_initial_current_params = gtk_button_new_with_label("Submit Initial Current Params");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_initial_current_params, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("amp/slop:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_amplifier = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_amplifier, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_amplifier), "0");
	gtk_widget_set_size_request(entry_amplifier, 50, 25) ;

	lbl = gtk_label_new("freq:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_freq = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_freq, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_freq), "0");
	gtk_widget_set_size_request(entry_freq, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Start time (ms):	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_start_time = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_start_time, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_start_time), "0");
	gtk_widget_set_size_request(entry_start_time, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("End time (ms):	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_end_time = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_end_time, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_end_time), "0");
	gtk_widget_set_size_request(entry_end_time, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Initial Current:		");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_initial_current = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_initial_current, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_initial_current), "0");
	gtk_widget_set_size_request(entry_initial_current, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	combo_signal_type = gtk_combo_box_new_text();
        gtk_box_pack_start(GTK_BOX(hbox),combo_signal_type, TRUE,TRUE,0);
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_signal_type), "Line");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_signal_type), "Sin");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_signal_type), "Cos");
 	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_signal_type), 0);
 	
	btn_draw_template = gtk_button_new_with_label("Draw");
	gtk_box_pack_start (GTK_BOX (hbox), btn_draw_template, TRUE, TRUE, 0);
	
	btn_clear_template = gtk_button_new_with_label("Clear");
	gtk_box_pack_start (GTK_BOX (hbox), btn_clear_template, TRUE, TRUE, 0);	

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_copy_drawn_to_raw_stimuli = gtk_button_new_with_label("Copy Drawn to Raw Stimuli");
	gtk_box_pack_start (GTK_BOX (hbox), btn_copy_drawn_to_raw_stimuli, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Noise Var:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);

        entry_noise_variance = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_noise_variance, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_variance), "0");
	gtk_widget_set_size_request(entry_noise_variance, 50, 25) ;

	lbl = gtk_label_new("Interval:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);

        entry_noise_period = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_noise_period, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_period), "1");
	gtk_widget_set_size_request(entry_noise_period, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_add_noise = gtk_button_new_with_label("Add Noise");
	gtk_box_pack_start (GTK_BOX (hbox), btn_add_noise, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Initial Volt-");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        
  	lbl = gtk_label_new("Mean:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        
        entry_initial_neuron_voltage = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_initial_neuron_voltage , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_initial_neuron_voltage ), "0");
	gtk_widget_set_size_request(entry_initial_neuron_voltage, 50, 25) ;

	lbl = gtk_label_new("Var:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_initial_neuron_voltage_variance = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_initial_neuron_voltage_variance , FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_initial_neuron_voltage_variance ), "0");
	gtk_widget_set_size_request(entry_initial_neuron_voltage_variance, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_initial_neuron_voltage = gtk_button_new_with_label("Submit Initial Voltage");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_initial_neuron_voltage, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_simulate = gtk_button_new_with_label("Simulate");
	gtk_box_pack_start (GTK_BOX (hbox), btn_simulate, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);
	
	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
        
	btn_display_neuron_dynamics = gtk_button_new_with_label("Display Neuron Dynamics");
	gtk_box_pack_start (GTK_BOX (hbox), btn_display_neuron_dynamics, TRUE, TRUE, 0);   

	combo_dynamics_type = gtk_combo_box_new_text();
        gtk_box_pack_start(GTK_BOX(hbox),combo_dynamics_type, FALSE,FALSE,0);
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_dynamics_type), "v");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_dynamics_type), "u");
 	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_dynamics_type), 0);
 	gtk_widget_set_size_request(combo_dynamics_type, 45, 25) ;

///	GRAPHS

 	current_pattern_graph_hbox = gtk_hbox_new(TRUE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), current_pattern_graph_hbox, 2,7, 0, 3);

 	neuron_dynamics_graph_hbox = gtk_hbox_new(TRUE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), neuron_dynamics_graph_hbox, 2,7, 3, 6);

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
	fill_notes_text_view();

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);  	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Folder : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);

	btn_select_directory = gtk_file_chooser_button_new ("Select Folder", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox),btn_select_directory, TRUE,TRUE,0);
	set_directory_btn_select_directory();
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_create_directory = gtk_button_new_with_label("Create Data Directory");
	gtk_box_pack_start (GTK_BOX (hbox), btn_create_directory, TRUE, TRUE, 0);
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_save= gtk_button_new_with_label("Save");
	gtk_box_pack_start (GTK_BOX (hbox), btn_save, TRUE, TRUE, 0);	
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_load = gtk_button_new_with_label("Load");
	gtk_box_pack_start (GTK_BOX (hbox), btn_load, TRUE, TRUE, 0);   
	
	g_signal_connect(G_OBJECT(combo_neuron_type), "changed", G_CALLBACK(combo_neuron_type_func), NULL);
    	g_signal_connect(G_OBJECT(btn_add_neurons_to_layer), "clicked", G_CALLBACK(add_neurons_to_layer_button_func), NULL);
    	g_signal_connect(G_OBJECT(btn_submit_num_of_currents), "clicked", G_CALLBACK(submit_num_of_currents_button_func), NULL);
    	g_signal_connect(G_OBJECT(btn_submit_current_lengths), "clicked", G_CALLBACK(submit_current_lengths_button_func), NULL);
    	g_signal_connect(G_OBJECT(btn_generate_current_injection_graphs), "clicked", G_CALLBACK(generate_current_injection_graphs_button_func), NULL);
      	g_signal_connect(G_OBJECT(btn_submit_parker_sochacki_params), "clicked", G_CALLBACK(submit_parker_sochacki_params_button_func), NULL);

	g_signal_connect(G_OBJECT(combos_select_neuron->combo_layer), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_layer);
	g_signal_connect(G_OBJECT(combos_select_neuron->combo_neuron_group), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_neuron_group);	
	g_signal_connect(G_OBJECT(combos_select_neuron->combo_neuron), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_neuron);

/*     	g_signal_connect(G_OBJECT(btn_refresh_screen), "clicked", G_CALLBACK(refresh_screen_button_func), NULL);
     	g_signal_connect(G_OBJECT(btn_submit_initial_current_params), "clicked", G_CALLBACK(submit_initial_current_params_button_func), NULL);	     		  	
     	g_signal_connect(G_OBJECT(btn_create_firing_rate_view), "clicked", G_CALLBACK(create_firing_rate_view_button_func), NULL);	     		  	
    	g_signal_connect(G_OBJECT(btn_interrogate_network), "clicked", G_CALLBACK(interrogate_network_button_func), NULL);		
      	g_signal_connect(G_OBJECT(btn_interrogate_neuron), "clicked", G_CALLBACK(interrogate_neuron_button_func), NULL); 

      	g_signal_connect(G_OBJECT(btn_add_trial_type), "clicked", G_CALLBACK(add_trial_type_button_func ), NULL);       	
	g_signal_connect(G_OBJECT(btn_draw_template), "clicked", G_CALLBACK(draw_template_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_clear_template), "clicked", G_CALLBACK(clear_template_button_func), NULL);	
	g_signal_connect(G_OBJECT(btn_copy_drawn_to_raw_stimuli), "clicked", G_CALLBACK(copy_drawn_to_raw_stimuli_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_add_noise), "clicked", G_CALLBACK(add_noise_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_display_current_template), "clicked", G_CALLBACK(display_current_template_button_func), NULL);	
	g_signal_connect(G_OBJECT(btn_display_patterns_current), "clicked", G_CALLBACK(display_patterns_current_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_simulate), "clicked", G_CALLBACK(simulate_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_display_neuron_dynamics), "clicked", G_CALLBACK(display_neuron_dynamics_button_func), NULL);		
	g_signal_connect(G_OBJECT(btn_create_directory), "clicked", G_CALLBACK(create_directory_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_save), "clicked", G_CALLBACK(save_button_func), NULL);		
	g_signal_connect(G_OBJECT(btn_load), "clicked", G_CALLBACK(load_button_func), NULL);	
	
/*	gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, FALSE);	
	gtk_widget_set_sensitive(btn_add_trial_type, FALSE);
	gtk_widget_set_sensitive(btn_display_raw_stimuli, FALSE);
	gtk_widget_set_sensitive(btn_display_noisy_stimuli, FALSE);
	gtk_widget_set_sensitive(btn_draw_stimuli, FALSE);
	gtk_widget_set_sensitive(btn_simulate, FALSE);
	gtk_widget_set_sensitive(btn_add_noise, FALSE);
	gtk_widget_set_sensitive(btn_submit_initial_neuron_voltage, FALSE);
	gtk_widget_set_sensitive(btn_create_directory, FALSE);	
	gtk_widget_set_sensitive(btn_save, FALSE);		
*/
// 	initialize_graphs(table);

	gtk_widget_set_sensitive(btn_submit_num_of_currents, FALSE);		
	gtk_widget_set_sensitive(btn_submit_current_lengths, FALSE);	
	gtk_widget_set_sensitive(btn_generate_current_injection_graphs, FALSE);	
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
	SpikeGenData *data;
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

	if ((data = get_bmi_simulation_spike_generator_spike_gen_data()) == NULL)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "add_neurons_to_layer_button_func", "spike_gen_data == NULL.");
							
	if (!add_neurons_to_layer(data->network, num_of_neuron, layer, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return;
	if(!update_texts_of_combos_when_add_remove(combos_select_neuron, data->network))
		return;
	gtk_widget_set_sensitive(btn_submit_num_of_currents, TRUE);			
	return;
}

static void submit_num_of_currents_button_func(void)
{
	char *end_ptr;
	TrialsData *trials_data = get_bmi_simulation_spike_generator_trials_data();
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();
	unsigned int num_of_trial_start_available_currents =  strtoull(gtk_entry_get_text(GTK_ENTRY(entry_num_of_trial_start_available_currents)), &end_ptr, 10);
	unsigned int num_of_in_refractory_currents =  strtoull(gtk_entry_get_text(GTK_ENTRY(entry_num_of_in_refractory_currents)), &end_ptr, 10);
	unsigned int num_of_in_trial_currents =  strtoull(gtk_entry_get_text(GTK_ENTRY(entry_num_of_in_trial_currents)), &end_ptr, 10);
	if (trials_data == NULL)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "submit_num_of_currents_button_func", "trials_data == NULL.");
	if (spike_gen_data == NULL)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "submit_num_of_currents_button_func", "spike_gen_data == NULL.");

	spike_gen_data->injection_current = allocate_injection_current_data(spike_gen_data->network, trials_data, spike_gen_data->injection_current , num_of_trial_start_available_currents, num_of_in_refractory_currents, num_of_in_trial_currents);
	gtk_widget_set_sensitive(btn_submit_num_of_currents, FALSE);			
	gtk_widget_set_sensitive(btn_submit_current_lengths, TRUE);	
}

static void submit_current_lengths_button_func(void)
{
	char *end_ptr;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();
	unsigned int trial_start_available_current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_trial_start_available_current_num)), &end_ptr, 10);
	unsigned int in_refractory_current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_in_refractory_current_num)), &end_ptr, 10);
	TimeStamp trial_start_available_current_length = 1000000*strtoull(gtk_entry_get_text(GTK_ENTRY(entry_trial_start_available_current_length)), &end_ptr, 10);
	TimeStamp in_refractory_current_length = 1000000*strtoull(gtk_entry_get_text(GTK_ENTRY(entry_in_refractory_current_length)), &end_ptr, 10);
	if (! submit_current_length_trial_start_available_status(spike_gen_data->network, spike_gen_data->injection_current , trial_start_available_current_num, trial_start_available_current_length))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "submit_current_lengths_button_func", "! submit_current_length_trial_start_available_status().");
	if (! submit_current_length_in_refractory_status(spike_gen_data->network, spike_gen_data->injection_current, in_refractory_current_num, in_refractory_current_length))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "submit_current_lengths_button_func", "! submit_current_length_in_refractory_status().");
	gtk_widget_set_sensitive(btn_submit_num_of_currents, FALSE);			
	gtk_widget_set_sensitive(btn_generate_current_injection_graphs, TRUE);	
}

static void generate_current_injection_graphs_button_func(void)
{
	TimeStamp max_num_of_samples = 0;
	unsigned int i, j;
	TrialsData *trials_data = get_bmi_simulation_spike_generator_trials_data();
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();
	for (i = 0;  i < spike_gen_data->injection_current->current_templates->num_of_trial_start_available_currents; i++)
	{	
		if (spike_gen_data->injection_current->current_templates->trial_start_available_currents[i].num_of_current_samples > max_num_of_samples)
			max_num_of_samples = spike_gen_data->injection_current->current_templates->trial_start_available_currents[i].num_of_current_samples;
	}
	for (i = 0;  i < spike_gen_data->injection_current->current_templates->num_of_in_refractory_currents; i++)
	{	
		if (spike_gen_data->injection_current->current_templates->in_refractory_currents[i].num_of_current_samples > max_num_of_samples)
			max_num_of_samples = spike_gen_data->injection_current->current_templates->in_refractory_currents[i].num_of_current_samples;
	}
	for (i = 0; i < trials_data->trial_types_data.num_of_types; i++)
	{
		for (j = 0; j < spike_gen_data->injection_current->current_templates->num_of_in_trial_currents; j++)  // actually unnecessary, just to be straightforward
		{	
			if (spike_gen_data->injection_current->current_templates->in_trial_currents[i][j].num_of_current_samples > max_num_of_samples)
				max_num_of_samples = spike_gen_data->injection_current->current_templates->in_trial_currents[i][j].num_of_current_samples;
		}
	}
	current_pattern_graph = allocate_current_pattern_graph(current_pattern_graph_hbox, current_pattern_graph, max_num_of_samples, PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
	neuron_dynamics_graph = allocate_neuron_dynamics_graph(neuron_dynamics_graph_hbox, neuron_dynamics_graph, max_num_of_samples, PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
	gtk_widget_set_sensitive(btn_submit_current_lengths, FALSE);			
	gtk_widget_set_sensitive(btn_generate_current_injection_graphs, FALSE);	
}

static void submit_parker_sochacki_params_button_func(void)
{	
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();
	if (! parker_sochacki_set_order_tolerance(spike_gen_data->network, (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_parker_sochacki_max_order))), atof(gtk_entry_get_text(GTK_ENTRY(entry_parker_sochacki_err_tol)))))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "submit_parker_sochacki_params_button_func", "! parker_sochacki_set_order_tolerance().");	
}

void interrogate_network_button_func(void)
{
//	interrogate_network(spike_pattern_generator_get_network());
} 		

void interrogate_neuron_button_func(void)
{
/*	interrogate_neuron	(	spike_pattern_generator_get_network(), gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_layer)), gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron_group)), 
						gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron)) 
					);*/
}

static void add_trial_type_button_func(void)
{
/*	TrialType trial_type;
	TimeStamp min_len, max_len;
	TimeStamp sampling_interval;
	char *end_ptr;

	trial_type = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_trial_type)), &end_ptr, 10);
	min_len = (TimeStamp) (1000000 * strtoull(gtk_entry_get_text(GTK_ENTRY(entry_min_trial_length)), &end_ptr, 10));
	max_len = (TimeStamp) (1000000 * strtoull(gtk_entry_get_text(GTK_ENTRY(entry_max_trial_length)), &end_ptr, 10));
	sampling_interval = (TimeStamp) (1000 * strtoull(gtk_entry_get_text(GTK_ENTRY(entry_current_sampling_interval)), &end_ptr, 10));


	if (add_trial_type_to_trial_stats(spike_pattern_generator_get_trial_stats(), trial_type, max_len, min_len))
	{
		if(! update_trial_types_combo(spike_pattern_generator_get_trial_stats(), combo_trial_type))
			return (void)print_message(ERROR_MSG ,"SpikePatternGenerator", "SimulationView", "add_trial_type_button_func", "! update_trial_types_combo");
		if(!  increment_current_template_types_in_current_pattern_templates(spike_pattern_generator_get_current_patterns(), sampling_interval))
			return (void)print_message(ERROR_MSG ,"SpikePatternGenerator", "SimulationView", "add_trial_type_button_func", "!  increment_current_template_types_in_current_pattern_templates");					
		gtk_widget_set_size_request(combo_trial_type->combo, 50, 29) ;			
/*		allocate_graphs(max_len);
		gtk_widget_set_sensitive(btn_add_neurons_to_layer, FALSE);
		gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, FALSE);			
		gtk_widget_set_sensitive(btn_display_raw_stimuli, TRUE);
		gtk_widget_set_sensitive(btn_display_noisy_stimuli, TRUE);
		gtk_widget_set_sensitive(btn_draw_stimuli, TRUE);
		gtk_widget_set_sensitive(btn_simulate, TRUE);
		gtk_widget_set_sensitive(btn_add_noise, TRUE);
		gtk_widget_set_sensitive(btn_submit_initial_neuron_voltage, TRUE);	
		gtk_widget_set_sensitive(btn_create_directory, TRUE);	
		gtk_widget_set_sensitive(btn_save, TRUE);	
	}
*/
}

static void draw_template_button_func(void)
{
/*	char *end_ptr;
	unsigned int i;

	unsigned int trial_type = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_trial_type->combo));
	unsigned int layer = gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_layer));
	unsigned int neuron_group = gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron_group));
	unsigned int neuron = gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron));
	double amplifier= atof(gtk_entry_get_text(GTK_ENTRY(entry_amplifier)));
	double freq= atof(gtk_entry_get_text(GTK_ENTRY(entry_freq)));
	TimeStamp start_time = 1000000*strtoull(gtk_entry_get_text(GTK_ENTRY(entry_start_time)), &end_ptr, 10);
	TimeStamp end_time = 1000000*strtoull(gtk_entry_get_text(GTK_ENTRY(entry_end_time)), &end_ptr, 10);
	double initial_current = atof(gtk_entry_get_text(GTK_ENTRY(entry_initial_current)));
	TimeStamp sampling_interval = spike_pattern_generator_get_current_patterns()->current_pattern_templates[trial_type].sampling_interval;
	double starting_curr_val = spike_pattern_generator_get_current_patterns()->current_pattern_templates[trial_type].neuron_current_templates[layer][neuron_group][neuron].pattern_template.pattern[start_time/sampling_interval];
	double *pattern = spike_pattern_generator_get_current_patterns()->current_pattern_templates[trial_type].neuron_current_templates[layer][neuron_group][neuron].pattern_template.pattern;
	TimeStamp template_length = spike_pattern_generator_get_current_patterns()->trial_stats->trial_type_stats[trial_type].max_trial_type_length_assigned;
	int combo_idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_signal_type));

	if (start_time > end_time)
		return (void)print_message(ERROR_MSG ,"SpikePatternGenerator", "SimulationView", "draw_template_button_func", "start_time > end_time");	
	if (start_time > template_length)
		return (void)print_message(ERROR_MSG ,"SpikePatternGenerator", "SimulationView", "draw_template_button_func", "start_time > max_trial_type_length_assigned.");		
	
	if (end_time >= template_length)
	{
		print_message(WARNING_MSG ,"SpikePatternGenerator", "SimulationView", "draw_template_button_func", "end_time >= template_length.");	
		print_message(INFO_MSG ,"SpikePatternGenerator", "SimulationView", "draw_template_button_func", "end_time >= template_length.");	
		end_time = template_length;
	}
	if (combo_idx == LINE)
	{
		for (i=start_time; i<end_time; i+= sampling_interval)
		{
			pattern[i/sampling_interval] = amplifier * ((i-start_time)/1.0E+9) + initial_current+ starting_curr_val;
		}
	}
	else if (combo_idx == SIN)
	{
		for (i=start_time; i<end_time; i+= sampling_interval)
		{
			pattern[i/sampling_interval] = amplifier * sin (2*pi*freq* ((i-start_time)/1.0E+9)) + initial_current + starting_curr_val;
		}
	}	
	else if (combo_idx == COS)
	{
		for (i=start_time; i<end_time; i+= sampling_interval)
		{
			pattern[i/sampling_interval] = amplifier * cos (2*pi*freq* ((i-start_time)/1.0E+9) ) + initial_current + starting_curr_val;
		}
	}
	for (i=0; i < template_length; i+= sampling_interval)
	{
		stimulus_graph_y_axis[i/sampling_interval] = (float)all_stimulus_currents.drawn_stimulus_currents[layer][neuron_group][neuron][i];
	}
	gtk_databox_set_total_limits (GTK_DATABOX (stimulus_box), 0, all_stimulus_patterns_info.max_pattern_length - 1, MAX_CURRENT_VALUE, MIN_CURRENT_VALUE);		
	return; */
}


void clear_template_button_func(void)
{
/*	int layer, group, neuron_num;
	int start_time, end_time;
	int i;

//	layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_layer_num)));
//	group = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_group_num)));
//	neuron_num = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_num)));

	start_time = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_start_time)));
	end_time = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_end_time)));

	if ((start_time <= end_time) && (start_time >= 0) && (end_time >= 0) && (start_time < all_stimulus_patterns_info.max_pattern_length))
	{
		if (end_time >= all_stimulus_patterns_info.max_pattern_length )
		{
			printf("WARNING: End time entered is larger than maximum stimuli length %llu\n", all_stimulus_patterns_info.max_pattern_length);
			end_time = all_stimulus_patterns_info.max_pattern_length-1;
		}
		for (i=start_time; i<end_time; i++)
		{
			all_stimulus_currents.drawn_stimulus_currents[layer][group][neuron_num][i] = 0;
		}
		for (i=0; i<all_stimulus_patterns_info.max_pattern_length; i++)
		{
			stimulus_graph_y_axis[i] = (float)all_stimulus_currents.drawn_stimulus_currents[layer][group][neuron_num][i];
		}		
		gtk_databox_set_total_limits (GTK_DATABOX (stimulus_box), 0, all_stimulus_patterns_info.max_pattern_length - 1, MAX_CURRENT_VALUE, MIN_CURRENT_VALUE);		
	}
	else
	{
		printf ("ERROR: Submitted Start/End time is invalid to clear stimuli\n");
	}
*/
}


void add_noise_button_func(void)
{
/*
	int layer, group, neuron_num;
	int noise_period_cntr = 0; 
	double noise;
	int i,j;

//	layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_layer_num)));
//	group = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_group_num)));
//	neuron_num = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_num)));
	
	all_stimulus_currents.noise_variances[layer][group][neuron_num] = atof(gtk_entry_get_text(GTK_ENTRY(entry_noise_variance)));
	all_stimulus_currents.noise_addition_intervals[layer][group][neuron_num] = (TimeStamp)atof(gtk_entry_get_text(GTK_ENTRY(entry_noise_period)));

	if (all_stimulus_currents.noise_addition_intervals[layer][group][neuron_num] < MIN_INJECTED_CURRENT_NOISE_ADDITION_INTERVAL)
	{
		printf("ERROR: Noise insertion period cannot be smaller than %llu ns\n", MIN_INJECTED_CURRENT_NOISE_ADDITION_INTERVAL);
		return;
	}
	
	srand ( time(NULL) );
	
	noise = randn_notrig(0.0, all_stimulus_currents.noise_variances[layer][group][neuron_num]);
	
	for (i = 0; i< all_stimulus_patterns_info.num_of_patterns; i++)
	{
		for (j = 0; j < all_stimulus_patterns_info.pattern_lengths[i]; j++)
		{
			if (noise_period_cntr == all_stimulus_currents.noise_addition_intervals[layer][group][neuron_num])
			{
				noise = randn_notrig(0, all_stimulus_currents.noise_variances[layer][group][neuron_num]);
				noise_period_cntr = 0;		
			}
			noise_period_cntr++;				
			all_stimulus_currents.noisy_stimulus_currents[i][layer][group][neuron_num][j] = all_stimulus_currents.raw_stimulus_currents[i][layer][group][neuron_num][j] + noise;
			if (all_stimulus_currents.noisy_stimulus_currents[i][layer][group][neuron_num][j] < MIN_CURRENT_VALUE)
				all_stimulus_currents.noisy_stimulus_currents[i][layer][group][neuron_num][j] = MIN_CURRENT_VALUE;
			if (all_stimulus_currents.noisy_stimulus_currents[i][layer][group][neuron_num][j] > MAX_CURRENT_VALUE)
				all_stimulus_currents.noisy_stimulus_currents[i][layer][group][neuron_num][j] = MAX_CURRENT_VALUE;							
		}			
	}	*/
}


void display_current_template_button_func(void)
{
/*	int layer, group, neuron_num;
	int i;	
//	layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_layer_num)));
//	group = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_group_num)));
//	neuron_num = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_num)));

	if (is_neuron(spike_pattern_generator_get_network(), layer, group, neuron_num))
	{
		for (i=0; i<all_stimulus_patterns_info.max_pattern_length; i++)
		{
			stimulus_graph_y_axis[i] = (float)all_stimulus_currents.drawn_stimulus_currents[layer][group][neuron_num][i];
		}
		gtk_databox_set_total_limits (GTK_DATABOX (stimulus_box), 0, all_stimulus_patterns_info.max_pattern_length - 1, MAX_CURRENT_VALUE, MIN_CURRENT_VALUE);		
	}*/
}

void display_patterns_current_button_func(void)
{
/*	int pattern, layer, group, neuron_num;
	int i;	
//	layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_layer_num)));
//	group = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_group_num)));
//	neuron_num = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_num)));
//	pattern = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_pattern_num)));

	if ((pattern < all_stimulus_patterns_info.num_of_patterns) && (is_neuron(spike_pattern_generator_get_network(), layer, group, neuron_num)))
	{
		for (i=0; i<all_stimulus_patterns_info.pattern_lengths[pattern]; i++)
		{
			stimulus_graph_y_axis[i] = (float)all_stimulus_currents.raw_stimulus_currents[pattern][layer][group][neuron_num][i];
		}
		gtk_databox_set_total_limits (GTK_DATABOX (stimulus_box), 0, all_stimulus_patterns_info.max_pattern_length - 1, MAX_CURRENT_VALUE, MIN_CURRENT_VALUE);		
	}*/
}

void copy_drawn_to_raw_stimuli_button_func(void)
{
/*	int i,j, k, m, n;
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
		
	for (i = 0; i< all_stimulus_patterns_info.num_of_patterns; i++)
	{
		for (j = 0; j < all_stimulus_patterns_info.pattern_lengths[i]; j++)
		{
			for (k=0; k<spike_pattern_generator_get_network()->layer_count; k++)
			{
				ptr_layer = spike_pattern_generator_get_network()->layers[k];			
				for (m=0; m<ptr_layer->neuron_group_count; m++)
				{
					ptr_neuron_group = ptr_layer->neuron_groups[m];
					for (n=0; n<ptr_neuron_group->neuron_count; n++)
					{
						all_stimulus_currents.raw_stimulus_currents[i][k][m][n][j] = all_stimulus_currents.drawn_stimulus_currents[k][m][n][j];   // directly uses drawn stimulus to generate raw. squeezing, or modifications can be reflected to raw stimuli in the future. 
					}
				}
			}
		}			
	}
*/
}


void submit_initial_neuron_voltage_button_func(void)
{
/*	int layer, group, neuron_num;
	double voltage;
	Neuron* nrn;
	int i;

//	layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_layer_num)));
//	group = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_group_num)));
//	neuron_num = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_num)));

	neuron_dynamics.initial_v_means[layer][group][neuron_num] = atof(gtk_entry_get_text(GTK_ENTRY(entry_initial_neuron_voltage)));
	neuron_dynamics.initial_v_variances[layer][group][neuron_num] = atof(gtk_entry_get_text(GTK_ENTRY(entry_initial_neuron_voltage_variance)));

	nrn = get_neuron_address(spike_pattern_generator_get_network(), layer, group, neuron_num);
	if (nrn == NULL)
		return;

	if (neuron_dynamics.initial_v_means[layer][group][neuron_num] > nrn->v_peak)
	{
		printf("Initial neuron voltage cannot be larger than peak voltage %f\n",  nrn->v_peak);
		return;
	}
			
	for (i = 0; i< all_stimulus_patterns_info.num_of_patterns; i++)
	{
		voltage = neuron_dynamics.initial_v_means[layer][group][neuron_num]  + randn_notrig(0, neuron_dynamics.initial_v_variances[layer][group][neuron_num]);
		if (voltage < 0)
			voltage = 0;
		if (voltage > nrn->v_peak)	
			voltage = nrn->v_peak;
		neuron_dynamics.initial_v[i][layer][group][neuron_num] = voltage;
		neuron_dynamics.initial_u[i][layer][group][neuron_num] = nrn->b * voltage;
	}*/
}


void simulate_button_func(void)
{
/*	int i, k, m, n;
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
	Neuron		*ptr_neuron;		
	
	TimeStamp start_time_ns, end_time_ns, time_ns;
	ParkerSochackiStepSize step_size = 250000;
	TimeStamp  spike_time;

	clear_spike_pattern_time_stamps();
	
	for (i = 0; i< all_stimulus_patterns_info.num_of_patterns; i++)
	{
		start_time_ns = 0;
		end_time_ns = all_stimulus_patterns_info.pattern_lengths[i];
		for (time_ns = start_time_ns; time_ns < end_time_ns; time_ns+=step_size)
		{	
			for (k=0; k<spike_pattern_generator_get_network()->layer_count; k++)
			{
				ptr_layer = spike_pattern_generator_get_network()->layers[k];			
				for (m=0; m<ptr_layer->neuron_group_count; m++)
				{
					ptr_neuron_group = ptr_layer->neuron_groups[m];
					for (n=0; n<ptr_neuron_group->neuron_count; n++)
					{
						ptr_neuron = &(ptr_neuron_group->neurons[n]);
						if (time_ns == start_time_ns)
						{
							ptr_neuron->v = neuron_dynamics.initial_v[i][k][m][n]; 
							ptr_neuron->u = neuron_dynamics.initial_u[i][k][m][n];
						} 
						ptr_neuron -> I_inject = all_stimulus_currents.noisy_stimulus_currents[i][k][m][n][time_ns/1000000];
						spike_time = evaluate_neuron_dyn(ptr_neuron, time_ns, time_ns+step_size);
						if (spike_time != MAX_TIME_STAMP)
						{
							printf ("Spike time nano: %llu\n", spike_time);
							if (!add_time_stamp_to_spike_pattern_time_stamps(i , k, m, n, spike_time))
								return;
						}
						neuron_dynamics.v[i][k][m][n][time_ns/1000000] = ptr_neuron->v;	
						neuron_dynamics.u[i][k][m][n][time_ns/1000000] = ptr_neuron->u;								
					}
				}
			}
		}
	}
	display_neuron_dynamics();	*/
}

void display_neuron_dynamics_button_func(void)
{
//	display_neuron_dynamics();
}

void display_neuron_dynamics(void)
{
/*	int pattern, layer, group, neuron_num;
	int i;
	int active_neuron_dyn_to_disp;	
//	layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_layer_num)));
//	group = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_group_num)));
//	neuron_num = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_num)));
//	pattern = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_pattern_num)));
	
	active_neuron_dyn_to_disp = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_dynamics_type));
	if ((pattern < all_stimulus_patterns_info.num_of_patterns) && (is_neuron(spike_pattern_generator_get_network(), layer, group, neuron_num)))
	{
		for (i=0; i<all_stimulus_patterns_info.pattern_lengths[pattern]; i++)
		{
			if (active_neuron_dyn_to_disp == 0)	// display v 
			{
				neuron_dynamics_graph_y_axis[i] = (float)neuron_dynamics.v[pattern][layer][group][neuron_num][i];
				gtk_databox_set_total_limits (GTK_DATABOX (neuron_dynamics_box), 0, all_stimulus_patterns_info.max_pattern_length - 1, MAX_V_VALUE, MIN_V_VALUE);		
			}
			else if  (active_neuron_dyn_to_disp == 1) // display u
			{
				neuron_dynamics_graph_y_axis[i] = (float)neuron_dynamics.u[pattern][layer][group][neuron_num][i];
				gtk_databox_set_total_limits (GTK_DATABOX (neuron_dynamics_box), 0, all_stimulus_patterns_info.max_pattern_length - 1, MAX_U_VALUE, MIN_U_VALUE);		
			}			
		}

	}*/
}

void create_directory_button_func(void)
{
/*	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory));
	path = &path_temp[7];   // since     uri returns file:///home/....	
	if ((*create_spike_pattern_generator_data_directory[SPIKEPATTERNGENERATOR_MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, path, spike_pattern_generator_get_network()))		// record in last format version
		printf("SpikePatternGenerator: INFO: SpikePatternGenerator Data directory creation is successful\n");
	else
		printf("SpikePatternGenerator: ERROR: SpikePatternGenerator Data directory creation FAILED\n");	
	return;*/
}

void save_button_func(void)
{
/*	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory));
	path = &path_temp[7];   // since     uri returns file:///home/....
	if (is_spike_pattern_generator_data(path)) 		// First check if data directory was created previously
	{			
		if ((*save_spike_pattern_generator_data_directory[SPIKEPATTERNGENERATOR_MAX_NUMBER_OF_DATA_FORMAT_VER-1])(3, path, spike_pattern_generator_get_network(), txv_notes))		// record in last format version
			printf("SimulationView: INFO: SpikePatternGenerator Data save is successful\n");
		else
			printf("SimulationView: ERROR: SpikePatternGenerator Data save FAILED\n");
	}
	else
	{
			printf("SimulationView: ERROR: Not a SpikePatternGenerator Data\n");		
	}		
	return;*/
}

void load_button_func(void)
{
/*	char *path_temp = NULL, *path = NULL;
	int version;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory));
	path = &path_temp[7];   // since     uri returns file:///home/....
		
	if (get_spike_pattern_generator_data_format_version(&version, path))
	{	
		if ((*load_spike_pattern_generator_data_directory[version])(3, path, spike_pattern_generator_get_network(), txv_notes))
		{
			allocate_graphs(all_stimulus_patterns_info.max_pattern_length);
			gtk_widget_set_sensitive(btn_add_neurons_to_layer, FALSE);
			gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, FALSE);			
			gtk_widget_set_sensitive(btn_draw_template, TRUE);
			gtk_widget_set_sensitive(btn_simulate, TRUE);
			gtk_widget_set_sensitive(btn_add_noise, TRUE);
			gtk_widget_set_sensitive(btn_submit_initial_neuron_voltage, TRUE);	
			gtk_widget_set_sensitive(btn_create_directory, TRUE);	
			gtk_widget_set_sensitive(btn_save, TRUE);			
			printf("SpikePatternGenerator: INFO: SpikePatternGenerator Data load is successful\n");
		}
		else
		{
			printf("SpikePatternGenerator: ERROR: SpikePatternGenerator Data load FAILED\n");
		}					
	}
	return;*/
}

void set_directory_btn_select_directory(void)
{
/*	char line[600];
	FILE *fp = NULL;
       	if ((fp = fopen("./path_initial_directory", "r")) == NULL)  
       	{ 
       		printf ("ERROR: SpikePatternGenerator: Couldn't find file: ./path_initial_directory\n"); 
       		printf ("ERROR: SpikePatternGenerator: /home is loaded as initial direcoty to create data folder\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory),"/home");
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: SpikePatternGenerator: Couldn' t read ./path_initial_directory\n"); 
       			printf ("ERROR: SpikePatternGenerator: /home is loaded as initial direcoty to create data folder\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory),"/home");
		}
		else
		{
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory),line);
		}
		fclose(fp); 		
	}  	 */
}

void fill_notes_text_view(void)
{
/*	char line[1000];
	FILE *fp = NULL;
	int i = 0;
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(txv_notes));	
       	gtk_text_buffer_get_start_iter ( buffer, &start);
        gtk_text_buffer_get_end_iter ( buffer, &end);
        
        if ((fp = fopen("./initial_note", "r")) == NULL)  
       	{ 
       		printf ("ERROR: SpikePatternGenerator: Couldn't find file ./initial_note\n"); 
       		printf ("ERROR: SpikePatternGenerator: N/A is loaded as initial note\n");
		gtk_text_buffer_set_text (gtk_text_view_get_buffer(GTK_TEXT_VIEW(txv_notes)), "N/A\n", -1);       		 
       	}
       	else
       	{
   		
		while (fgets(line, sizeof line, fp ) != NULL) 
		{ 
        		gtk_text_buffer_get_end_iter ( buffer, &end);   				
			gtk_text_buffer_insert(buffer, &end, line, -1);
			i++;
			if (i == 200)
			{
       				printf ("ERROR: SpikePatternGenerator: ./initial_note is longer than 200 lines.\n");
       				printf ("ERROR: SpikePatternGenerator: Only initial 200 lines were read.\n\n");      
       				break; 				 				
			}
		}
		fclose(fp);       		
       	}*/
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


void create_firing_rate_view_button_func(void)
{
/*	char *end_ptr;
	TimeStamp bin_size = (TimeStamp)(1000000*strtoull(gtk_entry_get_text(GTK_ENTRY(entry_bin_size_ms)), &end_ptr, 10));
	if (!create_firing_rate_view_gui(bin_size, all_stimulus_patterns_info.num_of_patterns, all_stimulus_patterns_info.min_pattern_length))
	return;*/

}

static void combos_select_neuron_func(GtkWidget *changed_combo)
{
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();
	if (spike_gen_data == NULL)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "main", "spike_gen_data == NULL.");
	if(!update_texts_of_combos_when_change(combos_select_neuron, spike_gen_data->network, changed_combo))
		return;
}

static void submit_initial_current_params_button_func(void)
{
/*	TimeStamp init_current_duration;
	double init_current_max;
	char *end_ptr;

	init_current_duration = (TimeStamp) (1000000 * strtoull(gtk_entry_get_text(GTK_ENTRY(entry_init_current_duration)), &end_ptr, 10));
	init_current_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_init_current_max)));	

	if(! submit_initialization_current(	spike_pattern_generator_get_current_patterns(), 
							gtk_combo_box_get_active (GTK_COMBO_BOX(combo_trial_type->combo)), 
							gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_layer)), 
							gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron_group)), 
							gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron)), 
							init_current_duration, 
							init_current_max)
	)
		return (void)print_message(ERROR_MSG ,"SpikePatternGenerator", "SimulationView", "submit_initial_current_params_button_func", "! submit_initialization_current().");		
	return;*/
}


static void refresh_screen_button_func(void)
{
/*	unsigned int trial_type = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_trial_type->combo));
	unsigned int layer = gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_layer));
	unsigned int neuron_group = gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron_group));
	unsigned int neuron = gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron));
	unsigned int init_current_duration = spike_pattern_generator_get_current_patterns()->current_pattern_templates[trial_type].neuron_current_templates[layer][neuron_group][neuron].init_params.duration/1000000;
	double init_current_max =  spike_pattern_generator_get_current_patterns()->current_pattern_templates[trial_type].neuron_current_templates[layer][neuron_group][neuron].init_params.max_current;
	char temp_str[40];
      	sprintf(temp_str, "%.2f", init_current_max);
	gtk_entry_set_text(GTK_ENTRY(entry_init_current_max), temp_str);	
      	sprintf(temp_str, "%u", init_current_duration);
	gtk_entry_set_text(GTK_ENTRY(entry_init_current_duration), temp_str);*/
}
