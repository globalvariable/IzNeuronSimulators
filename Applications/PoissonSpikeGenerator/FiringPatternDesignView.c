#include "FiringPatternDesignView.h"



// FIRST COLUMN

static LayerNrnGrpNeuronCombo *combos_select_neuron;
static GtkWidget *combo_target_num;

static GtkWidget *btn_create_default_network;

static GtkWidget *entry_firing_rate;
static GtkWidget *btn_apply_firing_rate_to_all;
static GtkWidget *btn_apply_firing_rate_in_trial;
static GtkWidget *btn_apply_firing_rate_trial_available;
static GtkWidget *btn_apply_firing_rate_in_refractory;

static GtkWidget *btn_start_spike_generation;




// SECOND COLUMN




/// LAST COLUMN
static GtkWidget *txv_notes;
static GtkWidget *btn_select_directory_to_save;
static GtkWidget *btn_select_file_to_load;
static GtkWidget *btn_save;
static GtkWidget *btn_load;


// THIRD COLUMN
///   at graphs.h


static void start_spike_generation_button_func(void);
static void create_default_network_button_func(void);
static void combos_select_neuron_func(GtkWidget *changed_combo);
static void combo_select_target_func(void);

static void apply_firing_rate_to_all_button_func(void);
static void apply_firing_rate_in_trial_button_func(void);
static void apply_firing_rate_trial_available_button_func(void);
static void apply_firing_rate_in_refractory_button_func(void);

static void save_button_func(void);
static void load_button_func(void);


static void fill_notes_text_view(void);
static void set_directory_btn_select_directory(void);

bool create_current_pattern_view_gui(void)
{
	GtkWidget *frame, *frame_label, *table, *vbox, *hbox, *lbl;

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

  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);


	btn_create_default_network = gtk_button_new_with_label("Create Default Network");
	gtk_box_pack_start (GTK_BOX (hbox), btn_create_default_network, TRUE, TRUE, 0);

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

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);


 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Firing Rate: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
        entry_firing_rate = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_firing_rate, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_firing_rate), "10");
	gtk_widget_set_size_request(entry_firing_rate, 60, 25) ;

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_apply_firing_rate_to_all = gtk_button_new_with_label("Apply to ALL");
	gtk_box_pack_start (GTK_BOX (hbox), btn_apply_firing_rate_to_all, TRUE, TRUE, 0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_apply_firing_rate_in_trial = gtk_button_new_with_label("InTrial");
	gtk_box_pack_start (GTK_BOX (hbox), btn_apply_firing_rate_in_trial, TRUE, TRUE, 0);
	btn_apply_firing_rate_trial_available = gtk_button_new_with_label("TrialAvail");
	gtk_box_pack_start (GTK_BOX (hbox), btn_apply_firing_rate_trial_available, TRUE, TRUE, 0);
	btn_apply_firing_rate_in_refractory = gtk_button_new_with_label("InRefrac");
	gtk_box_pack_start (GTK_BOX (hbox), btn_apply_firing_rate_in_refractory, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);


        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);
///////////////////////////////////////////// SECOND COLUMN  ///////////////////////////////////////////////////////////////

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 1,2,0,6);     ///  Stimulus graph control

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);



        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE,5);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);        
  
	btn_start_spike_generation = gtk_button_new_with_label("Start Spike Generation");
	gtk_box_pack_start (GTK_BOX (hbox), btn_start_spike_generation, TRUE, TRUE, 0);
	
///	GRAPHS


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


      	g_signal_connect(G_OBJECT(btn_start_spike_generation), "clicked", G_CALLBACK(start_spike_generation_button_func), NULL);

      	g_signal_connect(G_OBJECT(btn_create_default_network), "clicked", G_CALLBACK(create_default_network_button_func), NULL);

	g_signal_connect(G_OBJECT(combos_select_neuron->combo_layer), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_layer);
	g_signal_connect(G_OBJECT(combos_select_neuron->combo_neuron_group), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_neuron_group);	
	g_signal_connect(G_OBJECT(combos_select_neuron->combo_neuron), "changed", G_CALLBACK(combos_select_neuron_func), combos_select_neuron->combo_neuron);
	g_signal_connect(G_OBJECT(combo_target_num), "changed", G_CALLBACK(combo_select_target_func), NULL);

	g_signal_connect(G_OBJECT(btn_apply_firing_rate_to_all), "clicked", G_CALLBACK(apply_firing_rate_to_all_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_apply_firing_rate_in_trial), "clicked", G_CALLBACK(apply_firing_rate_in_trial_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_apply_firing_rate_trial_available), "clicked", G_CALLBACK(apply_firing_rate_trial_available_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_apply_firing_rate_in_refractory), "clicked", G_CALLBACK(apply_firing_rate_in_refractory_button_func), NULL);


	g_signal_connect(G_OBJECT(btn_save), "clicked", G_CALLBACK(save_button_func), NULL);		
	g_signal_connect(G_OBJECT(btn_load), "clicked", G_CALLBACK(load_button_func), NULL);	
	

	gtk_widget_set_sensitive(btn_start_spike_generation, FALSE);	
	
	return TRUE;

}



