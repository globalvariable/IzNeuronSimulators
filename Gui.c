 #include "Gui.h"

void create_gui(void)
{
	color_bg_network.red = 0;
	color_bg_network.green = 0;
	color_bg_network.blue = 0;

	color_bg_neuron_dynamics.red = 65535;
	color_bg_neuron_dynamics.green = 65535;
	color_bg_neuron_dynamics.blue = 65535;

	color_neuron_dynamics_line.red = 0;
	color_neuron_dynamics_line.green = 0;
	color_neuron_dynamics_line.blue = 0;

	color_neuron_dynamics_grid.red = 50000;
	color_neuron_dynamics_grid.green = 50000;
	color_neuron_dynamics_grid.blue = 50000;

	rt_simulation_ongoing = 0;
	non_rt_simulation_ongoing = 0;
	SelfClock = 1;

	turn_neuron_dynamics_logging_off();
	num_of_frames_for_neuron_dynamics = 0;

	GtkWidget *window;
	GtkWidget *hbox, *hbox1, *vbox, *vbox1, *lbl, *table;

 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 1920, 900);
  	gtk_window_set_title(GTK_WINDOW(window), "NeuroSim");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  	vbox = gtk_vbox_new(TRUE, 5);
	gtk_container_add(GTK_CONTAINER(window), vbox);

  	hbox = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 10);

	tabs = gtk_notebook_new ();
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (tabs), GTK_POS_TOP);
        gtk_box_pack_start(GTK_BOX(hbox),tabs, TRUE, TRUE, 0);

///////////////////////////////////////////// ENTRIES ///////////////////////////////////////////////////////////////

        frame_entries = gtk_frame_new ("Entries");
        label_frame_entries = gtk_label_new ("Entries");

  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_container_add (GTK_CONTAINER (frame_entries), hbox);

 	vbox = gtk_vbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(hbox),vbox, FALSE,FALSE,0);


  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,10);

	lbl = gtk_label_new("NETWORK");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("Number of Layers:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_num_of_layers = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_num_of_layers, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_num_of_layers), "3");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	btn_create_network = gtk_button_new_with_label("Create Network");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_create_network, TRUE, TRUE, 0);

  	hbox1 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

 	btn_submit_neuron_dynamics_logging_buffer_size = gtk_button_new_with_label("Submit Buffer Size for Logging");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_submit_neuron_dynamics_logging_buffer_size, TRUE, TRUE, 0);

	entry_buffer_size_for_dynamics_log = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_buffer_size_for_dynamics_log, TRUE,TRUE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_buffer_size_for_dynamics_log), "4000");

 	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	btn_log_neuron_dynamics_on_off= gtk_button_new_with_label("Neuron Dynamics Logging OFF");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_log_neuron_dynamics_on_off, TRUE, TRUE, 0);

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,10);

	lbl = gtk_label_new("NEURON PARAMETERS");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	combo_neuron_type = gtk_combo_box_new_text();
        gtk_box_pack_start(GTK_BOX(hbox1),combo_neuron_type, TRUE,TRUE,0);
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_neuron_type), "Regular Spiking");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_neuron_type), "Intrinsically Bursting");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_neuron_type), "Chattering");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_neuron_type), "(Inhibitory) Low-Threshold  Spiking");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_neuron_type), "(Inhibitory) Fast Spiking");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("v:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_v = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_v, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_v), "0");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("a:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_a = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_a, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_a), "0.03");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("b:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_b = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_b, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_b), "-9.5");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("c:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_c = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_c, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_c), "-85");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("d:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_d = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_d, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_d), "0");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("k:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_k = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_k, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_k), "1.3");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("C:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_C = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_C, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_C), "200");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("E Excitatory:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_E_excitatory = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_E_excitatory, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_E_excitatory), "0.0");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("E Inhibitory:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_E_inhibitory = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_E_inhibitory, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_E_inhibitory), "-80.0");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("Excitatory Time Constant (ms):");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_excitatory_tau = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_excitatory_tau, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_excitatory_tau), "5.0");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("Inhibitory Time Constant (ms):");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_inhibitory_tau = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_inhibitory_tau, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_inhibitory_tau), "10.0");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("v threshold:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_v_threshold = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_v_threshold, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_v_threshold), "-50");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("v peak:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_v_peak = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_v_peak, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_v_peak), "48");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("v resting:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_v_resting = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_v_resting, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_v_resting), "-65");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("I_inject:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_I_inject = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_I_inject, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_I_inject), "30");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("Randomize Params:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_randomize_params = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_randomize_params, TRUE,TRUE,0);
	gtk_entry_set_max_length (GTK_ENTRY(entry_randomize_params), 5);
	gtk_entry_set_text(GTK_ENTRY(entry_randomize_params), "00000");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("inhibitory:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_inhibitory = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_inhibitory, TRUE,TRUE,0);
	gtk_entry_set_max_length (GTK_ENTRY(entry_inhibitory), 1);
	gtk_entry_set_text(GTK_ENTRY(entry_inhibitory), "0");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("Num of Neuron to Add:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_num_of_neuron_for_group = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_num_of_neuron_for_group, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_num_of_neuron_for_group), "1");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	btn_add_nrn_grp_to_layer = gtk_button_new_with_label("Add Neurons to Layer");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_add_nrn_grp_to_layer, TRUE, TRUE, 0);
        entry_add_nrn_grp_to_layer = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_add_nrn_grp_to_layer, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_add_nrn_grp_to_layer), "0");

  	hbox1 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,10);

	lbl = gtk_label_new("SYNAPSE PARAMETERS");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);

 	hbox1 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("Excitatory Weight Min:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_weight_excitatory_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_weight_excitatory_min, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_weight_excitatory_min), "0.001");

	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_weight_excitatory_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_weight_excitatory_max, FALSE,FALSE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_weight_excitatory_max), "1");

  	hbox1 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("Inhibitory Weight Min: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_weight_inhibitory_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_weight_inhibitory_min, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_weight_inhibitory_min), "0.001");

	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_weight_inhibitory_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_weight_inhibitory_max, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_weight_inhibitory_max), "1");

  	hbox1 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("EPSP Delay Min(ns): ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_excitatory_delay_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_excitatory_delay_min , TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_excitatory_delay_min ), "1000000");

	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_excitatory_delay_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_excitatory_delay_max, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_excitatory_delay_max), "5000000");

  	hbox1 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("IPSP Delay Min(ns): ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_inhibitory_delay_min = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_inhibitory_delay_min , TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_inhibitory_delay_min ), "1000000");

	lbl = gtk_label_new("Max:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_inhibitory_delay_max = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_inhibitory_delay_max, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_inhibitory_delay_max), "5000000");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,10);

	lbl = gtk_label_new("CONNECT LAYERS");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);

  	hbox1 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("Connection Probability (0-1)");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_connection_probability = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox1), entry_connection_probability, TRUE,TRUE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_connection_probability), "1");

 	hbox1 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,10);

	btn_connect_layer_to_layer = gtk_button_new_with_label("Connect Layer");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_connect_layer_to_layer, TRUE, TRUE, 0);
        entry_layer_num_to_connect = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_layer_num_to_connect, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_layer_num_to_connect), "0");
	lbl = gtk_label_new("to");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, FALSE,FALSE,0);	
        entry_layer_num_to_connect_to = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_layer_num_to_connect_to, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_layer_num_to_connect_to), "0");
