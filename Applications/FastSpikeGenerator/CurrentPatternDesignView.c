#include "CurrentPatternDesignView.h"



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



static GtkWidget *current_pattern_graph_hbox;
static CurrentPatternGraph *current_pattern_graph = NULL;
static GtkWidget *neuron_dynamics_graph_hbox;
static NeuronDynamicsGraph *neuron_dynamics_graph = NULL;



// SECOND COLUMN
static LayerNrnGrpNeuronCombo *combos_select_neuron;
static GtkWidget *combo_target_num;
static GtkWidget *entry_current_pattern_number;

static GtkWidget *entry_init_current;
static GtkWidget *entry_amplifier;
static GtkWidget *entry_freq;
static GtkWidget *entry_start_time;
static GtkWidget *entry_end_time;
static GtkWidget *combo_signal_type;
static GtkWidget *btn_draw_template;
static GtkWidget *btn_clear_template;

static GtkWidget *btn_copy_drawn_to_template_in_trial;
static GtkWidget *btn_copy_drawn_to_template_trial_available;
static GtkWidget *btn_copy_drawn_to_template_in_refractory;

static GtkWidget *btn_display_currents_in_trial;
static GtkWidget *btn_display_currents_trial_available;
static GtkWidget *btn_display_currents_in_refractory;

static GtkWidget *btn_display_dynamics_in_trial;
static GtkWidget *btn_display_dynamics_trial_available;
static GtkWidget *btn_display_dynamics_in_refractory;
static NeuronDynamicsCombo *combo_neuron_dynamics = NULL;

static GtkWidget *btn_add_noise_in_trial;
static GtkWidget *btn_add_noise_trial_available;
static GtkWidget *btn_add_noise_in_refractory;
static GtkWidget *entry_noise_variance;
static GtkWidget *entry_noise_period;

static GtkWidget *btn_start_spike_generation;



/// LAST COLUMN
static GtkWidget *txv_notes;
static GtkWidget *btn_select_directory_to_save;
static GtkWidget *btn_select_file_to_load;
static GtkWidget *btn_save;
static GtkWidget *btn_load;
static GtkWidget *btn_quit;

// THIRD COLUMN
///   at graphs.h


static void combo_neuron_type_func (void);
static void add_neurons_to_layer_button_func(void);
static void interrogate_network_button_func(void); 		
static void interrogate_neuron_button_func(void);
static void submit_num_of_currents_button_func(void);
static void submit_current_lengths_button_func(void);
static void generate_current_injection_graphs_button_func(void);
static void submit_parker_sochacki_params_button_func(void);
static void start_spike_generation_button_func(void);
static void combos_select_neuron_func(GtkWidget *changed_combo);
static void draw_template_button_func(void);
static void clear_template_button_func(void);
static void copy_drawn_to_template_in_trial_button_func(void);
static void copy_drawn_to_template_trial_available_button_func(void);
static void copy_drawn_to_template_in_refractory_button_func(void);
static void display_currents_in_trial_button_func(void);
static void display_currents_trial_available_button_func(void);
static void display_currents_in_refractory_button_func(void);
static void display_dynamics_in_trial_button_func(void);
static void display_dynamics_trial_available_button_func(void);
static void display_dynamics_in_refractory_button_func(void);
static void add_noise_in_trial_button_func(void);
static void add_noise_trial_available_button_func(void);
static void add_noise_in_refractory_button_func(void);

static void save_button_func(void);
static void load_button_func(void);

static void quit_button_func(void);

static void set_neuron_param_entries(int neuron_type);
static void fill_notes_text_view(void);
static void set_directory_btn_select_directory(void);

