#ifndef INJECTION_CURRENT_VIEW_H
#define INJECTION_CURRENT_VIEW_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdlib.h>
#include "BMISimulationSpikeGenerator.h"
#include "../../Library/Network/Network.h"
#include "../../Library/IzNeuron/Neuron.h"
#include "../../Library/IzNeuron/NeuronTypes.h"
#include "../../../BlueSpike/Library/Misc/Randn.h"
#include "../../../ExperimentControllers/TrialControllers/Library/Combos/TrialTypesCombo.h"
#include "../../Library/Combos/LayerNrnGrpNeuronCombo.h"
#include "../../Library/SpikeGenData/SpikeGenData.h"
#include "../../Library/Graphs/CurrentPatternGraph.h"
#include "../../Library/Graphs/NeuronDynamicsGraph.h"

#define LINE 0
#define SIN 1
#define COS 2
#define CLEAR 3
#define pi 3.141592653589793

bool create_current_pattern_view_gui(GtkWidget *tabs);

void interrogate_network_button_func(void); 		
void interrogate_neuron_button_func(void);




void add_noise_button_func(void);
void copy_drawn_to_raw_stimuli_button_func(void);
void simulate_button_func(void);
void display_neuron_dynamics_button_func(void);
void display_neuron_dynamics(void);
void create_directory_button_func(void);
void save_button_func(void);
void load_button_func(void);
void fill_notes_text_view(void);
void set_directory_btn_select_directory(void);
void set_neuron_param_entries(int neuron_type);







#endif
