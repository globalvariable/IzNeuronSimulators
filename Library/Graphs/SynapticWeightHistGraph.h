#ifndef SYNAPTIC_WEIGHT_HIST_GRAPH_H
#define SYNAPTIC_WEIGHT_HIST_GRAPH_H


typedef struct 	__SynapticWeightHistGraph 	SynapticWeightHistGraph;

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Synapse/Synapse.h"

struct  __SynapticWeightHistGraph
{
	GtkWidget *box;
	GtkWidget *databox;
	GtkDataboxGraph *graph;
	float *x;
	float *y;
};


SynapticWeightHistGraph* allocate_synaptic_weight_history_graph(GtkWidget *hbox, SynapticWeightHistGraph *graph);
bool update_synapse_history_graph(SynapticWeightHistGraph *graph, Network *network, unsigned int layer_num, unsigned int nrn_grp_num, unsigned int nrn_num, unsigned int syn_num);






#endif
