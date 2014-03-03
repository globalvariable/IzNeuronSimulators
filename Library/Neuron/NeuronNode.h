#ifndef NEURON_NODE_H
#define NEURON_NODE_H



#include <stdbool.h>
#include <gtk/gtk.h>
#include "Neuron.h"


bool evaluate_node_inputs (Neuron *nrn, unsigned int *num_of_rxed_spikes, TimeStamp start_time, TimeStamp end_time);


#endif
