#include "Eligibility.h"



bool create_ps_eligibility_for_neuron_group(Network *network, unsigned int layer, unsigned int nrn_grp, unsigned int parker_sochacki_max_order,  double eligibility_tau_max, double  eligibility_tau_min, double depol_eligibility_change_max, double  depol_eligibility_change_min, double max_eligibility, double eligibility_rate)
{
	unsigned int i, num_of_neurons = 0;
	char msg[200];
	Neuron *neuron = NULL;
	if (is_neuron_group_free (network, layer, nrn_grp))
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron_group", "is_neuron_group_free ().");
	if (! get_num_of_neurons_in_neuron_group(network, layer, nrn_grp, &num_of_neurons))
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron_group", "! get_num_of_neurons_in_neuron_group().");
	for (i = 0; i < num_of_neurons; i++)
	{
		neuron = get_neuron_address(network, layer, nrn_grp, i);
		if (! create_ps_eligibility_for_neuron(neuron , parker_sochacki_max_order, eligibility_tau_max, eligibility_tau_min, depol_eligibility_change_max, depol_eligibility_change_min, max_eligibility, eligibility_rate))
			return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron_group", "! create_ps_eligibility_for_neuron().");					
	}
	sprintf(msg, "Created Eligibility data for %u neurons in Layer: %u NeuronGroup: %u.", num_of_neurons, layer, nrn_grp);
	print_message(INFO_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron_group", msg);
	return TRUE;
}

bool create_ps_eligibility_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order, double eligibility_tau_max, double  eligibility_tau_min, double depol_eligibility_change_max, double  depol_eligibility_change_min, double max_eligibility, double eligibility_rate)
{
	unsigned int i, j, num_of_synapses;
	EligibilityList		*eligibility_list;

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron", "parker_sochacki_max_order <= 0.");
	if (eligibility_tau_max < eligibility_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron", "eligibility_tau_max < eligibility_tau_min.");
	if (eligibility_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron", "eligibility_tau_max <= 0.");
	if (eligibility_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron", "eligibility_tau_min <= 0.");

	if (depol_eligibility_change_max < depol_eligibility_change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron", "depol_eligibility_change_max < depol_eligibility_change_min.");
	if (depol_eligibility_change_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron", "depol_eligibility_change_max <= 0.");
	if (depol_eligibility_change_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron", "depol_eligibility_change_min <= 0.");

	if ((eligibility_rate < 0) || (eligibility_rate > 1))
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron", "(eligibility_rate < 0) || (eligibility_rate > 1).");

	if (max_eligibility < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron", "max_eligibility < 0.");

	if (max_eligibility < depol_eligibility_change_max)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron", "(max_eligibility < depol_eligibility_change_max).");

	eligibility_list = neuron->eligibility_list;
	num_of_synapses = neuron->syn_list->num_of_synapses;

	eligibility_list->eligibility = g_new0(double, num_of_synapses);
	eligibility_list->eligibility_saved = g_new0(double, num_of_synapses);
	eligibility_list->eligibility_decay_rate = g_new0(double, num_of_synapses);
	eligibility_list->eligibility_iter_prev = g_new0(double, num_of_synapses);
	eligibility_list->eligibility_iter_curr = g_new0(double, num_of_synapses);
	eligibility_list->eligibility_pol_vals = g_new0(double*, num_of_synapses);
	eligibility_list->eligibility_decay_rate_pol_vals = g_new0(double*, num_of_synapses);
	eligibility_list->depol_eligibility_change = g_new0(double, num_of_synapses);

	for (i = 0; i < num_of_synapses; i++)
	{
		eligibility_list->eligibility_decay_rate[i] = -1.0 / ( ( (eligibility_tau_max-eligibility_tau_min) * get_rand_number() ) + eligibility_tau_min );
		eligibility_list->eligibility_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		eligibility_list->eligibility_decay_rate_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		for (j = 0; j < parker_sochacki_max_order + 1; j++)
		{
			eligibility_list->eligibility_decay_rate_pol_vals[i][j] = eligibility_list->eligibility_decay_rate[i]/(j+1);	
		}
		eligibility_list->depol_eligibility_change[i] =  ((depol_eligibility_change_max-depol_eligibility_change_min) * get_rand_number())  + depol_eligibility_change_min;
	}
	eligibility_list->max_eligibility = max_eligibility;	
	eligibility_list->eligibility_rate = eligibility_rate;

	return TRUE;
}
bool submit_new_ps_eligibility_vals_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order,  double eligibility_tau_max, double  eligibility_tau_min, double depol_eligibility_change_max, double  depol_eligibility_change_min, double max_eligibility, double eligibility_rate)
{
	unsigned int i, j, num_of_synapses;
	EligibilityList		*eligibility_list;

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "parker_sochacki_max_order <= 0.");
	if (eligibility_tau_max < eligibility_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "eligibility_tau_max < eligibility_tau_min.");
	if (eligibility_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "eligibility_tau_max <= 0.");
	if (eligibility_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "eligibility_tau_min <= 0.");

	if (depol_eligibility_change_max < depol_eligibility_change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "depol_eligibility_change_max < depol_eligibility_change_min.");
	if (depol_eligibility_change_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "depol_eligibility_change_max <= 0.");
	if (depol_eligibility_change_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "depol_eligibility_change_max <= 0.");

	if ((eligibility_rate < 0) || (eligibility_rate > 1))
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "(eligibility_rate < 0) || (eligibility_rate > 1).");

	if (max_eligibility < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "max_eligibility < 0.");

	if (max_eligibility < depol_eligibility_change_max)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "(max_eligibility < depol_eligibility_change_max).");

	eligibility_list = neuron->eligibility_list;
	num_of_synapses = neuron->syn_list->num_of_synapses;

	for (i = 0; i < num_of_synapses; i++)
	{
		if (neuron->stdp_list->change_stdp_pre_post[i] > max_eligibility)
			return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "neuron->stdp_list->change_stdp_pre_post[i] > max_eligibility.");
		if (fabs(neuron->stdp_list->change_stdp_post_pre[i]) > max_eligibility)
			return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "fabs(neuron->stdp_list->change_stdp_post_pre[i]) > max_eligibility.");
	}

	for (i = 0; i < num_of_synapses; i++)
	{
		eligibility_list->eligibility_decay_rate[i] = -1.0 / ( ( (eligibility_tau_max-eligibility_tau_min) * get_rand_number() ) + eligibility_tau_min );
		for (j = 0; j < parker_sochacki_max_order + 1; j++)
		{
			eligibility_list->eligibility_decay_rate_pol_vals[i][j] = eligibility_list->eligibility_decay_rate[i]/(j+1);	
		}
		eligibility_list->depol_eligibility_change[i] =  ((depol_eligibility_change_max-depol_eligibility_change_min) * get_rand_number())  + depol_eligibility_change_min;
	}

	eligibility_list->max_eligibility = max_eligibility;	
	eligibility_list->eligibility_rate = eligibility_rate;
	return TRUE;
}

bool submit_new_ps_eligibility_vals_for_synapse(Neuron* neuron , unsigned int parker_sochacki_max_order,  double eligibility_tau_max, double  eligibility_tau_min, unsigned int synapse, double depol_eligibility_change_max, double  depol_eligibility_change_min, double max_eligibility, double eligibility_rate)
{
	unsigned int i, j;
	EligibilityList		*eligibility_list;

	if (synapse >= neuron->syn_list->num_of_synapses)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "synapse >= neuron->syn_list->num_of_synapses.");

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "parker_sochacki_max_order <= 0.");
	if (eligibility_tau_max < eligibility_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "eligibility_tau_max < eligibility_tau_min.");
	if (eligibility_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "eligibility_tau_max <= 0.");
	if (eligibility_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "eligibility_tau_min <= 0.");

	if (depol_eligibility_change_max < depol_eligibility_change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_synapse", "depol_eligibility_change_max < depol_eligibility_change_min.");
	if (depol_eligibility_change_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_synapse", "depol_eligibility_change_max <= 0.");
	if (depol_eligibility_change_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_synapse", "depol_eligibility_change_min <= 0.");

	if ((eligibility_rate < 0) || (eligibility_rate > 1))
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_synapse", "(eligibility_rate < 0) || (eligibility_rate > 1).");

	if (max_eligibility < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_synapse", "max_eligibility < 0.");

	if (max_eligibility < depol_eligibility_change_max)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_synapse", "(max_eligibility < depol_eligibility_change_max).");

	eligibility_list = neuron->eligibility_list;
	i = synapse;

	if (neuron->stdp_list->change_stdp_pre_post[i] > max_eligibility)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "neuron->stdp_list->change_stdp_pre_post[i] > max_eligibility.");
	if (fabs(neuron->stdp_list->change_stdp_post_pre[i]) > max_eligibility)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_eligibility_vals_for_neuron", "fabs(neuron->stdp_list->change_stdp_post_pre[i]) > max_eligibility.");

	eligibility_list->eligibility_decay_rate[i] = -1.0 / ( ( (eligibility_tau_max-eligibility_tau_min) * get_rand_number() ) + eligibility_tau_min );
	for (j = 0; j < parker_sochacki_max_order + 1; j++)
	{
		eligibility_list->eligibility_decay_rate_pol_vals[i][j] = eligibility_list->eligibility_decay_rate[i]/(j+1);	
	}
	eligibility_list->depol_eligibility_change[i] =  ((depol_eligibility_change_max-depol_eligibility_change_min) * get_rand_number())  + depol_eligibility_change_min;

	eligibility_list->max_eligibility = max_eligibility;	
	eligibility_list->eligibility_rate = eligibility_rate;

	return TRUE;
}

void clear_eligibility_for_neuron(Neuron *neuron)
{
	unsigned int i, num_of_synapses;
	double		*eligibility;

	eligibility = neuron->eligibility_list->eligibility;
	num_of_synapses = neuron->syn_list->num_of_synapses;
	for (i = 0; i < num_of_synapses; i++)
	{
		eligibility[i] = 0;
	}		
}

void save_eligibility_for_neuron(Neuron *neuron)
{
	unsigned int i, num_of_synapses;
	double		*eligibility;
	double		*eligibility_saved;
	eligibility = neuron->eligibility_list->eligibility;
	eligibility_saved = neuron->eligibility_list->eligibility_saved;
	num_of_synapses = neuron->syn_list->num_of_synapses;
	for (i = 0; i < num_of_synapses; i++)
	{
		eligibility_saved[i] = eligibility[i];
	}
}	
