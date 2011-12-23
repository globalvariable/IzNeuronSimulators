#ifndef PARKERSOCHACKI_H
#define PARKERSOCHACKI_H

typedef struct __ParkerSochackiPolynomialVals ParkerSochackiPolynomialVals;

#include "../IzNeuron/Neuron.h"

#include <math.h>

#define MAX_BUFF_FOR_EVENT_SORTING 1000

#define MAX_PARKER_SOCHACKI_ITERATION_PER_NEURON 100

struct __ParkerSochackiPolynomialVals
{
	double E_pol_vals[MAX_PARKER_SOCHACKI_ITERATION_PER_NEURON];
	double a_pol_vals[MAX_PARKER_SOCHACKI_ITERATION_PER_NEURON];
	double conductance_decay_rate_excitatory_pol_vals[MAX_PARKER_SOCHACKI_ITERATION_PER_NEURON];
	double conductance_decay_rate_inhibitory_pol_vals[MAX_PARKER_SOCHACKI_ITERATION_PER_NEURON];
};


unsigned int parker_sochacki_time_prev;
unsigned int parker_sochacki_time_curr;

unsigned int parker_sochacki_integ_start_time;

int parker_sochacki_max_order;
double parker_sochacki_error_tolerance;

int newton_raphson_max_iter;
double newton_raphson_error_tolerance;

bool parker_sochacki_set_order_tolerance(int arg_max_ps_order, double arg_ps_error_tolerance, int arg_max_nr_iter,  double arg_nr_error_tolerance);
int parker_sochacki_evaluate_neuron_dyn(Neuron *arg_nrn);
int parker_sochacki_init_params(Neuron *arg_nrn);
int parker_sochacki_sort_synaptic_events(void);
int parker_sochacki_integration(double dt);
int parker_sochacki_step(double dt);
int parker_sochacki_update(double dt, int p);
double newton_raphson_peak_detection(int p, double dt);
int schedule_events(double dt);
void clear_parker_sochacki_polynomials(void); /// For debugging, delete when testing complete


#endif
