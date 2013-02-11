#include "DepolEligibility.h"



bool create_ps_depol_eligibility_for_neuron_group(Network *network, unsigned int layer, unsigned int nrn_grp, unsigned int parker_sochacki_max_order,  double depol_eligibility_tau_max, double  depol_eligibility_tau_min, double depol_eligibility_change_scaler_max, double  depol_eligibility_change_scaler_min)
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
		if (! create_ps_depol_eligibility_for_neuron(neuron , parker_sochacki_max_order, depol_eligibility_tau_max, depol_eligibility_tau_min, depol_eligibility_change_scaler_max,  depol_eligibility_change_scaler_min))
			return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_depol_eligibility_for_neuron_group", "! create_ps_depol_eligibility_for_neuron().");					
	}
	sprintf(msg, "Created DepolEligibility data for %u neurons in Layer: %u NeuronGroup: %u.", num_of_neurons, layer, nrn_grp);
	print_message(INFO_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_depol_eligibility_for_neuron_group", msg);
	return TRUE;
}

bool create_ps_depol_eligibility_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order, double depol_eligibility_tau_max, double  depol_eligibility_tau_min, double depol_eligibility_change_scaler_max, double  depol_eligibility_change_scaler_min)
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

	if (depol_eligibility_change_scaler_max < depol_eligibility_change_scaler_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_change_scaler_max < depol_eligibility_change_scaler_min.");
	if (depol_eligibility_change_scaler_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_memb_v_coeff_max <= 0.");
	if (depol_eligibility_change_scaler_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "create_ps_stdp_for_neuron_group", "depol_eligibility_memb_v_coeff_min <= 0.");

	depol_eligibility_list = neuron->depol_eligibility_list;
	num_of_synapses = neuron->syn_list->num_of_synapses;

	depol_eligibility_list->depol_eligibility = g_new0(double, num_of_synapses);
	depol_eligibility_list->depol_eligibility_saved = g_new0(double, num_of_synapses);
	depol_eligibility_list->depol_eligibility_decay_rate = g_new0(double, num_of_synapses);
	depol_eligibility_list->depol_eligibility_iter_prev = g_new0(double, num_of_synapses);
	depol_eligibility_list->depol_eligibility_iter_curr = g_new0(double, num_of_synapses);
	depol_eligibility_list->depol_eligibility_pol_vals = g_new0(double*, num_of_synapses);
	depol_eligibility_list->depol_eligibility_decay_rate_pol_vals = g_new0(double*, num_of_synapses);
	depol_eligibility_list->depol_eligibility_change_scaler = g_new0(double, num_of_synapses);

	for (i = 0; i < num_of_synapses; i++)
	{
		depol_eligibility_list->depol_eligibility_decay_rate[i] = -1.0 / ( ( (depol_eligibility_tau_max-depol_eligibility_tau_min) * get_rand_number() ) + depol_eligibility_tau_min );
		depol_eligibility_list->depol_eligibility_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		depol_eligibility_list->depol_eligibility_decay_rate_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		for (j = 0; j < parker_sochacki_max_order + 1; j++)
		{
			depol_eligibility_list->depol_eligibility_decay_rate_pol_vals[i][j] = depol_eligibility_list->depol_eligibility_decay_rate[i]/(j+1);	
		}
		depol_eligibility_list->depol_eligibility_change_scaler[i] =  ((depol_eligibility_change_scaler_max-depol_eligibility_change_scaler_min) * get_rand_number())  + depol_eligibility_change_scaler_min;
	}
	return TRUE;
}

bool submit_new_ps_depol_eligibility_vals_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order, double depol_eligibility_tau_max, double  depol_eligibility_tau_min, double depol_eligibility_change_scaler_max, double  depol_eligibility_change_scaler_min)
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

	if (depol_eligibility_change_scaler_max < depol_eligibility_change_scaler_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_change_scaler_max < depol_eligibility_change_scaler_min.");
	if (depol_eligibility_change_scaler_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memb_v_coeff_max <= 0.");
	if (depol_eligibility_change_scaler_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_neuron", "depol_eligibility_memb_v_coeff_min <= 0.");

	depol_eligibility_list = neuron->depol_eligibility_list;
	num_of_synapses = neuron->syn_list->num_of_synapses;

	for (i = 0; i < num_of_synapses; i++)
	{
		depol_eligibility_list->depol_eligibility_decay_rate[i] = -1.0 / ( ( (depol_eligibility_tau_max-depol_eligibility_tau_min) * get_rand_number() ) + depol_eligibility_tau_min );
		for (j = 0; j < parker_sochacki_max_order + 1; j++)
		{
			depol_eligibility_list->depol_eligibility_decay_rate_pol_vals[i][j] = depol_eligibility_list->depol_eligibility_decay_rate[i]/(j+1);	
		}
		depol_eligibility_list->depol_eligibility_change_scaler[i] =  ((depol_eligibility_change_scaler_max-depol_eligibility_change_scaler_min) * get_rand_number())  + depol_eligibility_change_scaler_min;
	}
	return TRUE;
}

bool submit_new_ps_depol_eligibility_vals_for_synapse(Neuron* neuron , unsigned int parker_sochacki_max_order, double depol_eligibility_tau_max, double  depol_eligibility_tau_min, double depol_eligibility_change_scaler_max, double  depol_eligibility_change_scaler_min, unsigned int synapse)
{
	unsigned int i, j;
	DepolEligibilityList		*depol_eligibility_list;

	if (synapse >= neuron->syn_list->num_of_synapses)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_synapse", "synapse >= neuron->syn_list->num_of_synapses.");

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_synapse", "parker_sochacki_max_order <= 0.");
	if (depol_eligibility_tau_max < depol_eligibility_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_synapse", "depol_eligibility_tau_max < depol_eligibility_tau_min.");
	if (depol_eligibility_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_synapse", "depol_eligibility_tau_max <= 0.");
	if (depol_eligibility_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_synapse", "depol_eligibility_tau_min <= 0.");

	if (depol_eligibility_change_scaler_max < depol_eligibility_change_scaler_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_synapse", "depol_eligibility_change_scaler_max < depol_eligibility_change_scaler_min.");
	if (depol_eligibility_change_scaler_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_synapse", "depol_eligibility_memb_v_coeff_max <= 0.");
	if (depol_eligibility_change_scaler_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "DepolEligibility", "submit_new_ps_depol_eligibility_vals_for_synapse", "depol_eligibility_memb_v_coeff_min <= 0.");

	depol_eligibility_list = neuron->depol_eligibility_list;

	i = synapse;

	depol_eligibility_list->depol_eligibility_decay_rate[i] = -1.0 / ( ( (depol_eligibility_tau_max-depol_eligibility_tau_min) * get_rand_number() ) + depol_eligibility_tau_min );
	for (j = 0; j < parker_sochacki_max_order + 1; j++)
	{
		depol_eligibility_list->depol_eligibility_decay_rate_pol_vals[i][j] = depol_eligibility_list->depol_eligibility_decay_rate[i]/(j+1);	
	}
	depol_eligibility_list->depol_eligibility_change_scaler[i] =  ((depol_eligibility_change_scaler_max-depol_eligibility_change_scaler_min) * get_rand_number())  + depol_eligibility_change_scaler_min;

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
