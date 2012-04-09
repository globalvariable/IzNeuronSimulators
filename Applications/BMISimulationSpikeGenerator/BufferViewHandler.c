#include "BufferViewHandler.h"

static Network* network;
static CurrentPatternBuffer* current_pattern_buffer;
static NeuronDynamicsBuffer *neuron_dynamics_buffer;

static CurrentPatternGraphScroll *current_pattern_graph = NULL;
static NeuronDynamicsGraphScroll *neuron_dynamics_graph = NULL;
static NetworkSpikePatternGraphScroll *spike_pattern_graph = NULL;

static bool buffer_visualization_global_pause_request = TRUE;
static bool buffer_visualization_global_resume_request = FALSE;

static bool current_pattern_graph_visualization_resume_request = FALSE;
static bool neuron_dynamics_graph_visualization_resume_request = FALSE;
static bool spike_pattern_graph_visualization_resume_request = FALSE;

static bool buffer_view_handler_paused_global = FALSE;


static gboolean timeout_callback(gpointer user_data) ;


bool buffer_view_handler(void)
{
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();
	network = spike_gen_data->network;
	current_pattern_buffer = spike_gen_data->current_pattern_buffer;
	neuron_dynamics_buffer = spike_gen_data->neuron_dynamics_pattern_buffer;
	if (!create_neuron_dynamics_and_current_buffer_view_gui())
		return  print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BufferViewHandler", "buffer_view_handler","! create_neuron_dynamics_and_current_buffer_view_gui().");
	if (!create_spike_pattern_buffer_view_gui())
		return  print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BufferViewHandler", "buffer_view_handler","! create_spike_pattern_buffer_view_gui().");
	current_pattern_graph = get_current_pattern_graph_w_scroll_ptr();
	neuron_dynamics_graph = get_neuron_dynamics_graph_w_scroll_ptr();
	spike_pattern_graph = get_generated_spike_pattern_graph_ptr();

	g_timeout_add(50, timeout_callback, NULL);		// timeout shoud be less than buffer_followup_latency,

	return TRUE;
}

static gboolean timeout_callback(gpointer user_data) 
{
	TimeStamp current_system_time;
	unsigned int buffer_write_idx;
	TimeStamp last_sample_time;

	current_system_time = (shared_memory->rt_tasks_data.current_system_time/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE) *PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
	if (buffer_visualization_global_pause_request)
	{
		buffer_view_handler_paused_global = TRUE;
		buffer_visualization_global_pause_request = FALSE;
	}
	if  (buffer_visualization_global_resume_request)	// shoud determine start_idx for neuron dynamics graphs and spike pattern graphs for every request to sync them all. 
	{
		buffer_view_handler_paused_global = FALSE;
		buffer_visualization_global_resume_request = FALSE;

		get_current_pattern_buffer_last_sample_time_and_write_idx(current_pattern_buffer, &last_sample_time, &buffer_write_idx);   // lock/unlocks mutexes
		if (! determine_current_pattern_graph_scroll_start_indexes(current_pattern_graph, current_system_time, last_sample_time, buffer_write_idx, current_pattern_buffer->buffer_size))
			return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BufferViewHandler", "gboolean timeout_callback","! determine_current_pattern_graph_scroll_start_indexes().");	
		clear_current_pattern_graph_w_scroll(current_pattern_graph);
		if (current_pattern_graph_visualization_resume_request)
		{
			current_pattern_graph->paused = FALSE;
			current_pattern_graph_visualization_resume_request = FALSE;
		}

		get_neuron_dynamics_last_sample_time_and_write_idx(neuron_dynamics_buffer, &last_sample_time, &buffer_write_idx);   // lock/unlocks mutexes
		if (! determine_neuron_dynamics_graph_scroll_start_indexes(neuron_dynamics_graph, current_system_time, last_sample_time, buffer_write_idx, neuron_dynamics_buffer->buffer_size))
			return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BufferViewHandler", "gboolean timeout_callback","! determine_neuron_dynamics_graph_scroll_start_indexes().");	
		clear_neuron_dynamics_graph_w_scroll(neuron_dynamics_graph);
		if (neuron_dynamics_graph_visualization_resume_request)
		{
			neuron_dynamics_graph->paused = FALSE;
			neuron_dynamics_graph_visualization_resume_request = FALSE;
		}

		if (! determine_spike_pattern_graph_scroll_start_time_and_read_indexes(spike_pattern_graph, current_system_time))
			return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BufferViewHandler", "gboolean timeout_callback","! determine_spike_pattern_graph_scroll_start_time().");	
		clear_network_spike_pattern_graph_w_scroll(network, spike_pattern_graph);
		if (spike_pattern_graph_visualization_resume_request)
		{
			spike_pattern_graph->paused = FALSE;
			spike_pattern_graph_visualization_resume_request = FALSE;
		}
	}
	if (! buffer_view_handler_paused_global)
	{
		if (!handle_current_pattern_graph_scrolling_and_plotting(current_pattern_graph, current_pattern_buffer, current_system_time))
			return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BufferViewHandler", "gboolean timeout_callback","! handle_current_pattern_graph_scrolling_and_plotting().");
		if (! handle_neuron_dynamics_graph_scrolling_and_plotting(neuron_dynamics_graph, neuron_dynamics_buffer, current_system_time))
			return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BufferViewHandler", "gboolean timeout_callback","! handle_neuron_dynamics_graph_scrolling_and_plotting().");		
		if (! handle_spike_pattern_graph_scrolling_and_plotting(spike_pattern_graph, network, current_system_time))
			return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BufferViewHandler", "gboolean timeout_callback","! handle_spike_pattern_graph_scrolling_and_plotting().");	
	}
	
	return TRUE;
}


bool send_current_pattern_graph_resume_request_to_buffer_view_handler(void)
{
	if (! buffer_view_handler_paused_global)
	{
		current_pattern_graph_visualization_resume_request = TRUE;
		buffer_visualization_global_resume_request = TRUE;
		return TRUE;
	}
	else
		return FALSE;
}
bool send_neuron_dynamics_graph_resume_request_to_buffer_view_handler(void)
{
	if (! buffer_view_handler_paused_global)
	{
		neuron_dynamics_graph_visualization_resume_request = TRUE;
		buffer_visualization_global_resume_request = TRUE;
		return TRUE;
	}
	else
		return FALSE;
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
