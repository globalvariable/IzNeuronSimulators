#ifndef POISSON_FIRING_H
#define POISSON_FIRING_H

typedef struct __ThreePhasePoissonFiring ThreePhasePoissonFiring;
typedef struct __NetworkPoissonFiring NetworkPoissonFiring;


#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"
#include "../../../ExperimentHandlers/Applications/BMIExpController/ExpParadigm/TrialHandParadigm.h"
#include "../Neuron/PoissonNeuron.h"

struct __NetworkPoissonFiring
{
	double	***rates;	
};


struct __ThreePhasePoissonFiring
{
	NetworkPoissonFiring	trial_start_available_firing;   // firing_rate
	NetworkPoissonFiring	in_refractory_firing;
	NetworkPoissonFiring	*in_trial_firing;    // [trials_data->trial_types_data->num_of_types]
};




ThreePhasePoissonFiring* allocate_three_phase_poisson_firing_data(Network *network, TrialHandParadigmRobotReach *paradigm, ThreePhasePoissonFiring* firing_data);
ThreePhasePoissonFiring* deallocate_three_phase_poisson_firing_data(Network *network, TrialHandParadigmRobotReach *paradigm, ThreePhasePoissonFiring* firing_data);

NetworkPoissonFiring* get_in_trial_firing_data(ThreePhasePoissonFiring *firing_data, unsigned int idx); 
NetworkPoissonFiring* get_trial_start_available_firing_data(ThreePhasePoissonFiring *firing_data); 
NetworkPoissonFiring* get_in_refractory_firing_data(ThreePhasePoissonFiring *firing_data); 

void apply_firing_data_to_network(Network *network, NetworkPoissonFiring *firing_data, TimeStamp interval);   // interval = evaluation preiod in nanosec

#endif
