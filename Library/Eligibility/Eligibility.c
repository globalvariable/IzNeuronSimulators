#include "Eligibility.h"



bool create_ps_eligiblity_for_neuron_group(Network *network, unsigned int layer, unsigned int nrn_grp, unsigned int parker_sochacki_max_order,  double eligibility_tau_max, double  eligibility_tau_min)
{
	unsigned int i, num_of_neurons = 0;
	char msg[200];
	Neuron *neuron = NULL;
	if (is_neuron_group_free (network, layer, nrn_grp))
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligiblity_for_neuron_group", "is_neuron_group_free ().");
	if (! get_num_of_neurons_in_neuron_group(network, layer, nrn_grp, &num_of_neurons))
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligiblity_for_neuron_group", "! get_num_of_neurons_in_neuron_group().");
	for (i = 0; i < num_of_neurons; i++)
	{
		neuron = get_neuron_address(network, layer, nrn_grp, i);
		if (! create_ps_eligiblity_for_neuron(neuron , parker_sochacki_max_order, eligibility_tau_max, eligibility_tau_min))
			return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron_group", "! create_ps_eligibility_for_neuron().");					
	}
	sprintf(msg, "Created Eligibility data for %u neurons in Layer: %u NeuronGroup: %u.", num_of_neurons, layer, nrn_grp);
	print_message(INFO_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_eligibility_for_neuron_group", msg);
	return TRUE;
}

bool create_ps_eligiblity_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order, double eligibility_tau_max, double  eligibility_tau_min)
{
	unsigned int i, j, num_of_synapses;
	EligibilityList		*eligibility_list;

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_stdp_for_neuron_group", "parker_sochacki_max_order <= 0.");
	if (eligibility_tau_max < eligibility_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_stdp_for_neuron_group", "eligibility_tau_max < eligibility_tau_min.");
	if (eligibility_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_stdp_for_neuron_group", "eligibility_tau_max <= 0.");
	if (eligibility_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "create_ps_stdp_for_neuron_group", "eligibility_tau_min <= 0.");

	eligibility_list = neuron->eligibility_list;
	num_of_synapses = neuron->syn_list->num_of_synapses;

	eligibility_list->eligibility = g_new0(double, num_of_synapses);
	eligibility_list->eligibility_decay_rate = g_new0(double, num_of_synapses);
	eligibility_list->eligibility_iter_prev = g_new0(double, num_of_synapses);
	eligibility_list->eligibility_iter_curr = g_new0(double, num_of_synapses);
	eligibility_list->eligibility_pol_vals = g_new0(double*, num_of_synapses);
	eligibility_list->eligibility_decay_rate_pol_vals = g_new0(double*, num_of_synapses);

	for (i = 0; i < num_of_synapses; i++)
	{
		eligibility_list->eligibility_decay_rate[i] = -1.0 / ( ( (eligibility_tau_max-eligibility_tau_min) * get_rand_number() ) + eligibility_tau_min );
		eligibility_list->eligibility_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		eligibility_list->eligibility_decay_rate_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		for (j = 0; j < parker_sochacki_max_order + 1; j++)
		{
			eligibility_list->eligibility_decay_rate_pol_vals[i][j] = eligibility_list->eligibility_decay_rate[i]/(j+1);	
		}
	}
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