bool create_current_pattern_view_gui(void)
{
	GtkWidget *frame, *frame_label, *table, *vbox, *hbox, *lbl;

	initialize_spike_gen_config_data_read_write_handlers();

        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Design Current Pattern    ");      
   
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

 	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_neuron_type), NRN_TYPE_REGULAR_SPIKING);	
	set_neuron_param_entries(NRN_TYPE_REGULAR_SPIKING);			

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

	btn_interrogate_network = gtk_button_new_with_label("Interrogate Network");
	gtk_box_pack_start (GTK_BOX (hbox), btn_interrogate_network, TRUE, TRUE, 0);	

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_interrogate_neuron = gtk_button_new_with_label("Interrogate Neuron");
	gtk_box_pack_start (GTK_BOX (hbox), btn_interrogate_neuron, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);        
  
	lbl = gtk_label_new("Error. Tol:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
                
        entry_parker_sochacki_err_tol= gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_parker_sochacki_err_tol, FALSE,FALSE,0);
	char temp_str[40];
      	sprintf(temp_str, "%.1E", 0.001);	
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
///////////////////////////////////////////// SECOND COLUMN  ///////////////////////////////////////////////////////////////

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 1,2,0,6);     ///  Stimulus graph control

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("init:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_init_current = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_init_current, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_init_current), "0");
	gtk_widget_set_size_request(entry_init_current, 35, 25) ;
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	lbl = gtk_label_new("amp/slop:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_amplifier = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_amplifier, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_amplifier), "0");
	gtk_widget_set_size_request(entry_amplifier, 35, 25) ;
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	lbl = gtk_label_new("freq:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_freq = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_freq, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_freq), "0");
	gtk_widget_set_size_request(entry_freq, 35, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Start (ms):");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_start_time = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_start_time, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_start_time), "0");
	gtk_widget_set_size_request(entry_start_time, 50, 25) ;
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	lbl = gtk_label_new("End (ms):");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_end_time = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_end_time, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_end_time), "0");
	gtk_widget_set_size_request(entry_end_time, 50, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	combo_signal_type = gtk_combo_box_new_text();
        gtk_box_pack_start(GTK_BOX(hbox),combo_signal_type, TRUE,TRUE,0);
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_signal_type), "Line");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_signal_type), "Sin");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_signal_type), "Cos");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_signal_type), "Exp");
 	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_signal_type), 0);
 	
	btn_draw_template = gtk_button_new_with_label("Draw");
	gtk_box_pack_start (GTK_BOX (hbox), btn_draw_template, TRUE, TRUE, 0);
	
	btn_clear_template = gtk_button_new_with_label("Clear");
	gtk_box_pack_start (GTK_BOX (hbox), btn_clear_template, TRUE, TRUE, 0);	

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	combos_select_neuron = allocate_layer_neuron_group_neuron_combos();
        gtk_box_pack_start(GTK_BOX(hbox),combos_select_neuron->combo_layer , TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_neuron->combo_neuron_group, TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox),combos_select_neuron->combo_neuron , TRUE,TRUE,0);

  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	combo_target_num = gtk_combo_box_new_text();
        gtk_box_pack_start(GTK_BOX(hbox), combo_target_num, TRUE,TRUE,0);
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_target_num), "Left");    // ignore uncessary string part
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_target_num), "Right");    // ignore uncessary string part
	gtk_combo_box_set_active (GTK_COMBO_BOX(combo_target_num), 0);

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

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Copy Drawn to Template:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_copy_drawn_to_template_in_trial = gtk_button_new_with_label("InTrial");
	gtk_box_pack_start (GTK_BOX (hbox), btn_copy_drawn_to_template_in_trial, TRUE, TRUE, 0);
	btn_copy_drawn_to_template_trial_available = gtk_button_new_with_label("TrialAvail");
	gtk_box_pack_start (GTK_BOX (hbox), btn_copy_drawn_to_template_trial_available, TRUE, TRUE, 0);
	btn_copy_drawn_to_template_in_refractory = gtk_button_new_with_label("InRefrac");
	gtk_box_pack_start (GTK_BOX (hbox), btn_copy_drawn_to_template_in_refractory, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Display Currents:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_display_currents_in_trial = gtk_button_new_with_label("InTrial");
	gtk_box_pack_start (GTK_BOX (hbox), btn_display_currents_in_trial, TRUE, TRUE, 0);
	btn_display_currents_trial_available = gtk_button_new_with_label("TrialAvail");
	gtk_box_pack_start (GTK_BOX (hbox), btn_display_currents_trial_available, TRUE, TRUE, 0);
	btn_display_currents_in_refractory = gtk_button_new_with_label("InRefrac");
	gtk_box_pack_start (GTK_BOX (hbox), btn_display_currents_in_refractory, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Display Dynamics:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	combo_neuron_dynamics = allocate_neuron_dynamics_combo(hbox, combo_neuron_dynamics);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_display_dynamics_in_trial = gtk_button_new_with_label("InTrial");
	gtk_box_pack_start (GTK_BOX (hbox), btn_display_dynamics_in_trial, TRUE, TRUE, 0);
	btn_display_dynamics_trial_available = gtk_button_new_with_label("TrialAvail");
	gtk_box_pack_start (GTK_BOX (hbox), btn_display_dynamics_trial_available, TRUE, TRUE, 0);
	btn_display_dynamics_in_refractory = gtk_button_new_with_label("InRefrac");
	gtk_box_pack_start (GTK_BOX (hbox), btn_display_dynamics_in_refractory, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Noise: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE,0);
	lbl = gtk_label_new("Var:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_noise_variance = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_noise_variance, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_variance), "0");
	gtk_widget_set_size_request(entry_noise_variance, 40, 25) ;

	lbl = gtk_label_new(" Interval(ms):");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);

        entry_noise_period = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_noise_period, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_period), "1");
	gtk_widget_set_size_request(entry_noise_period, 30, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_add_noise_in_trial = gtk_button_new_with_label("InTrial");
	gtk_box_pack_start (GTK_BOX (hbox), btn_add_noise_in_trial, TRUE, TRUE, 0);
	btn_add_noise_trial_available = gtk_button_new_with_label("TrialAvail");
	gtk_box_pack_start (GTK_BOX (hbox), btn_add_noise_trial_available, TRUE, TRUE, 0);
	btn_add_noise_in_refractory = gtk_button_new_with_label("InRefrac");
	gtk_box_pack_start (GTK_BOX (hbox), btn_add_noise_in_refractory, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);        
  
	btn_start_spike_generation = gtk_button_new_with_label("Start Spike Generation");
	gtk_box_pack_start (GTK_BOX (hbox), btn_start_spike_generation, TRUE, TRUE, 0);
	
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

	lbl = gtk_label_new("Folder:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);

	btn_select_directory_to_save = gtk_file_chooser_button_new ("Select Save Folder", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox),btn_select_directory_to_save, TRUE,TRUE,0);
 	gtk_widget_set_size_request(btn_select_directory_to_save, 50, 30);
	set_directory_btn_select_directory();
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_save= gtk_button_new_with_label("Save");
	gtk_box_pack_start (GTK_BOX (hbox), btn_save, TRUE, TRUE, 0);	

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Folder:");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);

	btn_select_file_to_load = gtk_file_chooser_button_new ("Select Load File", GTK_FILE_CHOOSER_ACTION_OPEN);
        gtk_box_pack_start(GTK_BOX(hbox),btn_select_file_to_load, TRUE,TRUE,0);
 	gtk_widget_set_size_request(btn_select_file_to_load, 50, 30);
	set_directory_btn_select_directory();
	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_load = gtk_button_new_with_label("Load");
	gtk_box_pack_start (GTK_BOX (hbox), btn_load, TRUE, TRUE, 0); 

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_quit  = gtk_button_new_with_label("Quit");
	gtk_box_pack_start (GTK_BOX (hbox), btn_quit, TRUE, TRUE, 0); 	

	g_signal_connect(G_OBJECT(combo_neuron_type), "changed", G_CALLBACK(combo_neuron_type_func), NULL);
    	g_signal_connect(G_OBJECT(btn_add_neurons_to_layer), "clicked", G_CALLBACK(add_neurons_to_layer_button_func), NULL);
    	g_signal_connect(G_OBJECT(btn_submit_num_of_currents), "clicked", G_CALLBACK(submit_num_of_currents_button_func), NULL);
    	g_signal_connect(G_OBJECT(btn_submit_current_lengths), "clicked", G_CALLBACK(submit_current_lengths_button_func), NULL);
    	g_signal_connect(G_OBJECT(btn_generate_current_injection_graphs), "clicked", G_CALLBACK(generate_current_injection_graphs_button_func), NULL);
    	g_signal_connect(G_OBJECT(btn_interrogate_network), "clicked", G_CALLBACK(interrogate_network_button_func), NULL);		
      	g_signal_connect(G_OBJECT(btn_interrogate_neuron), "clicked", G_CALLBACK(interrogate_neuron_button_func), NULL); 
      	g_signal_connect(G_OBJECT(btn_submit_parker_sochacki_params), "clicked", G_CALLBACK(submit_parker_sochacki_params_button_func), NULL);
      	g_signal_connect(G_OBJECT(btn_start_spike_generation), "clicked", G_CALLBACK(start_spike_generation_button_func), NULL);

	g_signal_connect(G_OBJECT(combos_select_neuron->combo_layer), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_layer);
	g_signal_connect(G_OBJECT(combos_select_neuron->combo_neuron_group), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_neuron_group);	
	g_signal_connect(G_OBJECT(combos_select_neuron->combo_neuron), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_neuron);

	g_signal_connect(G_OBJECT(btn_draw_template), "clicked", G_CALLBACK(draw_template_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_clear_template), "clicked", G_CALLBACK(clear_template_button_func), NULL);	

	g_signal_connect(G_OBJECT(btn_copy_drawn_to_template_in_trial), "clicked", G_CALLBACK(copy_drawn_to_template_in_trial_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_copy_drawn_to_template_trial_available), "clicked", G_CALLBACK(copy_drawn_to_template_trial_available_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_copy_drawn_to_template_in_refractory), "clicked", G_CALLBACK(copy_drawn_to_template_in_refractory_button_func), NULL);

	g_signal_connect(G_OBJECT(btn_display_currents_in_trial), "clicked", G_CALLBACK(display_currents_in_trial_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_display_currents_trial_available), "clicked", G_CALLBACK(display_currents_trial_available_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_display_currents_in_refractory), "clicked", G_CALLBACK(display_currents_in_refractory_button_func), NULL);

	g_signal_connect(G_OBJECT(btn_display_dynamics_in_trial), "clicked", G_CALLBACK(display_dynamics_in_trial_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_display_dynamics_trial_available), "clicked", G_CALLBACK(display_dynamics_trial_available_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_display_dynamics_in_refractory), "clicked", G_CALLBACK(display_dynamics_in_refractory_button_func), NULL);

	g_signal_connect(G_OBJECT(btn_add_noise_in_trial), "clicked", G_CALLBACK(add_noise_in_trial_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_add_noise_trial_available), "clicked", G_CALLBACK(add_noise_trial_available_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_add_noise_in_refractory), "clicked", G_CALLBACK(add_noise_in_refractory_button_func), NULL);

	g_signal_connect(G_OBJECT(btn_quit), "clicked", G_CALLBACK(quit_button_func), NULL);	
	
	g_signal_connect(G_OBJECT(btn_save), "clicked", G_CALLBACK(save_button_func), NULL);		
	g_signal_connect(G_OBJECT(btn_load), "clicked", G_CALLBACK(load_button_func), NULL);	
	
	gtk_widget_set_sensitive(btn_submit_num_of_currents, FALSE);		
	gtk_widget_set_sensitive(btn_submit_current_lengths, FALSE);	
	gtk_widget_set_sensitive(btn_generate_current_injection_graphs, FALSE);	

	gtk_widget_set_sensitive(btn_draw_template, FALSE);
	gtk_widget_set_sensitive(btn_clear_template, FALSE);
	gtk_widget_set_sensitive(btn_copy_drawn_to_template_in_trial, FALSE);
	gtk_widget_set_sensitive(btn_copy_drawn_to_template_trial_available, FALSE);
	gtk_widget_set_sensitive(btn_copy_drawn_to_template_in_refractory, FALSE);
	gtk_widget_set_sensitive(btn_display_currents_in_trial, FALSE);
	gtk_widget_set_sensitive(btn_display_currents_trial_available, FALSE);
	gtk_widget_set_sensitive(btn_display_currents_in_refractory, FALSE);
	gtk_widget_set_sensitive(btn_add_noise_in_trial, FALSE);
	gtk_widget_set_sensitive(btn_add_noise_trial_available, FALSE);
	gtk_widget_set_sensitive(btn_add_noise_in_refractory, FALSE);
	gtk_widget_set_sensitive(btn_start_spike_generation, FALSE);
	
	return TRUE;

}


