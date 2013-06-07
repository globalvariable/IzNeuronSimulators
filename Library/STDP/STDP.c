#include "STDP.h"


bool create_ps_stdp_for_neuron_group(Network *network, unsigned int layer, unsigned int nrn_grp, unsigned int parker_sochacki_max_order, double STDP_pre_post_change_max, double STDP_pre_post_change_min, double STDP_pre_post_tau_max, double STDP_pre_post_tau_min, double  STDP_post_pre_change_max, double  STDP_post_pre_change_min, double STDP_post_pre_tau_max, double  STDP_post_pre_tau_min)
{
	char msg[200];
	unsigned int i, num_of_neurons = 0;
	Neuron *neuron = NULL;
	if (is_neuron_group_free (network, layer, nrn_grp))
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "is_neuron_group_free ().");
	if (! get_num_of_neurons_in_neuron_group(network, layer, nrn_grp, &num_of_neurons))
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "! get_num_of_neurons_in_neuron_group().");
	for (i = 0; i < num_of_neurons; i++)
	{
		neuron = get_neuron_address(network, layer, nrn_grp, i);
		if (! create_ps_stdp_for_neuron(neuron , parker_sochacki_max_order, STDP_pre_post_change_max, STDP_pre_post_change_min, STDP_pre_post_tau_max, STDP_pre_post_tau_min, STDP_post_pre_change_max, STDP_post_pre_change_min, STDP_post_pre_tau_max, STDP_post_pre_tau_min))
			return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "! create_ps_stdp_for_neuron().");					
	}
	sprintf(msg, "Created STDP data for %u neurons in Layer: %u NeuronGroup: %u.", num_of_neurons, layer, nrn_grp);
	print_message(INFO_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", msg);
	return TRUE;
}

bool create_ps_stdp_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order, double STDP_pre_post_change_max, double STDP_pre_post_change_min, double STDP_pre_post_tau_max, double STDP_pre_post_tau_min, double  STDP_post_pre_change_max, double  STDP_post_pre_change_min, double STDP_post_pre_tau_max, double  STDP_post_pre_tau_min)
{
	unsigned int i, j, num_of_synapses;
	STDPList *stdp_list;

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "parker_sochacki_max_order <= 0.");
	if (STDP_pre_post_change_max < STDP_pre_post_change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "STDP_pre_post_change_max < STDP_pre_post_change_min.");
	if (STDP_pre_post_change_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "STDP_pre_post_change_max.");
	if (STDP_pre_post_change_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "STDP_pre_post_change_min.");
	if (STDP_pre_post_tau_max < STDP_pre_post_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "STDP_pre_post_tau_max < STDP_pre_post_tau_min.");
	if (STDP_pre_post_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "STDP_pre_post_tau_max <= 0.");
	if (STDP_pre_post_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "STDP_pre_post_tau_min <= 0.");
	if (STDP_post_pre_change_max < STDP_post_pre_change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "STDP_post_pre_change_max < STDP_post_pre_change_min.");
	if (STDP_post_pre_change_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "STDP_post_pre_change_max.");
	if (STDP_post_pre_change_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "STDP_post_pre_change_min.");
	if (STDP_post_pre_tau_max < STDP_post_pre_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "STDP_post_pre_tau_max < STDP_post_pre_tau_min.");
	if (STDP_post_pre_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "STDP_post_pre_tau_max <= 0.");
	if (STDP_post_pre_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "STDP_post_pre_tau_min <= 0.");

	if (STDP_pre_post_change_max > neuron->eligibility_list->max_eligibility)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "STDP_pre_post_change_max > neuron->eligibility_list->max_eligibility.");

	if (STDP_post_pre_change_max > neuron->eligibility_list->max_eligibility)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "create_ps_stdp_for_neuron_group", "STDP_post_pre_change_max > neuron->eligibility_list->max_eligibility.");

	stdp_list = neuron->stdp_list;
	num_of_synapses = neuron->syn_list->num_of_synapses;

	stdp_list->stdp_pre_post = g_new0(double, num_of_synapses); 					
	stdp_list->change_stdp_pre_post = g_new0(double, num_of_synapses);			
	stdp_list->decay_rate_stdp_pre_post = g_new0(double, num_of_synapses);	
	stdp_list->stdp_pre_post_iter_prev = g_new0(double, num_of_synapses);	
	stdp_list->stdp_pre_post_iter_curr = g_new0(double, num_of_synapses);	
	stdp_list->stdp_pre_post_pol_vals = g_new0(double*, num_of_synapses);			
	stdp_list->stdp_pre_post_decay_rate_pol_vals = g_new0(double*, num_of_synapses);	

	stdp_list->stdp_post_pre = g_new0(double, num_of_synapses);					
	stdp_list->change_stdp_post_pre = g_new0(double, num_of_synapses);			
	stdp_list->decay_rate_stdp_post_pre = g_new0(double, num_of_synapses);
	stdp_list->stdp_post_pre_iter_prev = g_new0(double, num_of_synapses);	
	stdp_list->stdp_post_pre_iter_curr = g_new0(double, num_of_synapses);				
	stdp_list->stdp_post_pre_pol_vals = g_new0(double*, num_of_synapses);			
	stdp_list->stdp_post_pre_decay_rate_pol_vals = g_new0(double*, num_of_synapses);	

	for (i = 0; i < num_of_synapses; i++)
	{
		stdp_list->change_stdp_pre_post[i] = ( (STDP_pre_post_change_max-STDP_pre_post_change_min) * get_rand_number() ) + STDP_pre_post_change_min;
		stdp_list->decay_rate_stdp_pre_post[i] = -1.0 / ( ( (STDP_pre_post_tau_max-STDP_pre_post_tau_min) * get_rand_number() ) + STDP_pre_post_tau_min );
		stdp_list->stdp_pre_post_pol_vals[i] = g_new0(double, parker_sochacki_max_order+1);
		stdp_list->stdp_pre_post_decay_rate_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		for (j = 0; j < parker_sochacki_max_order + 1; j++)
		{
			stdp_list->stdp_pre_post_decay_rate_pol_vals[i][j] = stdp_list->decay_rate_stdp_pre_post[i]/(j+1);	
		}
		stdp_list->change_stdp_post_pre[i] = - ( (STDP_post_pre_change_max-STDP_post_pre_change_min) * get_rand_number() ) - STDP_post_pre_change_min;
		stdp_list->decay_rate_stdp_post_pre[i] = -1.0 / ( ( (STDP_post_pre_tau_max-STDP_post_pre_tau_min) * get_rand_number() ) + STDP_post_pre_tau_min );
		stdp_list->stdp_post_pre_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		stdp_list->stdp_post_pre_decay_rate_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		for (j = 0; j < parker_sochacki_max_order + 1; j++)
		{
			stdp_list->stdp_post_pre_decay_rate_pol_vals[i][j] = stdp_list->decay_rate_stdp_post_pre[i]/(j+1);	
		}
	}
	return TRUE;
}

