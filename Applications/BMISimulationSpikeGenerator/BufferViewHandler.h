#ifndef BUFFER_VIEW_HANDLER_H
#define BUFFER_VIEW_HANDLER_H


#include "NeuronDynamicsCurrentBufferView.h"
#include "SpikePatternBufferView.h"

bool buffer_view_handler(void);
void send_global_resume_request_to_buffer_view_handler(void);
void send_global_pause_request_to_buffer_view_handler(void);
bool send_current_pattern_graph_resume_request_to_buffer_view_handler(void);
bool send_neuron_dynamics_graph_resume_request_to_buffer_view_handler(void);
bool send_spike_pattern_graph_resume_request_to_buffer_view_handler(void);
bool is_buffer_view_handler_paused(void);







#endif
