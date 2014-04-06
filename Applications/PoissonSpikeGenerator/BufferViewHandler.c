#include "BufferViewHandler.h"

static RtTasksData *rt_tasks_data = NULL;
static Network* network;

static NetworkSpikePatternGraphScroll *spike_pattern_graph = NULL;

static bool buffer_visualization_global_pause_request = TRUE;
static bool buffer_visualization_global_resume_request = FALSE;

static bool spike_pattern_graph_visualization_resume_request = FALSE;

static bool buffer_view_handler_paused_global = FALSE;


static gboolean timeout_callback(gpointer user_data) ;


bool buffer_view_handler(void)
{
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	rt_tasks_data = spike_gen_data->rt_tasks_data;
	network = spike_gen_data->network;

	if (!create_spike_pattern_buffer_view_gui())
		return  print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BufferViewHandler", "buffer_view_handler","! create_spike_pattern_buffer_view_gui().");

	spike_pattern_graph = get_generated_spike_pattern_graph_ptr();

	g_timeout_add(50, timeout_callback, NULL);		// timeout shoud be less than buffer_followup_latency,

	return TRUE;
}

static gboolean timeout_callback(gpointer user_data) 
{
	TimeStamp current_system_time;

	current_system_time = (rt_tasks_data->current_periodic_system_time/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE) *PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
	if (buffer_visualization_global_pause_request)
	{
		buffer_view_handler_paused_global = TRUE;
		buffer_visualization_global_pause_request = FALSE;
		spike_pattern_graph->global_pause_request = TRUE;
	}
	if  (buffer_visualization_global_resume_request)	// shoud determine start_idx for neuron dynamics graphs and spike pattern graphs for every request to sync them all. 
	{
		buffer_view_handler_paused_global = FALSE;
		buffer_visualization_global_resume_request = FALSE;

		if (! determine_spike_pattern_graph_scroll_start_time_and_read_indexes(spike_pattern_graph, current_system_time))
			return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BufferViewHandler", "gboolean timeout_callback","! determine_spike_pattern_graph_scroll_start_time().");	
		if (!spike_pattern_graph->locally_paused)
			clear_network_spike_pattern_graph_w_scroll(network, spike_pattern_graph);

		spike_pattern_graph->globally_paused = FALSE;
		spike_pattern_graph->global_pause_request = FALSE;
		if (spike_pattern_graph_visualization_resume_request)
		{
			spike_pattern_graph->locally_paused = FALSE;
			spike_pattern_graph_visualization_resume_request = FALSE;
			clear_network_spike_pattern_graph_w_scroll(network, spike_pattern_graph);
		}
	}

	if (! handle_spike_pattern_graph_scrolling_and_plotting(spike_pattern_graph, network, current_system_time))
		return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BufferViewHandler", "gboolean timeout_callback","! handle_spike_pattern_graph_scrolling_and_plotting().");	

	
	return TRUE;
}


bool send_spike_pattern_graph_resume_request_to_buffer_view_handler(void)
{
	if (! buffer_view_handler_paused_global)
	{
		spike_pattern_graph_visualization_resume_request  = TRUE;
		buffer_visualization_global_resume_request = TRUE;
		return TRUE;
	}
	else
		return FALSE;
}

bool is_buffer_view_handler_paused(void)
{
	return buffer_view_handler_paused_global;
}

void send_global_pause_request_to_buffer_view_handler(void)
{
	buffer_visualization_global_pause_request = TRUE;
}

void send_global_resume_request_to_buffer_view_handler(void)
{
	buffer_visualization_global_resume_request = TRUE;
}
