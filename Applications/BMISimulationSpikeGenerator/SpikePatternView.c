#include "SpikePatternView.h"

static GtkWidget *btn_pause;
static NetworkSpikePatternGraph *network_spike_pattern_graph = NULL;
static bool paused = TRUE;
static gboolean timeout_callback(gpointer user_data) ;


bool create_spike_pattern_view_gui(void)
{
	GtkWidget *frame, *frame_label, *table, *vbox, *hbox;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();
        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Spike Patterns     ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (get_gui_tabs()), frame, frame_label);  

 	hbox = gtk_hbox_new(TRUE, 0);
        gtk_container_add (GTK_CONTAINER (frame), hbox);

	vbox = gtk_vbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(hbox),vbox, TRUE,TRUE,0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, TRUE,TRUE,0);

	network_spike_pattern_graph = allocate_network_spike_pattern_graph(spike_gen_data->network, hbox, network_spike_pattern_graph, 3000, 1000000, 900);    // 3 seconds, 1000 samples/sec

	btn_pause = gtk_button_new_with_label("R");
	gtk_box_pack_start (GTK_BOX (hbox), btn_pause, FALSE, FALSE, 0);

	gtk_widget_show_all(get_gui_tabs());

	g_timeout_add(100, timeout_callback, NULL);

	return TRUE;
}

static gboolean timeout_callback(gpointer user_data) 
{
	TimeStamp current_time = shared_memory->rt_tasks_data.current_system_time;

	


	return TRUE;
}