bool allocate_ps_stdp_for_neuron(Neuron* neuron , int parker_sochacki_max_order)
{
	unsigned int i, num_of_synapses;
	STDPList *stdp_list;

	stdp_list = neuron->stdp_list;
	num_of_synapses = neuron->syn_list->num_of_synapses;

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "allocate_ps_stdp_for_neuron", "parker_sochacki_max_order <= 0.");

	stdp_list->stdp_pre_post = g_new0(double, num_of_synapses); 					
	stdp_list->change_stdp_pre_post = g_new0(double, num_of_synapses);			
	stdp_list->decay_rate_stdp_pre_post = g_new0(double, num_of_synapses);	
	stdp_list->stdp_pre_post_iter_prev = g_new0(double, num_of_synapses);	
	stdp_list->stdp_pre_post_iter_curr = g_new0(double, num_of_synapses);	
	stdp_list->stdp_pre_post_pol_vals = g_new0(double*, num_of_synapses);			
	stdp_list->stdp_pre_post_decay_rate_pol_vals = g_new0(double*, num_of_synapses);	

	stdp_list->stdp_post_pre = g_new0(double, num_of_synapses);					
	stdp_list->change_stdp_post_pre = g_new0(double, num_of_synapses);			
	stdp_list->decay_rate_stdp_post_pre = g_new0(double, num_of_synapses);
	stdp_list->stdp_post_pre_iter_prev = g_new0(double, num_of_synapses);	
	stdp_list->stdp_post_pre_iter_curr = g_new0(double, num_of_synapses);				
	stdp_list->stdp_post_pre_pol_vals = g_new0(double*, num_of_synapses);			
	stdp_list->stdp_post_pre_decay_rate_pol_vals = g_new0(double*, num_of_synapses);	

	for (i = 0; i < num_of_synapses; i++)
	{
		stdp_list->stdp_pre_post_pol_vals[i] = g_new0(double, parker_sochacki_max_order+1);
		stdp_list->stdp_pre_post_decay_rate_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		stdp_list->stdp_post_pre_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
		stdp_list->stdp_post_pre_decay_rate_pol_vals[i] = g_new0(double, parker_sochacki_max_order + 1);
	}
	return TRUE;
}