///// Second Column
 	vbox = gtk_vbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(hbox),vbox, TRUE,TRUE,0);

 	vbox1 = gtk_vbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),vbox1, FALSE,FALSE,0);

  	hbox1 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, TRUE,TRUE,0);

	lbl = gtk_label_new("PARKER-SOCHACKI PARAMETERS");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,20);

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, TRUE, TRUE,0);

	lbl = gtk_label_new("Parker-Sochacki Max Order:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_parker_sochacki_max_order = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_parker_sochacki_max_order , TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_parker_sochacki_max_order), "200");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, TRUE,TRUE,0);

	lbl = gtk_label_new("Parker-Sochacki Error Tolerance:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_parker_sochacki_error_tolerance = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_parker_sochacki_error_tolerance , TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_parker_sochacki_error_tolerance), "1e-16");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, TRUE,TRUE,0);

	lbl = gtk_label_new("Newton-Raphson Max Iter:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_newton_raphson_max_iter = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_newton_raphson_max_iter , TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_newton_raphson_max_iter), "200");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, TRUE,TRUE,0);

	lbl = gtk_label_new("Newton-Raphson Error Tolerance:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_newton_raphson_error_tolerance = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_newton_raphson_error_tolerance, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_newton_raphson_error_tolerance), "1e-16");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, TRUE,TRUE,0);

	lbl = gtk_label_new("Step Size(ns): ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);

        entry_simulation_given_step_size_ns = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_simulation_given_step_size_ns, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_simulation_given_step_size_ns), "250000");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, TRUE,TRUE,0);

	btn_submit_parker_sochacki_params = gtk_button_new_with_label("Submit Parameters");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_submit_parker_sochacki_params, TRUE, TRUE, 0);

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, TRUE,TRUE,10);

	btn_clear_parker_sochacki_polynomials = gtk_button_new_with_label("Clear Polynomials");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_clear_parker_sochacki_polynomials, TRUE, TRUE, 0);

 	vbox1 = gtk_vbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),vbox1, FALSE,FALSE,0);

  	hbox1 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, TRUE,TRUE,0);

	lbl = gtk_label_new("NON-REAL-TIME Simulation");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,20);

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, TRUE,TRUE,20);

	btn_start_stop_simulation = gtk_button_new_with_label("Start Simulation");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_start_stop_simulation, TRUE, TRUE, 0);

	lbl = gtk_label_new("Simulation Duration (ms): ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);

        entry_simulation_duration = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_simulation_duration, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_simulation_duration), "1000");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, TRUE,TRUE,0);

	btn_resume_nonrt_simulation = gtk_button_new_with_label("Resume");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_resume_nonrt_simulation, TRUE, TRUE, 0);

 	vbox1 = gtk_vbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),vbox1, FALSE,FALSE,0);

  	hbox1 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, TRUE,TRUE,0);

	lbl = gtk_label_new("REAL-TIME SIMULATION");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,20);

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1),hbox1, TRUE,TRUE,20);

	btn_self_clock = gtk_button_new_with_label("Self-Clock ON");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_self_clock, TRUE, TRUE, 0);

	btn_start_stop_rt_simulation = gtk_button_new_with_label("Start RT Simulation");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_start_stop_rt_simulation, TRUE, TRUE, 0);

 	vbox1 = gtk_vbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),vbox1, FALSE,FALSE,0);

////    Third Column
  	vbox = gtk_vbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (hbox), vbox, FALSE, FALSE, 0);

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,10);

	lbl = gtk_label_new("INTERROGATIONS");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("Layer #:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_interrogate_layer_num = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_interrogate_layer_num, TRUE,TRUE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_interrogate_layer_num), "0");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("Group #:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_interrogate_neuron_group_num= gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_interrogate_neuron_group_num, TRUE,TRUE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_interrogate_neuron_group_num), "0");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	lbl = gtk_label_new("Neuron #:");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);
        entry_interrogate_neuron_num = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_interrogate_neuron_num, TRUE,TRUE,0);
	gtk_entry_set_text(GTK_ENTRY(entry_interrogate_neuron_num), "0");

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	btn_interrogate_neuron = gtk_button_new_with_label("Interrogate Neuron");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_interrogate_neuron, TRUE, TRUE, 0);

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	btn_interrogate_neuron_group = gtk_button_new_with_label("Interrogate Neuron Group");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_interrogate_neuron_group, TRUE, TRUE, 0);


  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	btn_interrogate_layer = gtk_button_new_with_label("Interrogate Layer");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_interrogate_layer, TRUE, TRUE, 0);

 	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,10);

 	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	btn_reset_neurons_in_group = gtk_button_new_with_label("Reset Neurons in Group with New Parameters");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_reset_neurons_in_group, TRUE, TRUE, 0);

 	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	btn_reset_neuron = gtk_button_new_with_label("Reset Neuron with New Parameters");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_reset_neuron, TRUE, TRUE, 0);

 	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,20);

 	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

	btn_reset_all_synapses_with_new_params = gtk_button_new_with_label("Reset All Synapses with New Parameters");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_reset_all_synapses_with_new_params, TRUE, TRUE, 0);

 	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,20);

	btn_reset_all_neurons = gtk_button_new_with_label("Reset All Neurons to Their Initial Value");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_reset_all_neurons, TRUE, TRUE, 0);

 	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,20);

  	hbox1 = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,10);

	lbl = gtk_label_new("NEURON DYNAMICS GRAPHS");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, TRUE,TRUE,0);

  	hbox1 = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox1, FALSE,FALSE,0);

 	btn_submit_number_of_frames_for_neuron_dynamics = gtk_button_new_with_label("Submit Number of Frames");
	gtk_box_pack_start (GTK_BOX (hbox1), btn_submit_number_of_frames_for_neuron_dynamics, TRUE, TRUE, 0);

	entry_number_of_frames_for_neuron_dynamics = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1), entry_number_of_frames_for_neuron_dynamics, TRUE,TRUE,0);	
	gtk_entry_set_text(GTK_ENTRY(entry_number_of_frames_for_neuron_dynamics), "2");

