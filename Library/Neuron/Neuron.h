#ifndef NEURON_H
#define NEURON_H

typedef struct __Neuron Neuron;
typedef struct __IzNeuronParams IzNeuronParams;
typedef unsigned long long int NeuronAddress;

#include <stdbool.h>
#include <gtk/gtk.h>
#include "../Event/Event.h"
#include "../Synapse/Synapse.h"
#include "../Network/Network.h"
#include "../ParkerSochacki/ParkerSochacki.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "NeuronStats.h"

#define MAX_V_VALUE	150
#define MIN_V_VALUE	-100
#define MAX_U_VALUE	400
#define MIN_U_VALUE	-50
#define MAX_EXCITATORY_CONDUCTANCE_VALUE	150
#define MIN_EXCITATORY_CONDUCTANCE_VALUE		-50
#define MAX_INHIBITORY_CONDUCTANCE_VALUE		150
#define MIN_INHIBITORY_CONDUCTANCE_VALUE		-50

#define MAX_NUM_OF_NEURON_DYNAMICS_TYPE	4
#define DYNAMICS_TYPE_V						0
#define DYNAMICS_TYPE_U						1
#define DYNAMICS_TYPE_E						2     // excitatory
#define DYNAMICS_TYPE_I						3	// inhibitory


struct __IzNeuronParams
{
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
	double		E_excitatory;
	double		E_inhibitory;	
	double		decay_rate_excitatory;
	double		decay_rate_inhibitory;
	double		conductance_excitatory;
	double		conductance_inhibitory;
	double		k_v_threshold;	
};

struct __Neuron
{
	unsigned int					layer;
	unsigned int					neuron_group;
	unsigned int					neuron_num;	
	bool						inhibitory;	
	IzNeuronParams				*iz_params;
	NeuronSynapseList			*syn_list;
	NeuronEventBuffer				*event_buff;
	ParkerSochackiPolynomialVals	*ps_vals;
	NeuronStats					*stats;
};

bool initialize_iz_neuron_params(Neuron *nrn, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, double a, double b, double c, double d, double k, double C, double v_resting, double v_threshold, double v_peak,bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory); 
bool initialize_neuron_node(Neuron *nrn, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, bool inhibitory);    // to represent blue spike neurons to connect to in silico network
bool interrogate_neuron(Network *network, int layer, int neuron_group, int neuron_num);
bool submit_new_iz_neuron_params(Network *network, Neuron *nrn, double a, double b, double c, double d, double k, double C, double v_resting, double v_threshold, double v_peak, bool inhibitory, double E_excitatory, double E_inhibitory, double tau_excitatory, double tau_inhibitory); 
bool inject_current_to_neuron(Neuron *nrn, double I_inject);
bool get_iz_neuron_parameters(Neuron *neuron, double *a, double *b, double *c, double *d, double *k, double *C, double *v_resting, double *v_threshold, double *v_peak, bool *inhibitory, double *E_excitatory, double *E_inhibitory, double *tau_excitatory, double *tau_inhibitory);
#endif