bool submit_ps_stdp_for_synapse(Neuron* neuron , unsigned int syn_idx, double STDP_pre_post_change, double STDP_pre_post_tau, double  STDP_post_pre_change, double STDP_post_pre_tau)
{
	unsigned int i, j;
	STDPList *stdp_list;

	stdp_list = neuron->stdp_list;


	if (STDP_pre_post_change < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_ps_ps_stdp_for_synapse", "STDP_pre_post_change.");
	if (STDP_pre_post_tau <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_ps_ps_stdp_for_synapse", "STDP_pre_post_tau_max <= 0.");
	if (STDP_post_pre_change < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_ps_ps_stdp_for_synapse", "STDP_post_pre_change_max.");
	if (STDP_post_pre_tau <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_ps_ps_stdp_for_synapse", "STDP_post_pre_tau_max <= 0.");

	i = syn_idx;

	stdp_list->change_stdp_pre_post[i] = STDP_pre_post_change;
	stdp_list->decay_rate_stdp_pre_post[i] = -1.0 / STDP_pre_post_tau;
	for (j = 0; j < parker_sochacki_max_order + 1; j++)
	{
		stdp_list->stdp_pre_post_decay_rate_pol_vals[i][j] = stdp_list->decay_rate_stdp_pre_post[i]/(j+1);	
	}
	stdp_list->change_stdp_post_pre[i] = - STDP_post_pre_change;
	stdp_list->decay_rate_stdp_post_pre[i] = -1.0 / STDP_post_pre_tau;
	for (j = 0; j < parker_sochacki_max_order + 1; j++)
	{
		stdp_list->stdp_post_pre_decay_rate_pol_vals[i][j] = stdp_list->decay_rate_stdp_post_pre[i]/(j+1);	
	}
	
	return TRUE;
}

bool submit_new_ps_stdp_vals_for_neuron(Neuron* neuron , unsigned int parker_sochacki_max_order, double STDP_pre_post_change_max, double STDP_pre_post_change_min, double STDP_pre_post_tau_max, double STDP_pre_post_tau_min, double  STDP_post_pre_change_max, double  STDP_post_pre_change_min, double STDP_post_pre_tau_max, double  STDP_post_pre_tau_min)
{
	unsigned int i, j, num_of_synapses;
	STDPList *stdp_list;

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "parker_sochacki_max_order <= 0.");
	if (STDP_pre_post_change_max < STDP_pre_post_change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "STDP_pre_post_change_max < STDP_pre_post_change_min.");
	if (STDP_pre_post_change_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "STDP_pre_post_change_max.");
	if (STDP_pre_post_change_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "STDP_pre_post_change_min.");
	if (STDP_pre_post_tau_max < STDP_pre_post_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "STDP_pre_post_tau_max < STDP_pre_post_tau_min.");
	if (STDP_pre_post_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "STDP_pre_post_tau_max <= 0.");
	if (STDP_pre_post_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "STDP_pre_post_tau_min <= 0.");
	if (STDP_post_pre_change_max < STDP_post_pre_change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "STDP_post_pre_change_max < STDP_post_pre_change_min.");
	if (STDP_post_pre_change_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "STDP_post_pre_change_max.");
	if (STDP_post_pre_change_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "STDP_post_pre_change_min.");
	if (STDP_post_pre_tau_max < STDP_post_pre_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "STDP_post_pre_tau_max < STDP_post_pre_tau_min.");
	if (STDP_post_pre_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "STDP_post_pre_tau_max <= 0.");
	if (STDP_post_pre_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "STDP_post_pre_tau_min <= 0.");

	if (STDP_pre_post_change_max > neuron->eligibility_list->max_eligibility)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "STDP_pre_post_change_max > neuron->eligibility_list->max_eligibility.");

	if (STDP_post_pre_change_max > neuron->eligibility_list->max_eligibility)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_neuron", "STDP_post_pre_change_max > neuron->eligibility_list->max_eligibility.");

	stdp_list = neuron->stdp_list;
	num_of_synapses = neuron->syn_list->num_of_synapses;

	for (i = 0; i < num_of_synapses; i++)
	{
		stdp_list->change_stdp_pre_post[i] = ( (STDP_pre_post_change_max-STDP_pre_post_change_min) * get_rand_number() ) + STDP_pre_post_change_min;
		stdp_list->decay_rate_stdp_pre_post[i] = -1.0 / ( ( (STDP_pre_post_tau_max-STDP_pre_post_tau_min) * get_rand_number() ) + STDP_pre_post_tau_min );
		for (j = 0; j < parker_sochacki_max_order + 1; j++)
		{
			stdp_list->stdp_pre_post_decay_rate_pol_vals[i][j] = stdp_list->decay_rate_stdp_pre_post[i]/(j+1);	
		}
		stdp_list->change_stdp_post_pre[i] = - ( (STDP_post_pre_change_max-STDP_post_pre_change_min) * get_rand_number() ) - STDP_post_pre_change_min;
		stdp_list->decay_rate_stdp_post_pre[i] = -1.0 / ( ( (STDP_post_pre_tau_max-STDP_post_pre_tau_min) * get_rand_number() ) + STDP_post_pre_tau_min );
		for (j = 0; j < parker_sochacki_max_order + 1; j++)
		{
			stdp_list->stdp_post_pre_decay_rate_pol_vals[i][j] = stdp_list->decay_rate_stdp_post_pre[i]/(j+1);	
		}
	}
	return TRUE;
}

bool submit_new_ps_stdp_vals_for_synapse(Neuron* neuron , unsigned int parker_sochacki_max_order, double STDP_pre_post_change_max, double STDP_pre_post_change_min, double STDP_pre_post_tau_min, double STDP_pre_post_tau_max, double  STDP_post_pre_change_max, double  STDP_post_pre_change_min, double STDP_post_pre_tau_max, double  STDP_post_pre_tau_min, unsigned int synapse)
{
	unsigned int i, j;
	STDPList *stdp_list;

	if (synapse >= neuron->syn_list->num_of_synapses)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Eligibility", "submit_new_ps_stdp_vals_for_synapse", "synapse >= neuron->syn_list->num_of_synapses.");

	if (parker_sochacki_max_order <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "parker_sochacki_max_order <= 0.");
	if (STDP_pre_post_change_max < STDP_pre_post_change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "STDP_pre_post_change_max < STDP_pre_post_change_min.");
	if (STDP_pre_post_change_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "STDP_pre_post_change_max.");
	if (STDP_pre_post_change_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "STDP_pre_post_change_min.");
	if (STDP_pre_post_tau_max < STDP_pre_post_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "STDP_pre_post_tau_max < STDP_pre_post_tau_min.");
	if (STDP_pre_post_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "STDP_pre_post_tau_max <= 0.");
	if (STDP_pre_post_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "STDP_pre_post_tau_min <= 0.");
	if (STDP_post_pre_change_max < STDP_post_pre_change_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "STDP_post_pre_change_max < STDP_post_pre_change_min.");
	if (STDP_post_pre_change_max < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "STDP_post_pre_change_max.");
	if (STDP_post_pre_change_min < 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "STDP_post_pre_change_min.");
	if (STDP_post_pre_tau_max < STDP_post_pre_tau_min)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "STDP_post_pre_tau_max < STDP_post_pre_tau_min.");
	if (STDP_post_pre_tau_max <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "STDP_post_pre_tau_max <= 0.");
	if (STDP_post_pre_tau_min <= 0)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "STDP_post_pre_tau_min <= 0.");

	if (STDP_pre_post_change_max > neuron->eligibility_list->max_eligibility)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "STDP_pre_post_change_max > neuron->eligibility_list->max_eligibility.");

	if (STDP_post_pre_change_max > neuron->eligibility_list->max_eligibility)
		return print_message(BUG_MSG ,"IzNeuronSimulators", "STDP", "submit_new_ps_stdp_vals_for_synapse", "STDP_post_pre_change_max > neuron->eligibility_list->max_eligibility.");

	stdp_list = neuron->stdp_list;
	i = synapse;

	stdp_list->change_stdp_pre_post[i] = ( (STDP_pre_post_change_max-STDP_pre_post_change_min) * get_rand_number() ) + STDP_pre_post_change_min;
	stdp_list->decay_rate_stdp_pre_post[i] = -1.0 / ( ( (STDP_pre_post_tau_max-STDP_pre_post_tau_min) * get_rand_number() ) + STDP_pre_post_tau_min );
	for (j = 0; j < parker_sochacki_max_order + 1; j++)
	{
		stdp_list->stdp_pre_post_decay_rate_pol_vals[i][j] = stdp_list->decay_rate_stdp_pre_post[i]/(j+1);	
	}
	stdp_list->change_stdp_post_pre[i] = - ( (STDP_post_pre_change_max-STDP_post_pre_change_min) * get_rand_number() ) - STDP_post_pre_change_min;
	stdp_list->decay_rate_stdp_post_pre[i] = -1.0 / ( ( (STDP_post_pre_tau_max-STDP_post_pre_tau_min) * get_rand_number() ) + STDP_post_pre_tau_min );
	for (j = 0; j < parker_sochacki_max_order + 1; j++)
	{
		stdp_list->stdp_post_pre_decay_rate_pol_vals[i][j] = stdp_list->decay_rate_stdp_post_pre[i]/(j+1);	
	}

	return TRUE;
}
