#include "ExtNeuron.h"



bool initialize_ext_neuron(ExtNeuron *nrn)
{
	if (!is_allocated(nrn->syn_list, TRUE, "ExtNeuron", "initialize_ext_neuron", ALLOCATION_BUG, NULL))
		return FALSE;
	nrn->syn_list = g_new0(NeuronSynapseList,1);
	if (!is_allocated(nrn->syn_list, TRUE, "ExtNeuron", "initialize_ext_neuron", ALLOCATION_FAILED, NULL))
		return FALSE;	
	return TRUE;
} 


