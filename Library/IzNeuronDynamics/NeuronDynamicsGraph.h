#ifndef NEURON_DYNAMICS_GRAPH_H
#define NEURON_DYNAMICS_GRAPH_H


#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include "../../../BlueSpike/TimeStamp.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../IzNeuron/Neuron.h"

bool add_neuron_dynamics_box_and_graph(GtkWidget *hbox, TimeStamp pattern_length, int *graph_idx);




#endif