////////////////////////////////////////////////// VISUALIZE NETWORK////////////////////////////////////

        frame_network_visualization = gtk_frame_new ("Visualize Network");
        label_network_visualization = gtk_label_new ("Visualize Network");
  	gtk_databox_create_box_with_scrollbars_and_rulers (&box_input_neurons, &databox_input_neurons,TRUE, TRUE, TRUE, TRUE);
  	gtk_databox_create_box_with_scrollbars_and_rulers (&box_hidden_neurons, &databox_hidden_neurons,TRUE, TRUE, TRUE, TRUE);
  	gtk_databox_create_box_with_scrollbars_and_rulers (&box_output_neurons, &databox_output_neurons,TRUE, TRUE, TRUE, TRUE);
  	gtk_widget_modify_bg (box_input_neurons, GTK_STATE_NORMAL, &color_bg_network);
  	gtk_widget_modify_bg (box_hidden_neurons, GTK_STATE_NORMAL, &color_bg_network);
  	gtk_widget_modify_bg (box_output_neurons, GTK_STATE_NORMAL, &color_bg_network);

    	table = gtk_table_new(1,10,TRUE);
        gtk_container_add (GTK_CONTAINER (frame_network_visualization ), table);
    	gtk_table_attach_defaults(GTK_TABLE(table), databox_input_neurons, 0,1,0,1);
    	gtk_table_attach_defaults(GTK_TABLE(table), databox_hidden_neurons, 1,9,0,1);
   	gtk_table_attach_defaults(GTK_TABLE(table), databox_output_neurons, 9,10,0,1);

	gtk_databox_set_total_limits (GTK_DATABOX (box_input_neurons), 0, 30, +10, -10);
	gtk_databox_set_total_limits (GTK_DATABOX (box_hidden_neurons), 0, 30, +10, -10);
	gtk_databox_set_total_limits (GTK_DATABOX (box_output_neurons), 0, 30, +10, -10);


        gtk_notebook_append_page (GTK_NOTEBOOK (tabs), frame_entries, label_frame_entries);  
        gtk_notebook_append_page (GTK_NOTEBOOK (tabs), frame_network_visualization, label_network_visualization);  

	gtk_widget_show_all(window);

	g_signal_connect_swapped(G_OBJECT(btn_create_network), "clicked", G_CALLBACK(create_network_button_func), G_OBJECT(window));
	g_signal_connect_swapped(G_OBJECT(btn_add_nrn_grp_to_layer), "clicked", G_CALLBACK(add_nrn_grp_to_layer_button_func), G_OBJECT(window));
	g_signal_connect_swapped(G_OBJECT(combo_neuron_type), "changed", G_CALLBACK(combo_neuron_type_func), G_OBJECT(window));
	
	g_signal_connect_swapped(G_OBJECT(btn_connect_layer_to_layer), "clicked", G_CALLBACK(connect_layer_to_layer_button_func), G_OBJECT(window));

	g_signal_connect_swapped(G_OBJECT(btn_submit_parker_sochacki_params), "clicked", G_CALLBACK(submit_parker_sochacki_params_button_func), G_OBJECT(window));
	g_signal_connect_swapped(G_OBJECT(btn_clear_parker_sochacki_polynomials), "clicked", G_CALLBACK(clear_parker_sochacki_polynomials_button_func), G_OBJECT(window));

	g_signal_connect_swapped(G_OBJECT(btn_start_stop_simulation), "clicked", G_CALLBACK(start_stop_simulation_button_func), G_OBJECT(window));
	g_signal_connect_swapped(G_OBJECT(btn_resume_nonrt_simulation), "clicked", G_CALLBACK(resume_nonrt_simulation_button_func), G_OBJECT(window));
	g_signal_connect_swapped(G_OBJECT(btn_start_stop_rt_simulation), "clicked", G_CALLBACK(start_stop_rt_simulation_button_func), G_OBJECT(window));
	g_signal_connect_swapped(G_OBJECT(btn_self_clock), "clicked", G_CALLBACK(self_clock_button_func), G_OBJECT(window));

	g_signal_connect_swapped(G_OBJECT(btn_reset_all_neurons), "clicked", G_CALLBACK(reset_all_neurons_button_func), G_OBJECT(window));
	g_signal_connect_swapped(G_OBJECT(btn_reset_all_synapses_with_new_params), "clicked", G_CALLBACK(reset_all_synapses_with_new_params_button_func), G_OBJECT(window));

	g_signal_connect_swapped(G_OBJECT(btn_interrogate_neuron), "clicked", G_CALLBACK(interrogate_neuron_button_func), G_OBJECT(window));
	g_signal_connect_swapped(G_OBJECT(btn_interrogate_neuron_group), "clicked", G_CALLBACK(interrogate_neuron_group_button_func), G_OBJECT(window));
	g_signal_connect_swapped(G_OBJECT(btn_interrogate_layer), "clicked", G_CALLBACK(interrogate_layer_button_func), G_OBJECT(window));

	g_signal_connect_swapped(G_OBJECT(btn_log_neuron_dynamics_on_off), "clicked", G_CALLBACK(log_neuron_dynamics_on_off_button_func), G_OBJECT(window));
	g_signal_connect_swapped(G_OBJECT(btn_submit_neuron_dynamics_logging_buffer_size), "clicked", G_CALLBACK(submit_logging_buffer_size_button_func), G_OBJECT(window));

	g_signal_connect_swapped(G_OBJECT(btn_reset_neurons_in_group), "clicked", G_CALLBACK(reset_neurons_in_group_button_func), G_OBJECT(window));
	g_signal_connect_swapped(G_OBJECT(btn_reset_neuron), "clicked", G_CALLBACK(reset_neuron_button_func), G_OBJECT(window));

	g_signal_connect_swapped(G_OBJECT(btn_submit_number_of_frames_for_neuron_dynamics), "clicked", G_CALLBACK(submit_number_of_frames_for_neuron_dynamics_button_func), G_OBJECT(window));


  	g_signal_connect (GTK_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);

	gtk_widget_set_sensitive(btn_create_network, TRUE);
	gtk_widget_set_sensitive(btn_add_nrn_grp_to_layer, FALSE);
	gtk_widget_set_sensitive(btn_connect_layer_to_layer, FALSE);
	gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, TRUE);
	gtk_widget_set_sensitive(btn_start_stop_rt_simulation, FALSE);
	gtk_widget_set_sensitive(btn_self_clock, FALSE);
	gtk_widget_set_sensitive(btn_start_stop_simulation, FALSE);
	gtk_widget_set_sensitive(btn_resume_nonrt_simulation , FALSE);
	gtk_widget_set_sensitive(btn_reset_all_synapses_with_new_params, FALSE);
	gtk_widget_set_sensitive(btn_reset_all_neurons, FALSE);
	gtk_widget_set_sensitive(btn_interrogate_neuron, FALSE);
	gtk_widget_set_sensitive(btn_interrogate_neuron_group, FALSE);
	gtk_widget_set_sensitive(btn_interrogate_layer, FALSE);
	gtk_widget_set_sensitive(btn_log_neuron_dynamics_on_off, FALSE);
	gtk_widget_set_sensitive(btn_submit_neuron_dynamics_logging_buffer_size, FALSE);
	gtk_widget_set_sensitive(btn_reset_neurons_in_group, FALSE);
 	gtk_widget_set_sensitive(btn_reset_neuron, FALSE);
	gtk_widget_set_sensitive(btn_submit_number_of_frames_for_neuron_dynamics, FALSE);
	return;
}

