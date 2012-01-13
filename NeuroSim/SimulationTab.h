#ifndef SIMULATION_TAB_H
#define SIMULATION_TAB_H


#include <gtk/gtk.h>
#include <stdbool.h>

#include "../Library/Network/Network.h"
#include "../Library/IzNeuron/NeuronTypes.h"
#include "../Library/IzNeuron/NeuronDynamicsGraph.h"
#include "../Library/IzNeuron/NeuronDynamicsCombo.h"
#include "../../SpikePatternGenerator/DataHandling/DataHandling.h"

bool create_simulation_tab(GtkWidget * tabs);
void set_neuron_param_entries(int neuron_type);










#endif
