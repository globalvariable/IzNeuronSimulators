#ifndef BUFFERS_VIEW_H
#define BUFFERS_VIEW_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdlib.h>
#include "BMISimulationSpikeGenerator.h"
#include "../../Library/Graphs/CurrentPatternGraph.h"
#include "../../Library/Graphs/NeuronDynamicsGraph.h"

bool create_neuron_dynamics_and_current_buffer_view_gui(void);
CurrentPatternGraphScroll* get_current_pattern_graph_w_scroll_ptr(void);
NeuronDynamicsGraphScroll* get_neuron_dynamics_graph_w_scroll_ptr(void);
void send_global_pause_button_sensitive_request(void);





#endif
