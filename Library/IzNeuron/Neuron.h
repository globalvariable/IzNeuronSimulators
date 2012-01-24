#ifndef NEURON_H
#define NEURON_H

typedef struct __Neuron Neuron;
typedef unsigned int NeuronAddress;

#include <stdbool.h>
#include <gtk/gtk.h>
#include "../Event/Event.h"
#include "../Synapse/Synapse.h"
#include "../Network/Network.h"
#include "../ParkerSochacki/ParkerSochacki.h"

#define MAX_V_VALUE	150
#define MIN_V_VALUE	-100
#define MAX_U_VALUE	400
#define MIN_U_VALUE	-50
#define MAX_EXCITATORY_CONDUCTANCE_VALUE	150
#define MIN_EXCITATORY_CONDUCTANCE_VALUE		-50
#define MAX_INHIBITORY_CONDUCTANCE_VALUE		150
#define MIN_INHIBITORY_CONDUCTANCE_VALUE		-50

#define DYNAMICS_TYPE_V	0
#define DYNAMICS_TYPE_U	1
#define DYNAMICS_TYPE_E	2     // excitatory
#define DYNAMICS_TYPE_I	3	// inhibitory


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
	NeuronSynapseList	*syn_list;
	NeuronEventBuffer	*event_buff;
	ParkerSochackiPolynomialVals	*ps_vals;
};

bool initialize_neuron_params(Neuron *nrn, int layer, int neuron_group, int neuron_num, double a, double b, double c, double d, double k, double C, double v_resting, double v_threshold, double v_peak, bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory) ;
bool interrogate_neuron(Network *network, int layer, int neuron_group, int neuron_num);
bool submit_new_neuron_params(Network *network, Neuron *nrn, double v, double a, double b, double c, double d, double k, double C, double v_resting, double v_threshold, double v_peak, double I_inject, bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory);
bool inject_current_to_neuron(Neuron *nrn, double I_inject);
#endif