void create_network_button_func(void)
{
	if (create_neural_network((int)atof(gtk_entry_get_text(GTK_ENTRY(entry_num_of_layers)))))
	{
		gtk_widget_set_sensitive(btn_add_nrn_grp_to_layer, TRUE);
 		gtk_widget_set_sensitive(btn_submit_neuron_dynamics_logging_buffer_size, TRUE);
		gtk_widget_set_sensitive(btn_create_network, FALSE);
	}
	else
	{
		printf("Couldn't create network\n");	
		gtk_widget_set_sensitive(btn_add_nrn_grp_to_layer, FALSE);	
	}	
	return;
}

void add_nrn_grp_to_layer_button_func(void)
{
	int num_of_neuron, layer, randomize;
	double v, a, b, c, d, I_inject, C, E_excitatory, E_inhibitory, v_resting, v_threshold, v_peak, k, tau_excitatory, tau_inhibitory;
	bool inhibitory;
	num_of_neuron = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_num_of_neuron_for_group)));
	layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_add_nrn_grp_to_layer)));

	v = atof(gtk_entry_get_text(GTK_ENTRY(entry_v)));
	a = atof(gtk_entry_get_text(GTK_ENTRY(entry_a)));
	b = atof(gtk_entry_get_text(GTK_ENTRY(entry_b)));
	c = atof(gtk_entry_get_text(GTK_ENTRY(entry_c)));
	d = atof(gtk_entry_get_text(GTK_ENTRY(entry_d)));
	I_inject = atof(gtk_entry_get_text(GTK_ENTRY(entry_I_inject)));
	C = atof(gtk_entry_get_text(GTK_ENTRY(entry_C)));
	E_excitatory = atof(gtk_entry_get_text(GTK_ENTRY(entry_E_excitatory)));
	E_inhibitory = atof(gtk_entry_get_text(GTK_ENTRY(entry_E_inhibitory)));
	v_resting = atof(gtk_entry_get_text(GTK_ENTRY(entry_v_resting)));
	v_threshold = atof(gtk_entry_get_text(GTK_ENTRY(entry_v_threshold)));
	v_peak = atof(gtk_entry_get_text(GTK_ENTRY(entry_v_peak)));
	k = atof(gtk_entry_get_text(GTK_ENTRY(entry_k)));
	tau_excitatory = atof(gtk_entry_get_text(GTK_ENTRY(entry_excitatory_tau)));
	tau_inhibitory = atof(gtk_entry_get_text(GTK_ENTRY(entry_inhibitory_tau)));
	randomize = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_randomize_params)));
	inhibitory = (bool)atof(gtk_entry_get_text(GTK_ENTRY(entry_inhibitory)));

	if (add_neuron_group_to_layer(num_of_neuron, layer, v, a, b, c, d, I_inject, inhibitory, randomize, C, E_excitatory, E_inhibitory,  v_resting, v_threshold, v_peak, k, tau_excitatory, tau_inhibitory, get_buffer_size_for_neuron_dynamics_logging()))
	{
		printf ("Added %d neurons to layer %d.\n", num_of_neuron , layer);
		gtk_widget_set_sensitive(btn_connect_layer_to_layer, TRUE);
		gtk_widget_set_sensitive(btn_interrogate_neuron, TRUE);
		gtk_widget_set_sensitive(btn_interrogate_neuron_group, TRUE);
		gtk_widget_set_sensitive(btn_interrogate_layer, TRUE);
		gtk_widget_set_sensitive(btn_submit_neuron_dynamics_logging_buffer_size, TRUE);
		gtk_widget_set_sensitive(btn_reset_neurons_in_group, TRUE);
		gtk_widget_set_sensitive(btn_reset_neuron, TRUE);
 		gtk_widget_set_sensitive(btn_submit_neuron_dynamics_logging_buffer_size, FALSE);
		if (neuron_dynamics_logging_on)
			gtk_widget_set_sensitive(btn_submit_number_of_frames_for_neuron_dynamics, TRUE);
		else
			gtk_widget_set_sensitive(btn_submit_number_of_frames_for_neuron_dynamics, FALSE);
			
	}
	else
	{
		printf ("Couldn't add %d neurons to layer %d.\n", num_of_neuron , layer);
	}

	return;
}

void connect_layer_to_layer_button_func(void)
{
	int this_layer, target_layer;
	unsigned int IPSP_delay_min, IPSP_delay_max, EPSP_delay_min, EPSP_delay_max;
	float weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min;
	float conn_probability;

	this_layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_layer_num_to_connect)));	
	target_layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_layer_num_to_connect_to)));

	weight_excitatory_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_weight_excitatory_min)));
	weight_excitatory_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_weight_excitatory_max)));
	weight_inhibitory_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_weight_inhibitory_min)));
	weight_inhibitory_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_weight_inhibitory_max)));	

	EPSP_delay_min = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_excitatory_delay_min)));
	EPSP_delay_max= (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_excitatory_delay_max)));
	IPSP_delay_min = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_inhibitory_delay_min)));
	IPSP_delay_max = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_inhibitory_delay_max)));

	conn_probability = atof(gtk_entry_get_text(GTK_ENTRY(entry_connection_probability)));

	if (connect_layer_to_layer(this_layer, target_layer, weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min, EPSP_delay_min, EPSP_delay_max, IPSP_delay_min,  IPSP_delay_max, conn_probability))
	{
		printf ("Connection from layer %d to layer %d successful.\n", this_layer , target_layer);
		gtk_widget_set_sensitive(btn_reset_all_synapses_with_new_params, TRUE);
		gtk_widget_set_sensitive(btn_reset_all_neurons, TRUE);
		return;
	}
	else
	{	
		printf ("ERROR: Connection from layer %d to layer %d FAILED.\n", this_layer , target_layer);
	}
	return;
}

