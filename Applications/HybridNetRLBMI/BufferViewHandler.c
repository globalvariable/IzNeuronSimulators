#include "BufferViewHandler.h"



static NeuronDynamicsBuffer *neuron_dynamics_buffer;

static unsigned int num_of_neuron_dynamics_graphs;
static NeuronDynamicsGraphScroll **neuron_dynamics_graph_arr;

static bool buffer_visualization_global_pause_request = TRUE;
static bool buffer_visualization_global_resume_request = FALSE;
static bool *neuron_dynamics_graph_visualization_resume_request = NULL;

static bool buffer_view_handler_paused_global = FALSE;

static gboolean timeout_callback(gpointer user_data) ;


bool buffer_view_handler(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();

	neuron_dynamics_buffer = bmi_data->neuron_dynamics_pattern_buffer;

	if (!create_neuron_dynamics_view_gui())
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "buffer_view_handler","! create_neuron_dynamics_view_gui().");

	num_of_neuron_dynamics_graphs = get_num_neuron_dynamics_graphs_w_scroll();
	neuron_dynamics_graph_arr = get_neuron_dynamics_graphs_w_scroll_ptr();
	neuron_dynamics_graph_visualization_resume_request = g_new0(bool, num_of_neuron_dynamics_graphs);

	g_timeout_add(500, timeout_callback, NULL);		// timeout shoud be less than sliding length/2

	return TRUE;
}


static gboolean timeout_callback(gpointer user_data) 
{
	unsigned int i;
	TimeStamp current_system_time;
	unsigned int neuron_dynamics_buffer_write_idx;
	TimeStamp last_sample_time;

	current_system_time = shared_memory->rt_tasks_data.current_system_time;
	if (buffer_visualization_global_pause_request)
	{
		buffer_view_handler_paused_global = TRUE;
		buffer_visualization_global_pause_request = FALSE;
	}
	if  (buffer_visualization_global_resume_request)	// shoud determine start_idx for neuron dynamics graphs and spike pattern graphs for every request to sync them all. 
	{
		buffer_view_handler_paused_global = FALSE;
		buffer_visualization_global_resume_request = FALSE;
		get_neuron_dynamics_last_sample_time_and_write_idx(neuron_dynamics_buffer, &last_sample_time, &neuron_dynamics_buffer_write_idx);   // lock/unlocks mutexes
		for (i=0; i < num_of_neuron_dynamics_graphs; i++)
		{
			if (! determine_neuron_dynamics_graph_scroll_start_indexes(neuron_dynamics_graph_arr[i] , current_system_time, last_sample_time, neuron_dynamics_buffer_write_idx, neuron_dynamics_buffer->buffer_size))
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "gboolean timeout_callback","! determine_neuron_dynamics_graph_scroll_start_indexes().");	
			scroll_neuron_dynamics_graph(neuron_dynamics_graph_arr[i]);
			if (neuron_dynamics_graph_visualization_resume_request[i])
			{
				neuron_dynamics_graph_arr[i]->paused = FALSE;
				neuron_dynamics_graph_visualization_resume_request[i] = FALSE;
			}
		}	

	}
	if (! buffer_view_handler_paused_global)
	{
		for (i=0; i < num_of_neuron_dynamics_graphs; i++)
		{
			if (!handle_neuron_dynamics_graph_scrolling_and_plotting(neuron_dynamics_graph_arr[i], neuron_dynamics_buffer, current_system_time))
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "gboolean timeout_callback","! handle_neuron_dynamics_graph_scrolling_and_plotting().");
		}
	}
	return TRUE;
}

void send_global_pause_request_to_buffer_view_handler(void)
{
	buffer_visualization_global_pause_request = TRUE;
}

void send_global_resume_request_to_buffer_view_handler(void)
{
	buffer_visualization_global_resume_request = TRUE;
}

void send_neuron_dynamics_graph_resume_request_to_buffer_view_handler(unsigned int graph_idx)
{
	if (! buffer_view_handler_paused_global)
	{
		neuron_dynamics_graph_visualization_resume_request[graph_idx] = TRUE;
		buffer_visualization_global_resume_request = TRUE;
	}
}

bool is_buffer_view_handler_paused(void)
{
	return buffer_view_handler_paused_global;	
}

