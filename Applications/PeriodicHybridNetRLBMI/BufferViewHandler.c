#include "BufferViewHandler.h"

static Network *blue_spike_network = NULL;
static Network *in_silico_network = NULL;

static RtTasksData *rt_tasks_data = NULL;


static NeuronDynamicsBufferLimited *neuron_dynamics_buffer_limited;
static unsigned int num_of_neuron_dynamics_graphs;
static NeuronDynamicsGraphScrollLimited **neuron_dynamics_graph_arr;

static STDPBufferLimited *stdp_buffer_limited;
static unsigned int num_of_stdp_graphs;
static STDPGraphScrollLimited **stdp_graph_arr;

static EligibilityBufferLimited *eligibility_buffer_limited;
static unsigned int num_of_eligibility_graphs;
static EligibilityGraphScrollLimited **eligibility_graph_arr;

static NetworkSpikePatternGraphScroll *blue_spike_spike_graph;
static NetworkSpikePatternGraphScroll *in_silico_spike_graph;

static bool buffer_visualization_global_pause_request = FALSE;
static bool buffer_visualization_global_resume_request = FALSE;

static bool *neuron_dynamics_graph_visualization_resume_request = NULL;
static bool *stdp_graph_visualization_resume_request = NULL;
static bool *eligibility_graph_visualization_resume_request = NULL;
static bool blue_spike_spike_graph_visualization_resume_request = FALSE;
static bool in_silico_spike_graph_visualization_resume_request = FALSE;


static bool buffer_view_handler_paused_global = TRUE;

static gboolean timeout_callback(gpointer user_data) ;


bool buffer_view_handler(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();
	blue_spike_network = bmi_data->blue_spike_network;
	in_silico_network = bmi_data->in_silico_network;
	rt_tasks_data = bmi_data->rt_tasks_data;
	if (!create_neuron_dynamics_view_gui())
		return  print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "buffer_view_handler","! create_neuron_dynamics_view_gui().");
	if (!create_stdp_view_gui())
		return  print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "buffer_view_handler","! create_stdp_view_gui().");
	if (!create_eligibility_view_gui())
		return  print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "buffer_view_handler","! create_eligibility_view_gui().");
	if (!create_blue_spike_spike_pattern_view_gui())
		return  print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "buffer_view_handler","! create_blue_spike_spike_pattern_view_gui().");
	if (!create_in_silico_spike_pattern_view_gui())
		return  print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "buffer_view_handler","! create_in_silico_spike_pattern_view_gui().");
	if (!create_synaptic_wegiht_history_view_gui())
		return  print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "buffer_view_handler","! create_synaptic_wegiht_history_view_gui().");
	if (!create_axon_to_layer_weight_view_gui())
		return  print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "buffer_view_handler","! create_axon_to_layer_weight_view_gui().");
	neuron_dynamics_buffer_limited = bmi_data->neuron_dynamics_limited_buffer;
	num_of_neuron_dynamics_graphs = get_num_neuron_dynamics_graphs_w_scroll();
	neuron_dynamics_graph_arr = get_neuron_dynamics_graphs_w_scroll_ptr();
	neuron_dynamics_graph_visualization_resume_request = g_new0(bool, num_of_neuron_dynamics_graphs);

	stdp_buffer_limited = bmi_data->stdp_limited_buffer;
	num_of_stdp_graphs = get_num_stdp_graphs_w_scroll();
	stdp_graph_arr = get_stdp_graphs_w_scroll_ptr();
	stdp_graph_visualization_resume_request = g_new0(bool, num_of_stdp_graphs);

	eligibility_buffer_limited = bmi_data->eligibility_limited_buffer;
	num_of_eligibility_graphs = get_num_eligibility_graphs_w_scroll();
	eligibility_graph_arr = get_eligibility_graphs_w_scroll_ptr();
	eligibility_graph_visualization_resume_request = g_new0(bool, num_of_eligibility_graphs);

	blue_spike_spike_graph = get_blue_spike_spike_pattern_graph_ptr();
	in_silico_spike_graph = get_in_silico_spike_pattern_graph_ptr();

	g_timeout_add(500 , timeout_callback, NULL);		// timeout shoud be less than buffer_followup_latency,

	return TRUE;
}