void start_stop_rt_simulation_button_func(void)
{
	if (rt_simulation_ongoing)
	{
		rt_simulation_ongoing = 0;
		gtk_button_set_label(GTK_BUTTON(btn_start_stop_rt_simulation), "Start RT Simulation");
		gtk_widget_set_sensitive(btn_reset_all_synapses_with_new_params, TRUE);
		gtk_widget_set_sensitive(btn_reset_all_neurons, TRUE);
		gtk_widget_set_sensitive(btn_self_clock, TRUE);
		gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, TRUE);
		EndTask = 1;
		rt_thread_join(NeuroSimThread);
		printf("Killed thread(NeuroSimThread) for NeuroSim\n");	
	}
	else
	{
		rt_simulation_ongoing =1;
		gtk_button_set_label(GTK_BUTTON(btn_start_stop_rt_simulation), "Stop RT Simulation");
		gtk_widget_set_sensitive(btn_reset_all_synapses_with_new_params, FALSE);
		gtk_widget_set_sensitive(btn_reset_all_neurons, FALSE);
		gtk_widget_set_sensitive(btn_start_stop_simulation, FALSE);
		gtk_widget_set_sensitive(btn_self_clock, FALSE);
		gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, FALSE);

		simulation_given_step_size_ns = (unsigned int) atof(gtk_entry_get_text(GTK_ENTRY(entry_simulation_given_step_size_ns)));	

		if (SelfClock)
		{
	        	start_rt_timer(0);
	        	rt_make_hard_real_time();
		}
		EndTask = 0;
		NeuroSimThread= rt_thread_create(NeuroSimHandler, NULL, 10000);
		printf("Created thread(NeuroSimThread) for NeuroSim\n");	
	}

	return;
}

void start_stop_simulation_button_func(void)
{
	unsigned int cpu_time_start, i_time = 0;
	double i;

	nonrt_simulation_duration_ms = atof(gtk_entry_get_text(GTK_ENTRY(entry_simulation_duration)));
	simulation_given_step_size_ns = (unsigned int) atof(gtk_entry_get_text(GTK_ENTRY(entry_simulation_given_step_size_ns)));
	if ( (simulation_given_step_size_ns <100000) || (simulation_given_step_size_ns> 1000000))
	{
		printf ("ERROR: Invalid step size submitted\n");
		printf("Killed thread(NeuroSimThread) for NeuroSim\n");	
		return;
	}

	gtk_widget_set_sensitive(btn_reset_all_synapses_with_new_params, FALSE);
	gtk_widget_set_sensitive(btn_reset_all_neurons, FALSE);
	gtk_widget_set_sensitive(btn_self_clock, FALSE);
	gtk_widget_set_sensitive(btn_start_stop_rt_simulation, FALSE);
	gtk_widget_set_sensitive(btn_start_stop_simulation, FALSE);
	gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, FALSE);
	if (neuron_dynamics_logging_on)
	{
		//clear_neuron_dynamics_log();
		if (((1000000.0/simulation_given_step_size_ns)*nonrt_simulation_duration_ms) > ((double)(neuron_dynamics_log_buffer_size)))
		{
			printf ("WARNING: Circular buffer for visualization will be overwritten\n");
			printf ("WARNING: Increase buffer size for visualization\n");
		}
	}
	non_rt_simulation_ongoing =1;

	cpu_time_start = (unsigned int)rt_get_cpu_time_ns();
	parker_sochacki_time_prev =0;
	for (i=0; i<nonrt_simulation_duration_ms; i+=(simulation_given_step_size_ns/1000000.0))
	{   
		i_time += simulation_given_step_size_ns;
		parker_sochacki_time_curr = i_time;
		simulate_parker_sochacki();
		parker_sochacki_time_prev = parker_sochacki_time_curr;
	}

	printf("Simulation lasted %u nanoseconds\n",  (((unsigned int)rt_get_cpu_time_ns()) - cpu_time_start)  );

	non_rt_simulation_ongoing = 0;

	gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, TRUE);
	gtk_widget_set_sensitive(btn_reset_all_synapses_with_new_params, TRUE);
	gtk_widget_set_sensitive(btn_reset_all_neurons, TRUE);
	gtk_widget_set_sensitive(btn_resume_nonrt_simulation, TRUE);

	return;
}

void resume_nonrt_simulation_button_func(void)
{
	unsigned int cpu_time_start, i_time;
	double i;

	gtk_widget_set_sensitive(btn_reset_all_synapses_with_new_params, FALSE);
	gtk_widget_set_sensitive(btn_reset_all_neurons, FALSE);
	gtk_widget_set_sensitive(btn_self_clock, FALSE);
	gtk_widget_set_sensitive(btn_start_stop_rt_simulation, FALSE);
	gtk_widget_set_sensitive(btn_start_stop_simulation, FALSE);
	gtk_widget_set_sensitive(btn_resume_nonrt_simulation, FALSE);
	gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, FALSE);

	if (neuron_dynamics_logging_on)
	{
		//clear_neuron_dynamics_log();
		if (((1000000.0/simulation_given_step_size_ns)*nonrt_simulation_duration_ms) > ((double)(neuron_dynamics_log_buffer_size)))
		{
			printf ("WARNING: Circular buffer for visualization will be overwritten\n");
			printf ("WARNING: Increase buffer size for visualization\n");
		}
	}
	non_rt_simulation_ongoing =1;

	cpu_time_start = (unsigned int)rt_get_cpu_time_ns();
	 i_time = parker_sochacki_time_prev;
	for (i=0; i<nonrt_simulation_duration_ms; i+=(simulation_given_step_size_ns/1000000.0))
	{   
		i_time += simulation_given_step_size_ns;
		parker_sochacki_time_curr = i_time;
		simulate_parker_sochacki();
		parker_sochacki_time_prev = parker_sochacki_time_curr;
	}

	printf("Simulation lasted %u nanoseconds\n",  (((unsigned int)rt_get_cpu_time_ns()) - cpu_time_start)  );

	non_rt_simulation_ongoing = 0;

	gtk_widget_set_sensitive(btn_submit_parker_sochacki_params, TRUE);
	gtk_widget_set_sensitive(btn_reset_all_synapses_with_new_params, TRUE);
	gtk_widget_set_sensitive(btn_reset_all_neurons, TRUE);
	gtk_widget_set_sensitive(btn_resume_nonrt_simulation, TRUE);
	return;
}

