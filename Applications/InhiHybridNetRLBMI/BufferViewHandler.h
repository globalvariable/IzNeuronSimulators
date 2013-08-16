#ifndef BUFFER_VIEW_HANDLER_H
#define BUFFER_VIEW_HANDLER_H

#define NUM_OF_STATUS_MARKERS		3
#define BUFFER_FOLLOWUP_LATENCY	1000000000ULL	
#define GRAPH_LENGTHS				3000000000ULL	
#define GRAPH_SCROLL_LENGTHS		1000000000ULL	

#include <gtk/gtk.h>
#include <stdbool.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "NeuronDynamicsView.h"
#include "BlueSpikeSpikePatternView.h"
#include "InSilicoSpikePatternView.h"
#include "STDPView.h"
#include "EligibilityView.h"
#include "WeightHistoryView.h"
#include "AxonToLayerWeightView.h"

bool buffer_view_handler(void);
void send_global_resume_request_to_buffer_view_handler(void);
void send_global_pause_request_to_buffer_view_handler(void);
bool send_neuron_dynamics_graph_resume_request_to_buffer_view_handler(unsigned int graph_idx);
bool send_stdp_graph_resume_request_to_buffer_view_handler(unsigned int graph_idx);
bool send_eligibility_graph_resume_request_to_buffer_view_handler(unsigned int graph_idx);
bool send_depol_eligibility_graph_resume_request_to_buffer_view_handler(unsigned int graph_idx);
bool send_blue_spike_spike_graph_resume_request_to_buffer_view_handler(void);
bool send_in_silico_spike_graph_resume_request_to_buffer_view_handler(void);
bool is_buffer_view_handler_paused(void);


#endif