static void combo_neuron_type_func (void)
{
	int neuron_type;
	neuron_type = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_neuron_type));
	if (neuron_type != NRN_TYPE_NO_TYPE)
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

	if ((data = get_bmi_simulation_spike_generator_data()) == NULL)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "add_neurons_to_layer_button_func", "spike_gen_data == NULL.");
							
	if (!add_iz_neurons_to_layer(data->network, num_of_neuron, layer, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return;
	if(!update_texts_of_combos_when_add_remove(combos_select_neuron, data->network))
		return;
	gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, TRUE);		
	return;
}

static void submit_parker_sochacki_params_button_func(void)
{	
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();

	if(!update_texts_of_combos_when_add_remove(combos_select_neuron, spike_gen_data->network))
		return;
	if (! parker_sochacki_set_order_tolerance(spike_gen_data->network, (unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_parker_sochacki_max_order))), atof(gtk_entry_get_text(GTK_ENTRY(entry_parker_sochacki_err_tol)))))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "submit_parker_sochacki_params_button_func", "! parker_sochacki_set_order_tolerance().");	
	gtk_widget_set_sensitive(btn_add_neurons_to_layer, FALSE);			
	gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, FALSE);	
	gtk_widget_set_sensitive(btn_submit_num_of_currents, TRUE);					
}

static void submit_num_of_currents_button_func(void)
{
	char *end_ptr;
	TrialHandParadigmRobotReach	*trial_hand_paradigm = get_bmi_simulation_spike_generator_data()->trial_hand_paradigm;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	unsigned int num_of_trial_start_available_currents =  strtoull(gtk_entry_get_text(GTK_ENTRY(entry_num_of_trial_start_available_currents)), &end_ptr, 10);
	unsigned int num_of_in_refractory_currents =  strtoull(gtk_entry_get_text(GTK_ENTRY(entry_num_of_in_refractory_currents)), &end_ptr, 10);
	unsigned int num_of_in_trial_currents =  strtoull(gtk_entry_get_text(GTK_ENTRY(entry_num_of_in_trial_currents)), &end_ptr, 10);
	if (trial_hand_paradigm == NULL)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "submit_num_of_currents_button_func", "trial_hand_paradigm == NULL.");
	if (spike_gen_data == NULL)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "submit_num_of_currents_button_func", "spike_gen_data == NULL.");

	spike_gen_data->current_templates = allocate_current_templates(spike_gen_data->network, trial_hand_paradigm, spike_gen_data->current_templates , num_of_trial_start_available_currents, num_of_in_refractory_currents, num_of_in_trial_currents);

	gtk_widget_set_sensitive(btn_submit_num_of_currents, FALSE);			
	gtk_widget_set_sensitive(btn_submit_current_lengths, TRUE);	
}

static void submit_current_lengths_button_func(void)
{
	char *end_ptr;
	bool trial_start_available_currents_has_unsubmitted_current_len = TRUE;
	bool in_refractory_currents_has_unsubmitted_current_len = TRUE;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	unsigned int trial_start_available_current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_trial_start_available_current_num)), &end_ptr, 10);
	unsigned int in_refractory_current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_in_refractory_current_num)), &end_ptr, 10);
	TimeStamp trial_start_available_current_length = 1000000*strtoull(gtk_entry_get_text(GTK_ENTRY(entry_trial_start_available_current_length)), &end_ptr, 10);
	TimeStamp in_refractory_current_length = 1000000*strtoull(gtk_entry_get_text(GTK_ENTRY(entry_in_refractory_current_length)), &end_ptr, 10);
	if (! submit_current_length_trial_start_available_status(spike_gen_data->network, spike_gen_data->current_templates , trial_start_available_current_num, trial_start_available_current_length, &trial_start_available_currents_has_unsubmitted_current_len))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "submit_current_lengths_button_func", "! submit_current_length_trial_start_available_status().");
	if (! submit_current_length_in_refractory_status(spike_gen_data->network, spike_gen_data->current_templates, in_refractory_current_num, in_refractory_current_length, &in_refractory_currents_has_unsubmitted_current_len))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "submit_current_lengths_button_func", "! submit_current_length_in_refractory_status().");
	gtk_widget_set_sensitive(btn_submit_num_of_currents, FALSE);			
	if ((!trial_start_available_currents_has_unsubmitted_current_len) && (!in_refractory_currents_has_unsubmitted_current_len))
	{
		gtk_widget_set_sensitive(btn_generate_current_injection_graphs, TRUE);	
	}		
}

