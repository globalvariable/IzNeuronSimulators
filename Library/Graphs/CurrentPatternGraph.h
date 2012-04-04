#ifndef CURRENT_PATTERN_GRAPH_H
#define CURRENT_PATTERN_GRAPH_H

typedef struct 	__CurrentPatternGraph 	CurrentPatternGraph;
typedef struct 	__CurrentPatternGraphScroll CurrentPatternGraphScroll;
#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include <pthread.h>
#include "../../../BlueSpike/TimeStamp.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../InjectionCurrentData/InjectionCurrentData.h"


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
};

CurrentPatternGraph* allocate_current_pattern_graph(GtkWidget *hbox, CurrentPatternGraph *graph, unsigned int num_of_data_points, TimeStamp sampling_interval);
bool update_current_pattern_graph(CurrentPatternGraph *graph);
bool clear_current_pattern_graph(CurrentPatternGraph *graph);

CurrentPatternGraphScroll* allocate_current_pattern_graph_scroll(GtkWidget *hbox, CurrentPatternGraphScroll *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, unsigned int num_of_data_points_to_scroll, TimeStamp buffer_followup_latency);
bool determine_current_pattern_graph_scroll_start_indexes(CurrentPatternGraphScroll *graph, TimeStamp current_system_time, TimeStamp last_sample_time, unsigned int current_pattern_buffer_write_idx, unsigned int current_pattern_buffer_size);
bool handle_current_pattern_graph_scrolling_and_plotting(CurrentPatternGraphScroll *graph, CurrentPatternBuffer *current_pattern_buffer, TimeStamp current_system_time);
bool scroll_current_pattern_graph(CurrentPatternGraphScroll *graph);
bool submit_current_pattern_graph_neuron(CurrentPatternGraphScroll *graph, unsigned int layer, unsigned int neuron_grp, unsigned int neuron_num);
bool set_total_limits_current_pattern_graph_scroll(CurrentPatternGraphScroll *graph);
bool clear_current_pattern_graph_w_scroll(CurrentPatternGraphScroll *graph);
#endif
