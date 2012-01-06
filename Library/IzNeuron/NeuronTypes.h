#ifndef NEURONTYPES_H
#define NEURONTYPES_H

#include <stdbool.h>

// Neuron Types
#define 	NRN_TYPE_VIRTUAL 				0
#define 	NRN_TYPE_REGULAR_SPIKING 		1
#define 	NRN_TYPE_INTRINSICALLY_BURSTING	2
#define 	NRN_TYPE_CHATTERING			3
#define 	NRN_TYPE_LOW_THRESHOLD		4
#define 	NRN_TYPE_FAST_SPIKING			5


#define MAX_NUM_OF_NEURON_TYPES  6

void get_neuron_type_parameters(int neuron_type, double *v, double *a, double *b, double *c, double *d, double *k, double *C, double *v_resting, double *v_threshold, double *v_peak, bool *inhibitory, double *E_excitatory, double *E_inhibitory, double *tau_excitatory, double *tau_inhibitory);

char *get_neuron_type_string(int neuron_type, char *str);
#endif
