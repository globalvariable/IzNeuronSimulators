#ifndef SPIKE_PATTERN_BUFFER_VIEW_H
#define SPIKE_PATTERN_BUFFER_VIEW_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdlib.h>
#include "BMISpikeGenerator.h"
#include "../../Library/Graphs/SpikePatternGraph.h"

bool create_spike_pattern_buffer_view_gui(void);
NetworkSpikePatternGraphScroll* get_generated_spike_pattern_graph_ptr(void);








#endif
