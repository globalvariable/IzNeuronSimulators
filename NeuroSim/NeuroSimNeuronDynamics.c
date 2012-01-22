#include "NeuroSimNeuronDynamics.h"



AllNeuronsDynamicsSinglePattern * neurosim_all_neurons_dynamics_single_pattern = NULL;



AllNeuronsDynamicsSinglePattern* neurosim_get_all_neurons_dynamics_single_pattern(void)
{
	return neurosim_all_neurons_dynamics_single_pattern;
}


void neurosim_set_all_neurons_dynamics_single_pattern(AllNeuronsDynamicsSinglePattern* single_pattern)
{
	neurosim_all_neurons_dynamics_single_pattern = single_pattern;
}
