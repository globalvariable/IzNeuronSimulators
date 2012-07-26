#ifndef DEPOL_ELIGIBILITY_VIEW_H
#define DEPOL_ELIGIBILITY_VIEW_H

#define NUM_OF_DEPOL_ELIGIBILITY_GRAPHS	2

#include <gtk/gtk.h>
#include <stdbool.h>
#include "HybridNetRLBMI.h"
#include "BufferViewHandler.h"
#include "../../Library/Graphs/DepolEligibilityGraph.h"
#include "../../Library/Combos/LayerNrnGrpNrnSynapseCombo.h"

bool create_depol_eligibility_view_gui(void);
unsigned int get_num_depol_eligibility_graphs_w_scroll(void);
DepolEligibilityGraphScrollLimited** get_depol_eligibility_graphs_w_scroll_ptr(void);


#endif
