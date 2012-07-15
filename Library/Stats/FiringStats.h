#ifndef FIRING_STATS_H
#define FIRING_STATS_H



typedef unsigned int FiringCount;


void clear_firing_count_of_neurons_in_range(FiringCount *firing_counts_of_all_neurons_in_network, unsigned int start_idx, unsigned int end_idx);

#endif 
