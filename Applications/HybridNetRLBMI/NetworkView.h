#ifndef NETWORK_VIEW_H
#define NETWORK_VIEW_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include "Gui.h"
#include "HybridNetRLBMI.h"
#include "HybridNetRLBMIRtTask.h"
#include "BufferViewHandler.h"
#include "../../Library/Neuron/Neuron.h"
#include "../../Library/Neuron/NeuronTypes.h"
#include "../../Library/Combos/LayerNrnGrpNeuronCombo.h"
#include "../../Library/Combos/NeuronDynamicsCombo.h"
#include "../../Library/Graphs/NeuronDynamicsGraph.h"
#include "../../Library/InjectionCurrentData/InjectionCurrentData.h"



bool create_network_view_gui(void);












#endif
