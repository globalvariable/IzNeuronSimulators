#ifndef OUTPUT_H
#define OUTPUT_H

#include "Neuron.h"
#include "Synapse.h"

#define MAX_NUM_OF_OUTPUT_PER_NEURON 100

typedef struct __OutputData OutputData;
typedef struct __OutputItem OutputItem;

struct __OutputItem
{
	Neuron 			*to;
	SynapticDelay 		delay;
	SynapticWeight 	weight;
};

struct __OutputData
{
	OutputItem	output_list[MAX_NUM_OF_OUTPUT_PER_NEURON]; 
	int 			num_of_active_output;
};

#endif
