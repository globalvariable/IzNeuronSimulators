#ifndef NEURON_H
#define NEURON_H

#include <stdbool.h>
#include <gtk/gtk.h>

typedef struct __Neuron Neuron;

struct __Neuron
{
	int 			layer;
	int			neuron_group;
	int			neuron_num;	
	double 		v;
	double		u;
	double		a;
	double		b;
	double		c;
	double		d;
	double		k;
	double		E;	/// 1/C    elastance
	double		v_resting;
	double		v_threshold;
	double		v_peak;	// maximum membrane voltage
	double		I_inject;	
	bool		inhibitory;	
	double		E_excitatory;
	double		E_inhibitory;	
	double		decay_rate_excitatory;
	double		decay_rate_inhibitory;
	double		conductance_excitatory;
	double		conductance_inhibitory;
	double		k_v_threshold;		
};

bool initialize_neuron(Neuron *nrn, int layer, int neuron_group, int neuron_num; double v, double a, double b, double c, double d, double k, double C, double v_resting, double v_threshold, double v_peak, double I_inject, bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory); 

#endif
