#ifndef NEURON_STATS_H
#define NEURON_STATS_H

typedef struct __NeuronStats NeuronStats;

#include "../../../BlueSpike/TimeStamp.h"

struct __NeuronStats
{
	unsigned int		num_of_firings_for_firing_rate_bin;
	unsigned int		num_of_firings_for_motor_output;
};





#endif
