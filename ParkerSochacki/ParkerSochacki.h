#ifndef PARKERSOCHACKI_H
#define PARKERSOCHACKI_H

#include "../IzNeuron/Neuron.h"
#include "../Event/Event.h"
#include "../Synapse/Synapse.h"
#include "../../BlueSpike/TimeStamp.h"

#include <math.h>

#define PARKER_SOCHACKI_EMBEDDED_STEP_SIZE 1000000.0   // 1 milliseconds


bool parker_sochacki_set_order_tolerance(int max_ps_order, double ps_error_tolerance, int max_nr_iter,  double nr_error_tolerance);
int parker_sochacki_integration(Neuron *nrn, int layer, int neuron_group, int neuron_num, TimeStamp integration_start_time, TimeStamp integration_end_time);
int parker_sochacki_step (Neuron *nrn, double *v_pol_vals, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double *chi_pol_vals, double *E_pol_vals, double *a_pol_vals, double *conductance_decay_rate_excitatory_pol_vals , double *conductance_decay_rate_inhibitory_pol_vals, double dt);
int parker_sochacki_update(Neuron *nrn, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double dt, int p);
double newton_raphson_peak_detection(double v_peak, double *v_pol_vals, int p, double dt);
void clear_parker_sochacki_polynomials(int num_of_layers, int *num_of_neuron_groups,  int **num_of_neurons_in_group); /// For debugging, delete when testing complete


#endif
