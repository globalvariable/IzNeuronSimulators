#include "NeuronDynamicsCombo.h"

static GtkWidget **combos = NULL;
static int num_of_combos = 0;

bool add_neuron_dynamics_combo(GtkWidget *hbox, int *combo_idx)
{
	int i;;
	GtkWidget **lcl_combos = NULL;

	is_allocated(combos, TRUE, "NeuronDynamicsCombo", "add_neuron_dynamics_combo", ALLOCATION_WARNING, "This is the first time for allocation of neuron dynamics combos.", EXPECTS_ALLOCATED);	

	lcl_combos = g_new0(GtkWidget*, num_of_combos + 1);	

	if (!is_allocated(lcl_combos, TRUE, "NeuronDynamicsCombo", "add_neuron_dynamics_combo", ALLOCATION_FAILED, "lcl_combos", EXPECTS_ALLOCATED))	
		return FALSE;

	for (i = 0; i< num_of_combos; i++)
	{
		lcl_combos[i] = combos[i];
	}		
	g_free(combos);
	combos = lcl_combos;

	if (is_allocated(combos[num_of_combos], TRUE, "NeuronDynamicsCombo", "add_neuron_dynamics_combo", ALLOCATION_BUG, "combos[num_of_combos]", EXPECTS_NOT_ALLOCATED))	
		return FALSE;	
	combos[num_of_combos] = gtk_combo_box_new_text();
	if (!is_allocated(combos[num_of_combos], TRUE, "NeuronDynamicsCombo", "add_neuron_dynamics_combo", ALLOCATION_FAILED, "combos[num_of_combos]", EXPECTS_ALLOCATED))	
		return FALSE;	

	gtk_combo_box_append_text(GTK_COMBO_BOX(combos[num_of_combos]), "v");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combos[num_of_combos]), "u");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combos[num_of_combos]), "e");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combos[num_of_combos]), "i");
		
 	gtk_combo_box_set_active(GTK_COMBO_BOX(combos[num_of_combos]), 0);
 	
	gtk_box_pack_start (GTK_BOX (hbox), combos[num_of_combos], TRUE, TRUE, 0);
	
	*combo_idx = num_of_combos;	
	num_of_combos ++;
	
	printf ("NeuronDynamicsCombo: INFO: Number of total combos in neuron dynamics combo_boxes inventory is %d\n", num_of_combos);	
	
	return TRUE;
}

