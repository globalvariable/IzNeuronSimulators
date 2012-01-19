#ifndef SIMULATION_TAB_H
#define SIMULATION_TAB_H


#include <gtk/gtk.h>
#include <stdbool.h>

#include "NeuroSimNetwork.h"
#include "NeuroSimSpikePatterns.h"
#include "../Library/Network/Network.h"
#include "../Library/IzNeuron/NeuronTypes.h"
#include "../Library/IzNeuronDynamics/NeuronDynamicsGraph.h"
#include "../Library/IzNeuronDynamics/NeuronDynamicsCombo.h"
#include "../../SpikePatternGenerator/DataHandling/DataHandling.h"
#include "../Library/ExtNetwork/ExtNetwork.h"


bool create_simulation_tab(GtkWidget * tabs);
void set_neuron_param_entries(int neuron_type);
void set_path_for_btn_select_directory_load_spike_pattern_generator_data(void);
bool write_path_for_btn_select_directory_load_spike_pattern_generator_data(char *path);







#endif
