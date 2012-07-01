#include "STDPCombo.h"

STDPCombo* allocate_stdp_combo(GtkWidget *hbox, STDPCombo *combo)
{
	if (combo != NULL)
	{
		print_message(ERROR_MSG ,"IzNeuronSimulators", "STDPCombo", "allocate_stdp_combo", "combo != NULL");
		return combo;
	}

	combo = g_new0(STDPCombo,1);
	combo->combo = gtk_combo_box_new_text();

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo->combo), "pre_post");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo->combo), "post_pre");

	gtk_combo_box_set_active(GTK_COMBO_BOX(combo->combo), 0);
 
	gtk_box_pack_start (GTK_BOX (hbox), combo->combo, TRUE, TRUE, 0);

	return combo;
}

bool stdp_combo_get_selected(STDPCombo *combo, int *idx)
{
	*idx =  gtk_combo_box_get_active (GTK_COMBO_BOX(combo->combo));		
	return TRUE;
}

