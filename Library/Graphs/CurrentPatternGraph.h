#ifndef CURRENT_PATTERN_GRAPH_H
#define CURRENT_PATTERN_GRAPH_H

typedef struct 	__CurrentPatternGraph 	CurrentPatternGraph;
typedef struct 	__CurrentPatternGraphScroll CurrentPatternGraphScroll;
typedef struct 	__CurrentPatternGraphScrollLimited CurrentPatternGraphScrollLimited;
#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include <pthread.h>
#include "../../../BlueSpike/System/TimeStamp/TimeStamp.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../InjectionCurrentData/InjectionCurrentData.h"
#include "StatusMarker.h"
#include "../../../ExperimentHandlers/Library/Status/TrialStatus.h"

struct  __CurrentPatternGraph
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

struct  __CurrentPatternGraphScroll
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
	StatusMarkers	*status_markers;
	TrialStatusEvents	*trial_status_events;
	unsigned int		trial_status_event_buffer_read_idx;
};

struct  __CurrentPatternGraphScrollLimited			// To be faster when buffering. The most robust current graph due to bug fixes. 
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
	bool			globally_paused;
	bool			locally_paused;
	bool			global_pause_request;
	bool			local_pause_request;
	bool			scroll_request;	// scrolling just after set_total_limits prevents plotting of new data part. then, rise a request and handle scrolling in the next timeout_callback.
	unsigned int		limited_current_buffer_list_idx;  // to get selected neuron from current pattern buffer limited
	CurrentPatternBufferLimited* limited_current_buffer;
	StatusMarkers	*status_markers;
	TrialStatusEvents	*trial_status_events;
	unsigned int		trial_status_event_buffer_read_idx;
};

CurrentPatternGraph* allocate_current_pattern_graph(GtkWidget *hbox, CurrentPatternGraph *graph, unsigned int num_of_data_points, TimeStamp sampling_interval);
bool update_current_pattern_graph(CurrentPatternGraph *graph);
bool clear_current_pattern_graph(CurrentPatternGraph *graph);

CurrentPatternGraphScroll* allocate_current_pattern_graph_scroll(GtkWidget *hbox, CurrentPatternGraphScroll *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, unsigned int num_of_data_points_to_scroll, TimeStamp buffer_followup_latency, unsigned int num_of_markers, TrialStatusEvents *trial_status_events);
bool determine_current_pattern_graph_scroll_start_indexes(CurrentPatternGraphScroll *graph, TimeStamp current_system_time, TimeStamp last_sample_time, unsigned int current_pattern_buffer_write_idx, unsigned int current_pattern_buffer_size);
bool handle_current_pattern_graph_scrolling_and_plotting(CurrentPatternGraphScroll *graph, CurrentPatternBuffer *current_pattern_buffer, TimeStamp current_system_time);
bool scroll_current_pattern_graph(CurrentPatternGraphScroll *graph);
bool submit_current_pattern_graph_neuron(CurrentPatternGraphScroll *graph, unsigned int layer, unsigned int neuron_grp, unsigned int neuron_num);
bool set_total_limits_current_pattern_graph_scroll(CurrentPatternGraphScroll *graph);
bool clear_current_pattern_graph_w_scroll(CurrentPatternGraphScroll *graph);

CurrentPatternGraphScrollLimited* allocate_current_pattern_graph_scroll_limited(GtkWidget *hbox, CurrentPatternGraphScrollLimited *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, unsigned int num_of_data_points_to_scroll, TimeStamp buffer_followup_latency, unsigned int num_of_markers, TrialStatusEvents *trial_status_events, CurrentPatternBufferLimited* limited_current_buffer, unsigned int limited_current_buffer_list_idx);
bool determine_current_pattern_graph_scroll_limited_start_indexes(CurrentPatternGraphScrollLimited *graph, TimeStamp current_system_time, TimeStamp last_sample_time, unsigned int current_pattern_limited_buffer_write_idx, unsigned int current_pattern_limited_buffer_size);
bool handle_limited_current_pattern_graph_scrolling_and_plotting(CurrentPatternGraphScrollLimited *graph, TimeStamp current_system_time);
bool scroll_limited_current_pattern_graph(CurrentPatternGraphScrollLimited *graph);
bool set_total_limits_limited_current_pattern_graph_scroll(CurrentPatternGraphScrollLimited *graph);
bool clear_limited_current_pattern_graph_w_scroll(CurrentPatternGraphScrollLimited *graph);
#endif
