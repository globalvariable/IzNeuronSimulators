#ifndef STDP_GRAPH_H
#define STDP_GRAPH_H


typedef struct 	__STDPGraphScrollLimited 	STDPGraphScrollLimited;

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include "../../../BlueSpike/TimeStamp.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"
#include "../STDPData/STDPData.h"
#include "StatusMarker.h"
#include "../../../ExperimentHandlers/Library/Status/TrialStatus.h"

struct  __STDPGraphScrollLimited		// To be faster when buffering. The most robust current graph due to bug fixes. 
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
	unsigned int		limited_stdp_buffer_list_idx;  // to get selected neuron, synapse and stdp type' s value from stdp buffer limited
	STDPBufferLimited	*limited_stdp_buffer;
	StatusMarkers	*status_markers;
	TrialStatusEvents	*trial_status_events;
	unsigned int		trial_status_event_buffer_read_idx;
};

STDPGraphScrollLimited* allocate_stdp_graph_scroll_limited(GtkWidget *hbox, STDPGraphScrollLimited *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, unsigned int num_of_data_points_to_scroll, TimeStamp buffer_followup_latency, unsigned int num_of_markers, TrialStatusEvents *trial_status_events, STDPBufferLimited* limited_stdp_buffer, unsigned int limited_stdp_buffer_list_idx);
bool determine_stdp_graph_scroll_limited_start_indexes(STDPGraphScrollLimited *graph, TimeStamp current_system_time, TimeStamp last_sample_time, unsigned int stdp_limited_buffer_write_idx, unsigned int stdp_limited_buffer_size);
bool handle_limited_stdp_graph_scrolling_and_plotting(STDPGraphScrollLimited *graph, TimeStamp current_system_time);
bool scroll_limited_stdp_graph(STDPGraphScrollLimited *graph);
bool set_total_limits_limited_stdp_graph_scroll(STDPGraphScrollLimited *graph);
bool clear_limited_stdp_graph_w_scroll(STDPGraphScrollLimited *graph);
#endif
