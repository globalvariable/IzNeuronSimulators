#include "BlueSpikeSpikePatternView.h"

static NetworkSpikePatternGraphScroll *blue_spike_spike_pattern_graph = NULL;
static GtkWidget *btn_pause;

static void pause_button_func(void);

bool create_blue_spike_spike_pattern_view_gui(void)
{
	GtkWidget *frame, *frame_label, *vbox, *hbox;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     BlueSpike Spikes     ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (get_gui_tabs()), frame, frame_label);  

 	hbox = gtk_hbox_new(TRUE, 0);
        gtk_container_add (GTK_CONTAINER (frame), hbox);

	vbox = gtk_vbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(hbox),vbox, TRUE,TRUE,0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, TRUE,TRUE,0);

	btn_pause = gtk_button_new_with_label("P");
	gtk_box_pack_start (GTK_BOX (hbox), btn_pause, FALSE, FALSE, 0);
	blue_spike_spike_pattern_graph = allocate_network_spike_pattern_graph_scroll(bmi_data->blue_spike_network, hbox, blue_spike_spike_pattern_graph, 3000, 1000000, 900, 1000, BUFFER_FOLLOWUP_LATENCY, bmi_data->blue_spike_spike_data_for_graph,  NUM_OF_STATUS_MARKERS, bmi_data->trial_status_events, 1);    // 3 seconds, 1000 samples/sec, 100 ms latency


	g_signal_connect(G_OBJECT(btn_pause), "clicked", G_CALLBACK(pause_button_func), NULL);

	gtk_widget_show_all(get_gui_tabs());

	return TRUE;
}

static void pause_button_func(void)
{
	if (blue_spike_spike_pattern_graph->locally_paused)
	{
		if (send_blue_spike_spike_graph_resume_request_to_buffer_view_handler())   // should resume all graphs at the same time to provide sync
			gtk_button_set_label (GTK_BUTTON(btn_pause),"R");  
	}
	else
	{
		if (is_buffer_view_handler_paused())
			blue_spike_spike_pattern_graph->locally_paused = TRUE;
		else
			blue_spike_spike_pattern_graph->local_pause_request = TRUE;
		gtk_button_set_label (GTK_BUTTON(btn_pause),"P");		
	}  
}

NetworkSpikePatternGraphScroll* get_blue_spike_spike_pattern_graph_ptr(void)
{
	return blue_spike_spike_pattern_graph;
}

