#include "InSilicoSpikePatternView.h"




static NetworkSpikePatternGraphScroll *in_silico_spike_pattern_graph = NULL;
static GtkWidget *btn_pause;

static void pause_button_func(void);


bool create_in_silico_spike_pattern_view_gui(void)
{
	GtkWidget *frame, *frame_label, *vbox, *hbox;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     InSilico Spikes     ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (get_gui_tabs()), frame, frame_label);  

 	hbox = gtk_hbox_new(TRUE, 0);
        gtk_container_add (GTK_CONTAINER (frame), hbox);

	vbox = gtk_vbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(hbox),vbox, TRUE,TRUE,0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, TRUE,TRUE,0);

	btn_pause = gtk_button_new_with_label("P");
	gtk_box_pack_start (GTK_BOX (hbox), btn_pause, FALSE, FALSE, 0);
	in_silico_spike_pattern_graph = allocate_network_spike_pattern_graph_scroll(bmi_data->in_silico_network, hbox, in_silico_spike_pattern_graph, 3000, 1000000, 900, 1000, BUFFER_FOLLOWUP_LATENCY, bmi_data->in_silico_spike_data_for_graph,  NUM_OF_STATUS_MARKERS, bmi_data->trial_status_events, bmi_data->num_of_dedicated_cpu_threads);    // 3 seconds, 1000 samples/sec, 100 ms latency
	if (in_silico_spike_pattern_graph == NULL)
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "InSilicoSpikePatternView", "create_in_silico_spike_pattern_view_gui", "allocate_network_spike_pattern_graph_scroll_exclude_poisson()."); 		

	g_signal_connect(G_OBJECT(btn_pause), "clicked", G_CALLBACK(pause_button_func), NULL);

	gtk_widget_show_all(get_gui_tabs());

	return TRUE;
}

static void pause_button_func(void)
{
	if (in_silico_spike_pattern_graph->locally_paused)
	{
		if (send_in_silico_spike_graph_resume_request_to_buffer_view_handler())   // should resume all graphs at the same time to provide sync
			gtk_button_set_label (GTK_BUTTON(btn_pause),"R");  
	}
	else
	{
		if (is_buffer_view_handler_paused())
			in_silico_spike_pattern_graph->locally_paused = TRUE;
		else
			in_silico_spike_pattern_graph->local_pause_request = TRUE;
		gtk_button_set_label (GTK_BUTTON(btn_pause),"P");		
	}
}

NetworkSpikePatternGraphScroll* get_in_silico_spike_pattern_graph_ptr(void)
{
	return in_silico_spike_pattern_graph;
}
