#ifndef NEURON_DYNAMICS_GRAPH_H
#define NEURON_DYNAMICS_GRAPH_H


typedef struct 	__NeuronDynamicsGraph 		NeuronDynamicsGraph;

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include "../../../BlueSpike/TimeStamp.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"

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


NeuronDynamicsGraph* allocate_neuron_dynamics_graph(GtkWidget *hbox, NeuronDynamicsGraph *graph, unsigned int num_of_data_points, TimeStamp sampling_interval);
bool update_neuron_dynamics_graph(NeuronDynamicsGraph *graph);
bool clear_neuron_dynamics_graph(NeuronDynamicsGraph *graph);


#endif
