#include "FiringStats.h"



void clear_firing_count_of_neurons_in_range(FiringCount *firing_counts_of_all_neurons_in_network, unsigned int start_idx, unsigned int end_idx)
{
	unsigned int i;
	for (i = start_idx; i < end_idx; i++)
	{
		firing_counts_of_all_neurons_in_network[i] = 0;
	}
}
