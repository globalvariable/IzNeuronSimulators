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

bool update_neuron_synaptic_weights(Neuron *neuron, double reward)
{
/*	unsigned int i, num_of_synapses = neuron->syn_list->num_of_synapses;
	Synapse	*synapses = neuron->syn_list->synapses;
	Synapse	*synapse;
//	double	*eligibility_saved;
	double weight_change;

	if (! neuron->syn_list->num_of_synapses) // no synapse, no need to go further. moreover,  this can be a poisson neuron with no eligibility etc.
		return TRUE;

//	eligibility_saved = neuron->eligibility_list->eligibility_saved;

	for (i = 0; i < num_of_synapses; i++)
	{
		synapse = &(synapses[i]);

//		weight_change = reward * eligibility_saved[i];  

		if (synapse->type == EXCITATORY_SYNAPSE)
		{
			if ((synapse->weight + weight_change) < 1)
			{
				synapse->weight = 1;
			}
			else if ((synapse->weight + weight_change) > 20)
			{			
				synapse->weight = 20;
			}
			else
			{
				synapse->weight = synapse->weight+weight_change;
			}
		}
		else if (synapse->type == INHIBITORY_SYNAPSE)
		{
		}
		else
		{		
			return print_message(BUG_MSG ,"IzNeuronSimulators", "Synapse", "update_neuron_synaptic_weights_with_history", "Invalid synapse->type.");
		}
	}
*/
	return TRUE;
}

bool update_neuron_synaptic_weights_with_history(Neuron *neuron, double reward, double learning_rate)
{
/*	unsigned int i, num_of_synapses = neuron->syn_list->num_of_synapses;
	Synapse	*synapses = neuron->syn_list->synapses;
	Synapse	*synapse;
//	double	*eligibility_saved;
	double weight_change;
	
	if ( ! neuron->syn_list->num_of_synapses) // no synapse, no need to go further. moreover,  this can be a poisson neuron with no eligibility etc.
		return TRUE;
	
//	eligibility_saved = neuron->eligibility_list->eligibility_saved;
	for (i = 0; i < num_of_synapses; i++)
	{
		synapse = &(synapses[i]);

//		weight_change = learning_rate * reward * eligibility_saved[i];  

		printf("Synapse: %u\t ", i);
		printf("Elig: %.8f\t", eligibility_saved[i]);
		printf("Weight: %.8f\t", synapse->weight);
		printf("WeightChange: %.8f\n", weight_change);
		if (synapse->type == EXCITATORY_SYNAPSE)
		{
			if ((synapse->weight + weight_change) < 0.1)
			{
				synapse->weight = 0.1;
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
			write_to_synapse_history_buffer(synapse);
		}
		else
		{		
			return print_message(BUG_MSG ,"IzNeuronSimulators", "Synapse", "update_neuron_synaptic_weights_with_history", "Invalid synapse->type.");
		}
	}
*/
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

bool set_neuron_excitatory_synaptic_weights(Neuron *neuron, double lower_limit, double upper_limit)
{
	unsigned int i, num_of_synapses = neuron->syn_list->num_of_synapses;
	Synapse	*synapses = neuron->syn_list->synapses;
	Synapse	*synapse;

	if (upper_limit < lower_limit)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "set_neuron_excitatory_synaptic_weights", "upper_limit < lower_limit.");

	for (i = 0; i < num_of_synapses; i++)
	{
		synapse = &(synapses[i]);

		if (synapse->type == EXCITATORY_SYNAPSE)
		{
			synapse->weight = ((upper_limit-lower_limit) * get_rand_number()) + lower_limit;
		}
		else if (synapse->type == INHIBITORY_SYNAPSE)
		{
			print_message(WARNING_MSG ,"IzNeuronSimulators", "Synapse", "set_neuron_excitatory_synaptic_weights", "skipped this synapse since synapse->type == INHIBITORY_SYNAPSE.");
			continue;
		}
		else
		{		
			return print_message(BUG_MSG ,"IzNeuronSimulators", "Synapse", "set_neuron_excitatory_synaptic_weights", "Invalid synapse->type.");
		}
	}
	return TRUE;
}

bool set_neuron_synapse_synaptic_weight(Neuron *neuron, double lower_limit, double upper_limit, SynapseIndex syn_idx)
{
	Synapse	*synapse = &(neuron->syn_list->synapses[syn_idx]);

	if (upper_limit < lower_limit)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Synapse", "set_neuron_synapse_synaptic_weight", "upper_limit < lower_limit.");

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
		return print_message(BUG_MSG ,"IzNeuronSimulators", "Synapse", "set_neuron_synapse_synaptic_weight", "Invalid synapse->type.");
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

bool write_neuron_synaptic_weights_to_history(Neuron *neuron)
{
	unsigned int i, num_of_synapses = neuron->syn_list->num_of_synapses;
	Synapse	*synapses = neuron->syn_list->synapses;
	Synapse	*synapse;
	
	if (! neuron->syn_list->num_of_synapses) // no synapse, no need to go further. moreover,  this can be a poisson neuron with no eligibility etc.
		return TRUE;

	for (i = 0; i < num_of_synapses; i++)
	{
		synapse = &(synapses[i]);		
		write_to_synapse_history_buffer(synapse);
	}
	return TRUE;
}	

