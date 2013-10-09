#include "LayerNrnGrpNeuronCombo.h"






LayerNrnGrpNeuronCombo* allocate_layer_neuron_group_neuron_combos(void)
{
	LayerNrnGrpNeuronCombo* combos;

	combos = g_new0(LayerNrnGrpNeuronCombo, 1);
	combos->combo_layer = gtk_combo_box_new_text();
	combos->combo_neuron_group = gtk_combo_box_new_text();	
	combos->combo_neuron = gtk_combo_box_new_text();		
	return combos;
}

bool update_texts_of_combos_when_add_remove(LayerNrnGrpNeuronCombo *combos, Network *network)
{
	int i;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
 	char temp[20];	
 	
 	combos->updating =1;	
 	
	for (i=combos->num_of_texts_in_layer_combo; i >0 ; i--)
		gtk_combo_box_remove_text (GTK_COMBO_BOX(combos->combo_layer), i-1);
	
	for (i=combos->num_of_texts_in_neuron_group_combo; i > 0; i--)
		gtk_combo_box_remove_text (GTK_COMBO_BOX(combos->combo_neuron_group),i-1);
		
	for (i=combos->num_of_texts_in_neuron_combo; i > 0; i--)
		gtk_combo_box_remove_text (GTK_COMBO_BOX(combos->combo_neuron), i-1 );
 
	if (!get_num_of_layers_in_network(network, &num_of_layers))
		return print_message(ERROR_MSG ,"NeuroSim", "NeuroSimCombos", "update_texts_of_combos_when_add_remove", NULL);
	
	if (num_of_layers == 0)
		return print_message(ERROR_MSG ,"NeuroSim", "NeuroSimCombos", "construct_texts_of_combos", "Couldn' t construct layer combo texts; num_of_layers = 0");	

	if (!get_num_of_neuron_groups_in_layer(network, 0, &num_of_neuron_groups_in_layer))
		return print_message(ERROR_MSG ,"NeuroSim", "NeuroSimCombos", "construct_texts_of_combos", NULL);	
	
	if (num_of_neuron_groups_in_layer == 0)	
		return print_message(ERROR_MSG ,"NeuroSim", "NeuroSimCombos", "construct_texts_of_combos", "Couldn' t construct neuron group combo texts; num_of_neuron_groups_in_layer 0 is 0");
		
	if (!get_num_of_neurons_in_neuron_group(network,0, 0, &num_of_neurons_in_neuron_group))
		return print_message(ERROR_MSG ,"NeuroSim", "NeuroSimCombos", "construct_texts_of_combos", NULL);			

	if (num_of_neurons_in_neuron_group == 0)	
		return print_message(ERROR_MSG ,"NeuroSim", "NeuroSimCombos", "construct_texts_of_combos", "Couldn' t construct neuron group combo texts; num of neurons in layer 0, neuron group 0 is 0");

	for (i=0; i<num_of_layers; i++)
	{
		sprintf(temp, "%d" , i);
	 	gtk_combo_box_append_text(GTK_COMBO_BOX(combos->combo_layer), temp);
	} 
			
	for (i=0; i < num_of_neuron_groups_in_layer ; i++)
	{
		sprintf(temp, "%d" , i);
	 	gtk_combo_box_append_text(GTK_COMBO_BOX(combos->combo_neuron_group), temp);
	} 
	
	for (i=0; i < num_of_neurons_in_neuron_group ; i++)
	{
		sprintf(temp, "%d" , i);
	 	gtk_combo_box_append_text(GTK_COMBO_BOX(combos->combo_neuron), temp);
	}
	
	combos->num_of_texts_in_layer_combo = num_of_layers;
	combos->num_of_texts_in_neuron_group_combo = num_of_neuron_groups_in_layer;
	combos->num_of_texts_in_neuron_combo = num_of_neurons_in_neuron_group;	

	gtk_combo_box_set_active(GTK_COMBO_BOX(combos->combo_layer), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(combos->combo_neuron_group), 0);	
	gtk_combo_box_set_active(GTK_COMBO_BOX(combos->combo_neuron), 0);	

 	combos->updating = 0;
 			
	return TRUE;
}