static void generate_current_injection_graphs_button_func(void)
{
	TimeStamp max_num_of_samples = 0;
	unsigned int i, j;
	TrialHandParadigmRobotReach	*trial_hand_paradigm = get_bmi_simulation_spike_generator_data()->trial_hand_paradigm;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	for (i = 0;  i < spike_gen_data->current_templates->num_of_trial_start_available_currents; i++)
	{	
		if (spike_gen_data->current_templates->trial_start_available_currents[i].num_of_template_samples > max_num_of_samples)
			max_num_of_samples = spike_gen_data->current_templates->trial_start_available_currents[i].num_of_template_samples;
	}
	for (i = 0;  i < spike_gen_data->current_templates->num_of_in_refractory_currents; i++)
	{	
		if (spike_gen_data->current_templates->in_refractory_currents[i].num_of_template_samples > max_num_of_samples)
			max_num_of_samples = spike_gen_data->current_templates->in_refractory_currents[i].num_of_template_samples;
	}
	for (i = 0; i < trial_hand_paradigm->num_of_robot_target_positions; i++)
	{
		for (j = 0; j < spike_gen_data->current_templates->num_of_in_trial_currents; j++)  // actually unnecessary, just to be straightforward
		{	
			if (spike_gen_data->current_templates->in_trial_currents[i][j].num_of_template_samples > max_num_of_samples)
				max_num_of_samples = spike_gen_data->current_templates->in_trial_currents[i][j].num_of_template_samples;
		}
	}
	current_pattern_graph = allocate_current_pattern_graph(current_pattern_graph_hbox, current_pattern_graph, max_num_of_samples, PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
	neuron_dynamics_graph = allocate_neuron_dynamics_graph(neuron_dynamics_graph_hbox, neuron_dynamics_graph, max_num_of_samples, PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
	spike_gen_data->limited_current_pattern_buffer = allocate_current_pattern_buffer_limited(spike_gen_data->network, spike_gen_data->limited_current_pattern_buffer, 3000000000/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, NUM_OF_CURRENT_PATTERN_GRAPHS); // 3 second buffer
	spike_gen_data->limited_neuron_dynamics_buffer = allocate_neuron_dynamics_buffer_limited(spike_gen_data->network, spike_gen_data->limited_neuron_dynamics_buffer, 3000000000/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, NUM_OF_NEURON_DYNAMICS_GRAPHS); // 3 second buffer for 1 second graph refresh rate. 
	spike_gen_data->spike_data = g_new0(SpikeData*,1);
	spike_gen_data->spike_data[0] = allocate_spike_data(spike_gen_data->spike_data[0], get_num_of_neurons_in_network(spike_gen_data->network)*3*500 ); // 3 seconds buffer assuming a neuron firing rate cannot be more than 500 Hz 

	if (! buffer_view_handler())
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "generate_current_injection_graphs_button_func", "! buffer_view_handler()");	

	gtk_widget_set_sensitive(btn_submit_current_lengths, FALSE);	
	gtk_widget_set_sensitive(btn_generate_current_injection_graphs, FALSE);
	gtk_widget_set_sensitive(btn_draw_template, TRUE);
	gtk_widget_set_sensitive(btn_clear_template, TRUE);
	gtk_widget_set_sensitive(btn_copy_drawn_to_template_in_trial, TRUE);
	gtk_widget_set_sensitive(btn_copy_drawn_to_template_trial_available, TRUE);
	gtk_widget_set_sensitive(btn_copy_drawn_to_template_in_refractory, TRUE);
	gtk_widget_set_sensitive(btn_display_currents_in_trial, TRUE);
	gtk_widget_set_sensitive(btn_display_currents_trial_available, TRUE);
	gtk_widget_set_sensitive(btn_display_currents_in_refractory, TRUE);
	gtk_widget_set_sensitive(btn_add_noise_in_trial, TRUE);
	gtk_widget_set_sensitive(btn_add_noise_trial_available, TRUE);
	gtk_widget_set_sensitive(btn_add_noise_in_refractory, TRUE);
	gtk_widget_set_sensitive(btn_start_spike_generation, TRUE);

}

static void start_spike_generation_button_func(void)
{	
	gtk_widget_set_sensitive(btn_start_spike_generation, FALSE);	
	bmi_simulation_spike_generator_create_rt_thread();	
	send_global_pause_button_sensitive_request();
}


static void draw_template_button_func(void)
{
	char *end_ptr;
	TimeStamp i;
	int combo_idx;
	double starting_curr_val;
	double init_current = atof(gtk_entry_get_text(GTK_ENTRY(entry_init_current)));
	double amplifier = atof(gtk_entry_get_text(GTK_ENTRY(entry_amplifier)));
	double freq = atof(gtk_entry_get_text(GTK_ENTRY(entry_freq)));
	TimeStamp start_time = 1000000*strtoull(gtk_entry_get_text(GTK_ENTRY(entry_start_time)), &end_ptr, 10);
	TimeStamp end_time = 1000000*strtoull(gtk_entry_get_text(GTK_ENTRY(entry_end_time)), &end_ptr, 10);
	TimeStamp sampling_interval = current_pattern_graph->sampling_interval;
	if (start_time > end_time)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "draw_template_button_func", "start_time > end_time");	
	if (start_time > (current_pattern_graph->num_of_data_points* current_pattern_graph->sampling_interval))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "draw_template_button_func", "start_time > max_template_length.");	
	if (end_time > (current_pattern_graph->num_of_data_points* current_pattern_graph->sampling_interval))
	{
		print_message(WARNING_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "draw_template_button_func", "end_time > max_template_length.");	
		print_message(INFO_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "draw_template_button_func", "end_time = max_template_length - sampling_interval.");	
		end_time = current_pattern_graph->num_of_data_points* current_pattern_graph->sampling_interval;
	}
	if ((start_time/current_pattern_graph->sampling_interval) == 0)
		starting_curr_val = 0;
	else  // for continuity of current pattern drawing
		starting_curr_val = current_pattern_graph->y[(start_time-current_pattern_graph->sampling_interval)/current_pattern_graph->sampling_interval];

	combo_idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_signal_type));

	if (combo_idx == LINE)
	{
		for (i = start_time; i < end_time; i+= sampling_interval)
		{
			current_pattern_graph->y[i/sampling_interval] = amplifier * ((i-start_time)/1.0E+9) + starting_curr_val + init_current;
		}
	}
	else if (combo_idx == SIN)
	{
		for (i = start_time; i < end_time; i+= sampling_interval)
		{
			current_pattern_graph->y[i/sampling_interval] = amplifier * sin (2*pi*freq* ((i-start_time)/1.0E+9)) + starting_curr_val + init_current;
		}
	}	
	else if (combo_idx == COS)
	{
		for (i = start_time; i < end_time; i+= sampling_interval)
		{
			current_pattern_graph->y[i/sampling_interval] = amplifier * cos (2*pi*freq* ((i-start_time)/1.0E+9) ) + starting_curr_val + init_current;
		}
	}
	else if (combo_idx == EXP)
	{
		for (i = start_time; i < end_time; i+= sampling_interval)
		{
			current_pattern_graph->y[i/sampling_interval] =  init_current *exp (- amplifier * (i-start_time)/1.0E+9);
		}
	}
	if (!update_current_pattern_graph(current_pattern_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "draw_template_button_func", "!update_current_pattern_graph().");			
	return; 
}


void clear_template_button_func(void)
{
	unsigned int i;
	for (i = 0; i < current_pattern_graph->num_of_data_points; i++)
		current_pattern_graph->y[i] = 0;
	if (!update_current_pattern_graph(current_pattern_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "draw_template_button_func", "!update_current_pattern_graph().");		
}

static void combos_select_neuron_func(GtkWidget *changed_combo)
{
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	if (spike_gen_data == NULL)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "combos_select_neuron_func", "spike_gen_data == NULL.");
	if(!update_texts_of_combos_when_change(combos_select_neuron, spike_gen_data->network, changed_combo))
		return;
}


