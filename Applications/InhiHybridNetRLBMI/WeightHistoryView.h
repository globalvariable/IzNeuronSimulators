#ifndef WEIGHT_HISTORY_VIEW_H
#define WEIGHT_HISTORY_VIEW_H

#define NUM_OF_WEIGHT_HISTORY_GRAPHS	4

#include <gtk/gtk.h>
#include <stdbool.h>
#include "HybridNetRLBMI.h"
#include "../../Library/Combos/LayerNrnGrpNrnSynapseCombo.h"
#include "../../Library/Graphs/SynapticWeightHistGraph.h"

bool create_synaptic_wegiht_history_view_gui(void);


#endif
