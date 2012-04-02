#ifndef NEURON_DYNAMICS_VIEW_H
#define NEURON_DYNAMICS_VIEW_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include "HybridNetRLBMI.h"
#include "../../Library/Graphs/NeuronDynamicsGraph.h"


bool create_neuron_dynamics_view_gui(void);
unsigned int get_num_neuron_dynamics_graphs_w_scroll(void);
NeuronDynamicsGraphScroll** get_neuron_dynamics_graphs_w_scroll_ptr(void);




#endif
