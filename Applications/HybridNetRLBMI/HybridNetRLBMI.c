#include "HybridNetRLBMI.h"

static HybridNetRLBMIData *hybrid_net_rl_bmi_data = NULL;

int main( int argc, char *argv[])
{
	unsigned int i, j;
   	shared_memory = (SharedMemStruct*)rtai_malloc(nam2num(SHARED_MEM_NAME), SHARED_MEM_SIZE);
	hybrid_net_rl_bmi_data = g_new0(HybridNetRLBMIData, 1);
	hybrid_net_rl_bmi_data->trials_data = rtai_malloc(nam2num("TRIALDATA"), 0);
	if (hybrid_net_rl_bmi_data->trials_data == NULL) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "hybrid_net_rl_bmi_data->trials_data == NULL."); return -1; }
	hybrid_net_rl_bmi_data->trials_data->num_of_other_procs++;
	hybrid_net_rl_bmi_data->int_network = allocate_network(hybrid_net_rl_bmi_data->int_network);
	hybrid_net_rl_bmi_data->ext_network = allocate_external_network(hybrid_net_rl_bmi_data->ext_network);
	hybrid_net_rl_bmi_data->ext_network->connected_to_internal_network = hybrid_net_rl_bmi_data->int_network;
	hybrid_net_rl_bmi_data->int_network->connection_from_ext_network = hybrid_net_rl_bmi_data->ext_network;
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j = 0; j < MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			add_neurons_to_external_network_layer(hybrid_net_rl_bmi_data->ext_network, MAX_NUM_OF_UNIT_PER_CHAN, i, FALSE);
		}
	}
//	bmi_simulation_spike_gen_data = allocate_spike_generator_data(bmi_simulation_spike_gen_data, bmi_simulation_spike_generator_trials_data);
	gtk_init(&argc, &argv);
	create_gui();
	gtk_main();
	return 0;
}

HybridNetRLBMIData * get_hybrid_net_rl_bmi_data(void)
{
	return hybrid_net_rl_bmi_data;
}
