#include "SpikePatternView.h"

static GtkWidget *btn_pause;
static NetworkSpikePatternGraph *network_spike_pattern_graph = NULL;
static bool paused = TRUE;
static bool slide_request = FALSE;

static unsigned int blue_spike_spike_buffer_read_idx;
static unsigned int blue_spike_spike_buffer_size;
static SpikeTimeStamp *blue_spike_handling_buffer;
static SpikeTimeStampItem *blue_spike_handling_buffer_buff;

static unsigned int graph_spike_buffer_read_idx;
static unsigned int graph_spike_buffer_size;
static SpikeData *graph_spike_handling_buffer;
static SpikeTimeStampItem *graph_spike_handling_buffer_buff;

static SpikeGenData *spike_gen_data;
static NeuronSpikePatternGraph	***neuron_graphs;
static TimeStamp sampling_interval;
static unsigned int data_point_placement_start_idx;


static void pause_button_func (void);
static gboolean timeout_callback(gpointer user_data) ;


bool create_spike_pattern_view_gui(void)
{
	GtkWidget *frame, *frame_label, *vbox, *hbox;
	spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();
        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Spike Patterns     ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (get_gui_tabs()), frame, frame_label);  

 	hbox = gtk_hbox_new(TRUE, 0);
        gtk_container_add (GTK_CONTAINER (frame), hbox);

	vbox = gtk_vbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(hbox),vbox, TRUE,TRUE,0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, TRUE,TRUE,0);

	network_spike_pattern_graph = allocate_network_spike_pattern_graph(spike_gen_data->network, hbox, network_spike_pattern_graph, 3000, 1000000, 900, 1000, 100000000);    // 3 seconds, 1000 samples/sec, 100 ms latency
	btn_pause = gtk_button_new_with_label("P");
	gtk_box_pack_start (GTK_BOX (hbox), btn_pause, FALSE, FALSE, 0);

	g_signal_connect(G_OBJECT(btn_pause), "clicked", G_CALLBACK(pause_button_func), NULL);

	gtk_widget_show_all(get_gui_tabs());

	blue_spike_spike_buffer_size = SPIKE_TIME_STAMP_BUFF_SIZE;
	blue_spike_handling_buffer = &(shared_memory->spike_time_stamp);
	blue_spike_handling_buffer_buff = blue_spike_handling_buffer->spike_time_stamp_buff;

	graph_spike_buffer_size = network_spike_pattern_graph->spike_handling_buffer->buffer_size;
	graph_spike_handling_buffer = network_spike_pattern_graph->spike_handling_buffer;
	graph_spike_handling_buffer_buff = graph_spike_handling_buffer->buff;

	network_spike_pattern_graph->buffering_start_time = shared_memory->rt_tasks_data.current_system_time;
	blue_spike_spike_buffer_read_idx = shared_memory->spike_time_stamp.buff_idx_write;
	graph_spike_buffer_read_idx = network_spike_pattern_graph->spike_handling_buffer->buff_idx_write;

	neuron_graphs = network_spike_pattern_graph->neuron_graphs;
	sampling_interval = network_spike_pattern_graph->sampling_interval;
	data_point_placement_start_idx = network_spike_pattern_graph->num_of_data_points - network_spike_pattern_graph->num_of_data_points_to_slide;

	paused = TRUE;

	g_timeout_add(500, timeout_callback, NULL);		// timeout shoud be less than sliding length/2

	return TRUE;
}

