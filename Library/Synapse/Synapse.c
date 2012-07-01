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
