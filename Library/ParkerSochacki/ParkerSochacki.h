#ifndef PARKER_SOCHACKI_H
#define PARKER_SOCHACKI_H

typedef struct 	__ParkerSochackiPolynomialVals 	ParkerSochackiPolynomialVals;

typedef unsigned int ParkerSochackiStepSize;

#include "../Neuron/Neuron.h"
#include "../Event/Event.h"
#include "../Axon/Axon.h"
#include "../../../BlueSpike/System/TimeStamp/TimeStamp.h"

#include <math.h>

#define PARKER_SOCHACKI_EMBEDDED_STEP_SIZE 1000000.0   // 1 milliseconds		// // DO NOT MAKE IT INTEGER. it is for double usage.

#define PARKER_SOCHACKI_INTEGRATION_STEP_SIZE 250000ULL   // 250000 nanoseconds 	// DO NOT MAKE IT DOUBLE (floating point).

#define NEWTON_RAPHSON_MAX_ITERATION 100
#define NEWTON_RAPHSON_ERROR_TOLERANCE 1e-7  // 1e-20 is used by Stewart & Bair

struct __ParkerSochackiPolynomialVals
{
	double *v_pol_vals;			// size should be parker_sochacki_max_order + 1
	double *u_pol_vals;
	double *conductance_excitatory_pol_vals;
	double *conductance_inhibitory_pol_vals;
	double *chi_pol_vals;
	double *E_pol_vals;		// Dynamically allocate
	double *a_pol_vals;
	double *conductance_decay_rate_excitatory_pol_vals;
	double *conductance_decay_rate_inhibitory_pol_vals;
	
};      // Use dynamic allocation for each neuron.    Layer ----> Neuron Group ---> Neuron   3D array. 

unsigned int parker_sochacki_max_order;
double parker_sochacki_error_tolerance;


bool parker_sochacki_set_order_tolerance(Network *network, unsigned int max_ps_order, double ps_error_tolerance);
int get_maximum_parker_sochacki_order(void);
double get_maximum_parker_sochacki_error_tolerance(void);

void clear_parker_sochacki_polynomials(Network *network, int num_of_layers, int *num_of_neuron_groups,  int **num_of_neurons_in_group); /// For debugging, delete when testing complete
void destroy_neuron_parker_sochacki_pol_vals(Neuron *neuron);

double newton_raphson_peak_detection(double v_peak, double *v_pol_vals, int p, double dt);





#endif
