#include "PoissonNeuron.h"


void evaluate_poisson_neuron (Neuron *nrn, bool *spike_generated, TimeStamp *spike_time, TimeStamp time_ns)
{
	*spike_generated = FALSE;
	if (nrn->poisson_params->firing_rate > get_rand_number())
	{
		*spike_generated = TRUE;
		*spike_time = time_ns;
		if (!schedule_synaptic_event(nrn, *spike_time))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "PoissonNeuron", "evaluate_poisson_neuron", "! schedule_events().");
	}
	return ;
}
