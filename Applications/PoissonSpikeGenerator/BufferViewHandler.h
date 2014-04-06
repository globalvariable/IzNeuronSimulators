#ifndef BUFFER_VIEW_HANDLER_H
#define BUFFER_VIEW_HANDLER_H


#define NUM_OF_STATUS_MARKERS		3
#define BUFFER_FOLLOWUP_LATENCY	100000000ULL	
#define GRAPH_LENGTHS				3000000000ULL	
#define GRAPH_SCROLL_LENGTHS		1000000000ULL	

#include "SpikePatternBufferView.h"

bool buffer_view_handler(void);
void send_global_resume_request_to_buffer_view_handler(void);
void send_global_pause_request_to_buffer_view_handler(void);
bool send_spike_pattern_graph_resume_request_to_buffer_view_handler(void);
bool is_buffer_view_handler_paused(void);







#endif