static void copy_drawn_to_template_in_trial_button_func(void)
{	
	char *end_ptr;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	unsigned int target_num_idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_target_num));
	unsigned int current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_current_pattern_number)), &end_ptr, 10);
	unsigned int i, j;
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "copy_drawn_to_template_in_trial_button_func", "! layer_neuron_group_neuron_get_selected().");
	if (current_num >= current_templates->num_of_in_trial_currents)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "copy_drawn_to_template_in_trial_button_func", "current_num >= num_of_in_trial_currents.");	 
	for (j = 0; j < spike_gen_data->network->layers[layer_num]->neuron_group_count; j++)
	{
		for (i = 0; i < current_templates->in_trial_currents[target_num_idx][current_num].num_of_template_samples; i++)
			current_templates->in_trial_currents[target_num_idx][current_num].template_samples[i].current_sample[layer_num][j][0] = current_pattern_graph->y[i];
	}
}
static void copy_drawn_to_template_trial_available_button_func(void)
{
	char *end_ptr;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	unsigned int current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_current_pattern_number)), &end_ptr, 10);
	unsigned int i, j;
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "copy_drawn_to_template_trial_available_button_func", "! layer_neuron_group_neuron_get_selected().");
	if (current_num >= current_templates->num_of_trial_start_available_currents)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "copy_drawn_to_template_trial_available_button_func", "current_num >= num_of_trial_start_available_currents.");	 
	for (j = 0; j < spike_gen_data->network->layers[layer_num]->neuron_group_count; j++)
	{
		for (i = 0; i < current_templates->trial_start_available_currents[current_num].num_of_template_samples; i++)
			current_templates->trial_start_available_currents[current_num].template_samples[i].current_sample[layer_num][j][0] = current_pattern_graph->y[i];
	}
}
static void copy_drawn_to_template_in_refractory_button_func(void)
{
	char *end_ptr;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	unsigned int current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_current_pattern_number)), &end_ptr, 10);
	unsigned int i, j;
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "copy_drawn_to_template_in_refractory_button_func", "! layer_neuron_group_neuron_get_selected().");
	if (current_num >= current_templates->num_of_in_refractory_currents)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "copy_drawn_to_template_in_refractory_button_func", "current_num >= num_of_in_refractory_currents.");
	for (j = 0; j < spike_gen_data->network->layers[layer_num]->neuron_group_count; j++)
	{	 
		for (i = 0; i < current_templates->in_refractory_currents[current_num].num_of_template_samples; i++)
			current_templates->in_refractory_currents[current_num].template_samples[i].current_sample[layer_num][j][0] = current_pattern_graph->y[i];
	}
}
static void display_currents_in_trial_button_func(void)
{
	char *end_ptr;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	Neuron* neuron; 
	TimeStamp sampling_interval;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	unsigned target_num_idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_target_num));
	unsigned int current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_current_pattern_number)), &end_ptr, 10);
	unsigned int i;
	float *y;
	char str[20];
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_trial_button_func", "! layer_neuron_group_neuron_get_selected().");
	neuron = get_neuron_address(spike_gen_data->network, layer_num, nrn_grp_num, nrn_num);
	if (current_num >= current_templates->num_of_in_trial_currents)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_trial_button_func", "current_num >= num_of_in_trial_currents.");	 
	if (! clear_current_pattern_graph(current_pattern_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_trial_button_func", "!clear_current_pattern_graph().");	 
	if (! clear_neuron_dynamics_graph(neuron_dynamics_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_trial_button_func", "!clear_neuron_dynamics_graph().");
	sampling_interval = current_pattern_graph->sampling_interval;
	y = current_pattern_graph->y;
	for (i = 0; i < current_templates->in_trial_currents[target_num_idx][current_num].num_of_template_samples; i++)
	{
		y[i] = current_templates->in_trial_currents[target_num_idx][current_num].template_samples[i].current_sample[layer_num][nrn_grp_num][nrn_num];
	}
	if (!update_current_pattern_graph(current_pattern_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_trial_button_func", "!update_current_pattern_graph().");	

	sprintf(str, "%.2f", current_templates->in_trial_currents[target_num_idx][current_num].noise_params[layer_num][nrn_grp_num][nrn_num].noise_variance);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_variance), str);
	sprintf(str, "%llu", current_templates->in_trial_currents[target_num_idx][current_num].noise_params[layer_num][nrn_grp_num][nrn_num].noise_addition_interval/1000000);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_period), str);
}
static void display_currents_trial_available_button_func(void)
{
	char *end_ptr;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	Neuron* neuron; 
	TimeStamp sampling_interval;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	unsigned int current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_current_pattern_number)), &end_ptr, 10);
	unsigned int i;
	float *y;
	char str[20];
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_trial_available_button_func", "! layer_neuron_group_neuron_get_selected().");
	neuron = get_neuron_address(spike_gen_data->network, layer_num, nrn_grp_num, nrn_num);
	if (current_num >= current_templates->num_of_trial_start_available_currents)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_trial_available_button_func", "current_num >= num_of_trial_start_available_currents.");	
	if (! clear_current_pattern_graph(current_pattern_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_trial_available_button_func", "!clear_current_pattern_graph().");	 
	if (! clear_neuron_dynamics_graph(neuron_dynamics_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_trial_available_button_func", "!clear_neuron_dynamics_graph().");
	sampling_interval = current_pattern_graph->sampling_interval;
	y = current_pattern_graph->y;
	for (i = 0; i < current_templates->trial_start_available_currents[current_num].num_of_template_samples; i++)
	{
		y[i] = current_templates->trial_start_available_currents[current_num].template_samples[i].current_sample[layer_num][nrn_grp_num][nrn_num];
	}
	if (!update_current_pattern_graph(current_pattern_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_trial_available_button_func", "!update_current_pattern_graph().");	
	if (!update_neuron_dynamics_graph(neuron_dynamics_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_trial_available_button_func", "!update_neuron_dynamics_graph().");	
	sprintf(str, "%.2f", current_templates->trial_start_available_currents[current_num].noise_params[layer_num][nrn_grp_num][nrn_num].noise_variance);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_variance), str);
	sprintf(str, "%llu", current_templates->trial_start_available_currents[current_num].noise_params[layer_num][nrn_grp_num][nrn_num].noise_addition_interval/1000000);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_period), str);
}
static void display_currents_in_refractory_button_func(void)
{
	char *end_ptr;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	Neuron* neuron; 
	TimeStamp sampling_interval;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	unsigned int current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_current_pattern_number)), &end_ptr, 10);
	unsigned int i;
	float *y;
	char str[20];
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_refractory_button_func", "! layer_neuron_group_neuron_get_selected().");
	neuron = get_neuron_address(spike_gen_data->network, layer_num, nrn_grp_num, nrn_num);
	if (current_num >= current_templates->num_of_in_refractory_currents)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_refractory_button_func", "current_num >= num_of_in_refractory_currents.");	 
	if (! clear_current_pattern_graph(current_pattern_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_refractory_button_func", "!clear_current_pattern_graph().");	
	if (! clear_neuron_dynamics_graph(neuron_dynamics_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_refractory_button_func", "!clear_neuron_dynamics_graph().");
	sampling_interval = current_pattern_graph->sampling_interval;
	y = current_pattern_graph->y;
	for (i = 0; i < current_templates->in_refractory_currents[current_num].num_of_template_samples; i++)
	{
		y[i] = current_templates->in_refractory_currents[current_num].template_samples[i].current_sample[layer_num][nrn_grp_num][nrn_num];
	}
	if (!update_current_pattern_graph(current_pattern_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_refractory_button_func", "!update_current_pattern_graph().");	
	if (!update_neuron_dynamics_graph(neuron_dynamics_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_refractory_button_func", "!update_neuron_dynamics_graph().");	
	sprintf(str, "%.2f", current_templates->in_refractory_currents[current_num].noise_params[layer_num][nrn_grp_num][nrn_num].noise_variance);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_variance), str);
	sprintf(str, "%llu", current_templates->in_refractory_currents[current_num].noise_params[layer_num][nrn_grp_num][nrn_num].noise_addition_interval/1000000);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_period), str);
}
static void display_dynamics_in_trial_button_func(void)
{
	char *end_ptr;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	Neuron* neuron; 
	TimeStamp spike_time;
	bool spike_generated;
	TimeStamp sampling_interval;
	TimeStamp time_ns;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	unsigned int target_num_idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_target_num));
	unsigned int current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_current_pattern_number)), &end_ptr, 10);
	unsigned int i;
	float *y;
	float *y_dynamics;
	int neuron_dynamics_type_idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_neuron_dynamics->combo));
	char str[20];
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_trial_button_func", "! layer_neuron_group_neuron_get_selected().");
	neuron = get_neuron_address(spike_gen_data->network, layer_num, nrn_grp_num, nrn_num);
	neuron->iz_params->v = 0; neuron->iz_params->u = 0; neuron->iz_params->conductance_excitatory = 0; neuron->iz_params->conductance_inhibitory = 0; 
	if (current_num >= current_templates->num_of_in_trial_currents)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_trial_button_func", "current_num >= num_of_in_trial_currents.");	 
	if (! clear_current_pattern_graph(current_pattern_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_trial_button_func", "!clear_current_pattern_graph().");	 
	if (! clear_neuron_dynamics_graph(neuron_dynamics_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_trial_button_func", "!clear_neuron_dynamics_graph().");
	sampling_interval = current_pattern_graph->sampling_interval;
	y = current_pattern_graph->y;
	y_dynamics = neuron_dynamics_graph->y;
	for (i = 0; i < current_templates->in_trial_currents[target_num_idx][current_num].num_of_template_samples; i++)
	{
		y[i] = current_templates->in_trial_currents[target_num_idx][current_num].template_samples[i].current_sample[layer_num][nrn_grp_num][nrn_num];
		neuron->iz_params->I_inject = y[i];
		time_ns = i*sampling_interval;
		if (!evaluate_neuron_dyn(neuron, time_ns, time_ns+sampling_interval, &spike_generated, &spike_time))
			return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_trial_button_func", "! evaluate_neuron_dyn().");
		switch (neuron_dynamics_type_idx)
		{
			case DYNAMICS_TYPE_V:
				y_dynamics[i] = neuron->iz_params->v;
				break; 
			case DYNAMICS_TYPE_U:
				y_dynamics[i] = neuron->iz_params->u;
				break; 
			case DYNAMICS_TYPE_E:
				y_dynamics[i] = neuron->iz_params->conductance_excitatory;
				break; 
			case DYNAMICS_TYPE_I:
				y_dynamics[i] = neuron->iz_params->conductance_inhibitory;
				break; 
			default:
				return (void)print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_trial_button_func", "Invalid neuron_dynamics_type_idx.");
		}		
	}
	if (!update_current_pattern_graph(current_pattern_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_trial_button_func", "!update_current_pattern_graph().");	
	if (!update_neuron_dynamics_graph(neuron_dynamics_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_trial_button_func", "!update_neuron_dynamics_graph().");	

	sprintf(str, "%.2f", current_templates->in_trial_currents[target_num_idx][current_num].noise_params[layer_num][nrn_grp_num][nrn_num].noise_variance);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_variance), str);
	sprintf(str, "%llu", current_templates->in_trial_currents[target_num_idx][current_num].noise_params[layer_num][nrn_grp_num][nrn_num].noise_addition_interval/1000000);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_period), str);
}
static void display_dynamics_trial_available_button_func(void)
{
	char *end_ptr;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	Neuron* neuron; 
	TimeStamp spike_time;
	bool spike_generated;
	TimeStamp sampling_interval;
	TimeStamp time_ns;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	unsigned int current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_current_pattern_number)), &end_ptr, 10);
	unsigned int i;
	float *y;
	float *y_dynamics;
	int neuron_dynamics_type_idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_neuron_dynamics->combo));
	char str[20];
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_trial_available_button_func", "! layer_neuron_group_neuron_get_selected().");
	neuron = get_neuron_address(spike_gen_data->network, layer_num, nrn_grp_num, nrn_num);
	neuron->iz_params->v = 0; neuron->iz_params->u = 0; neuron->iz_params->conductance_excitatory = 0; neuron->iz_params->conductance_inhibitory = 0; 
	if (current_num >= current_templates->num_of_trial_start_available_currents)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_trial_available_button_func", "current_num >= num_of_trial_start_available_currents.");	
	if (! clear_current_pattern_graph(current_pattern_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_trial_available_button_func", "!clear_current_pattern_graph().");	 
	if (! clear_neuron_dynamics_graph(neuron_dynamics_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_trial_available_button_func", "!clear_neuron_dynamics_graph().");
	sampling_interval = current_pattern_graph->sampling_interval;
	y = current_pattern_graph->y;
	y_dynamics = neuron_dynamics_graph->y;
	for (i = 0; i < current_templates->trial_start_available_currents[current_num].num_of_template_samples; i++)
	{
		y[i] = current_templates->trial_start_available_currents[current_num].template_samples[i].current_sample[layer_num][nrn_grp_num][nrn_num];
		neuron->iz_params->I_inject = y[i];
		time_ns = i*sampling_interval;
		if (!evaluate_neuron_dyn(neuron, time_ns, time_ns+sampling_interval, &spike_generated, &spike_time))
			return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_trial_available_button_func", "! evaluate_neuron_dyn().");
		switch (neuron_dynamics_type_idx)
		{
			case DYNAMICS_TYPE_V:
				y_dynamics[i] = neuron->iz_params->v;
				break; 
			case DYNAMICS_TYPE_U:
				y_dynamics[i] = neuron->iz_params->u;
				break; 
			case DYNAMICS_TYPE_E:
				y_dynamics[i] = neuron->iz_params->conductance_excitatory;
				break; 
			case DYNAMICS_TYPE_I:
				y_dynamics[i] = neuron->iz_params->conductance_inhibitory;
				break; 
			default:
				return (void)print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_trial_button_func", "Invalid neuron_dynamics_type_idx.");
		}		
	}
	if (!update_current_pattern_graph(current_pattern_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_trial_available_button_func", "!update_current_pattern_graph().");	
	if (!update_neuron_dynamics_graph(neuron_dynamics_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_trial_available_button_func", "!update_neuron_dynamics_graph().");	
	sprintf(str, "%.2f", current_templates->trial_start_available_currents[current_num].noise_params[layer_num][nrn_grp_num][nrn_num].noise_variance);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_variance), str);
	sprintf(str, "%llu", current_templates->trial_start_available_currents[current_num].noise_params[layer_num][nrn_grp_num][nrn_num].noise_addition_interval/1000000);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_period), str);
}
static void display_dynamics_in_refractory_button_func(void)
{
	char *end_ptr;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	Neuron* neuron; 
	TimeStamp spike_time;
	bool spike_generated;
	TimeStamp sampling_interval;
	TimeStamp time_ns;;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	unsigned int current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_current_pattern_number)), &end_ptr, 10);
	unsigned int i;
	float *y;
	float *y_dynamics;
	int neuron_dynamics_type_idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_neuron_dynamics->combo));
	char str[20];
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_refractory_button_func", "! layer_neuron_group_neuron_get_selected().");
	neuron = get_neuron_address(spike_gen_data->network, layer_num, nrn_grp_num, nrn_num);
	neuron->iz_params->v = 0; neuron->iz_params->u = 0; neuron->iz_params->conductance_excitatory = 0; neuron->iz_params->conductance_inhibitory = 0; 
	if (current_num >= current_templates->num_of_in_refractory_currents)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_refractory_button_func", "current_num >= num_of_in_refractory_currents.");	 
	if (! clear_current_pattern_graph(current_pattern_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_refractory_button_func", "!clear_current_pattern_graph().");	
	if (! clear_neuron_dynamics_graph(neuron_dynamics_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_refractory_button_func", "!clear_neuron_dynamics_graph().");
	sampling_interval = current_pattern_graph->sampling_interval;
	y = current_pattern_graph->y;
	y_dynamics = neuron_dynamics_graph->y;
	for (i = 0; i < current_templates->in_refractory_currents[current_num].num_of_template_samples; i++)
	{
		y[i] = current_templates->in_refractory_currents[current_num].template_samples[i].current_sample[layer_num][nrn_grp_num][nrn_num];
		neuron->iz_params->I_inject = y[i];
		time_ns = i*sampling_interval;
		if (!evaluate_neuron_dyn(neuron, time_ns, time_ns+sampling_interval, &spike_generated, &spike_time))
			return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_refractory_button_func", "! evaluate_neuron_dyn().");
		switch (neuron_dynamics_type_idx)
		{
			case DYNAMICS_TYPE_V:
				y_dynamics[i] = neuron->iz_params->v;
				break; 
			case DYNAMICS_TYPE_U:
				y_dynamics[i] = neuron->iz_params->u;
				break; 
			case DYNAMICS_TYPE_E:
				y_dynamics[i] = neuron->iz_params->conductance_excitatory;
				break; 
			case DYNAMICS_TYPE_I:
				y_dynamics[i] = neuron->iz_params->conductance_inhibitory;
				break; 
			default:
				return (void)print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_refractory_button_func", "Invalid neuron_dynamics_type_idx.");
		}		
	}
	if (!update_current_pattern_graph(current_pattern_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_refractory_button_func", "!update_current_pattern_graph().");	
	if (!update_neuron_dynamics_graph(neuron_dynamics_graph))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "display_currents_in_refractory_button_func", "!update_neuron_dynamics_graph().");	
	sprintf(str, "%.2f", current_templates->in_refractory_currents[current_num].noise_params[layer_num][nrn_grp_num][nrn_num].noise_variance);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_variance), str);
	sprintf(str, "%llu", current_templates->in_refractory_currents[current_num].noise_params[layer_num][nrn_grp_num][nrn_num].noise_addition_interval/1000000);
	gtk_entry_set_text(GTK_ENTRY(entry_noise_period), str);
}


static void add_noise_in_trial_button_func(void)
{
	char *end_ptr;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	unsigned int target_num_idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_target_num));
	unsigned int current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_current_pattern_number)), &end_ptr, 10);
	unsigned int i, j;
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "add_noise_in_trial_button_func", "! layer_neuron_group_neuron_get_selected().");
	if (current_num >= current_templates->num_of_in_trial_currents)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "add_noise_in_trial_button_func", "current_num >= num_of_in_trial_currents.");	
	for (i = 0; i < spike_gen_data->network->layer_count; i++)
	{ 
		for (j = 0; j < spike_gen_data->network->layers[i]->neuron_group_count; j++)
		{
			current_templates->in_trial_currents[target_num_idx][current_num].noise_params[i][j][0].noise_variance = atof(gtk_entry_get_text(GTK_ENTRY(entry_noise_variance)));
			current_templates->in_trial_currents[target_num_idx][current_num].noise_params[i][j][0].noise_addition_interval = 1000000*strtoull(gtk_entry_get_text(GTK_ENTRY(entry_noise_period)), &end_ptr, 10);	
		}
	}
}
static void add_noise_trial_available_button_func(void)
{
	char *end_ptr;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	unsigned int current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_current_pattern_number)), &end_ptr, 10);
	unsigned int i, j;
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "add_noise_trial_available_button_func", "! layer_neuron_group_neuron_get_selected().");
	if (current_num >= current_templates->num_of_trial_start_available_currents)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "add_noise_trial_available_button_func", "current_num >= num_of_trial_start_available_currents.");	
	for (i = 0; i < spike_gen_data->network->layer_count; i++)
	{ 
		for (j = 0; j < spike_gen_data->network->layers[i]->neuron_group_count; j++)
		{
			current_templates->trial_start_available_currents[current_num].noise_params[i][j][0].noise_variance = atof(gtk_entry_get_text(GTK_ENTRY(entry_noise_variance)));
			current_templates->trial_start_available_currents[current_num].noise_params[i][j][0].noise_addition_interval = 1000000*strtoull(gtk_entry_get_text(GTK_ENTRY(entry_noise_period)), &end_ptr, 10);
		}
	}
}
static void add_noise_in_refractory_button_func(void)
{
	char *end_ptr;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	unsigned int current_num = strtoull(gtk_entry_get_text(GTK_ENTRY(entry_current_pattern_number)), &end_ptr, 10);
	unsigned int i, j;
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "add_noise_in_refractory_button_func", "! layer_neuron_group_neuron_get_selected().");
	if (current_num >= current_templates->num_of_in_refractory_currents)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "add_noise_in_refractory_button_func", "current_num >= num_of_trial_start_available_currents.");	
	for (i = 0; i < spike_gen_data->network->layer_count; i++)
	{ 
		for (j = 0; j < spike_gen_data->network->layers[i]->neuron_group_count; j++)
		{
			current_templates->in_refractory_currents[current_num].noise_params[i][j][0].noise_variance = atof(gtk_entry_get_text(GTK_ENTRY(entry_noise_variance)));
			current_templates->in_refractory_currents[current_num].noise_params[i][j][0].noise_addition_interval = 1000000*strtoull(gtk_entry_get_text(GTK_ENTRY(entry_noise_period)), &end_ptr, 10);
		}
	}
}

