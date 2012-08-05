#include "Synapse.h"



bool get_num_of_synapses_in_neuron(Network *network, unsigned int layer, unsigned int nrn_grp, unsigned int nrn_num, unsigned int *num_of_synapses)
{
	Neuron *neuron = NULL;
	*num_of_synapses = 0;
	if (( neuron = get_neuron_address(network, layer, nrn_grp, nrn_num)) == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "get_num_of_synapses_in_neuron", "get_neuron_address() == NULL.");
	if (neuron->syn_list == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "get_num_of_synapses_in_neuron", "neuron->syn_list == NULL.");
	*num_of_synapses = neuron->syn_list->num_of_synapses;
	return TRUE;
}

bool update_neuron_synaptic_weights(Neuron *neuron, double change_rate)
{
	unsigned int i, num_of_synapses = neuron->syn_list->num_of_synapses;
	Synapse	*synapses = neuron->syn_list->synapses;
	Synapse	*synapse;
	double	*eligibility_saved = neuron->eligibility_list->eligibility_saved;
	double	*depol_eligibility_saved = neuron->depol_eligibility_list->depol_eligibility_saved;
	double weight_change;
	for (i = 0; i < num_of_synapses; i++)
	{
		synapse = &(synapses[i]);
		if (change_rate == 0)
		{
			change_rate = -0.01;
		}
		weight_change = (eligibility_saved[i]-depol_eligibility_saved[i])*change_rate;
		if (synapse->type == EXCITATORY_SYNAPSE)
		{
			if ((synapse->weight - weight_change) < 0)
				synapse->weight = 0;
			else if ((synapse->weight + weight_change) > 15)
				synapse->weight = 15;
			else
				synapse->weight += weight_change;
		}
		else if (synapse->type == INHIBITORY_SYNAPSE)
		{
			return print_message(BUG_MSG ,"IzNeuronSimulators", "Synapse", "update_neuron_synaptic_weights", "No implementation for INHIBITORY_SYNAPSE currently supported.");
		}
		else
		{		
			return print_message(BUG_MSG ,"IzNeuronSimulators", "Synapse", "update_neuron_synaptic_weights", "Invalid synapse->type.");
		}
	}

	return TRUE;
}

bool update_neuron_synaptic_weights_with_history(Neuron *neuron, double change_rate)
{
	unsigned int i, num_of_synapses = neuron->syn_list->num_of_synapses;
	Synapse	*synapses = neuron->syn_list->synapses;
	Synapse	*synapse;
	double	*eligibility_saved = neuron->eligibility_list->eligibility_saved;
	double	*depol_eligibility_saved = neuron->depol_eligibility_list->depol_eligibility_saved;
	double weight_change;
	for (i = 0; i < num_of_synapses; i++)
	{
		synapse = &(synapses[i]);
		if (change_rate == 0)
			return print_message(BUG_MSG ,"IzNeuronSimulators", "Synapse", "update_neuron_synaptic_weights_with_history", "change_rate == 0.");		
		if (change_rate < 0)
			weight_change = (eligibility_saved[i]-depol_eligibility_saved[i])*(-1+change_rate);   // major change in weight due to incomplete trial.
		else if (change_rate > 0)
			weight_change = (eligibility_saved[i]-depol_eligibility_saved[i])*(1-change_rate);  // slight change in weight, if the reward(change_rate) is close to 1, so little change in weight. 
		else
			weight_change = 0;

		printf("Synapse: %u\t ", i);
		printf("Elig: %.8f\t", eligibility_saved[i]);
		printf("Depol: %.8f\t", depol_eligibility_saved[i]);
		printf("Weight: %.8f\t", synapse->weight);
		printf("WeightChange: %.8f\n", weight_change);
		if (synapse->type == EXCITATORY_SYNAPSE)
		{
			if ((synapse->weight + weight_change) < 0.01)
			{
				synapse->weight = 0.01;
				write_to_synapse_history_buffer(synapse);
			}
			else if ((synapse->weight + weight_change) > 20)
			{			
				synapse->weight = 20;
				write_to_synapse_history_buffer(synapse);
			}
			else
			{
				synapse->weight = synapse->weight+weight_change;
				write_to_synapse_history_buffer(synapse);
			}
		}
		else if (synapse->type == INHIBITORY_SYNAPSE)
		{
			return print_message(BUG_MSG ,"IzNeuronSimulators", "Synapse", "update_neuron_synaptic_weights_with_history", "No implementation for INHIBITORY_SYNAPSE currently supported.");
		}
		else
		{		
			return print_message(BUG_MSG ,"IzNeuronSimulators", "Synapse", "update_neuron_synaptic_weights_with_history", "Invalid synapse->type.");
		}
	}

	return TRUE;
}

bool set_neuron_synaptic_weights(Neuron *neuron, double lower_limit, double upper_limit)
{
	unsigned int i, num_of_synapses = neuron->syn_list->num_of_synapses;
	Synapse	*synapses = neuron->syn_list->synapses;
	Synapse	*synapse;

	if (upper_limit < lower_limit)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "set_neuron_synaptic_weights", "upper_limit < lower_limit.");

	for (i = 0; i < num_of_synapses; i++)
	{
		synapse = &(synapses[i]);

		if (synapse->type == EXCITATORY_SYNAPSE)
		{
			synapse->weight = ((upper_limit-lower_limit) * get_rand_number()) + lower_limit;
		}
		else if (synapse->type == INHIBITORY_SYNAPSE)
		{
			synapse->weight = -(((upper_limit-lower_limit) * get_rand_number()) + lower_limit);
		}
		else
		{		
			return print_message(BUG_MSG ,"IzNeuronSimulators", "Synapse", "set_neuron_synaptic_weights", "Invalid synapse->type.");
		}
	}
	return TRUE;
}

void write_to_synapse_history_buffer(Synapse *synapse)
{
	unsigned int *write_idx = &(synapse->weight_history_write_idx);
	synapse->weight_history[*write_idx] = synapse->weight;
	if (((*write_idx) + 1) == SYNAPTIC_WEIGHT_HISTORY_BUFFER_SIZE)
		*write_idx = 0;
	else
		(*write_idx)++;
	return;	
}


