#ifndef NEURON_H
#define NEURON_H

#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>



#define EVENT_BUFF_SIZE_PER_NEURON 120
#define MAX_OUTPUT_PER_NEURON 100
#define MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON 1000

#define V_LOG 0
#define U_LOG 1
#define CONDUCTANCE_EXCITATORY_LOG 2
#define CONDUCTANCE_INHIBITORY_LOG 3
#define MAX_NUM_OF_VALUE_PER_NEURON_TO_LOG 4

typedef struct nron neuron;

struct nron
{
	double v;
	double u;
	double a;
	double b;
	double c;
	double d;
	double k;
	double E;	/// 1/C    elastance
	double conductance_excitatory;
	double E_excitatory;
	double decay_rate_excitatory;
	double conductance_inhibitory;	
	double E_inhibitory;
	double decay_rate_inhibitory;
	double v_threshold;
	double v_peak;	// maximum membrane voltage
	double v_resting;
	double I_inject;
	bool inhibitory;
	double k_v_threshold;
	// Axons	
	neuron **axon_to;
	unsigned int *axonal_delay;
	double *axonal_weight;
	int num_of_output;
	// Sorted Event Buffer
	unsigned int *event_time;
	neuron **event_from;   // interrogate ptrArrayEventFrom->len to reach buffer size from anywhere
	double *event_weight;
	int event_buffer_write_idx;
	int num_of_event;
	/// Parker_Sochacki polynomial params
	double *E_pol_vals;
	double *a_pol_vals;
	double *conductance_decay_rate_excitatory_pol_vals;
	double *conductance_decay_rate_inhibitory_pol_vals;
	// Logs
	double *dynamics_log[MAX_NUM_OF_VALUE_PER_NEURON_TO_LOG];
	int dynamics_log_buffer_write_idx;
};


neuron *create_neuron(double v, double a, double b, double c, double d, double I_inject, bool inhibitory, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory, int logging_buff_size);

bool reset_neuron_params(neuron *nrn, double v, double a, double b, double c, double d, double I_inject, bool inhibitory, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory);

void clear_neuron_event_buffer(neuron *nrn);

#endif
