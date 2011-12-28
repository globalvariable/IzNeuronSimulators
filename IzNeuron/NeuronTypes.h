#ifndef NEURONTYPES_H
#define NEURONTYPES_H

#include <stdbool.h>

// Neuron Types
#define 	REGULAR_SPIKING 		0
#define 	INTRINSICALLY_BURSTING	1
#define 	CHATTERING				2
#define 	LOW_THRESHOLD			3
#define 	FAST_SPIKING			4

#define MAX_NUM_OF_NEURON_TYPES  5

void get_neuron_type_parameters(int neuron_type, double *v, double *a, double *b, double *c, double *d, double *k, double *C, double *v_resting, double *v_threshold, double *v_peak, bool *inhibitory, double *E_excitatory, double *E_inhibitory, double *tau_excitatory, double *tau_inhibitory);

char *get_neuron_type_string(int neuron_type, char *str);
#endif
