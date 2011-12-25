#ifndef SYNAPSE_H
#define SYNAPSE_H

typedef unsigned int 	SynapticDelay;		// No problem would appear when adding to TimeStamp type (long long unsigned int)
typedef double 		SynapticWeight;
typedef struct 		__NeuronSynapseList		NeuronSynapseList;

NeuronSynapseList	***synapse_lists;		 // Use dynamic allocation for each neuron.    Layer ----> Neuron Group ---> Neuron   3D array. 
struct __NeuronSynapseList
{
	Neuron 			**to;
	SynapticDelay		*delay;
	SynapticWeight		*weight;
	int 				num_of_synapses;	// Determine event_buff_size by incrementing with each connection. Finally allocate the event buffers (time, *from, weight)
};



#endif
