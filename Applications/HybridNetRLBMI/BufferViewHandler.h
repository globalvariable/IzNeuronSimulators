#ifndef BUFFER_VIEW_HANDLER_H
#define BUFFER_VIEW_HANDLER_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "NeuronDynamicsView.h"



bool buffer_view_handler(void);
void send_global_resume_request_to_buffer_view_handler(void);
void send_global_pause_request_to_buffer_view_handler(void);
void send_neuron_dynamics_graph_resume_request_to_buffer_view_handler(unsigned int graph_idx);
bool is_buffer_view_handler_paused(void);


#endif
