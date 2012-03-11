#include "NeuroSimNeuronDynamics.h"



AllNeuronsDynamicsSinglePattern * neurosim_all_neurons_dynamics_single_pattern = NULL;
NeuronDynamicsGraph * neurosim_neuron_dynamics_graph_simulation_tab_upper = NULL;
NeuronDynamicsGraph * neurosim_neuron_dynamics_graph_simulation_tab_lower = NULL;
NeuronDynamicsCombo * neurosim_neuron_dynamics_combo_simulation_tab_upper = NULL;
NeuronDynamicsCombo * neurosim_neuron_dynamics_combo_simulation_tab_lower = NULL;

AllNeuronsDynamicsSinglePattern* neurosim_get_all_neurons_dynamics_single_pattern(void)
{
	return neurosim_all_neurons_dynamics_single_pattern;
}


void neurosim_set_all_neurons_dynamics_single_pattern(AllNeuronsDynamicsSinglePattern* single_pattern)
{
	neurosim_all_neurons_dynamics_single_pattern = single_pattern;
}


NeuronDynamicsGraph* neurosim_get_neuron_dynamics_graph_simulation_tab_upper(void) 
{
	return neurosim_neuron_dynamics_graph_simulation_tab_upper;
}
void neurosim_set_neuron_dynamics_graph_simulation_tab_upper(NeuronDynamicsGraph* graph)
{
	neurosim_neuron_dynamics_graph_simulation_tab_upper = graph;
}

NeuronDynamicsGraph* neurosim_get_neuron_dynamics_graph_simulation_tab_lower(void)
{
	return neurosim_neuron_dynamics_graph_simulation_tab_lower;
}
void neurosim_set_neuron_dynamics_graph_simulation_tab_lower(NeuronDynamicsGraph* graph)
{
	neurosim_neuron_dynamics_graph_simulation_tab_lower = graph;
}

NeuronDynamicsCombo* neurosim_get_neuron_dynamics_combo_simulation_tab_upper(void) 
{
	return neurosim_neuron_dynamics_combo_simulation_tab_upper;
}
void neurosim_set_neuron_dynamics_combo_simulation_tab_upper(NeuronDynamicsCombo* combo)
{
	neurosim_neuron_dynamics_combo_simulation_tab_upper = combo;
}

NeuronDynamicsCombo* neurosim_get_neuron_dynamics_combo_simulation_tab_lower(void) 
{
	return neurosim_neuron_dynamics_combo_simulation_tab_lower;
}
void neurosim_set_neuron_dynamics_combo_simulation_tab_lower(NeuronDynamicsCombo* combo)
{
	neurosim_neuron_dynamics_combo_simulation_tab_lower = combo;
}
