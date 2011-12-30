#ifndef PARKERSOCHACKI_H
#define PARKERSOCHACKI_H

typedef struct 	__ParkerSochackiPolynomialVals 	ParkerSochackiPolynomialVals;

typedef unsigned int ParkerSochackiStepSize;

#include "../IzNeuron/Neuron.h"
#include "../Event/Event.h"
#include "../Synapse/Synapse.h"
#include "../../../BlueSpike/TimeStamp.h"

#include <math.h>

#define PARKER_SOCHACKI_EMBEDDED_STEP_SIZE 1000000.0   // 1 milliseconds

#define NEWTON_RAPHSON_MAX_ITERATION 100
#define NEWTON_RAPHSON_ERROR_TOLERANCE 1e-20  // 1 milliseconds

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



bool parker_sochacki_set_order_tolerance(int max_ps_order, double ps_error_tolerance);
TimeStamp evaluate_neuron_dyn(Neuron *nrn, TimeStamp start_time, TimeStamp end_time);
TimeStamp parker_sochacki_integration(Neuron *nrn, TimeStamp integration_start_time, TimeStamp integration_end_time);
int parker_sochacki_step (Neuron *nrn, double *v_pol_vals, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double *chi_pol_vals, double *E_pol_vals, double *a_pol_vals, double *conductance_decay_rate_excitatory_pol_vals , double *conductance_decay_rate_inhibitory_pol_vals, double dt);
int parker_sochacki_update(Neuron *nrn, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double dt, int p);
double newton_raphson_peak_detection(double v_peak, double *v_pol_vals, int p, double dt);
void clear_parker_sochacki_polynomials(int num_of_layers, int *num_of_neuron_groups,  int **num_of_neurons_in_group); /// For debugging, delete when testing complete
void destroy_neuron_parker_sochacki_pol_vals(Neuron *neuron);
int get_maximum_parker_sochaki_order(void);
#endif
