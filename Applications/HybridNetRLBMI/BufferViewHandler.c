#include "BufferViewHandler.h"



static NeuronDynamicsBuffer *neuron_dynamics_buffer;

static unsigned int num_of_neuron_dynamics_graphs;
static NeuronDynamicsGraphScroll **neuron_dynamics_graph_arr;

static bool buffer_visualization_pause_request = FALSE;

static gboolean timeout_callback(gpointer user_data) ;


bool buffer_view_handler(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data();

	neuron_dynamics_buffer = bmi_data->neuron_dynamics_pattern_buffer;

	if (!create_neuron_dynamics_view_gui())
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "buffer_view_handler","! create_neuron_dynamics_view_gui().");

	num_of_neuron_dynamics_graphs = get_num_neuron_dynamics_graphs_w_scroll();
	neuron_dynamics_graph_arr = get_neuron_dynamics_graphs_w_scroll_ptr();

	g_timeout_add(500, timeout_callback, NULL);		// timeout shoud be less than sliding length/2

	return TRUE;
}


static gboolean timeout_callback(gpointer user_data) 
{
	unsigned int i;

	for (i=0; i < num_of_neuron_dynamics_graphs; i++)
	{
		if (!handle_neuron_dynamics_graph_scrolling_and_plotting(neuron_dynamics_graph_arr[i], neuron_dynamics_buffer))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "HybridNetRLBMI", "gboolean timeout_callback","! handle_neuron_dynamics_graph_scrolling().");
	}

	return TRUE;
}

void send_pause_request_to_buffer_view_handler(void)
{
	buffer_visualization_pause_request = TRUE;
}