void self_clock_button_func(void)
{
	if (SelfClock)
	{
		SelfClock = 0;
		gtk_button_set_label(GTK_BUTTON(btn_self_clock), "Self Clock OFF");
	}
	else
	{
		SelfClock =1;
		gtk_button_set_label(GTK_BUTTON(btn_self_clock), "Self Clock ON");
	}

	return;
}

void reset_all_neurons_button_func(void)
{
	reset_all_neurons();
	return;
}

void reset_all_synapses_with_new_params_button_func(void)
{
	float weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min;
	weight_excitatory_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_weight_excitatory_min)));
	weight_excitatory_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_weight_excitatory_max)));
	weight_inhibitory_min = atof(gtk_entry_get_text(GTK_ENTRY(entry_weight_inhibitory_min)));
	weight_inhibitory_max = atof(gtk_entry_get_text(GTK_ENTRY(entry_weight_inhibitory_max)));
	if(reset_all_synapses (weight_excitatory_max, weight_excitatory_min, weight_inhibitory_max, weight_inhibitory_min))
		return;
}

void interrogate_neuron_button_func(void)
{
	int layer, neuron_group, neuron_num;
	GPtrArray *ptrArrayLayer,  *ptrArrayNeuronGroup;
	neuron *nrn;

 	layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_interrogate_layer_num)));	
	neuron_group = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_interrogate_neuron_group_num)));	
	neuron_num = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_interrogate_neuron_num)));	

	if (layer >= ptrArrayAllNetwork->len)
	{
		printf ("ERROR: Layer num is greater than network size\n");
		return;
	}
	ptrArrayLayer = g_ptr_array_index(ptrArrayAllNetwork, layer);
	if (neuron_group >= ptrArrayLayer->len)
	{
		printf ("ERROR: Neuron group num is too large for this layer\n");
		return;
	}	
	ptrArrayNeuronGroup = g_ptr_array_index(ptrArrayLayer, neuron_group);
	if (neuron_num >= ptrArrayNeuronGroup->len)
	{
		printf ("ERROR: Neuron num is too large for this neuron group\n");
		return;
	}
	nrn = g_ptr_array_index(ptrArrayNeuronGroup, neuron_num);
	if (nrn == NULL)
	{
		printf("No Neuron in Layer %d Neuron_Group%d Neuron_num %d\n", layer, neuron_group, neuron_num);
		return;
	}
	
	printf("Layer: %d\t Group: %d\t Neuron: %d\n", layer, neuron_group, neuron_num);
	printf("nrn->v: %f\n", nrn->v);
	printf("nrn->u: %f\n", nrn->u);
	printf("nrn->a: %f\n", nrn->a);
	printf("nrn->b: %f\n", nrn->b);
	printf("nrn->c: %f\n", nrn->c);
	printf("nrn->d: %f\n", nrn->d);
	printf("nrn->k: %f\n", nrn->k);
	printf("nrn->(1/C): %f\n", nrn->E);
	printf("nrn->I_inject: %f\n", nrn->I_inject);
	printf("nrn->inhibitory: %d\n", nrn->inhibitory);
	printf("nrn->v_threshold: %f\n", nrn->v_threshold);
	printf("nrn->v_peak: %f\n", nrn->v_peak);
	printf("nrn->v_resting: %f\n", nrn->v_resting);
	printf("nrn->k_v_threshold: %f\n", nrn->k_v_threshold);
	printf("nrn->conductance_excitatory: %f\n", nrn->conductance_excitatory);
	printf("nrn->conductance_inhibitory: %f\n", nrn->conductance_inhibitory);
	printf("nrn->E_excitatory: %f\n", nrn->E_excitatory);
	printf("nrn->E_inhibitory: %f\n", nrn->E_inhibitory);
	printf("nrn->decay_rate_excitatory: %f\n", nrn->decay_rate_excitatory);
	printf("nrn->decay_rate_inhibitory: %f\n", nrn->decay_rate_inhibitory);

	printf("self address: %u\n", (unsigned int)nrn);

		
	int i;
	for (i=0; i<nrn->num_of_output; i++)
	{
		printf("weight: %.3f\t axon_to: %u\t axonal_delay: %u\n", nrn->axonal_weight[i], (unsigned int)nrn->axon_to[i] , nrn->axonal_delay[i] );
	}
	return;
}
void interrogate_neuron_group_button_func(void)
{
	int layer, neuron_group;
	GPtrArray *ptrArrayLayer,  *ptrArrayNeuronGroup;

 	layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_interrogate_layer_num)));	
	neuron_group = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_interrogate_neuron_group_num)));	

	if (layer >= ptrArrayAllNetwork->len)
	{
		printf ("ERROR: Layer num is greater than network size\n");
		return;
	}
	ptrArrayLayer = g_ptr_array_index(ptrArrayAllNetwork, layer);
	if (neuron_group >= ptrArrayLayer->len)
	{
		printf ("ERROR: Neuron group num is too large for this layer\n");
		return;
	}	
	ptrArrayNeuronGroup = g_ptr_array_index(ptrArrayLayer, neuron_group);

	printf("Layer: %d\t Group: %d\n", layer, neuron_group);
	printf ("\nNum of neurons in group: %d\n\n", ptrArrayNeuronGroup->len);

	return;	
}
void interrogate_layer_button_func(void)
{
	int layer, i, j;
	GPtrArray *ptrArrayLayer;

 	layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_interrogate_layer_num)));	

	if (layer >= ptrArrayAllNetwork->len)
	{
		printf ("ERROR: Layer num is greater than network size\n");
		return;
	}
	ptrArrayLayer = g_ptr_array_index(ptrArrayAllNetwork, layer);

	printf("Layer: %d\n", layer);
	printf ("\nNum of groups in layer: %d\n\n", ptrArrayLayer->len);

	printf("Layer Connection Matrix is (Rows connected to Columns):\n");
	for (i=0; i<ptrArrayAllNetwork->len;i++)
	{
		for (j=0; j<ptrArrayAllNetwork->len;j++)
		{		
			printf("%d\t", LayerConnectionsMatrix[i][j]);
		}
		printf("\n");
	}
	return;
}

