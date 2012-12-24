#ifndef SPIKE_PATTERN_GRAPH_H
#define SPIKE_PATTERN_GRAPH_H


typedef struct __NetworkSpikePatternGraphScroll NetworkSpikePatternGraphScroll;
typedef struct __NeuronSpikePatternGraphScroll NeuronSpikePatternGraphScroll;

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include "../../../BlueSpike/System/TimeStamp/TimeStamp.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"
#include "../SpikeData/SpikeData.h"
#include "StatusMarker.h"
#include "../../../ExperimentHandlers/Library/Status/TrialStatus.h"

struct  __NeuronSpikePatternGraphScroll	
{
	GtkWidget *databox;
	GtkDataboxGraph *graph;
	float *x;
	float *y;
};

struct  __NetworkSpikePatternGraphScroll		// when one needs real time scrolling to visualize real time spike data.  define new struct (i.e. NetworkSpikePatternGraph) for not real time spike data visualization.
{
	NeuronSpikePatternGraphScroll	***neuron_graphs;
	unsigned int				num_of_data_points;
	TimeStamp				sampling_interval;
	TimeStamp				graph_len;
	unsigned int				graph_len_ms;
	TimeStamp				new_part_start_time;
	unsigned int				num_of_data_points_to_scroll; // instead of sliding in a straigthforward manner, think of making a list which holds the data points having spikes. the size of the list would be num_of_data_points and num_of_spikes would determine the used part of the list by spikes.
	TimeStamp				graph_len_to_scroll;
	TimeStamp				spike_buffer_followup_latency;    // spike times larger than (buffering_start_time + graph_len) and smaller than current_time might be missed. this handling latency avoids it. 
	SpikeData				*source_spike_data_to_plot;
	SpikeData				*spike_handling_buffer;
	bool					globally_paused;
	bool					locally_paused;
	bool					global_pause_request;
	bool					local_pause_request;
	bool					scroll_request;	// scrolling just after set_total_limits prevents plotting of new data part. then, rise a request and handle scrolling in the next timeout_callback.
	unsigned int				source_spike_data_buffer_read_idx;
	unsigned int				spike_handling_buffer_read_idx;
	unsigned int				data_point_placement_start_idx;
	TrialStatusEvents			*trial_status_events;
	unsigned int				trial_status_event_buffer_read_idx;
	StatusMarkers 			*status_markers;
};

NetworkSpikePatternGraphScroll* allocate_network_spike_pattern_graph_scroll(Network* network, GtkWidget *hbox, NetworkSpikePatternGraphScroll *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, int graph_height, unsigned int num_of_data_points_to_scroll, TimeStamp spike_buffer_followup_latency, SpikeData *source_spike_data_to_plot, unsigned int num_of_markers, TrialStatusEvents *trial_status_events);  // this height should be adjusted manually so that the graph size will be determined.


bool determine_spike_pattern_graph_scroll_start_time_and_read_indexes(NetworkSpikePatternGraphScroll *graph, TimeStamp current_system_time);
bool handle_spike_pattern_graph_scrolling_and_plotting(NetworkSpikePatternGraphScroll *graph, Network *network, TimeStamp current_system_time);
bool scroll_network_spike_pattern_graph(Network* network, NetworkSpikePatternGraphScroll *graph);
bool set_total_limits_network_spike_pattern_graph(Network* network, NetworkSpikePatternGraphScroll *graph);
bool clear_network_spike_pattern_graph_w_scroll(Network* network, NetworkSpikePatternGraphScroll *graph);









#endif
