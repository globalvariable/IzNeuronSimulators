#ifndef SYNAPSE_H
#define SYNAPSE_H

#define	EXCITATORY_SYNAPSE	1
#define	INHIBITORY_SYNAPSE	0

typedef unsigned int SynapseIndex;   // excitatory = 1, inhibitory = 0;

typedef struct 	__NeuronSynapseList		NeuronSynapseList;
typedef double SynapticWeight;
typedef bool SynapseType;   // excitatory = 1, inhibitory = 0;

struct __NeuronSynapseList
{
	SynapticWeight	*weight;
	SynapseType		*type;    // save the list of the synapse type. Do not let excitatory synapses be negative or inhibitory synapses be positive during learning.
	unsigned int		num_of_synapses; 
};





#endif
