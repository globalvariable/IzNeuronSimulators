#ifndef SPIKE_PATTERN_GRAPH_H
#define SPIKE_PATTERN_GRAPH_H


#define NUM_OF_GRID_MARKERS	9

typedef struct __NetworkSpikePatternGraphScroll NetworkSpikePatternGraphScroll;
typedef struct __NeuronSpikePatternGraphScroll NeuronSpikePatternGraphScroll;

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include <gtkdatabox_markers.h>
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
	SpikeData				**source_spike_data_to_plot;
	unsigned int				num_of_source_spike_data_to_plot;   // multi threaded applications has multiple spike data buffers to get rid of mutex
	SpikeData				*spike_handling_buffer;
	bool					globally_paused;
	bool					locally_paused;
	bool					global_pause_request;
	bool					local_pause_request;
	bool					scroll_request;	// scrolling just after set_total_limits prevents plotting of new data part. then, rise a request and handle scrolling in the next timeout_callback.
	unsigned int				data_point_placement_start_idx;
	TrialStatusEvents			*trial_status_events;
	unsigned int				trial_status_event_buffer_read_idx;
	StatusMarkers 			*status_markers;
	unsigned int				num_of_network_layers_to_be_handled; // it is placed here to exclude unnecessary network layers's plotting(e.g. layers with poisson neurons. these layers should be placed at end of layer array)
	float					X_grid_markers[NUM_OF_GRID_MARKERS];
	float					Y_grid_markers[NUM_OF_GRID_MARKERS];
};

NetworkSpikePatternGraphScroll* allocate_network_spike_pattern_graph_scroll(Network* network, GtkWidget *hbox, NetworkSpikePatternGraphScroll *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, int graph_height, unsigned int num_of_data_points_to_scroll, TimeStamp spike_buffer_followup_latency, SpikeData **source_spike_data_to_plot, unsigned int num_of_markers, TrialStatusEvents *trial_status_events, unsigned int	num_of_spike_data_buffer);  // this height should be adjusted manually so that the graph size will be determined.


bool determine_spike_pattern_graph_scroll_start_time_and_read_indexes(NetworkSpikePatternGraphScroll *graph, TimeStamp current_system_time);
bool handle_spike_pattern_graph_scrolling_and_plotting(NetworkSpikePatternGraphScroll *graph, Network *network, TimeStamp current_system_time);
bool scroll_network_spike_pattern_graph(Network* network, NetworkSpikePatternGraphScroll *graph);
bool set_total_limits_network_spike_pattern_graph(Network* network, NetworkSpikePatternGraphScroll *graph);
bool clear_network_spike_pattern_graph_w_scroll(Network* network, NetworkSpikePatternGraphScroll *graph);

NetworkSpikePatternGraphScroll* allocate_network_spike_pattern_graph_scroll_exclude_poisson(Network* network, GtkWidget *hbox, NetworkSpikePatternGraphScroll *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, int graph_height, unsigned int num_of_data_points_to_scroll, TimeStamp spike_buffer_followup_latency, SpikeData **source_spike_data_to_plot, unsigned int num_of_markers, TrialStatusEvents *trial_status_events, unsigned int	num_of_spike_data_buffer);
// This function is used to exclude plotting of the network layers consisting of poisson neurons. To use this function, the layers with poisson neurons should take place at the end of the layered network structure. So that this funciton skips allocation of graphs for these layers and during plotting the layer counter cannot reach the layers with possion neurons.







#endif
