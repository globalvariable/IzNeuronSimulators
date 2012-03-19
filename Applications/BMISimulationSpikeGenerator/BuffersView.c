#include "BuffersView.h"

static GtkWidget *btn_start_spike_generation;
static GtkWidget *btn_pause_spike_generation;
static GtkWidget *btn_stop_spike_generation;
static GtkWidget *btn_enable_current_buffer_visualization;
static GtkWidget *btn_enable_neuron_dynamics_buffer_visualization;


static CurrentPatternGraph *current_pattern_buffer_graph = NULL;

bool create_buffers_view_gui(void)
{
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();
	GtkWidget *frame, *frame_label, *table, *vbox, *hbox, *lbl;

        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Buffers     ");      
   
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

	lbl = gtk_label_new("--- Spike Generator ---");
        gtk_box_pack_start(GTK_BOX(hbox),lbl,TRUE,TRUE,0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_start_spike_generation = gtk_button_new_with_label("Start");
	gtk_box_pack_start (GTK_BOX (hbox), btn_start_spike_generation, TRUE, TRUE, 0);
	btn_pause_spike_generation = gtk_button_new_with_label("Pause");
	gtk_box_pack_start (GTK_BOX (hbox), btn_pause_spike_generation, TRUE, TRUE, 0);
	btn_stop_spike_generation = gtk_button_new_with_label("Stop");
	gtk_box_pack_start (GTK_BOX (hbox), btn_stop_spike_generation, TRUE, TRUE, 0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("--- Visualizations ---");
        gtk_box_pack_start(GTK_BOX(hbox),lbl,TRUE,TRUE,0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_enable_current_buffer_visualization = gtk_button_new_with_label("InjectionCurrent: Enabled");
	gtk_box_pack_start (GTK_BOX (hbox), btn_enable_current_buffer_visualization, TRUE, TRUE, 0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_enable_neuron_dynamics_buffer_visualization = gtk_button_new_with_label("Neuron Dynamics: Enabled ");
	gtk_box_pack_start (GTK_BOX (hbox), btn_enable_neuron_dynamics_buffer_visualization, TRUE, TRUE, 0);

///////////////////////////////////////////// GRAPHS  ///////////////////////////////////////////////////////////////
 	hbox = gtk_hbox_new(TRUE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 1, 8, 0, 3);



	spike_gen_data->current_pattern_buffer = allocate_current_pattern_buffer(spike_gen_data->network, spike_gen_data->current_pattern_buffer, 2000000000/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE); // 2 second buffer
	current_pattern_buffer_graph = allocate_current_pattern_graph(hbox, current_pattern_buffer_graph, 1000000000/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);

 	hbox = gtk_hbox_new(TRUE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 1, 8, 3, 6);

	gtk_widget_show_all(get_gui_tabs());

	return TRUE;


}