static void start_spike_generation_button_func(void)
{	
	gtk_widget_set_sensitive(btn_start_spike_generation, FALSE);	
	bmi_simulation_spike_generator_create_rt_thread();	
	send_global_pause_button_sensitive_request();
}




static void combos_select_neuron_func(GtkWidget *changed_combo)
{
	char temp[200];
	int mwa, chan, unit, target_num;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	if (spike_gen_data == NULL)
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "FiringPatternDesignView", "combos_select_neuron_func", "spike_gen_data == NULL.");
	if(!update_texts_of_combos_when_change(combos_select_neuron, spike_gen_data->network, changed_combo))
		return;

	mwa = gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_layer));
	chan = gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron_group));
	unit = gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron));
	target_num = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_target_num));	

	if ((mwa < 0) || (chan < 0) || (unit < 0))  
		return;    /// during addition of neurons combos generates meesages to here. at the beginning, when there is no elements added for a combo, the generated message gets the active index of combo as -1. this leads to segfault.

	sprintf (temp, "%.2f", spike_gen_data->firing_rate_data->in_trial_firing[target_num].rates[mwa][chan][unit]);
	gtk_entry_set_text(GTK_ENTRY(entry_firing_rate), temp);
}

static void combo_select_target_func(void)
{
	char temp[200];
	int mwa, chan, unit, target_num;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();	

	mwa = gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_layer));
	chan = gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron_group));
	unit = gtk_combo_box_get_active (GTK_COMBO_BOX(combos_select_neuron->combo_neuron));
	target_num = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_target_num));	

	sprintf (temp, "%.2f", spike_gen_data->firing_rate_data->in_trial_firing[target_num].rates[mwa][chan][unit]);
	gtk_entry_set_text(GTK_ENTRY(entry_firing_rate), temp);
}


void save_button_func(void)
{
	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
	path = &path_temp[7];   // since     uri returns file:///home/....
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();

	if (!(*write_spike_gen_config_data[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, path, spike_gen_data, txv_notes))		// record in last format version
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "FiringPatternDesignView", "save_button_func", "! (*save_spike_pattern_generator_data_directory)().");
	return;
}

void load_button_func(void)
{

	char *path_temp = NULL, *path = NULL;
	int version;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_file_to_load));
	path = &path_temp[7];   // since     uri returns file:///home/....

	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();	

	if (!get_format_version(&version, path))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "FiringPatternDesignView", "load_button_func", "! get_spike_pattern_generator_data_format_version().");
	if (!(*read_spike_gen_config_data[version])(3, path, spike_gen_data, txv_notes))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "FiringPatternDesignView", "load_button_func", "! (*load_spike_pattern_generator_data_directory)().");
	if(!update_texts_of_combos_when_add_remove(combos_select_neuron, spike_gen_data->network))
		return;

	spike_gen_data->spike_data = g_new0(SpikeData*,1);
	spike_gen_data->spike_data[0] = allocate_spike_data(spike_gen_data->spike_data[0], get_num_of_neurons_in_network(spike_gen_data->network)*3*2000 ); // 3 seconds buffer assuming a neuron firing rate cannot be more than 2000 Hz  (max BABBLING frequency.)

	if (! buffer_view_handler())
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "FiringPatternDesignView", "generate_current_injection_graphs_button_func", "! buffer_view_handler()");	


	gtk_widget_set_sensitive(btn_start_spike_generation, TRUE);	
	gtk_widget_set_sensitive(btn_create_default_network, FALSE);	
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





