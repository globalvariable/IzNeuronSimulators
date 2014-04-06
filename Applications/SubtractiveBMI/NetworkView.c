#include "NetworkView.h"

static HybridNetRLBMIData *static_bmi_data = NULL;
static NeuralNet2GuiMsg *static_msgs_neural_net_2_gui = NULL;
static RtTasksData *static_rt_tasks_data = NULL;
static GtkWidget *btn_select_directory_to_save = NULL;
static SpikeData **in_silico_spike_data_for_recording = NULL;
static unsigned int num_of_dedicated_cpu_threads = 0;
static Neuron **static_all_neurons = NULL;
static unsigned int static_num_of_neurons = 0;

// FIRST COLUMN

static LayerNrnGrpNeuronCombo *combos_select_bluespike_neuron;
static LayerNrnGrpNeuronCombo *combos_select_in_silico_neuron;

static GtkWidget *btn_connect_neurons;

// SECOND COLUMN

static GtkWidget *btn_ready_for_simulation;

static GtkWidget *btn_start_hybrid_network;

static GtkWidget *btn_select_directory_to_save;
static GtkWidget *btn_create_recording_folder;

static GtkWidget *btn_select_directory_to_load;
static GtkWidget *btn_load_data;
static GtkWidget *entry_load_data_folder_num;


// FIRST COLUMN

static void combos_select_bluespike_neuron_func(GtkWidget *changed_combo);
static void combos_select_in_silico_neuron_func(GtkWidget *changed_combo);
static void connect_neurons_button_func(void);
// SECOND COLUMN






static void ready_for_simulation_button_func(void);

static void start_hybrid_network_button_func(void);

static void create_recording_folder_button_func (void);

static void load_data_button_func (void);

static void set_directory_btn_select_directory_to_save(void);

static gboolean timeout_callback(gpointer user_data) ;

bool create_network_view_gui(void)
{
	static_msgs_neural_net_2_gui = get_hybrid_net_rl_bmi_data()->msgs_neural_net_2_gui;
	static_rt_tasks_data = get_hybrid_net_rl_bmi_data()->rt_tasks_data;
	in_silico_spike_data_for_recording = get_hybrid_net_rl_bmi_data()->in_silico_spike_data_for_recording;
	num_of_dedicated_cpu_threads = get_hybrid_net_rl_bmi_data()->num_of_dedicated_cpu_threads;


	GtkWidget *frame, *frame_label, *table, *vbox, *hbox;

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

	combos_select_bluespike_neuron = allocate_layer_neuron_group_neuron_combos();
        gtk_box_pack_start(GTK_BOX(hbox),combos_select_bluespike_neuron->combo_layer , TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_bluespike_neuron->combo_neuron_group, TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox),combos_select_bluespike_neuron->combo_neuron , TRUE,TRUE,0);

	if(!update_texts_of_combos_when_add_remove(combos_select_bluespike_neuron, get_hybrid_net_rl_bmi_data()->blue_spike_network))  // it is put here since main() adds neurons to the layers previously
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "create_network_view_gui(", "! update_texts_of_combos_when_add_remove().");	

    	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	combos_select_in_silico_neuron = allocate_layer_neuron_group_neuron_combos();
        gtk_box_pack_start(GTK_BOX(hbox),combos_select_in_silico_neuron->combo_layer , TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox), combos_select_in_silico_neuron->combo_neuron_group, TRUE,TRUE,0);
        gtk_box_pack_start(GTK_BOX(hbox),combos_select_in_silico_neuron->combo_neuron , TRUE,TRUE,0);

	if(!update_texts_of_combos_when_add_remove(combos_select_in_silico_neuron, get_hybrid_net_rl_bmi_data()->in_silico_network))  // it is put here since main() adds neurons to the layers previously
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "create_network_view_gui(", "! update_texts_of_combos_when_add_remove().");	

    	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_connect_neurons = gtk_button_new_with_label("Connect Neurons");
	gtk_box_pack_start (GTK_BOX (hbox), btn_connect_neurons, TRUE, TRUE, 0);








///////////////////////////////////////////// SECOND COLUMN  ///////////////////////////////////////////////////////////////

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 1,2, 0, 6);  

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	
	btn_ready_for_simulation = gtk_button_new_with_label("Ready");
	gtk_box_pack_start (GTK_BOX (hbox), btn_ready_for_simulation, TRUE, TRUE, 0);

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



	g_signal_connect(G_OBJECT(combos_select_bluespike_neuron->combo_layer), "changed", G_CALLBACK(combos_select_bluespike_neuron_func), combos_select_bluespike_neuron->combo_layer);
	g_signal_connect(G_OBJECT(combos_select_bluespike_neuron->combo_neuron_group), "changed", G_CALLBACK(combos_select_bluespike_neuron_func), combos_select_bluespike_neuron->combo_neuron_group);	
	g_signal_connect(G_OBJECT(combos_select_bluespike_neuron->combo_neuron), "changed", G_CALLBACK(combos_select_bluespike_neuron_func), combos_select_bluespike_neuron->combo_neuron);

	g_signal_connect(G_OBJECT(combos_select_in_silico_neuron->combo_layer), "changed", G_CALLBACK(combos_select_in_silico_neuron_func), combos_select_in_silico_neuron->combo_layer);
	g_signal_connect(G_OBJECT(combos_select_in_silico_neuron->combo_neuron_group), "changed", G_CALLBACK(combos_select_in_silico_neuron_func), combos_select_in_silico_neuron->combo_neuron_group);	
	g_signal_connect(G_OBJECT(combos_select_in_silico_neuron->combo_neuron), "changed", G_CALLBACK(combos_select_in_silico_neuron_func), combos_select_in_silico_neuron->combo_neuron);

      	g_signal_connect(G_OBJECT(btn_connect_neurons), "clicked", G_CALLBACK(connect_neurons_button_func), NULL);

      	g_signal_connect(G_OBJECT(btn_ready_for_simulation), "clicked", G_CALLBACK(ready_for_simulation_button_func), NULL);

      	g_signal_connect(G_OBJECT(btn_start_hybrid_network), "clicked", G_CALLBACK(start_hybrid_network_button_func), NULL);

	g_signal_connect(G_OBJECT(btn_create_recording_folder), "clicked", G_CALLBACK(create_recording_folder_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_load_data), "clicked", G_CALLBACK(load_data_button_func), NULL);

	gtk_widget_set_sensitive(btn_create_recording_folder, FALSE);
	
	gtk_widget_set_sensitive(btn_start_hybrid_network, FALSE);	

	return TRUE;
}




