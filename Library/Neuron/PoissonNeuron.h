#ifndef POISSON_NEURON_H
#define POISSON_NEURON_H


typedef struct __PoissonNeuronParams PoissonNeuronParams;

#include <stdbool.h>
#include <gtk/gtk.h>
#include "Neuron.h"




struct __PoissonNeuronParams
{
	double 		firing_rate;	// firing per rt task period
};


void evaluate_poisson_neuron (Neuron *nrn, bool *spike_generated, TimeStamp *spike_time, TimeStamp time_ns);


#endif
