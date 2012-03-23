#include "NeuronDynamicsCombo.h"

NeuronDynamicsCombo* allocate_neuron_dynamics_combo(GtkWidget *hbox, NeuronDynamicsCombo *combo)
{
	if (combo != NULL)
	{
		print_message(ERROR_MSG ,"NeuroSim", "NeuronDynamicsCombo", "allocate_neuron_dynamics_combo", "combo != NULL");
		return combo;
	}

	combo = g_new0(NeuronDynamicsCombo,1);
	combo->combo = gtk_combo_box_new_text();

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo->combo), "v");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo->combo), "u");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo->combo), "e");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo->combo), "i");

	gtk_combo_box_set_active(GTK_COMBO_BOX(combo->combo), 0);
 
	gtk_box_pack_start (GTK_BOX (hbox), combo->combo, TRUE, TRUE, 0);

	return combo;
}

bool neuron_dynamics_combo_get_selected(NeuronDynamicsCombo *combo, int *idx)
{
	*idx =  gtk_combo_box_get_active (GTK_COMBO_BOX(combo->combo));		
	return TRUE;
}


