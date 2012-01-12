#ifndef EXTNEURON_H
#define EXTNEURON_H

typedef struct __ExtNeuron ExtNeuron;
typedef unsigned int ExtNeuronAddress;

#include <stdbool.h>
#include <gtk/gtk.h>
#include "../Synapse/Synapse.h"

struct __ExtNeuron
{
	NeuronSynapseList	*syn_list;
};

bool initialize_ext_neuron(ExtNeuron *nrn); 

#endif
