#include "HybridNetRLBMI.h"

static HybridNetRLBMIData *hybrid_net_rl_bmi_data = NULL;

int main( int argc, char *argv[])
{
	unsigned int i, j;
   	shared_memory = (SharedMemStruct*)rtai_malloc(nam2num(SHARED_MEM_NAME), SHARED_MEM_SIZE);
	hybrid_net_rl_bmi_data = g_new0(HybridNetRLBMIData, 1);

	hybrid_net_rl_bmi_data->blue_spike_data = rtai_malloc(nam2num(BLUE_SPIKE_DATA_SHM_NAME), 0);
	hybrid_net_rl_bmi_data->rt_tasks_data = rtai_malloc(nam2num(RT_TASKS_DATA_SHM_NAME), 0);
	hybrid_net_rl_bmi_data->blue_spike_network = allocate_network(hybrid_net_rl_bmi_data->blue_spike_network);
	hybrid_net_rl_bmi_data->in_silico_network = allocate_network(hybrid_net_rl_bmi_data->in_silico_network);
	hybrid_net_rl_bmi_data->motor_outputs = allocate_motor_outputs(hybrid_net_rl_bmi_data->motor_outputs, 1);

	for (i = 0; i < MAX_NUM_OF_MOTOR_OUTPUTS; i++)
	{
		if (!increment_num_of_classes_in_motor_output(hybrid_net_rl_bmi_data->motor_outputs, i, MOTOR_OUTPUT_BIN_SIZE)) {
			print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "! increment_num_of_classes_in_motor_output()."); return -1; }
		if (!increment_num_of_classes_in_motor_output(hybrid_net_rl_bmi_data->motor_outputs, i, MOTOR_OUTPUT_BIN_SIZE)) {
			print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "! increment_num_of_classes_in_motor_output()."); return -1; }
	}
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j = 0; j < MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			if (!add_neuron_nodes_to_layer(hybrid_net_rl_bmi_data->blue_spike_network, MAX_NUM_OF_UNIT_PER_CHAN,i, FALSE)) {
				print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "add_neuron_nodes_to_layer()."); return -1; }				
		}
	}
	gtk_init(&argc, &argv);
	create_gui();
	gtk_main();
	return 0;
}

HybridNetRLBMIData * get_hybrid_net_rl_bmi_data(void)
{
	return hybrid_net_rl_bmi_data;
}
