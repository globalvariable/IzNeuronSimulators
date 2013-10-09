#ifndef AXON_TO_LAYER_WEIGHT_VIEW_H
#define AXON_TO_LAYER_WEIGHT_VIEW_H

#define NUM_OF_AXON_TO_LAYER_GRAPHS	4

#include <gtk/gtk.h>
#include <stdbool.h>
#include "HybridNetRLBMI.h"
#include "../../Library/Combos/LayerNrnGrpNrnSynapseCombo.h"
#include "../../Library/Graphs/SynapticWeightHistGraph.h"

bool create_axon_to_layer_weight_view_gui(void);


#endif
