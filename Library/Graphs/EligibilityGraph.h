#ifndef ELIGIBILITY_GRAPH_H
#define ELIGIBILITY_GRAPH_H


typedef struct 	__EligibilityGraphScrollLimited 	EligibilityGraphScrollLimited;

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include "../../../BlueSpike/System/TimeStamp/TimeStamp.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"
#include "../EligibilityData/EligibilityData.h"
#include "StatusMarker.h"
#include "../../../ExperimentHandlers/Library/Status/TrialStatus.h"

struct  __EligibilityGraphScrollLimited		// To be faster when buffering. The most robust current graph due to bug fixes. 
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
	unsigned int		limited_eligibility_buffer_list_idx;  // to get selected neuron, synapse from eligibility buffer limited
	EligibilityBufferLimited	*limited_eligibility_buffer;
	StatusMarkers	*status_markers;
	TrialStatusEvents	*trial_status_events;
	unsigned int		trial_status_event_buffer_read_idx;
};

EligibilityGraphScrollLimited* allocate_eligibility_graph_scroll_limited(GtkWidget *hbox, EligibilityGraphScrollLimited *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, unsigned int num_of_data_points_to_scroll, TimeStamp buffer_followup_latency, unsigned int num_of_markers, TrialStatusEvents *trial_status_events, EligibilityBufferLimited* limited_eligibility_buffer, unsigned int limited_eligibility_buffer_list_idx);
bool determine_eligibility_graph_scroll_limited_start_indexes(EligibilityGraphScrollLimited *graph, TimeStamp last_sample_time, unsigned int eligibility_limited_buffer_write_idx, unsigned int eligibility_limited_buffer_size);
bool handle_limited_eligibility_graph_scrolling_and_plotting(EligibilityGraphScrollLimited *graph, TimeStamp current_system_time);
bool scroll_limited_eligibility_graph(EligibilityGraphScrollLimited *graph);
bool set_total_limits_limited_eligibility_graph_scroll(EligibilityGraphScrollLimited *graph);
bool clear_limited_eligibility_graph_w_scroll(EligibilityGraphScrollLimited *graph);
#endif
