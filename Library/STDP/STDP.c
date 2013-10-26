#include "STDP.h"


bool create_ps_pre_post_stdp_for_neuron_group(Network *network, unsigned int layer, unsigned int nrn_grp, double change_min, double change_max, double tau_min, double tau_max)
{
	char msg[200];
	unsigned int i, num_of_neurons = 0;
	Neuron *neuron = NULL;
	if (is_neuron_group_free (network, layer, nrn_grp))
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_neuron_group", "is_neuron_group_free ().");
	if (! get_num_of_neurons_in_neuron_group(network, layer, nrn_grp, &num_of_neurons))
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_neuron_group", "! get_num_of_neurons_in_neuron_group().");
	for (i = 0; i < num_of_neurons; i++)
	{
		neuron = get_neuron_address(network, layer, nrn_grp, i);
		if (! create_ps_pre_post_stdp_for_neuron(neuron, change_min, change_max, tau_min, tau_max))
			return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_neuron_group", "! create_ps_stdp_for_neuron().");					
	}
	sprintf(msg, "Created STDP Pre Post data for %u neurons in Layer: %u NeuronGroup: %u.", num_of_neurons, layer, nrn_grp);
	print_message(INFO_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_neuron_group", msg);
	return TRUE;
}

bool create_ps_pre_post_stdp_for_neuron(Neuron* neuron , double change_min, double change_max, double tau_min, double tau_max)
{
	unsigned int i, j, num_of_synapses;

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_neuron", "parker_sochacki_max_order <= 0.");
	if (change_max < change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_neuron", "change_max < change_min.");
	if (change_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_neuron", "change_max < 0.");
	if (change_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_neuron", "change_min < 0.");
	if (tau_max < tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_neuron", "tau_max < tau_min.");
	if (tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_neuron", "tau_max <= 0.");
	if (tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_neuron", "tau_min <= 0.");

	num_of_synapses = neuron->syn_list->num_of_synapses;

	for (i = 0; i < num_of_synapses; i++)
	{
		if (neuron->syn_list->synapses[i].plastic)
		{
			neuron->syn_list->synapses[i].ps_stdp_pre_post = g_new0(PsStdpPrePost, 1);
			neuron->syn_list->synapses[i].ps_stdp_pre_post->change_amount =  ( (change_max-change_min) * get_rand_number() ) + change_min;
			neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate = -1.0 / ( ( (tau_max-tau_min) * get_rand_number() ) + tau_min ) ;
			neuron->syn_list->synapses[i].ps_stdp_pre_post->ps_pol_vals = g_new0(double, parker_sochacki_max_order+1);
			neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate_ps_pol_vals = g_new0(double, parker_sochacki_max_order+1);
			for (j = 0; j < parker_sochacki_max_order + 1; j++)
			{
				neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate_ps_pol_vals[j] = neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate/(j+1);	
			}
		}
	}
	return TRUE;
}

bool create_ps_pre_post_stdp_for_synapse(Neuron* neuron , double change_min, double change_max, double tau_min, double tau_max, unsigned int synapse)
{
	unsigned int i, j;

	if (synapse >= neuron->syn_list->num_of_synapses)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_synapse", "synapse >= neuron->syn_list->num_of_synapses.");

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_synapse", "parker_sochacki_max_order <= 0.");
	if (change_max < change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_synapse", "change_max < change_min.");
	if (change_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_synapse", "change_max < 0.");
	if (change_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_synapse", "change_min < 0.");
	if (tau_max < tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_synapse", "tau_max < tau_min.");
	if (tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_synapse", "tau_max <= 0.");
	if (tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_synapse", "tau_min <= 0.");

	i = synapse; 

	if (! neuron->syn_list->synapses[i].plastic)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_pre_post_stdp_for_neuron", "! neuron->syn_list->synapses[i].plastic.");

	neuron->syn_list->synapses[i].ps_stdp_pre_post = g_new0(PsStdpPrePost, 1);
	neuron->syn_list->synapses[i].ps_stdp_pre_post->change_amount =  ( (change_max-change_min) * get_rand_number() ) + change_min;
	neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate = -1.0 / ( ( (tau_max-tau_min) * get_rand_number() ) + tau_min ) ;
	neuron->syn_list->synapses[i].ps_stdp_pre_post->ps_pol_vals = g_new0(double, parker_sochacki_max_order+1);
	neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate_ps_pol_vals = g_new0(double, parker_sochacki_max_order+1);
	for (j = 0; j < parker_sochacki_max_order + 1; j++)
	{
		neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate_ps_pol_vals[j] = neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate/(j+1);	
	}

	return TRUE;
}



bool allocate_ps_pre_post_stdp_for_neuron(Neuron* neuron)
{
	unsigned int i, num_of_synapses;

	num_of_synapses = neuron->syn_list->num_of_synapses;

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "allocate_ps_pre_post_stdp_for_neuron", "parker_sochacki_max_order <= 0.");

	for (i = 0; i < num_of_synapses; i++)
	{
		if (neuron->syn_list->synapses[i].plastic)
		{
			if (neuron->syn_list->synapses[i].ps_stdp_pre_post != NULL)
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "STDP", "allocate_ps_pre_post_stdp_for_neuron", "neuron->syn_list->synapses[i].ps_stdp_pre_post != NULL.");
			neuron->syn_list->synapses[i].ps_stdp_pre_post = g_new0(PsStdpPrePost, 1);
			neuron->syn_list->synapses[i].ps_stdp_pre_post->ps_pol_vals = g_new0(double, parker_sochacki_max_order+1);
			neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate_ps_pol_vals = g_new0(double, parker_sochacki_max_order+1);
		}
	}
	return TRUE;
}

bool submit_new_ps_pre_post_stdp_vals_for_neuron(Neuron* neuron, double change_min, double change_max, double tau_min, double tau_max)
{
	unsigned int i, j, num_of_synapses;

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_neuron", "parker_sochacki_max_order <= 0.");
	if (change_max < change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_neuron", "change_max < change_min.");
	if (change_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_neuron", "change_max < 0.");
	if (change_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_neuron", "change_min < 0.");
	if (tau_max < tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_neuron", "tau_max < tau_min.");
	if (tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_neuron", "tau_max <= 0.");
	if (tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_neuron", "tau_min <= 0.");

	num_of_synapses = neuron->syn_list->num_of_synapses;

	for (i = 0; i < num_of_synapses; i++)
	{
		if (neuron->syn_list->synapses[i].plastic)
		{
			neuron->syn_list->synapses[i].ps_stdp_pre_post->change_amount =  ( (change_max-change_min) * get_rand_number() ) + change_min;
			neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate = -1.0 / ( ( (tau_max-tau_min) * get_rand_number() ) + tau_min ) ;
			for (j = 0; j < parker_sochacki_max_order + 1; j++)
			{
				neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate_ps_pol_vals[j] = neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate/(j+1);	
			}
		}
	}
	return TRUE;
}

bool submit_new_ps_pre_post_stdp_vals_for_synapse(Neuron* neuron , double change_min, double change_max, double tau_min, double tau_max, unsigned int synapse)
{
	unsigned int i, j;

	if (synapse >= neuron->syn_list->num_of_synapses)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_synapse", "synapse >= neuron->syn_list->num_of_synapses.");

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_synapse", "parker_sochacki_max_order <= 0.");
	if (change_max < change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_synapse", "change_max < change_min.");
	if (change_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_synapse", "change_max < 0.");
	if (change_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_synapse", "change_min < 0.");
	if (tau_max < tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_synapse", "tau_max < tau_min.");
	if (tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_synapse", "tau_max <= 0.");
	if (tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_synapse", "tau_min <= 0.");

	i = synapse;

	if (! neuron->syn_list->synapses[i].plastic)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_synapse", "! neuron->syn_list->synapses[i].plastic.");

	neuron->syn_list->synapses[i].ps_stdp_pre_post->change_amount =  ( (change_max-change_min) * get_rand_number() ) + change_min;
	neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate = -1.0 / ( ( (tau_max-tau_min) * get_rand_number() ) + tau_min ) ;
	for (j = 0; j < parker_sochacki_max_order + 1; j++)
	{
		neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate_ps_pol_vals[j] = neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate/(j+1);	
	}

	return TRUE;
}

bool submit_new_ps_pre_post_stdp_vals_for_neuron_according_to_pre_synaptic_neuron_group(Neuron* neuron , Network *axon_from_network, unsigned int axon_from_layer, unsigned int axon_from_nrn_grp, double change_min, double change_max, double tau_min, double tau_max)
{
	unsigned int i, j, num_of_synapses;

	if (change_max < change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_neuron_according_to_pre_synaptic_neuron_group", "change_max < change_min.");
	if (change_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_neuron_according_to_pre_synaptic_neuron_group", "change_max < 0.");
	if (change_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_neuron_according_to_pre_synaptic_neuron_group", "change_min < 0.");
	if (tau_max < tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_neuron_according_to_pre_synaptic_neuron_group", "tau_max < tau_min.");
	if (tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_neuron_according_to_pre_synaptic_neuron_group", "tau_max <= 0.");
	if (tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_neuron_according_to_pre_synaptic_neuron_group", "tau_min <= 0.");

	num_of_synapses = neuron->syn_list->num_of_synapses;

	for (i = 0; i < num_of_synapses; i++)
	{
		if ((neuron->syn_list->synapses[i].axon_from_network != axon_from_network) || (neuron->syn_list->synapses[i].axon_from_layer != axon_from_layer) || (neuron->syn_list->synapses[i].axon_from_neuron_group != axon_from_nrn_grp))
		{
			continue;
		}
		if (! neuron->syn_list->synapses[i].plastic)
			return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_pre_post_stdp_vals_for_neuron_according_to_pre_synaptic_neuron_group", "! neuron->syn_list->synapses[i].plastic.");

		neuron->syn_list->synapses[i].ps_stdp_pre_post->change_amount =  ( (change_max-change_min) * get_rand_number() ) + change_min;
		neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate = -1.0 / ( ( (tau_max-tau_min) * get_rand_number() ) + tau_min ) ;
		for (j = 0; j < parker_sochacki_max_order + 1; j++)
		{
			neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate_ps_pol_vals[j] = neuron->syn_list->synapses[i].ps_stdp_pre_post->decay_rate/(j+1);	
		}
	}
	return TRUE;


}