void interrogate_network_button_func(void)
{
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	interrogate_network(spike_gen_data->network);
} 		

void interrogate_neuron_button_func(void)
{
	unsigned int layer_num, nrn_grp_num, nrn_num;

	char str[100];
	Neuron *neuron;	

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

	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	interrogate_neuron	(spike_gen_data->network, gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_layer)), gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron_group)), gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron)) );

	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "interrogate_neuron_button_func", "! layer_neuron_group_neuron_get_selected().");
	neuron = get_neuron_address(spike_gen_data->network, layer_num, nrn_grp_num, nrn_num);						
	if (!get_iz_neuron_parameters(neuron, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "interrogate_neuron_button_func", "!get_iz_neuron_parameters().");			

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

 	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_neuron_type), 0);	

     	sprintf(str, "%.1E", get_maximum_parker_sochacki_error_tolerance());	
	gtk_entry_set_text(GTK_ENTRY(entry_parker_sochacki_err_tol), str);
     	sprintf(str, "%d", get_maximum_parker_sochacki_order());	
	gtk_entry_set_text(GTK_ENTRY(entry_parker_sochacki_max_order), str);

}

void save_button_func(void)
{
	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
	path = &path_temp[7];   // since     uri returns file:///home/....
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();

	if (!(*write_spike_gen_config_data[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, path, spike_gen_data, txv_notes))		// record in last format version
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "save_button_func", "! (*save_spike_pattern_generator_data_directory)().");
	return;
}

