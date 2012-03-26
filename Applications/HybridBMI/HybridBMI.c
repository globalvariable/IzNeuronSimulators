#include "HybridBMI.h"

TrialsData* bmi_simulation_spike_generator_trials_data = NULL;

int main( int argc, char *argv[])
{
   	shared_memory = (SharedMemStruct*)rtai_malloc(nam2num(SHARED_MEM_NAME), SHARED_MEM_SIZE);
	bmi_simulation_spike_generator_trials_data = rtai_malloc(nam2num("TRIALDATA"), 0);
	if (bmi_simulation_spike_generator_trials_data == NULL) {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "main", "spike_generator_trials_data == NULL."); return -1; }
	bmi_simulation_spike_generator_trials_data->num_of_other_procs++;
//	bmi_simulation_spike_gen_data = allocate_spike_generator_data(bmi_simulation_spike_gen_data, bmi_simulation_spike_generator_trials_data);
	gtk_init(&argc, &argv);
	create_gui();
	gtk_main();
	return 0;
}
