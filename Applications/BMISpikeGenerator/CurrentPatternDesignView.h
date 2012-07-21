#ifndef CURRENT_PATTERN_DESIGN_VIEW_H
#define CURRENT_PATTERN_DESIGN_VIEW_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "BMISpikeGenerator.h"
#include "BufferViewHandler.h"
#include "FileHandler.h"
#include "../../Library/Network/Network.h"
#include "../../Library/Neuron/Neuron.h"
#include "../../Library/Neuron/NeuronTypes.h"
#include "../../../BlueSpike/Library/Misc/Randn.h"
#include "../../../ExperimentHandlers/Library/Combos/TrialTypesCombo.h"
#include "../../Library/Combos/LayerNrnGrpNeuronCombo.h"
#include "../../Library/Combos/NeuronDynamicsCombo.h"
#include "../../Library/Graphs/CurrentPatternGraph.h"
#include "../../Library/Graphs/NeuronDynamicsGraph.h"

#define LINE 0
#define SIN 1
#define COS 2
#define pi 3.141592653589793

bool create_current_pattern_view_gui(void);













#endif
