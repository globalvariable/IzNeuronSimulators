#ifndef SIMULATION_TAB_H
#define SIMULATION_TAB_H


#include <gtk/gtk.h>
#include <stdbool.h>

#include "NeuroSimNetwork.h"
#include "NeuroSimSpikePatterns.h"
#include "NeuroSimTrial.h"
#include "NeuroSimNeuronDynamics.h"
#include "NeuroSimSpikePatternGeneratorData.h"
#include "../Library/Network/Network.h"
#include "../Library/Combos/LayerNrnGrpNeuronCombo.h"
#include "../Library/IzNeuron/NeuronTypes.h"
#include "../Library/IzNeuronDynamics/NeuronDynamicsGraph.h"
#include "../Library/IzNeuronDynamics/NeuronDynamicsCombo.h"
#include "../../SpikePatternGenerator/DataHandling/DataHandling.h"
#include "../Library/ExtNetwork/ExtNetwork.h"


bool create_simulation_tab(GtkWidget * tabs);








#endif