void submit_parker_sochacki_params_button_func(void)
{
	int ps_order, nr_iter;
	double ps_error_tolerance, nr_error_tolerance;

	ps_order = (int)(atof(gtk_entry_get_text(GTK_ENTRY(entry_parker_sochacki_max_order))));
	ps_error_tolerance = (double)atof(gtk_entry_get_text(GTK_ENTRY(entry_parker_sochacki_error_tolerance)));

	nr_iter = (int)(atof(gtk_entry_get_text(GTK_ENTRY(entry_newton_raphson_max_iter))));
	nr_error_tolerance = (double)atof(gtk_entry_get_text(GTK_ENTRY(entry_newton_raphson_error_tolerance)));

	if (  (ps_order <=0) ||  (ps_error_tolerance < 1e-16) ||  (nr_iter <=0) || (nr_error_tolerance< 1e-16) )
	{
		printf ("ERROR: Invalid Parker- Sochacki parameters submitted\n");
		if  ((ps_error_tolerance < 1e-16) || (nr_error_tolerance< 1e-16))
			 printf ("ERROR: No need for error tolerance be so smaller than DBL_EPSILON = %E\n",  DBL_EPSILON);
		return;
	}

	parker_sochacki_set_order_tolerance(ps_order, ps_error_tolerance, nr_iter, nr_error_tolerance);

	printf ("Current Parker-Sochacki Max Order: %d\n", parker_sochacki_max_order);
	printf ("Current Parker-Sochacki Error Tolerance: %E\n", parker_sochacki_error_tolerance);
	printf ("Current Newton-Raphson Max Number of Iteration: %d\n", newton_raphson_max_iter);
	printf ("Current Newton-Raphson Error Tolerance: %E\n", newton_raphson_error_tolerance);
	gtk_widget_set_sensitive(btn_start_stop_simulation, TRUE);
	gtk_widget_set_sensitive(btn_self_clock, TRUE);
	gtk_widget_set_sensitive(btn_start_stop_rt_simulation, TRUE);	
	return;
}

void log_neuron_dynamics_on_off_button_func(void)
{
	if (neuron_dynamics_logging_on)
	{
		neuron_dynamics_logging_on = 0;
		gtk_button_set_label(GTK_BUTTON(btn_log_neuron_dynamics_on_off), "Neuron Dynamics Logging 'OFF' ");
	}
	else
	{
		neuron_dynamics_logging_on = 1;
		gtk_button_set_label(GTK_BUTTON(btn_log_neuron_dynamics_on_off), "Neuron Dynamics Logging 'ON' ");
	}
	return;
}

void submit_logging_buffer_size_button_func(void)
{	
	set_buffer_size_for_neuron_dynamics_logging( (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_buffer_size_for_dynamics_log)))  );
	turn_neuron_dynamics_logging_on();

	gtk_widget_set_sensitive(btn_submit_neuron_dynamics_logging_buffer_size, FALSE);	
	gtk_entry_set_editable(GTK_ENTRY(entry_buffer_size_for_dynamics_log), FALSE);
	gtk_widget_set_sensitive(btn_log_neuron_dynamics_on_off, TRUE);	
	gtk_button_set_label(GTK_BUTTON(btn_log_neuron_dynamics_on_off), "Neuron Dynamics Logging 'ON' ");

	return;
}

void reset_neurons_in_group_button_func(void)
{
	int randomize;
	double v, a, b, c, d, I_inject, C, E_excitatory, E_inhibitory, v_resting, v_threshold, v_peak, k, tau_excitatory, tau_inhibitory;
	bool inhibitory;
	int nrn_grp, layer;
	nrn_grp = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_interrogate_neuron_group_num)));
	layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_interrogate_layer_num)));

	if (is_neuron_allocated(layer, nrn_grp, 0) == NULL)
		return;

	v = atof(gtk_entry_get_text(GTK_ENTRY(entry_v)));
	a = atof(gtk_entry_get_text(GTK_ENTRY(entry_a)));
	b = atof(gtk_entry_get_text(GTK_ENTRY(entry_b)));
	c = atof(gtk_entry_get_text(GTK_ENTRY(entry_c)));
	d = atof(gtk_entry_get_text(GTK_ENTRY(entry_d)));
	I_inject = atof(gtk_entry_get_text(GTK_ENTRY(entry_I_inject)));
	C = atof(gtk_entry_get_text(GTK_ENTRY(entry_C)));
	E_excitatory = atof(gtk_entry_get_text(GTK_ENTRY(entry_E_excitatory)));
	E_inhibitory = atof(gtk_entry_get_text(GTK_ENTRY(entry_E_inhibitory)));
	v_resting = atof(gtk_entry_get_text(GTK_ENTRY(entry_v_resting)));
	v_threshold = atof(gtk_entry_get_text(GTK_ENTRY(entry_v_threshold)));
	v_peak = atof(gtk_entry_get_text(GTK_ENTRY(entry_v_peak)));
	k = atof(gtk_entry_get_text(GTK_ENTRY(entry_k)));
	tau_excitatory = atof(gtk_entry_get_text(GTK_ENTRY(entry_excitatory_tau)));
	tau_inhibitory = atof(gtk_entry_get_text(GTK_ENTRY(entry_inhibitory_tau)));
	randomize = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_randomize_params)));
	inhibitory = (bool)atof(gtk_entry_get_text(GTK_ENTRY(entry_inhibitory)));

	reset_neurons_in_group(nrn_grp, layer, v, a, b, c, d, I_inject, inhibitory, randomize, C, E_excitatory, E_inhibitory, v_resting, v_threshold, v_peak, k, tau_excitatory, tau_inhibitory);

	return;
}

void reset_neuron_button_func(void)
{
	int randomize;
	double v, a, b, c, d, I_inject, C, E_excitatory, E_inhibitory, v_resting, v_threshold, v_peak, k, tau_excitatory, tau_inhibitory;
	bool inhibitory;
	int nrn_grp, layer, nrn_num;
	
	nrn_num = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_interrogate_neuron_num)));
	nrn_grp = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_interrogate_neuron_group_num)));
	layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_interrogate_layer_num)));

	if (is_neuron_allocated(layer, nrn_grp, nrn_num ) == NULL)
		return;

	v = atof(gtk_entry_get_text(GTK_ENTRY(entry_v)));
	a = atof(gtk_entry_get_text(GTK_ENTRY(entry_a)));
	b = atof(gtk_entry_get_text(GTK_ENTRY(entry_b)));
	c = atof(gtk_entry_get_text(GTK_ENTRY(entry_c)));
	d = atof(gtk_entry_get_text(GTK_ENTRY(entry_d)));
	I_inject = atof(gtk_entry_get_text(GTK_ENTRY(entry_I_inject)));
	C = atof(gtk_entry_get_text(GTK_ENTRY(entry_C)));
	E_excitatory = atof(gtk_entry_get_text(GTK_ENTRY(entry_E_excitatory)));
	E_inhibitory = atof(gtk_entry_get_text(GTK_ENTRY(entry_E_inhibitory)));
	v_resting = atof(gtk_entry_get_text(GTK_ENTRY(entry_v_resting)));
	v_threshold = atof(gtk_entry_get_text(GTK_ENTRY(entry_v_threshold)));
	v_peak = atof(gtk_entry_get_text(GTK_ENTRY(entry_v_peak)));
	k = atof(gtk_entry_get_text(GTK_ENTRY(entry_k)));
	tau_excitatory = atof(gtk_entry_get_text(GTK_ENTRY(entry_excitatory_tau)));
	tau_inhibitory = atof(gtk_entry_get_text(GTK_ENTRY(entry_inhibitory_tau)));
	randomize = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_randomize_params)));
	inhibitory = (bool)atof(gtk_entry_get_text(GTK_ENTRY(entry_inhibitory)));

	reset_neuron(nrn_num,nrn_grp, layer, v, a, b, c, d, I_inject, inhibitory, randomize, C, E_excitatory, E_inhibitory, v_resting, v_threshold, v_peak, k, tau_excitatory, tau_inhibitory);

	return;
}


