#ifndef STDP_VIEW_H
#define STDP_VIEW_H

#define NUM_OF_STDP_GRAPHS	2

#include <gtk/gtk.h>
#include <stdbool.h>
#include "HybridNetRLBMI.h"
#include "BufferViewHandler.h"
#include "../../Library/Graphs/STDPGraph.h"
#include "../../Library/Combos/LayerNrnGrpNrnSynapseCombo.h"
#include "../../Library/Combos/STDPCombo.h"

bool create_stdp_view_gui(void);
unsigned int get_num_stdp_graphs_w_scroll(void);
STDPGraphScrollLimited** get_stdp_graphs_w_scroll_ptr(void);


#endif