static void apply_firing_rate_to_all_button_func(void)
{
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	ThreePhasePoissonFiring *firing_data = spike_gen_data->firing_rate_data;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	unsigned int i, j, k, p;
	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "FiringPatternDesignView", "apply_firing_rate_to_all_button_func", "! layer_neuron_group_neuron_get_selected().");
	for (i = 0; i < spike_gen_data->network->layer_count; i++)
	{ 
		for (j = 0; j < spike_gen_data->network->layers[i]->neuron_group_count; j++)
		{
			for (k = 0; k < spike_gen_data->network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				firing_data->trial_start_available_firing.rates[i][j][k] = atof(gtk_entry_get_text(GTK_ENTRY(entry_firing_rate)));
				firing_data->in_refractory_firing.rates[i][j][k] = atof(gtk_entry_get_text(GTK_ENTRY(entry_firing_rate)));
				for (p = 0; p < spike_gen_data->trial_hand_paradigm->num_of_robot_target_positions; p++)
					firing_data->in_trial_firing[p].rates[i][j][k] = atof(gtk_entry_get_text(GTK_ENTRY(entry_firing_rate)));
			}
		}
	}
}

static void apply_firing_rate_in_trial_button_func(void)
{

	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	ThreePhasePoissonFiring *firing_data = spike_gen_data->firing_rate_data;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;
	unsigned int target_num_idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_target_num));

	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "FiringPatternDesignView", "add_noise_in_refractory_button_func", "! layer_neuron_group_neuron_get_selected().");
	
	firing_data->in_trial_firing[target_num_idx].rates[layer_num][nrn_grp_num][nrn_num] = atof(gtk_entry_get_text(GTK_ENTRY(entry_firing_rate)));
}

static void apply_firing_rate_trial_available_button_func(void)
{
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	ThreePhasePoissonFiring *firing_data = spike_gen_data->firing_rate_data;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;

	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "FiringPatternDesignView", "add_noise_in_refractory_button_func", "! layer_neuron_group_neuron_get_selected().");
	
	firing_data->trial_start_available_firing.rates[layer_num][nrn_grp_num][nrn_num] = atof(gtk_entry_get_text(GTK_ENTRY(entry_firing_rate)));
}


static void apply_firing_rate_in_refractory_button_func(void)
{
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	ThreePhasePoissonFiring *firing_data = spike_gen_data->firing_rate_data;
	unsigned int layer_num;
	unsigned int nrn_grp_num;
	unsigned int nrn_num;

	if (! layer_neuron_group_neuron_get_selected(combos_select_neuron, &layer_num, &nrn_grp_num, &nrn_num))
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "FiringPatternDesignView", "add_noise_in_refractory_button_func", "! layer_neuron_group_neuron_get_selected().");
	
	firing_data->in_refractory_firing.rates[layer_num][nrn_grp_num][nrn_num] = atof(gtk_entry_get_text(GTK_ENTRY(entry_firing_rate)));
}

static void create_default_network_button_func(void)
{
	unsigned int i, j, k;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j = 0; j < MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			if (!add_poisson_neurons_to_layer(spike_gen_data->network, MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE, i, FALSE,  0))
				return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorConfig", "add_neurons_to_network", "! add_poisson_neurons_to_layer().");
			for (k = 0; k < MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE; k++)
			{
				(*spike_gen_data->sorted_spike_time_stamp)[i][j].included_units[k] = TRUE;	
			}				
		}
	}
	spike_gen_data->firing_rate_data = allocate_three_phase_poisson_firing_data(spike_gen_data->network, spike_gen_data->trial_hand_paradigm, spike_gen_data->firing_rate_data);

	spike_gen_data->spike_data = g_new0(SpikeData*,1);
	spike_gen_data->spike_data[0] = allocate_spike_data(spike_gen_data->spike_data[0], get_num_of_neurons_in_network(spike_gen_data->network)*3*2000 ); // 3 seconds buffer assuming a neuron firing rate cannot be more than 2000 Hz  (max BABBLING frequency.)


	gtk_widget_set_sensitive(btn_create_default_network, FALSE);	
	gtk_widget_set_sensitive(btn_load, FALSE);	
	gtk_widget_set_sensitive(btn_start_spike_generation, TRUE);	


	if(!update_texts_of_combos_when_add_remove(combos_select_neuron, spike_gen_data->network))
		return;

	if (! buffer_view_handler())
		return (void)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "FiringPatternDesignView", "generate_current_injection_graphs_button_func", "! buffer_view_handler()");	

}