static void start_hybrid_network_button_func(void)
{
	gtk_widget_set_sensitive(btn_start_hybrid_network, FALSE);	
	hybrid_net_rl_bmi_create_rt_threads();	
//	send_global_pause_button_sensitive_request();	 // enable pause/resume button to resume buffer visualization. resume adjusts buffer read indexes and start times. 
}

static void ready_for_simulation_button_func(void)
{
	unsigned int i;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	SpikeData *spike_data;

	bmi_data->blue_spike_spike_data_for_graph = g_new0(SpikeData*, 1);
	bmi_data->blue_spike_spike_data_for_graph[0] = allocate_spike_data(bmi_data->blue_spike_spike_data_for_graph[0], get_num_of_neurons_in_network(bmi_data->blue_spike_network)*3*500 ); // 3 seconds buffer assuming a neuron firing rate cannot be more than 500 Hz 
	for (i = 0; i < bmi_data->num_of_dedicated_cpu_threads; i ++)
		bmi_data->in_silico_spike_data_for_recording[i] = allocate_spike_data(bmi_data->in_silico_spike_data_for_recording[i], get_num_of_neurons_in_network(bmi_data->in_silico_network)*3*500 ); // 3 seconds buffer assuming a neuron firing rate cannot be more than 500 Hz 

	if (!buffer_view_handler())
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "ready_for_simulation_button_func", "! create_buffers_view_gui().");	

	gtk_widget_set_sensitive(btn_start_hybrid_network, TRUE);	


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



	while (get_next_neural_net_2_gui_msg_buffer_item(static_msgs_neural_net_2_gui, &msg_item))
	{
		switch (msg_item.msg_type)
		{
			case NEURAL_NET_2_GUI_MSG_START_RECORDING:
				path_temp = NULL; path = NULL;
				path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
				path = &path_temp[7];   // since     uri returns file:///home/....	
				recording_number = msg_item.additional_data.recording_number;
				if (!(*create_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(3, path, *(static_bmi_data->sys_time_ptr), recording_number))	
				{
					print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "timeout_callback", " *create_data_directory().");		
					exit(1);
				}
				recording = TRUE;	
				break;
			case NEURAL_NET_2_GUI_MSG_STOP_RECORDING:
				if (! (*fclose_all_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(1, *(static_bmi_data->sys_time_ptr)))	
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
				if (! (*fclose_all_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(1, *(static_bmi_data->sys_time_ptr)))	
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
	}
	else
	{
		print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "load_data_button_func", " *load_data_directory().");			
	}
}


static void combos_select_bluespike_neuron_func(GtkWidget *changed_combo)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	if (bmi_data == NULL)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "combos_select_bluespike_neuron_func", "bmi_data == NULL.");
	if(!update_texts_of_combos_when_change(combos_select_bluespike_neuron, bmi_data->blue_spike_network, changed_combo))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "combos_select_bluespike_neuron_funcc", "!update_texts_of_combos_when_change().");
}
static void combos_select_in_silico_neuron_func(GtkWidget *changed_combo)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	if (bmi_data == NULL)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "combos_select_bluespike_neuron_func", "bmi_data == NULL.");
	if(!update_texts_of_combos_when_change(combos_select_in_silico_neuron, bmi_data->in_silico_network, changed_combo))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "combos_select_bluespike_neuron_funcc", "!update_texts_of_combos_when_change().");
}

static void connect_neurons_button_func(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	HybridNetRLBMISynapseData	*synapse_data = &(bmi_data->synapse_data);
	unsigned int bs_layer_num, bs_nrn_grp_num, bs_nrn_num;
	unsigned int is_layer_num, is_nrn_grp_num, is_nrn_num;

	synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_max = 7;
	synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_min = 7;
	synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);

	synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_max = 7;
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_min = 7;
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_max = (AxonalDelay)(7.0 * 1000000);
	synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_min = (AxonalDelay)(7.0 * 1000000);


	if (! layer_neuron_group_neuron_get_selected(combos_select_bluespike_neuron, &bs_layer_num, &bs_nrn_grp_num, &bs_nrn_num))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "EligibilityView", "connect_neurons_button_func", "! layer_neuron_group_neuron_get_selected().");	

	if (! layer_neuron_group_neuron_get_selected(combos_select_in_silico_neuron, &is_layer_num, &is_nrn_grp_num, &is_nrn_num))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "EligibilityView", "connect_neurons_button_func", "! layer_neuron_group_neuron_get_selected().");	

	if (! connect_neurons(bmi_data->blue_spike_network, bs_layer_num, bs_nrn_grp_num, bs_nrn_num, bmi_data->in_silico_network, is_layer_num, is_nrn_grp_num, is_nrn_num, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_weight_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_weight_min, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_max, synapse_data->blue_spike_2_in_silico_excitatory_connection_delay_min, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_max, synapse_data->blue_spike_2_in_silico_inhibitory_connection_delay_min, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, TRUE, FALSE))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_neurons().");


}
