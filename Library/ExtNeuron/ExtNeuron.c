#include "ExtNeuron.h"



bool initialize_ext_neuron(ExtNeuron *nrn, bool inhibitory)
{
	nrn->inhibitory = inhibitory;
	if (is_allocated(nrn->syn_list, TRUE, "ExtNeuron", "initialize_ext_neuron", ALLOCATION_BUG, NULL, EXPECTS_NOT_ALLOCATED))
		return FALSE;
	nrn->syn_list = g_new0(NeuronSynapseList,1);
	if (!is_allocated(nrn->syn_list, TRUE, "ExtNeuron", "initialize_ext_neuron", ALLOCATION_FAILED, NULL, EXPECTS_ALLOCATED))
		return FALSE;	
	return TRUE;
} 


