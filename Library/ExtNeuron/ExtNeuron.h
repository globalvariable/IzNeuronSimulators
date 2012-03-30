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
	NeuronEventBuffer		*event_buff;	// no use currently but may appear in future to send spike to ext network. Should create new data type (like ExtNeuronEventBuffer) to connect to ext neuron.
};

bool initialize_ext_neuron(ExtNeuron *nrn, bool inhibitory);

#endif
