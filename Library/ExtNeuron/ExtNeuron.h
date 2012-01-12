#ifndef EXTNEURON_H
#define EXTNEURON_H

typedef struct __ExtNeuron ExtNeuron;
typedef unsigned int ExtNeuronAddress;

#include <stdbool.h>
#include <gtk/gtk.h>
#include "../Synapse/Synapse.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"

struct __ExtNeuron
{
	bool inhibitory;
	NeuronSynapseList	*syn_list;
};

bool initialize_ext_neuron(ExtNeuron *nrn, bool inhibitory);

#endif