static gboolean timeout_callback(gpointer user_data) 
{
	TimeStamp 				current_time;
	TimeStamp 				buffering_start_time;
	TimeStamp 				buffering_end_time;
	unsigned int				blue_spike_buff_idx_write;
	unsigned int 				spike_handling_buff_idx_write;
	TimeStamp 				spike_time;
	SpikeTimeStampItem		*spike_item;
	
	if (slide_request)
	{
		slide_network_spike_pattern_graph(spike_gen_data->network, network_spike_pattern_graph);
		slide_request = FALSE;
	}	
	if (!paused)
	{
		current_time = shared_memory->rt_tasks_data.current_system_time;
		blue_spike_buff_idx_write = shared_memory->spike_time_stamp.buff_idx_write;
		spike_handling_buff_idx_write = network_spike_pattern_graph->spike_handling_buffer->buff_idx_write;
		buffering_start_time = network_spike_pattern_graph->buffering_start_time;
		buffering_end_time = buffering_start_time + network_spike_pattern_graph->graph_len_to_slide;
		if (current_time > (buffering_end_time + network_spike_pattern_graph->spike_buffer_followup_latency) )
		{
			while (graph_spike_buffer_read_idx !=  spike_handling_buff_idx_write) 	// first handle graph' s spike buffer
			{
				spike_item = &(graph_spike_handling_buffer_buff[graph_spike_buffer_read_idx]);
				spike_time = spike_item->peak_time;
				if (spike_time < buffering_end_time)
					neuron_graphs[spike_item->mwa_or_layer][spike_item->channel_or_neuron_group][spike_item->unit_or_neuron].y[((spike_time - buffering_start_time) / sampling_interval) + data_point_placement_start_idx] = 1;
				else
					write_to_spike_data(graph_spike_handling_buffer, spike_item->mwa_or_layer, spike_item->channel_or_neuron_group, spike_item->unit_or_neuron, spike_time);
				if (spike_time < buffering_start_time)
					return print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "SpikePatternView", "gboolean timeout_callback", "graph_buffer_spike_time < buffering_start_time");	
				graph_spike_buffer_read_idx++;
				if (graph_spike_buffer_read_idx == graph_spike_buffer_size)
					graph_spike_buffer_read_idx = 0;
			}
			while (blue_spike_spike_buffer_read_idx !=  blue_spike_buff_idx_write) 	// second handle blue spike buffer
			{
				spike_item = &(blue_spike_handling_buffer_buff[blue_spike_spike_buffer_read_idx]);
				spike_time = spike_item->peak_time;
				if (spike_time < buffering_end_time)
					neuron_graphs[spike_item->mwa_or_layer][spike_item->channel_or_neuron_group][spike_item->unit_or_neuron].y[((spike_time - buffering_start_time) / sampling_interval) + data_point_placement_start_idx] = 1;
				else
					write_to_spike_data(graph_spike_handling_buffer, spike_item->mwa_or_layer, spike_item->channel_or_neuron_group, spike_item->unit_or_neuron, spike_time);
				if (spike_time < buffering_start_time)
					return print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "SpikePatternView", "gboolean timeout_callback", "blue_spike_spike_time < buffering_start_time");	
				blue_spike_spike_buffer_read_idx++;
				if (blue_spike_spike_buffer_read_idx == blue_spike_spike_buffer_size)
					blue_spike_spike_buffer_read_idx = 0;
			}
			set_total_limits_network_spike_pattern_graph(spike_gen_data->network, network_spike_pattern_graph);
			slide_request = TRUE;
			network_spike_pattern_graph->buffering_start_time = buffering_end_time; // prepare for next acquisition
		}
		else		// do not plot if current time is not larger than buffering_end_time. Only handle buffers.
		{
			while (graph_spike_buffer_read_idx !=  spike_handling_buff_idx_write) 	// first handle graph' s spike buffer
			{
				spike_item = &(graph_spike_handling_buffer_buff[graph_spike_buffer_read_idx]);
				spike_time = spike_item->peak_time;
				if (spike_time < buffering_end_time)
					neuron_graphs[spike_item->mwa_or_layer][spike_item->channel_or_neuron_group][spike_item->unit_or_neuron].y[((spike_time - buffering_start_time) / sampling_interval) + data_point_placement_start_idx] = 1;
				else
					write_to_spike_data(graph_spike_handling_buffer, spike_item->mwa_or_layer, spike_item->channel_or_neuron_group, spike_item->unit_or_neuron, spike_time);
				if (spike_time < buffering_start_time)
					return print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "SpikePatternView", "gboolean timeout_callback", "graph_buffer_spike_time < buffering_start_time");	
				graph_spike_buffer_read_idx++;
				if (graph_spike_buffer_read_idx == graph_spike_buffer_size)
					graph_spike_buffer_read_idx = 0;
			}
			while (blue_spike_spike_buffer_read_idx !=  blue_spike_buff_idx_write) 	// second handle blue spike buffer
			{
				spike_item = &(blue_spike_handling_buffer_buff[blue_spike_spike_buffer_read_idx]);
				spike_time = spike_item->peak_time;
				if (spike_time < buffering_end_time)
					neuron_graphs[spike_item->mwa_or_layer][spike_item->channel_or_neuron_group][spike_item->unit_or_neuron].y[((spike_time - buffering_start_time) / sampling_interval) + data_point_placement_start_idx] = 1;
				else
					write_to_spike_data(graph_spike_handling_buffer, spike_item->mwa_or_layer, spike_item->channel_or_neuron_group, spike_item->unit_or_neuron, spike_time);
				if (blue_spike_handling_buffer_buff[blue_spike_spike_buffer_read_idx].peak_time < buffering_start_time)	
					return print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "SpikePatternView", "gboolean timeout_callback", "blue_spike_spike_time < buffering_start_time");	
				blue_spike_spike_buffer_read_idx++;
				if (blue_spike_spike_buffer_read_idx == blue_spike_spike_buffer_size)
					blue_spike_spike_buffer_read_idx = 0;		
			}
		}
	}
	return TRUE;
}

static void pause_button_func (void)
{
	if (paused)
	{
		network_spike_pattern_graph->buffering_start_time = shared_memory->rt_tasks_data.current_system_time;
		blue_spike_spike_buffer_read_idx = shared_memory->spike_time_stamp.buff_idx_write;
		graph_spike_buffer_read_idx = network_spike_pattern_graph->spike_handling_buffer->buff_idx_write;
		slide_network_spike_pattern_graph(spike_gen_data->network, network_spike_pattern_graph);
		paused = FALSE;
		gtk_button_set_label (GTK_BUTTON(btn_pause),"R");
	}
	else
	{
		paused = TRUE;
		gtk_button_set_label (GTK_BUTTON(btn_pause),"P");		
	}
	return;
}
