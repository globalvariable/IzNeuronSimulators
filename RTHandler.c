 #include "RTHandler.h"

void *NeuroSimHandler(void *args) 
{
	RT_TASK *handler;
        RTIME period;
	
	if ( (simulation_given_step_size_ns <=100000) || (simulation_given_step_size_ns> 2500000))
	{
		printf ("ERROR: Invalid step size submitted\n");

		rt_thread_join(NeuroSimThread);
		return 0;
	}	

        if (!(handler = rt_task_init_schmod(nam2num("NSHDLR"), 9, 10000, 1000, SCHED_FIFO, 1 << MY_CPU_ID))) {
                printf("CANNOT INIT HANDLER TASK > SQHDLR <\n");
                exit(1);
        }


	period = nano2count((RTIME)(simulation_given_step_size_ns));  
	printf ("Timer for NeuroSim is %d nanoseconds\n", simulation_given_step_size_ns);

        rt_task_make_periodic(handler, rt_get_time() + period, period);
        mlockall(MCL_CURRENT | MCL_FUTURE);

	parker_sochacki_time_prev= (unsigned int)rt_get_cpu_time_ns();
        while (!EndTask) 
	{
        	rt_task_wait_period();
		parker_sochacki_time_curr = (unsigned int)rt_get_cpu_time_ns();
		simulate_parker_sochacki();   /// send current time to parker_sochacki integration
		parker_sochacki_time_prev = parker_sochacki_time_curr;
        }

	if (SelfClock)
	{
		stop_rt_timer();
        	rt_make_soft_real_time();
	}

        rt_task_delete(handler);  
	printf("Killed scheduled task for NeuroSim\nExiting NeuroSim\n");	
        return 0;
}
