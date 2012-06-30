#ifndef PARKERSOCHACKI_H
#define PARKERSOCHACKI_H

typedef struct 	__ParkerSochackiPolynomialVals 	ParkerSochackiPolynomialVals;

typedef unsigned int ParkerSochackiStepSize;

#include "../Neuron/Neuron.h"
#include "../Event/Event.h"
#include "../Axon/Axon.h"
#include "../../../BlueSpike/TimeStamp.h"

#include <math.h>

#define PARKER_SOCHACKI_EMBEDDED_STEP_SIZE 1000000.0   // 1 milliseconds		// // DO NOT MAKE IT INTEGER. it is for double usage.

#define PARKER_SOCHACKI_INTEGRATION_STEP_SIZE 250000ULL   // 250000 nanoseconds 	// DO NOT MAKE IT DOUBLE (floating point).

#define NEWTON_RAPHSON_MAX_ITERATION 100
#define NEWTON_RAPHSON_ERROR_TOLERANCE 1e-10  // 1e-20 is used by Stewart & Bair

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



bool parker_sochacki_set_order_tolerance(Network *network, unsigned int max_ps_order, double ps_error_tolerance);
int get_maximum_parker_sochacki_order(void);
double get_maximum_parker_sochacki_error_tolerance(void);
bool evaluate_neuron_dyn(Neuron *nrn, TimeStamp start_time, TimeStamp end_time, bool *spike_generated, TimeStamp *spike_time);
bool parker_sochacki_integration(Neuron *nrn, TimeStamp integration_start_time, TimeStamp integration_end_time, bool *spike_generated, TimeStamp *spike_time);
int parker_sochacki_step (Neuron *nrn, double *v_pol_vals, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double *chi_pol_vals, double *E_pol_vals, double *a_pol_vals, double *conductance_decay_rate_excitatory_pol_vals , double *conductance_decay_rate_inhibitory_pol_vals, double **stdp_pre_post_pol_vals, double **stdp_post_pre_pol_vals, double **eligibility_pol_vals, double **stdp_pre_post_decay_rate_pol_vals, double **stdp_post_pre_decay_rate_pol_vals, double **eligibility_decay_rate_pol_vals, double *stdp_pre_post_iter_prev, double *stdp_pre_post_iter_curr, double *stdp_post_pre_iter_prev, double *stdp_post_pre_iter_curr, double *eligibility_iter_prev, double *eligibility_iter_curr, unsigned int num_of_synapses, double dt);
void parker_sochacki_update(Neuron *nrn, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double **stdp_pre_post_pol_vals, double **stdp_post_pre_pol_vals, double **eligibility_pol_vals, double *stdp_pre_post, double *stdp_post_pre, double *eligibility, unsigned int num_of_synapses, double dt, int p);
double newton_raphson_peak_detection(double v_peak, double *v_pol_vals, int p, double dt);
void clear_parker_sochacki_polynomials(Network *network, int num_of_layers, int *num_of_neuron_groups,  int **num_of_neurons_in_group); /// For debugging, delete when testing complete
void destroy_neuron_parker_sochacki_pol_vals(Neuron *neuron);
#endif