static gboolean timeout_callback(gpointer user_data) 
{
	unsigned int i;
	TimeStamp current_system_time;
	unsigned int neuron_dynamics_buffer_write_idx;
	unsigned int stdp_buffer_write_idx;
	unsigned int eligibility_buffer_write_idx;
	TimeStamp last_sample_time;

	current_system_time = ((*(get_hybrid_net_rl_bmi_data()->sys_time_ptr))/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE) *PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
	if (buffer_visualization_global_pause_request)
	{
		buffer_view_handler_paused_global = TRUE;
		buffer_visualization_global_pause_request = FALSE;
		for (i=0; i < num_of_neuron_dynamics_graphs; i++)
		{
			neuron_dynamics_graph_arr[i]->global_pause_request = TRUE;
		}
		for (i=0; i < num_of_stdp_graphs; i++)
		{
			stdp_graph_arr[i]->global_pause_request = TRUE;
		}
		for (i=0; i < num_of_eligibility_graphs; i++)
		{
			eligibility_graph_arr[i]->global_pause_request = TRUE;
		}
		blue_spike_spike_graph->global_pause_request = TRUE;
		in_silico_spike_graph->global_pause_request = TRUE;
	}
	if  (buffer_visualization_global_resume_request)	// shoud determine start_idx for neuron dynamics graphs and spike pattern graphs for every request to sync them all. 
	{
		buffer_view_handler_paused_global = FALSE;
		buffer_visualization_global_resume_request = FALSE;

		for (i=0; i < num_of_neuron_dynamics_graphs; i++)
		{
			get_neuron_dynamics_limited_last_sample_time_and_write_idx(neuron_dynamics_buffer_limited, i, &last_sample_time, &neuron_dynamics_buffer_write_idx);   // lock/unlocks mutexes
			if (! determine_neuron_dynamics_graph_scroll_limited_start_indexes(neuron_dynamics_graph_arr[i] , current_system_time, last_sample_time, neuron_dynamics_buffer_write_idx, neuron_dynamics_buffer_limited->selected_dyns[i].buffer_size))
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "gboolean timeout_callback","! determine_neuron_dynamics_graph_scroll_limited_start_indexes().");	
			if (!neuron_dynamics_graph_arr[i]->locally_paused)
				clear_limited_neuron_dynamics_graph_w_scroll(neuron_dynamics_graph_arr[i]);

			neuron_dynamics_graph_arr[i]->globally_paused = FALSE;
			neuron_dynamics_graph_arr[i]->global_pause_request = FALSE;
			if (neuron_dynamics_graph_visualization_resume_request[i])
			{
				neuron_dynamics_graph_arr[i]->locally_paused = FALSE;
				neuron_dynamics_graph_visualization_resume_request[i] = FALSE;
				clear_limited_neuron_dynamics_graph_w_scroll(neuron_dynamics_graph_arr[i]);
			}
		}

		for (i=0; i < num_of_stdp_graphs; i++)
		{
			get_stdp_limited_last_sample_time_and_write_idx(stdp_buffer_limited, i, &last_sample_time, &stdp_buffer_write_idx);   // lock/unlocks mutexes
			if (! determine_stdp_graph_scroll_limited_start_indexes(stdp_graph_arr[i] , current_system_time, last_sample_time, stdp_buffer_write_idx, stdp_buffer_limited->selected_stdp[i].buffer_size))
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "gboolean timeout_callback","! determine_stdp_graph_scroll_limited_start_indexes().");	
			if (!stdp_graph_arr[i]->locally_paused)
				clear_limited_stdp_graph_w_scroll(stdp_graph_arr[i]);

			stdp_graph_arr[i]->globally_paused = FALSE;
			stdp_graph_arr[i]->global_pause_request = FALSE;
			if (stdp_graph_visualization_resume_request[i])
			{
				stdp_graph_arr[i]->locally_paused = FALSE;
				stdp_graph_visualization_resume_request[i] = FALSE;
				clear_limited_stdp_graph_w_scroll(stdp_graph_arr[i]);
			}
		}

		for (i=0; i < num_of_eligibility_graphs; i++)
		{
			get_eligibility_limited_last_sample_time_and_write_idx(eligibility_buffer_limited, i, &last_sample_time, &eligibility_buffer_write_idx);   // lock/unlocks mutexes
			if (! determine_eligibility_graph_scroll_limited_start_indexes(eligibility_graph_arr[i] , current_system_time, last_sample_time, eligibility_buffer_write_idx, eligibility_buffer_limited->selected_eligibility[i].buffer_size))
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "gboolean timeout_callback","! determine_eligibility_graph_scroll_limited_start_indexes().");	
			if (!eligibility_graph_arr[i]->locally_paused)
				clear_limited_eligibility_graph_w_scroll(eligibility_graph_arr[i]);

			eligibility_graph_arr[i]->globally_paused = FALSE;
			eligibility_graph_arr[i]->global_pause_request = FALSE;
			if (eligibility_graph_visualization_resume_request[i])
			{
				eligibility_graph_arr[i]->locally_paused = FALSE;
				eligibility_graph_visualization_resume_request[i] = FALSE;
				clear_limited_eligibility_graph_w_scroll(eligibility_graph_arr[i]);
			}
		}

		if (! determine_spike_pattern_graph_scroll_start_time_and_read_indexes(blue_spike_spike_graph, current_system_time))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "gboolean timeout_callback","! determine_spike_pattern_graph_scroll_start_time().");	
		if (!blue_spike_spike_graph->locally_paused)
			clear_network_spike_pattern_graph_w_scroll(blue_spike_network, blue_spike_spike_graph);
		blue_spike_spike_graph->globally_paused = FALSE;
		blue_spike_spike_graph->global_pause_request = FALSE;
		if (blue_spike_spike_graph_visualization_resume_request)
		{
			blue_spike_spike_graph->locally_paused = FALSE;
			blue_spike_spike_graph_visualization_resume_request = FALSE;
			clear_network_spike_pattern_graph_w_scroll(blue_spike_network, blue_spike_spike_graph);
		}	
	
		if (! determine_spike_pattern_graph_scroll_start_time_and_read_indexes(in_silico_spike_graph, current_system_time))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "gboolean timeout_callback","! determine_spike_pattern_graph_scroll_start_time().");	
		if (!in_silico_spike_graph->locally_paused)
			clear_network_spike_pattern_graph_w_scroll(in_silico_network, in_silico_spike_graph);
		in_silico_spike_graph->globally_paused = FALSE;
		in_silico_spike_graph->global_pause_request = FALSE;
		if (in_silico_spike_graph_visualization_resume_request)
		{
			in_silico_spike_graph->locally_paused = FALSE;
			in_silico_spike_graph_visualization_resume_request = FALSE;
			clear_network_spike_pattern_graph_w_scroll(in_silico_network, in_silico_spike_graph);
		}
	}
	for (i=0; i < num_of_neuron_dynamics_graphs; i++)
	{
		if (!handle_limited_neuron_dynamics_graph_scrolling_and_plotting(neuron_dynamics_graph_arr[i], current_system_time))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "gboolean timeout_callback","! handle_limited_neuron_dynamics_graph_scrolling_and_plotting().");
	}
	for (i=0; i < num_of_stdp_graphs; i++)
	{
		if (!handle_limited_stdp_graph_scrolling_and_plotting(stdp_graph_arr[i], current_system_time))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "gboolean timeout_callback","! handle_limited_stdp_graph_scrolling_and_plotting().");
	}
	for (i=0; i < num_of_eligibility_graphs; i++)
	{
		if (!handle_limited_eligibility_graph_scrolling_and_plotting(eligibility_graph_arr[i], current_system_time))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "gboolean timeout_callback","! handle_limited_eligibility_graph_scrolling_and_plotting().");
	}
	if (! handle_spike_pattern_graph_scrolling_and_plotting(blue_spike_spike_graph, blue_spike_network, current_system_time))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "gboolean timeout_callback","! handle_spike_pattern_graph_scrolling_and_plotting().");		
	if (! handle_spike_pattern_graph_scrolling_and_plotting(in_silico_spike_graph, in_silico_network, current_system_time))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "gboolean timeout_callback","! handle_spike_pattern_graph_scrolling_and_plotting().");	

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

