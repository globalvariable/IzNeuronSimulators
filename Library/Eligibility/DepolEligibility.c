#include "DepolEligibility.h"



bool create_ps_depol_eligibility_for_neuron_group(Network *network, unsigned int layer, unsigned int nrn_grp, unsigned int parker_sochacki_max_order,  double depol_eligibility_tau_max, double  depol_eligibility_tau_min, double depol_eligibility_memb_v_coeff_max, double  depol_eligibility_memb_v_coeff_min, double depol_eligibility_memo_change_max, double depol_eligibility_memo_change_min, double depol_eligibility_memo_tau_max, double depol_eligibility_memo_tau_min)
{
	unsigned int i, num_of_neurons = 0;
	char msg[200];
	Neuron *neuron = NULL;
	if (is_neuron_group_free (network, layer, nrn_grp))
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_depol_eligibility_for_neuron_group", "is_neuron_group_free ().");
	if (! get_num_of_neurons_in_neuron_group(network, layer, nrn_grp, &num_of_neurons))
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_depol_eligibility_for_neuron_group", "! get_num_of_neurons_in_neuron_group().");
	for (i = 0; i < num_of_neurons; i++)
	{
		neuron = get_neuron_address(network, layer, nrn_grp, i);
		if (! create_ps_depol_eligibility_for_neuron(neuron , parker_sochacki_max_order, depol_eligibility_tau_max, depol_eligibility_tau_min, depol_eligibility_memb_v_coeff_max, depol_eligibility_memb_v_coeff_min, depol_eligibility_memo_change_max, depol_eligibility_memo_change_min, depol_eligibility_memo_tau_max, depol_eligibility_memo_tau_min))
			return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_depol_eligibility_for_neuron_group", "! create_ps_depol_eligibility_for_neuron().");					
	}
	sprintf(msg, "Created DepolEligibility data for %u neurons in Layer: %u NeuronGroup: %u.", num_of_neurons, layer, nrn_grp);
	print_message(INFO_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_depol_eligibility_for_neuron_group", msg);
	return TRUE;
}

bool create_ps_depol_eligibility_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order, double depol_eligibility_tau_max, double  depol_eligibility_tau_min, double depol_eligibility_memb_v_coeff_max, double  depol_eligibility_memb_v_coeff_min, double depol_eligibility_memo_change_max, double depol_eligibility_memo_change_min, double depol_eligibility_memo_tau_max, double depol_eligibility_memo_tau_min)
{
	unsigned int i, j, num_of_synapses;
	DepolEligibilityList		*depol_eligibility_list;

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "parker_sochacki_max_order <= 0.");
	if (depol_eligibility_tau_max < depol_eligibility_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_tau_max < depol_eligibility_tau_min.");
	if (depol_eligibility_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_tau_max <= 0.");
	if (depol_eligibility_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_tau_min <= 0.");

	if (depol_eligibility_memb_v_coeff_max < depol_eligibility_memb_v_coeff_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_memb_v_coeff_max < depol_eligibility_memb_v_coeff_min.");
	if (depol_eligibility_memb_v_coeff_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_memb_v_coeff_max <= 0.");
	if (depol_eligibility_memb_v_coeff_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_memb_v_coeff_min <= 0.");

	if (depol_eligibility_memo_change_max < depol_eligibility_memo_change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_memo_change_max < depol_eligibility_memo_change_min.");
	if (depol_eligibility_memo_change_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_memo_change_max <= 0.");
	if (depol_eligibility_memo_change_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_memo_change_min <= 0.");

	if (depol_eligibility_memo_tau_max < depol_eligibility_memo_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_memo_tau_max < depol_eligibility_memo_tau_min.");
	if (depol_eligibility_memo_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_memo_tau_max <= 0.");
	if (depol_eligibility_memo_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_memo_tau_min <= 0.");

	depol_eligibility_list = neuron->depol_eligibility_list;
	num_of_synapses = neuron->syn_list->num_of_synapses;

	depol_eligibility_list->depol_eligibility = g_new0(double, num_of_synapses);
	depol_eligibility_list->depol_eligibility_saved = g_new0(double, num_of_synapses);
	depol_eligibility_list->depol_eligibility_decay_rate = g_new0(double, num_of_synapses);
	depol_eligibility_list->depol_eligibility_v_coeff = g_new0(double, num_of_synapses);
	depol_eligibility_list->depol_eligibility_iter_prev = g_new0(double, num_of_synapses);
	depol_eligibility_list->depol_eligibility_iter_curr = g_new0(double, num_of_synapses);
	depol_eligibility_list->depol_eligibility_pol_vals = g_new0(double*, num_of_synapses);
	depol_eligibility_list->depol_eligibility_v_coeff_pol_vals = g_new0(double*, num_of_synapses);
	depol_eligibility_list->memo_depol_eligibility = g_new0(double, num_of_synapses);
	depol_eligibility_list->memo_depol_eligibility_change = g_new0(double, num_of_synapses);
	depol_eligibility_list->memo_depol_eligibility_decay_rate = g_new0(double, num_of_synapses);
	depol_eligibility_list->memo_depol_eligibility_iter_prev = g_new0(double, num_of_synapses);
	depol_eligibility_list->memo_depol_eligibility_iter_curr = g_new0(double, num_of_synapses);
	depol_eligibility_list->memo_depol_eligibility_pol_vals = g_new0(double*, num_of_synapses);
	depol_eligibility_list->memo_depol_eligibility_decay_rate_pol_vals = g_new0(double*, num_of_synapses);

	for (i = 0; i < num_of_synapses; i++)
	{
		depol_eligibility_list->depol_eligibility_decay_rate[i] = -1.0 / ( ( (depol_eligibility_tau_max-depol_eligibility_tau_min) * get_rand_number() ) + depol_eligibility_tau_min );
		depol_eligibility_list->depol_eligibility_v_coeff[i] = ( ( (depol_eligibility_memb_v_coeff_max-depol_eligibility_memb_v_coeff_min) * get_rand_number() ) + depol_eligibility_memb_v_coeff_min );
		depol_eligibility_list->depol_eligibility_decay_rate[i] = depol_eligibility_list->depol_eligibility_decay_rate[i] / depol_eligibility_list->depol_eligibility_v_coeff[i];
		depol_eligibility_list->depol_eligibility_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		depol_eligibility_list->depol_eligibility_v_coeff_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		for (j = 0; j < parker_sochacki_max_order + 1; j++)
		{
			depol_eligibility_list->depol_eligibility_v_coeff_pol_vals[i][j] = depol_eligibility_list->depol_eligibility_v_coeff[i]/(j+1);	
		}

		depol_eligibility_list->memo_depol_eligibility_change[i] = ( (depol_eligibility_memo_change_max-depol_eligibility_memo_change_min) * get_rand_number() ) + depol_eligibility_memo_change_min;
		depol_eligibility_list->memo_depol_eligibility_decay_rate[i] = -1.0 / ( ( (depol_eligibility_memo_tau_max-depol_eligibility_memo_tau_min) * get_rand_number() ) + depol_eligibility_memo_tau_min );
		depol_eligibility_list->memo_depol_eligibility_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		depol_eligibility_list->memo_depol_eligibility_decay_rate_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		for (j = 0; j < parker_sochacki_max_order + 1; j++)
		{
			depol_eligibility_list->memo_depol_eligibility_decay_rate_pol_vals[i][j] = depol_eligibility_list->memo_depol_eligibility_decay_rate[i] / (j+1);	
		}
	}
	return TRUE;
}

bool submit_new_ps_depol_eligibility_vals_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order, double depol_eligibility_tau_max, double  depol_eligibility_tau_min, double depol_eligibility_memb_v_coeff_max, double  depol_eligibility_memb_v_coeff_min, double depol_eligibility_memo_change_max, double depol_eligibility_memo_change_min, double depol_eligibility_memo_tau_max, double depol_eligibility_memo_tau_min)
{
	unsigned int i, j, num_of_synapses;
	DepolEligibilityList		*depol_eligibility_list;

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "parker_sochacki_max_order <= 0.");
	if (depol_eligibility_tau_max < depol_eligibility_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_tau_max < depol_eligibility_tau_min.");
	if (depol_eligibility_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_tau_max <= 0.");
	if (depol_eligibility_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_tau_min <= 0.");

	if (depol_eligibility_memb_v_coeff_max < depol_eligibility_memb_v_coeff_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memb_v_coeff_max < depol_eligibility_memb_v_coeff_min.");
	if (depol_eligibility_memb_v_coeff_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memb_v_coeff_max <= 0.");
	if (depol_eligibility_memb_v_coeff_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memb_v_coeff_min <= 0.");

	if (depol_eligibility_memo_change_max < depol_eligibility_memo_change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memo_change_max < depol_eligibility_memo_change_min.");
	if (depol_eligibility_memo_change_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memo_change_max <= 0.");
	if (depol_eligibility_memo_change_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memo_change_min <= 0.");

	if (depol_eligibility_memo_tau_max < depol_eligibility_memo_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memo_tau_max < depol_eligibility_memo_tau_min.");
	if (depol_eligibility_memo_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memo_tau_max <= 0.");
	if (depol_eligibility_memo_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memo_tau_min <= 0.");

	depol_eligibility_list = neuron->depol_eligibility_list;
	num_of_synapses = neuron->syn_list->num_of_synapses;

	for (i = 0; i < num_of_synapses; i++)
	{
		depol_eligibility_list->depol_eligibility_decay_rate[i] = -1.0 / ( ( (depol_eligibility_tau_max-depol_eligibility_tau_min) * get_rand_number() ) + depol_eligibility_tau_min );
		depol_eligibility_list->depol_eligibility_v_coeff[i] = ( ( (depol_eligibility_memb_v_coeff_max-depol_eligibility_memb_v_coeff_min) * get_rand_number() ) + depol_eligibility_memb_v_coeff_min );
		depol_eligibility_list->depol_eligibility_decay_rate[i] = depol_eligibility_list->depol_eligibility_decay_rate[i] / depol_eligibility_list->depol_eligibility_v_coeff[i];
		for (j = 0; j < parker_sochacki_max_order + 1; j++)
		{
			depol_eligibility_list->depol_eligibility_v_coeff_pol_vals[i][j] = depol_eligibility_list->depol_eligibility_v_coeff[i]/(j+1);	
		}

		depol_eligibility_list->memo_depol_eligibility_change[i] = ( (depol_eligibility_memo_change_max-depol_eligibility_memo_change_min) * get_rand_number() ) + depol_eligibility_memo_change_min;
		depol_eligibility_list->memo_depol_eligibility_decay_rate[i] = -1.0 / ( ( (depol_eligibility_memo_tau_max-depol_eligibility_memo_tau_min) * get_rand_number() ) + depol_eligibility_memo_tau_min );
		for (j = 0; j < parker_sochacki_max_order + 1; j++)
		{
			depol_eligibility_list->memo_depol_eligibility_decay_rate_pol_vals[i][j] = depol_eligibility_list->memo_depol_eligibility_decay_rate[i] / (j+1);	
		}
	}
	return TRUE;
}

bool submit_new_ps_depol_eligibility_vals_for_synapse(Neuron* neuron , unsigned int parker_sochacki_max_order, double depol_eligibility_tau_max, double  depol_eligibility_tau_min, double depol_eligibility_memb_v_coeff_max, double  depol_eligibility_memb_v_coeff_min, double depol_eligibility_memo_change_max, double depol_eligibility_memo_change_min, double depol_eligibility_memo_tau_max, double depol_eligibility_memo_tau_min, unsigned int synapse)
{
	unsigned int i, j;
	DepolEligibilityList		*depol_eligibility_list;

	if (synapse >= neuron->syn_list->num_of_synapses)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "synapse >= neuron->syn_list->num_of_synapses.");

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "parker_sochacki_max_order <= 0.");
	if (depol_eligibility_tau_max < depol_eligibility_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_tau_max < depol_eligibility_tau_min.");
	if (depol_eligibility_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_tau_max <= 0.");
	if (depol_eligibility_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_tau_min <= 0.");

	if (depol_eligibility_memb_v_coeff_max < depol_eligibility_memb_v_coeff_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memb_v_coeff_max < depol_eligibility_memb_v_coeff_min.");
	if (depol_eligibility_memb_v_coeff_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memb_v_coeff_max <= 0.");
	if (depol_eligibility_memb_v_coeff_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memb_v_coeff_min <= 0.");

	if (depol_eligibility_memo_change_max < depol_eligibility_memo_change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memo_change_max < depol_eligibility_memo_change_min.");
	if (depol_eligibility_memo_change_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memo_change_max <= 0.");
	if (depol_eligibility_memo_change_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memo_change_min <= 0.");

	if (depol_eligibility_memo_tau_max < depol_eligibility_memo_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memo_tau_max < depol_eligibility_memo_tau_min.");
	if (depol_eligibility_memo_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memo_tau_max <= 0.");
	if (depol_eligibility_memo_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memo_tau_min <= 0.");

	depol_eligibility_list = neuron->depol_eligibility_list;

	i = synapse;

	depol_eligibility_list->depol_eligibility_decay_rate[i] = -1.0 / ( ( (depol_eligibility_tau_max-depol_eligibility_tau_min) * get_rand_number() ) + depol_eligibility_tau_min );
	depol_eligibility_list->depol_eligibility_v_coeff[i] = ( ( (depol_eligibility_memb_v_coeff_max-depol_eligibility_memb_v_coeff_min) * get_rand_number() ) + depol_eligibility_memb_v_coeff_min );
	depol_eligibility_list->depol_eligibility_decay_rate[i] = depol_eligibility_list->depol_eligibility_decay_rate[i] / depol_eligibility_list->depol_eligibility_v_coeff[i];
	for (j = 0; j < parker_sochacki_max_order + 1; j++)
	{
		depol_eligibility_list->depol_eligibility_v_coeff_pol_vals[i][j] = depol_eligibility_list->depol_eligibility_v_coeff[i]/(j+1);	
	}
	depol_eligibility_list->memo_depol_eligibility_change[i] = ( (depol_eligibility_memo_change_max-depol_eligibility_memo_change_min) * get_rand_number() ) + depol_eligibility_memo_change_min;
	depol_eligibility_list->memo_depol_eligibility_decay_rate[i] = -1.0 / ( ( (depol_eligibility_memo_tau_max-depol_eligibility_memo_tau_min) * get_rand_number() ) + depol_eligibility_memo_tau_min );
	for (j = 0; j < parker_sochacki_max_order + 1; j++)
	{
		depol_eligibility_list->memo_depol_eligibility_decay_rate_pol_vals[i][j] = depol_eligibility_list->memo_depol_eligibility_decay_rate[i] / (j+1);	
	}

	return TRUE;
}

void clear_depol_eligibility_for_neuron(Neuron *neuron)
{
	unsigned int i, num_of_synapses;
	double		*depol_eligibility;

	depol_eligibility = neuron->depol_eligibility_list->depol_eligibility;
	num_of_synapses = neuron->syn_list->num_of_synapses;
	for (i = 0; i < num_of_synapses; i++)
	{
		depol_eligibility[i] = 0;
	}		
}

void save_depol_eligibility_for_neuron(Neuron *neuron)
{
	unsigned int i, num_of_synapses;
	double		*depol_eligibility;
	double		*depol_eligibility_saved;
	depol_eligibility = neuron->depol_eligibility_list->depol_eligibility;
	depol_eligibility_saved = neuron->depol_eligibility_list->depol_eligibility_saved;
	num_of_synapses = neuron->syn_list->num_of_synapses;
	for (i = 0; i < num_of_synapses; i++)
	{
		depol_eligibility_saved[i] = depol_eligibility[i];
	}
}	
