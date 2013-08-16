#ifndef NEURON_DYNAMICS_VIEW_H
#define NEURON_DYNAMICS_VIEW_H

#define NUM_OF_NEURON_DYNAMICS_GRAPHS  1

#include <gtk/gtk.h>
#include <stdbool.h>
#include "HybridNetRLBMI.h"
#include "BufferViewHandler.h"
#include "../../Library/Graphs/NeuronDynamicsGraph.h"


bool create_neuron_dynamics_view_gui(void);
unsigned int get_num_neuron_dynamics_graphs_w_scroll(void);
NeuronDynamicsGraphScrollLimited** get_neuron_dynamics_graphs_w_scroll_ptr(void);
void send_global_pause_button_sensitive_request(void);



#endif