void load_button_func(void)
{
	TimeStamp max_num_of_samples = 0;
	unsigned int i, j;
	char *path_temp = NULL, *path = NULL;
	int version;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_file_to_load));
	path = &path_temp[7];   // since     uri returns file:///home/....
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();	
	TrialHandParadigmRobotReach	*trial_hand_paradigm = get_bmi_simulation_spike_generator_data()->trial_hand_paradigm;
	if (!get_format_version(&version, path))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "load_button_func", "! get_spike_pattern_generator_data_format_version().");
	if (!(*read_spike_gen_config_data[version])(3, path, spike_gen_data, txv_notes))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "load_button_func", "! (*load_spike_pattern_generator_data_directory)().");
	if(!update_texts_of_combos_when_add_remove(combos_select_neuron, spike_gen_data->network))
		return;


	for (i = 0;  i < spike_gen_data->current_templates->num_of_trial_start_available_currents; i++)
	{	
		if (spike_gen_data->current_templates->trial_start_available_currents[i].num_of_template_samples > max_num_of_samples)
			max_num_of_samples = spike_gen_data->current_templates->trial_start_available_currents[i].num_of_template_samples;
	}
	for (i = 0;  i < spike_gen_data->current_templates->num_of_in_refractory_currents; i++)
	{	
		if (spike_gen_data->current_templates->in_refractory_currents[i].num_of_template_samples > max_num_of_samples)
			max_num_of_samples = spike_gen_data->current_templates->in_refractory_currents[i].num_of_template_samples;
	}
	for (i = 0; i < trial_hand_paradigm->num_of_robot_target_positions; i++)
	{
		for (j = 0; j < spike_gen_data->current_templates->num_of_in_trial_currents; j++)  // actually unnecessary, just to be straightforward
		{	
			if (spike_gen_data->current_templates->in_trial_currents[i][j].num_of_template_samples > max_num_of_samples)
				max_num_of_samples = spike_gen_data->current_templates->in_trial_currents[i][j].num_of_template_samples;
		}
	}
	current_pattern_graph = allocate_current_pattern_graph(current_pattern_graph_hbox, current_pattern_graph, max_num_of_samples, PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
	neuron_dynamics_graph = allocate_neuron_dynamics_graph(neuron_dynamics_graph_hbox, neuron_dynamics_graph, max_num_of_samples, PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
	spike_gen_data->limited_current_pattern_buffer = allocate_current_pattern_buffer_limited(spike_gen_data->network, spike_gen_data->limited_current_pattern_buffer, 3000000000/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, NUM_OF_CURRENT_PATTERN_GRAPHS); // 3 second buffer
	spike_gen_data->limited_neuron_dynamics_buffer = allocate_neuron_dynamics_buffer_limited(spike_gen_data->network, spike_gen_data->limited_neuron_dynamics_buffer, 3000000000/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, NUM_OF_NEURON_DYNAMICS_GRAPHS); // 3 second buffer for 1 second graph refresh rate. 
	spike_gen_data->spike_data[0] = allocate_spike_data(spike_gen_data->spike_data[0], get_num_of_neurons_in_network(spike_gen_data->network)*3*500 ); // 3 seconds buffer assuming a neuron firing rate cannot be more than 500 Hz 

	if (! buffer_view_handler())
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternDesignView", "generate_current_injection_graphs_button_func", "! buffer_view_handler()");	

	gtk_widget_set_sensitive(btn_add_neurons_to_layer, FALSE);		
	gtk_widget_set_sensitive(btn_generate_current_injection_graphs, TRUE);
	gtk_widget_set_sensitive(btn_draw_template, TRUE);
	gtk_widget_set_sensitive(btn_clear_template, TRUE);
	gtk_widget_set_sensitive(btn_copy_drawn_to_template_in_trial, TRUE);
	gtk_widget_set_sensitive(btn_copy_drawn_to_template_trial_available, TRUE);
	gtk_widget_set_sensitive(btn_copy_drawn_to_template_in_refractory, TRUE);
	gtk_widget_set_sensitive(btn_display_currents_in_trial, TRUE);
	gtk_widget_set_sensitive(btn_display_currents_trial_available, TRUE);
	gtk_widget_set_sensitive(btn_display_currents_in_refractory, TRUE);
	gtk_widget_set_sensitive(btn_add_noise_in_trial, TRUE);
	gtk_widget_set_sensitive(btn_add_noise_trial_available, TRUE);
	gtk_widget_set_sensitive(btn_add_noise_in_refractory, TRUE);
	gtk_widget_set_sensitive(btn_start_spike_generation, TRUE);	
	gtk_widget_set_sensitive(btn_load, FALSE);	



	return;
}

void set_directory_btn_select_directory(void)
{
	char line[600];
	FILE *fp = NULL;
       	if ((fp = fopen("./path_initial_directory", "r")) == NULL)  
       	{ 
       		printf ("ERROR: SpikePatternGenerator: Couldn't find file: ./path_initial_directory\n"); 
       		printf ("ERROR: SpikePatternGenerator: /home is loaded as initial direcoty to create data folder\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),"/home");
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: SpikePatternGenerator: Couldn' t read ./path_initial_directory\n"); 
       			printf ("ERROR: SpikePatternGenerator: /home is loaded as initial direcoty to create data folder\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),"/home");
		}
		else
		{
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),line);
		}
		fclose(fp); 		
	}  	 
}

void fill_notes_text_view(void)
{
	char line[1000];
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
       	}
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

static void quit_button_func(void)
{
	bmi_simulation_spike_generator_kill_rt_task();
	if (! delete_rt_task_from_rt_tasks_data(get_bmi_simulation_spike_generator_data()->rt_tasks_data, SPIKE_GENERATOR_CPU_ID, SPIKE_GENERATOR_CPU_THREAD_ID, SPIKE_GENERATOR_CPU_THREAD_TASK_ID, SPIKE_GENERATOR_PERIOD ))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternView", "quit_button_func", "! delete_rt_task_from_rt_tasks_data().");	
	print_message(INFO_MSG ,"BMISimulationSpikeGenerator", "CurrentPatternView", "quit_button_func", "QUIT.");				
	gtk_main_quit();	

}
