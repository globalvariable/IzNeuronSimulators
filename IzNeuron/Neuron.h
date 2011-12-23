#ifndef NEURON_H
#define NEURON_H

#include <stdbool.h>
#include <gtk/gtk.h>

typedef struct __Neuron Neuron;

#include "Event.h"
#include "Output.h"
#include "../ParkerSochacki/ParkerSochacki.h"



struct __Neuron
{
	double 		v;
	double		u;
	double		a;
	double		b;
	double		c;
	double		d;
	double		k;
	double		E;	/// 1/C    elastance
	double		conductance_excitatory;
	double		E_excitatory;
	double		decay_rate_excitatory;
	double		conductance_inhibitory;	
	double		E_inhibitory;
	double		decay_rate_inhibitory;
	double		v_threshold;
	double		v_peak;	// maximum membrane voltage
	double		v_resting;
	double		I_inject;
	bool		inhibitory;
	double		k_v_threshold;
	// Axons	
	OutputData 	output_data;
	// Sorted Event Buffer
	EventData 	event_data;
	/// Parker_Sochacki polynomial params
	ParkerSochackiPolynomialVals ps_vals;
};

Neuron *create_neuron(double v, double a, double b, double c, double d, double I_inject, bool inhibitory, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory, int logging_buff_size);

#endif
