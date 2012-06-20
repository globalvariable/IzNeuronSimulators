#ifndef SYNAPSE_H
#define SYNAPSE_H

typedef struct 	__NeuronSynapseList		NeuronSynapseList;

typedef unsigned int 	SynapticDelay;		// No problem would appear when adding to TimeStamp type (long long unsigned int)
typedef double 		SynapticWeight;

#include <stdlib.h>
#include "../../../BlueSpike/KernelSpike/SpikeEndHandling.h"
#include "../../../BlueSpike/DaqCard.h"
#include "../Neuron/Neuron.h"

/// determine values below according to the latency due to PARKER_SOCHACKI_INTEGRATION_STEP_SIZE. 
#define MINIMUM_IN_SILICO_TO_IN_SILICO_SYNAPTIC_DELAY IZ_PS_NETWORK_SIM_PERIOD + 1000000    /// 1 ms buffer for jitters
#define MAXIMUM_IN_SILICO_TO_IN_SILICO_SYNAPTIC_DELAY MINIMUM_IN_SILICO_TO_IN_SILICO_SYNAPTIC_DELAY + 2000000   // it can be increased but it will lead to increase in event buffer size of neurons

/// determine values below according to the latency due to BlueSpike spike detection - Ext to Internal network event scheduler and internal network simulation period. 
#define MINIMUM_BLUE_SPIKE_TO_IN_SILICO_SYNAPTIC_DELAY KERNELSPIKE_PERIOD+(SPIKE_MIN_END_SAMP_NUM*SAMPLING_INTERVAL)+BLUE_SPIKE_BUFF_HANDLER_PERIOD+ 	IZ_PS_NETWORK_SIM_PERIOD+1000000	 /// 1 ms buffer for jitters
#define MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_SYNAPTIC_DELAY MINIMUM_BLUE_SPIKE_TO_IN_SILICO_SYNAPTIC_DELAY + 2000000 // it can be increased but it will lead to increase in event buffer size of neurons



struct __NeuronSynapseList
{
	Neuron 			**to;
	SynapticDelay		*delay;
	SynapticWeight	*weight;
	unsigned int		num_of_synapses;	// Determine event_buff_size by incrementing with each connection. Finally allocate the event buffers (time, *from, weight)
};

///////       Fuctions

bool create_synapse(Neuron *this_neuron, Neuron *target_neuron, SynapticWeight weight_excitatory_max, SynapticWeight weight_excitatory_min, SynapticWeight weight_inhibitory_max, SynapticWeight weight_inhibitory_min, SynapticDelay EPSP_delay_min, SynapticDelay EPSP_delay_max, SynapticDelay IPSP_delay_min, SynapticDelay IPSP_delay_max, SynapticDelay delay_hard_min, SynapticDelay delay_hard_max, double excitatory_connection_probability, double inhibitory_connection_probability, bool *did_connection);
void destroy_neuron_synapse_list(Neuron *neuron);
#endif
