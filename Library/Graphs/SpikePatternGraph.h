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
#include "../../../BlueSpike/TimeStamp.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"
#include "../SpikeData/SpikeData.h"

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
	unsigned int				spike_data_buffer_read_idx;
	TimeStamp				new_part_start_time;
	unsigned int				num_of_data_points_to_scroll;
	TimeStamp				graph_len_to_scroll;
	TimeStamp				spike_buffer_followup_latency;    // spike times larger than (buffering_start_time + graph_len) and smaller than current_time might be missed. this handling latency avoids it. 
	SpikeData				*spike_handling_buffer;
	bool					paused;
	bool					scroll_request;	// scrolling just after set_total_limits prevents plotting of new data part. then, rise a request and handle scrolling in the next timeout_callback.
};

NetworkSpikePatternGraphScroll* allocate_network_spike_pattern_graph_scroll(Network* network, GtkWidget *hbox, NetworkSpikePatternGraphScroll *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, int graph_height, unsigned int num_of_data_points_to_scroll, TimeStamp spike_buffer_followup_latency);  // this height should be adjusted manually so that the graph size will be determined.
bool scroll_network_spike_pattern_graph(Network* network, NetworkSpikePatternGraphScroll *graph);
bool set_total_limits_network_spike_pattern_graph(Network* network, NetworkSpikePatternGraphScroll *graph);












#endif
