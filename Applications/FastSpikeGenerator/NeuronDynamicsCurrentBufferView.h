#ifndef BUFFERS_VIEW_H
#define BUFFERS_VIEW_H

#define NUM_OF_CURRENT_PATTERN_GRAPHS	1
#define NUM_OF_NEURON_DYNAMICS_GRAPHS	1

#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdlib.h>
#include "BMISpikeGenerator.h"
#include "../../Library/Graphs/CurrentPatternGraph.h"
#include "../../Library/Graphs/NeuronDynamicsGraph.h"

bool create_neuron_dynamics_and_current_buffer_view_gui(void);
CurrentPatternGraphScrollLimited* get_current_pattern_graph_w_scroll_ptr(void);
NeuronDynamicsGraphScrollLimited* get_neuron_dynamics_graph_w_scroll_ptr(void);
void send_global_pause_button_sensitive_request(void);





#endif
