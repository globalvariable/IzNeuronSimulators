#ifndef LAYER_NRN_GRP_NRN_SYNAPSE_COMBO_H
#define LAYER_NRN_GRP_NRN_SYNAPSE_COMBO_H


typedef struct __LayerNrnGrpNrnSynapseCombo LayerNrnGrpNrnSynapseCombo;

#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"
#include <stdbool.h>
#include <gtk/gtk.h>

struct __LayerNrnGrpNrnSynapseCombo
{
	GtkWidget *combo_layer;
	GtkWidget *combo_neuron_group;
	GtkWidget *combo_neuron;
	GtkWidget *combo_synapse;
	unsigned int num_of_texts_in_layer_combo;
	unsigned int num_of_texts_in_neuron_group_combo;
	unsigned int num_of_texts_in_neuron_combo;
	unsigned int num_of_texts_in_synapse_combo;
	bool updating;
};

LayerNrnGrpNrnSynapseCombo* allocate_layer_neuron_group_neuron_synapse_combos(void);
bool update_texts_of_synapse_combos_when_add_remove(LayerNrnGrpNrnSynapseCombo *combos, Network *network);
bool update_texts_of_synapse_combos_when_change(LayerNrnGrpNrnSynapseCombo *combos, Network *network, GtkWidget *changed_combo);
bool layer_neuron_group_neuron_synapse_get_selected(LayerNrnGrpNrnSynapseCombo *combos, unsigned int *layer_num, unsigned int *nrn_grp_num, unsigned int *nrn_num, unsigned int *syn_num);







#endif
