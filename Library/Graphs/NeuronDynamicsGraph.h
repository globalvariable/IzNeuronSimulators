#ifndef NEURON_DYNAMICS_GRAPH_H
#define NEURON_DYNAMICS_GRAPH_H


typedef struct 	__NeuronDynamicsGraph 			NeuronDynamicsGraph;
typedef struct 	__NeuronDynamicsGraphScroll 		NeuronDynamicsGraphScroll;

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include "../../../BlueSpike/TimeStamp.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"
#include "../NeuronDynamicsData/NeuronDynamicsData.h"

struct  __NeuronDynamicsGraph
{
	GtkWidget *box;
	GtkWidget *databox;
	GtkDataboxGraph *graph;
	float *x;
	float *y;
	unsigned int num_of_data_points;
	TimeStamp sampling_interval;
	TimeStamp graph_len;
};

struct  __NeuronDynamicsGraphScroll
{
	pthread_mutex_t 	mutex;
	GtkWidget		*box;
	GtkWidget		*databox;
	GtkDataboxGraph	*graph;
	float			*x;
	float			*y;
	unsigned int		num_of_data_points;
	TimeStamp		sampling_interval;
	TimeStamp		graph_len;
	TimeStamp		new_part_start_time;
	TimeStamp		new_part_start_idx;
	unsigned int		num_of_data_points_to_scroll;
	TimeStamp		graph_len_to_scroll;
	TimeStamp		buffer_followup_latency;    // it is to provide sync' d plotting with spike pattern graph with scrolling
	bool			paused;
	bool			scroll_request;	// scrolling just after set_total_limits prevents plotting of new data part. then, rise a request and handle scrolling in the next timeout_callback.
	unsigned int		active_layer;
	unsigned int		active_neuron_group;
	unsigned int		active_neuron;
	int				active_dynamics_type;  // to get type from combo
};

NeuronDynamicsGraph* allocate_neuron_dynamics_graph(GtkWidget *hbox, NeuronDynamicsGraph *graph, unsigned int num_of_data_points, TimeStamp sampling_interval);
bool update_neuron_dynamics_graph(NeuronDynamicsGraph *graph);
bool clear_neuron_dynamics_graph(NeuronDynamicsGraph *graph);
bool change_length_of_neuron_dynamics_graph(NeuronDynamicsGraph *graph, TimeStamp new_length, bool log_request);

NeuronDynamicsGraphScroll* allocate_neuron_dynamics_graph_scroll(GtkWidget *hbox, NeuronDynamicsGraphScroll *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, unsigned int num_of_data_points_to_scroll, TimeStamp buffer_followup_latency);
bool determine_neuron_dynamics_graph_scroll_start_indexes(NeuronDynamicsGraphScroll *graph, TimeStamp current_system_time, TimeStamp last_sample_time, unsigned int neuron_dynamics_buffer_write_idx, unsigned int neuron_dynamics_buffer_size);
bool handle_neuron_dynamics_graph_scrolling_and_plotting(NeuronDynamicsGraphScroll *graph, NeuronDynamicsBuffer *neuron_dynamics_buffer, TimeStamp current_system_time);
bool scroll_neuron_dynamics_graph(NeuronDynamicsGraphScroll *graph);
bool submit_neuron_dynamics_graph_neuron_and_dynamics_type(NeuronDynamicsGraphScroll *graph, unsigned int layer, unsigned int neuron_grp, unsigned int neuron_num, int dynamics_type);
bool set_total_limits_neuron_dynamics_graph_scroll(NeuronDynamicsGraphScroll *graph);

#endif
