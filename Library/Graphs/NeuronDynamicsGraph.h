#ifndef NEURON_DYNAMICS_GRAPH_H
#define NEURON_DYNAMICS_GRAPH_H


typedef struct 	__NeuronDynamicsGraph 			NeuronDynamicsGraph;
typedef struct 	__NeuronDynamicsGraphScroll 		NeuronDynamicsGraphScroll;
typedef struct 	__NeuronDynamicsGraphScrollLimited 	NeuronDynamicsGraphScrollLimited;

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include "../../../BlueSpike/System/TimeStamp/TimeStamp.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"
#include "../NeuronDynamicsData/NeuronDynamicsData.h"
#include "StatusMarker.h"
#include "../../../ExperimentHandlers/Library/Status/TrialStatus.h"


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
	StatusMarkers	*status_markers;
	TrialStatusEvents	*trial_status_events;
	unsigned int		trial_status_event_buffer_read_idx;
};

struct  __NeuronDynamicsGraphScrollLimited		// To be faster when buffering. The most robust current graph due to bug fixes. 
{
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
	bool			globally_paused;
	bool			locally_paused;
	bool			global_pause_request;
	bool			local_pause_request;
	bool			scroll_request;	// scrolling just after set_total_limits prevents plotting of new data part. then, rise a request and handle scrolling in the next timeout_callback.
	unsigned int		limited_dynamics_buffer_list_idx;  // to get selected neuron and dynamics type' s value from neuron dynamics buffer limited
	NeuronDynamicsBufferLimited* limited_dynamics_buffer;
	StatusMarkers	*status_markers;
	TrialStatusEvents	*trial_status_events;
	unsigned int		trial_status_event_buffer_read_idx;
};

NeuronDynamicsGraph* allocate_neuron_dynamics_graph(GtkWidget *hbox, NeuronDynamicsGraph *graph, unsigned int num_of_data_points, TimeStamp sampling_interval);
bool update_neuron_dynamics_graph(NeuronDynamicsGraph *graph);
bool clear_neuron_dynamics_graph(NeuronDynamicsGraph *graph);
bool change_length_of_neuron_dynamics_graph(NeuronDynamicsGraph *graph, TimeStamp new_length, bool log_request);

NeuronDynamicsGraphScroll* allocate_neuron_dynamics_graph_scroll(GtkWidget *hbox, NeuronDynamicsGraphScroll *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, unsigned int num_of_data_points_to_scroll, TimeStamp buffer_followup_latency, unsigned int num_of_markers, TrialStatusEvents *trial_status_events);
bool determine_neuron_dynamics_graph_scroll_start_indexes(NeuronDynamicsGraphScroll *graph, TimeStamp current_system_time, TimeStamp last_sample_time, unsigned int neuron_dynamics_buffer_write_idx, unsigned int neuron_dynamics_buffer_size);
bool handle_neuron_dynamics_graph_scrolling_and_plotting(NeuronDynamicsGraphScroll *graph, NeuronDynamicsBuffer *neuron_dynamics_buffer, TimeStamp current_system_time);
bool scroll_neuron_dynamics_graph(NeuronDynamicsGraphScroll *graph);
bool submit_neuron_dynamics_graph_neuron_and_dynamics_type(NeuronDynamicsGraphScroll *graph, unsigned int layer, unsigned int neuron_grp, unsigned int neuron_num, int dynamics_type);
bool set_total_limits_neuron_dynamics_graph_scroll(NeuronDynamicsGraphScroll *graph);
bool clear_neuron_dynamics_graph_w_scroll(NeuronDynamicsGraphScroll *graph);

NeuronDynamicsGraphScrollLimited* allocate_neuron_dynamics_graph_scroll_limited(GtkWidget *hbox, NeuronDynamicsGraphScrollLimited *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, unsigned int num_of_data_points_to_scroll, TimeStamp buffer_followup_latency, unsigned int num_of_markers, TrialStatusEvents *trial_status_events, NeuronDynamicsBufferLimited* limited_dynamics_buffer, unsigned int limited_dynamics_buffer_list_idx);
bool determine_neuron_dynamics_graph_scroll_limited_start_indexes(NeuronDynamicsGraphScrollLimited *graph, TimeStamp current_system_time, TimeStamp last_sample_time, unsigned int neuron_dynamics_limited_buffer_write_idx, unsigned int neuron_dynamics_limited_buffer_size);
bool handle_limited_neuron_dynamics_graph_scrolling_and_plotting(NeuronDynamicsGraphScrollLimited *graph, TimeStamp current_system_time);
bool scroll_limited_neuron_dynamics_graph(NeuronDynamicsGraphScrollLimited *graph);
bool set_total_limits_limited_neuron_dynamics_graph_scroll(NeuronDynamicsGraphScrollLimited *graph);
bool clear_limited_neuron_dynamics_graph_w_scroll(NeuronDynamicsGraphScrollLimited *graph);
#endif
