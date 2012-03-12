#include "BMISimulationSpikeGenerator.h"

static int bmi_simulation_spike_generator_rt_thread = 0;
static bool bmi_simulation_spike_generator_rt_task_stay_alive = 1;

static void *bmi_simulation_spike_generator_rt_handler(void *args); 

int main( int argc, char *argv[])
{
	TrialsData *trials_data;
   	shared_memory = (SharedMemStruct*)rtai_malloc(nam2num(SHARED_MEM_NAME), SHARED_MEM_SIZE);
	if (! get_shm_trials_data(&trials_data)) {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "main", "! get_shm_trials_data()."); return -1; }
	set_shm_spike_generator_data(allocate_spike_generator_data(trials_data, NULL));
//	bmi_simulation_spike_generator_rt_thread = rt_thread_create( bmi_simulation_spike_generator_rt_handler, NULL, 10000);
	gtk_init(&argc, &argv);
	gtk_main();
	return 0;
}


static void *bmi_simulation_spike_generator_rt_handler(void *args)
{
	RT_TASK *handler;
        RTIME period;
	unsigned int prev_time, curr_time;

	if (! check_rt_task_specs_to_init(SPIKE_GENERATOR_CPU_ID, SPIKE_GENERATOR_CPU_THREAD_ID, SPIKE_GENERATOR_PERIOD))  {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(SPIKE_GENERATOR_TASK_NAME,SPIKE_GENERATOR_TASK_PRIORITY, SPIKE_GENERATOR_STACK_SIZE, SPIKE_GENERATOR_MSG_SIZE,SPIKE_GENERATOR_POLICY, 1 << ((SPIKE_GENERATOR_CPU_ID*MAX_NUM_OF_THREADS_PER_CPU)+SPIKE_GENERATOR_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(SPIKE_GENERATOR_CPU_ID, SPIKE_GENERATOR_CPU_THREAD_ID, SPIKE_GENERATOR_PERIOD, SPIKE_GENERATOR_POSITIVE_JITTER_THRES, SPIKE_GENERATOR_NEGATIVE_JITTER_THRES))  {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	

        period = nano2count(SPIKE_GENERATOR_PERIOD);
        rt_task_make_periodic(handler, rt_get_time() + period, period);
	prev_time = rt_get_cpu_time_ns();					
        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not for get this // check the task by nano /proc/rtai/scheduler (HD/SF) 
        while (bmi_simulation_spike_generator_rt_task_stay_alive) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(SPIKE_GENERATOR_CPU_ID, SPIKE_GENERATOR_CPU_THREAD_ID, prev_time, curr_time);
		prev_time = curr_time;
		// routines
		
		// routines	
		evaluate_and_save_period_run_time(SPIKE_GENERATOR_CPU_ID, SPIKE_GENERATOR_CPU_THREAD_ID, curr_time, rt_get_cpu_time_ns());		
        }
        rt_task_delete(handler);

	print_message(INFO_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "rt_task_delete().");		
        return 0;
}
