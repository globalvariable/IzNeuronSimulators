#ifndef NETWORK_H
#define NETWORK_H

#include "../IzNeuron/Neuron.h"
#include "../Logs.h"

GPtrArray *ptrArrayAllNetwork;

int **LayerConnectionsMatrix;

bool create_neural_network(int num_of_layer);
bool add_neuron_group_to_layer(int num_neuron, int layer, double v, double a, double b, double c,double d, double I_inject, bool inhibitory, int randomize_params, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory, int logging_buff_size);
bool connect_layer_to_layer(int ThisLayer, int TargetLayer, double weight_excitatory_max, double weight_excitatory_min, double weight_inhibitory_max, double weight_inhibitory_min, unsigned int EPSP_delay_min, unsigned int EPSP_delay_max, unsigned int IPSP_delay_min, unsigned int IPSP_delay_max, float connection_probability);
bool is_layer_free (GPtrArray *layer);
bool is_neuron_group_free (GPtrArray *nrn_group);
bool reset_all_neurons (void);
bool reset_all_synapses (double weight_excitatory_max, double weight_excitatory_min, double weight_inhibitory_max, double weight_inhibitory_min);
bool finalize_network_construction(void);
neuron* is_neuron_allocated(int layer, int nrn_grp, int nrn);
bool reset_neurons_in_group(int nrn_grp, int layer, double v, double a, double b, double c,double d, double I_inject, bool inhibitory, int randomize_params, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory);
bool reset_neuron(int nrn_num, int nrn_grp, int layer, double v, double a, double b, double c,double d, double I_inject, bool inhibitory, int randomize_params, double C, double E_excitatory, double E_inhibitory, double v_resting, double v_threshold, double v_peak, double k, double tau_excitatory, double tau_inhibitory);

#endif