bool update_texts_of_combos_when_change(LayerNrnGrpNeuronCombo *combos, Network *network, GtkWidget *changed_combo)
{
	int i;
	unsigned int num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
 	char temp[20];	
 	int layer_num; 
 	int neuron_group_num; 

	if (combos->updating)
		return TRUE;
	if (changed_combo == combos->combo_layer)
	{
		layer_num =  gtk_combo_box_get_active (GTK_COMBO_BOX(combos->combo_layer));
		if (layer_num < 0)
			return TRUE;
		for (i=combos->num_of_texts_in_neuron_group_combo; i > 0; i--)
			gtk_combo_box_remove_text (GTK_COMBO_BOX(combos->combo_neuron_group),i-1);
		if (!get_num_of_neuron_groups_in_layer(network, layer_num, &num_of_neuron_groups_in_layer))
			return print_message(ERROR_MSG ,"NeuroSim", "NeuroSimCombos", " update_texts_of_combos_when_change", NULL);	
		if (num_of_neuron_groups_in_layer == 0)	
			return print_message(ERROR_MSG ,"NeuroSim", "NeuroSimCombos", " update_texts_of_combos_when_change", "num_of_neuron_groups_in_layer 0");			
		for (i=0; i < num_of_neuron_groups_in_layer ; i++)
		{
			sprintf(temp, "%d" , i);
	 		gtk_combo_box_append_text(GTK_COMBO_BOX(combos->combo_neuron_group), temp);
		} 
		combos->num_of_texts_in_neuron_group_combo = num_of_neuron_groups_in_layer;	
		gtk_combo_box_set_active(GTK_COMBO_BOX(combos->combo_neuron_group), 0);	
		gtk_combo_box_set_active(GTK_COMBO_BOX(combos->combo_neuron), 0);	
	}
	else if (changed_combo == combos->combo_neuron_group)
	{
		layer_num =  gtk_combo_box_get_active (GTK_COMBO_BOX(combos->combo_layer));	
		if (layer_num < 0)
			return TRUE;		
 		neuron_group_num = gtk_combo_box_get_active (GTK_COMBO_BOX(combos->combo_neuron_group));
		if (neuron_group_num < 0)
			return TRUE;	 		
		for (i=combos->num_of_texts_in_neuron_combo; i > 0; i--)
			gtk_combo_box_remove_text (GTK_COMBO_BOX(combos->combo_neuron), i-1 );
		if (!get_num_of_neurons_in_neuron_group(network, layer_num, neuron_group_num  , &num_of_neurons_in_neuron_group))
			return print_message(ERROR_MSG ,"NeuroSim", "NeuroSimCombos", "update_texts_of_combos_when_change", NULL);		
		if (num_of_neurons_in_neuron_group == 0)	
			return print_message(ERROR_MSG ,"NeuroSim", "NeuroSimCombos", " update_texts_of_combos_when_change", "num of neurons in neuron group is 0.");		
		for (i=0; i < num_of_neurons_in_neuron_group ; i++)
		{
			sprintf(temp, "%d" , i);
		 	gtk_combo_box_append_text(GTK_COMBO_BOX(combos->combo_neuron), temp);
		}
		combos->num_of_texts_in_neuron_combo = num_of_neurons_in_neuron_group;	
		gtk_combo_box_set_active(GTK_COMBO_BOX(combos->combo_neuron), 0);	
	}
	return TRUE;
}

bool layer_neuron_group_neuron_get_selected(LayerNrnGrpNeuronCombo *combos, unsigned int *layer_num, unsigned int *nrn_grp_num, unsigned int *nrn_num)
{
	*layer_num =  gtk_combo_box_get_active (GTK_COMBO_BOX(combos->combo_layer));		
	*nrn_grp_num =  gtk_combo_box_get_active (GTK_COMBO_BOX(combos->combo_neuron_group));
	*nrn_num =  gtk_combo_box_get_active (GTK_COMBO_BOX(combos->combo_neuron));
	if ((int)(*layer_num) < 0)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "LayerNrnGrpNeuronCombo", "layer_neuron_group_neuron_get_selected", "Invalid *layer_num < 0.");		
	if ((int)(*nrn_grp_num)  < 0)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "LayerNrnGrpNeuronCombo", "layer_neuron_group_neuron_get_selected", "Invalid *nrn_grp_num < 0.");	
	if ((int)(*nrn_num)  < 0)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "LayerNrnGrpNeuronCombo", "layer_neuron_group_neuron_get_selected", "Invalid *nrn_num < 0.");
	return TRUE;
}