bool send_neuron_dynamics_graph_resume_request_to_buffer_view_handler(unsigned int graph_idx)
{
	if (! buffer_view_handler_paused_global)
	{
		neuron_dynamics_graph_visualization_resume_request[graph_idx] = TRUE;
		buffer_visualization_global_resume_request = TRUE;
		return TRUE;
	}
	return FALSE;
}

bool send_stdp_graph_resume_request_to_buffer_view_handler(unsigned int graph_idx)
{
	if (! buffer_view_handler_paused_global)
	{
		stdp_graph_visualization_resume_request[graph_idx] = TRUE;
		buffer_visualization_global_resume_request = TRUE;
		return TRUE;
	}
	return FALSE;
}

bool send_eligibility_graph_resume_request_to_buffer_view_handler(unsigned int graph_idx)
{
	if (! buffer_view_handler_paused_global)
	{
		eligibility_graph_visualization_resume_request[graph_idx] = TRUE;
		buffer_visualization_global_resume_request = TRUE;
		return TRUE;
	}
	return FALSE;
}

bool send_blue_spike_spike_graph_resume_request_to_buffer_view_handler(void)
{
	if (! buffer_view_handler_paused_global)
	{
		blue_spike_spike_graph_visualization_resume_request = TRUE;
		buffer_visualization_global_resume_request = TRUE;
		return TRUE;
	}
	return FALSE;
}

bool send_in_silico_spike_graph_resume_request_to_buffer_view_handler(void)
{
	if (! buffer_view_handler_paused_global)
	{
		in_silico_spike_graph_visualization_resume_request = TRUE;
		buffer_visualization_global_resume_request = TRUE;
		return TRUE;
	}
	return FALSE;
}

bool is_buffer_view_handler_paused(void)
{
	return buffer_view_handler_paused_global;	
}

