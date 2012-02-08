#ifndef LAYER_NRN_GRP_NEURON_COMBO_H
#define LAYER_NRN_GRP_NEURON_COMBO_H


typedef struct __LayerNrnGrpNeuronCombo LayerNrnGrpNeuronCombo;

#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"
#include <stdbool.h>
#include <gtk/gtk.h>

struct __LayerNrnGrpNeuronCombo
{
	GtkWidget *combo_layer;
	GtkWidget *combo_neuron_group;
	GtkWidget *combo_neuron;
	unsigned int num_of_texts_in_layer_combo;
	unsigned int num_of_texts_in_neuron_group_combo;
	unsigned int num_of_texts_in_neuron_combo;
	bool updating;
};

LayerNrnGrpNeuronCombo* allocate_layer_neuron_group_neuron_combos(void);
bool update_texts_of_combos_when_add_remove(LayerNrnGrpNeuronCombo *combos, Network *network);
bool update_texts_of_combos_when_change(LayerNrnGrpNeuronCombo *combos, Network *network, GtkWidget *changed_combo);







#endif