void destroy_all_connections_btwn_layers_button_func(void)
{
	return;
}

void clear_parker_sochacki_polynomials_button_func(void)
{
}

void submit_number_of_frames_for_neuron_dynamics_button_func(void)
{
	allocate_frames_and_graphs_for_neuron_dynamics((int)atof(gtk_entry_get_text(GTK_ENTRY(entry_number_of_frames_for_neuron_dynamics))));
	gtk_widget_set_sensitive(btn_submit_number_of_frames_for_neuron_dynamics, FALSE);
	gtk_widget_set_sensitive(btn_add_nrn_grp_to_layer, FALSE);
}

void combo_neuron_type_func(void)
{	
	int idx;
	idx=gtk_combo_box_get_active (GTK_COMBO_BOX(combo_neuron_type));
	if (idx == 0)  /// regular spiking
	{
		gtk_entry_set_text(GTK_ENTRY(entry_a), "0.03");
		gtk_entry_set_text(GTK_ENTRY(entry_b), "-2");
		gtk_entry_set_text(GTK_ENTRY(entry_c), "-50");
		gtk_entry_set_text(GTK_ENTRY(entry_d), "100");
		gtk_entry_set_text(GTK_ENTRY(entry_C), "100");
		gtk_entry_set_text(GTK_ENTRY(entry_k), "0.7");
		gtk_entry_set_text(GTK_ENTRY(entry_v_resting), "-60");
		gtk_entry_set_text(GTK_ENTRY(entry_v_threshold), "-40");
		gtk_entry_set_text(GTK_ENTRY(entry_v_peak), "35");
		gtk_entry_set_text(GTK_ENTRY(entry_inhibitory), "0");
		gtk_entry_set_text(GTK_ENTRY(entry_I_inject), "85");
	}
	else if (idx == 1)  /// intrisically bursting
	{
		gtk_entry_set_text(GTK_ENTRY(entry_a), "0.01");
		gtk_entry_set_text(GTK_ENTRY(entry_b), "5");
		gtk_entry_set_text(GTK_ENTRY(entry_c), "-56");
		gtk_entry_set_text(GTK_ENTRY(entry_d), "130");
		gtk_entry_set_text(GTK_ENTRY(entry_C), "150");
		gtk_entry_set_text(GTK_ENTRY(entry_k), "1.2");
		gtk_entry_set_text(GTK_ENTRY(entry_v_resting), "-75");
		gtk_entry_set_text(GTK_ENTRY(entry_v_threshold), "-45");
		gtk_entry_set_text(GTK_ENTRY(entry_v_peak), "50");
		gtk_entry_set_text(GTK_ENTRY(entry_inhibitory), "0");
		gtk_entry_set_text(GTK_ENTRY(entry_I_inject), "500");
	}
	else if (idx == 2)  /// chattering
	{
		gtk_entry_set_text(GTK_ENTRY(entry_a), "0.03");
		gtk_entry_set_text(GTK_ENTRY(entry_b), "1");
		gtk_entry_set_text(GTK_ENTRY(entry_c), "-40");
		gtk_entry_set_text(GTK_ENTRY(entry_d), "150");
		gtk_entry_set_text(GTK_ENTRY(entry_C), "50");
		gtk_entry_set_text(GTK_ENTRY(entry_k), "1.5");
		gtk_entry_set_text(GTK_ENTRY(entry_v_resting), "-60");
		gtk_entry_set_text(GTK_ENTRY(entry_v_threshold), "-40");
		gtk_entry_set_text(GTK_ENTRY(entry_v_peak), "25");
		gtk_entry_set_text(GTK_ENTRY(entry_inhibitory), "0");
		gtk_entry_set_text(GTK_ENTRY(entry_I_inject), "400");
	}
	else if (idx == 3)  /// low-threshold
	{
		gtk_entry_set_text(GTK_ENTRY(entry_a), "0.03");
		gtk_entry_set_text(GTK_ENTRY(entry_b), "8");
		gtk_entry_set_text(GTK_ENTRY(entry_c), "-53");
		gtk_entry_set_text(GTK_ENTRY(entry_d), "130");
		gtk_entry_set_text(GTK_ENTRY(entry_C), "150");
		gtk_entry_set_text(GTK_ENTRY(entry_k), "1.2");
		gtk_entry_set_text(GTK_ENTRY(entry_v_resting), "-75");
		gtk_entry_set_text(GTK_ENTRY(entry_v_threshold), "-45");
		gtk_entry_set_text(GTK_ENTRY(entry_v_peak), "50");
		gtk_entry_set_text(GTK_ENTRY(entry_inhibitory), "0");
		gtk_entry_set_text(GTK_ENTRY(entry_I_inject), "500");
	}
	else if (idx == 4)  /// fast-spiking
	{
		gtk_entry_set_text(GTK_ENTRY(entry_a), "0.03");
		gtk_entry_set_text(GTK_ENTRY(entry_b), "1");
		gtk_entry_set_text(GTK_ENTRY(entry_c), "-40");
		gtk_entry_set_text(GTK_ENTRY(entry_d), "150");
		gtk_entry_set_text(GTK_ENTRY(entry_C), "50");
		gtk_entry_set_text(GTK_ENTRY(entry_k), "1.5");
		gtk_entry_set_text(GTK_ENTRY(entry_v_resting), "-60");
		gtk_entry_set_text(GTK_ENTRY(entry_v_threshold), "-40");
		gtk_entry_set_text(GTK_ENTRY(entry_v_peak), "25");
		gtk_entry_set_text(GTK_ENTRY(entry_inhibitory), "0");
		gtk_entry_set_text(GTK_ENTRY(entry_I_inject), "400");
	}

}

