#ifndef SPIKE_PATTERN_GRAPH_H
#define SPIKE_PATTERN_GRAPH_H


typedef struct __NetworkSpikePatternGraph NetworkSpikePatternGraph;
typedef struct __NeuronSpikePatternGraph NeuronSpikePatternGraph;

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include "../../../BlueSpike/TimeStamp.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"

struct  __NeuronSpikePatternGraph
{
	GtkWidget *databox;
	GtkDataboxGraph *graph;
	float *x;
	float *y;
};

struct  __NetworkSpikePatternGraph
{
	NeuronSpikePatternGraph ***neuron_graphs;
	unsigned int num_of_data_points;
	TimeStamp sampling_interval;
	TimeStamp graph_len;
	unsigned int  graph_len_ms;
	unsigned int spike_data_buffer_read_idx;
	TimeStamp plotting_start_time;
};

NetworkSpikePatternGraph* allocate_network_spike_pattern_graph(Network* network, GtkWidget *hbox, NetworkSpikePatternGraph *graph, unsigned int num_of_data_points, TimeStamp sampling_interval, int graph_height);  // this height should be adjusted manually so that the graph size will be determined.
bool clear_network_spike_pattern_graph_y_axes(Network* network, NetworkSpikePatternGraph *graph);











#endif
