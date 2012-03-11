#include "BMISimulationSpikeGenerator.h"

static int bmi_simulation_spike_generator_rt_thread = 0;


int main( int argc, char *argv[])
{
	TrialsData *trials_data;
   	shared_memory = (SharedMemStruct*)rtai_malloc(nam2num(SHARED_MEM_NAME), SHARED_MEM_SIZE);
	if (! get_shm_trials_data(&trials_data)) {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "main", "! get_shm_trials_data()."); return -1; }
	bmi_simulation_spike_generator_rt_thread = rt_thread_create( bmi_simulation_spike_generator_rt_handler, NULL, 10000);
	gtk_init(&argc, &argv);
	create_gui();
	gtk_main();
	return 0;
}



