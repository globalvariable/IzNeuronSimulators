#include "HybridNetRLBMIRtTask.h"


static bool hybrid_net_rl_bmi_rt_tasks_stay_alive = 1;

static int hybrid_net_rl_bmi_internal_network_handling_rt_thread = 0;
static int hybrid_net_rl_bmi_blue_spike_handling_rt_thread = 0;



static void *hybrid_net_rl_bmi_internal_network_handler(void *args); 
static void *hybrid_net_rl_bmi_blue_spike_rt_handler(void *args); 

void hybrid_net_rl_bmi_create_rt_threads(void)
{
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data(); 
	static bool first = TRUE;
	if (first)
	{
		bmi_data->simulation_in_progress = TRUE;
		hybrid_net_rl_bmi_internal_network_handling_rt_thread =  rt_thread_create(hybrid_net_rl_bmi_internal_network_handler, NULL, 10000);
		hybrid_net_rl_bmi_blue_spike_handling_rt_thread = rt_thread_create( hybrid_net_rl_bmi_blue_spike_rt_handler, NULL, 10000);
		first = FALSE;
	}
	else
	{
		print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_create_rt_threads", "CANNOT create rt_thread again. Should restart the program. ");	
	}
}

static void *hybrid_net_rl_bmi_internal_network_handler(void *args)
{
	RT_TASK *handler;
        RTIME period;
	unsigned int prev_time, curr_time;
	TimeStamp integration_start_time, integration_end_time, time_ns;
	TimeStamp spike_time;
	bool spike_generated;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data(); 
	Network		*in_silico_network =  bmi_data->in_silico_network;
	Neuron		**all_neurons = in_silico_network->all_neurons;
	Neuron 		*nrn;
	NeuronDynamicsBufferLimited *neuron_dynamics_buffer_limited = bmi_data->neuron_dynamics_limited_buffer;
	SpikeData	*in_silico_spike_data = bmi_data->in_silico_spike_data ;
	unsigned int i, num_of_all_neurons = in_silico_network->num_of_neurons;
	if (! check_rt_task_specs_to_init(IZ_PS_NETWORK_SIM_CPU_ID, IZ_PS_NETWORK_SIM_CPU_THREAD_ID, IZ_PS_NETWORK_SIM_PERIOD))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(IZ_PS_NETWORK_SIM_PERIOD_SIM_TASK_NAME, IZ_PS_NETWORK_SIM_PERIOD_SIM_TASK_PRIORITY, IZ_PS_NETWORK_SIM_STACK_SIZE, IZ_PS_NETWORK_SIM_MSG_SIZE, IZ_PS_NETWORK_SIM_POLICY, 1 << ((IZ_PS_NETWORK_SIM_CPU_ID*MAX_NUM_OF_THREADS_PER_CPU)+IZ_PS_NETWORK_SIM_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(IZ_PS_NETWORK_SIM_CPU_ID, IZ_PS_NETWORK_SIM_CPU_THREAD_ID, IZ_PS_NETWORK_SIM_PERIOD, IZ_PS_NETWORK_SIM_POSITIVE_JITTER_THRES, IZ_PS_NETWORK_SIM_NEGATIVE_JITTER_THRES))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	
        period = nano2count(IZ_PS_NETWORK_SIM_PERIOD);
        rt_task_make_periodic(handler, rt_get_time() + period, period);
	prev_time = rt_get_cpu_time_ns();	
	integration_start_time = ((shared_memory->rt_tasks_data.current_system_time)/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE) *PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
	reset_all_network_iz_neuron_dynamics (in_silico_network);
        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 
	for (i = 0; i < num_of_all_neurons; i++)  // blue spike buffer handler might schedule events earlier.
		clear_neuron_event_buffer(all_neurons[i]);
        while (hybrid_net_rl_bmi_rt_tasks_stay_alive) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(IZ_PS_NETWORK_SIM_CPU_ID, IZ_PS_NETWORK_SIM_CPU_THREAD_ID, prev_time, curr_time);
		prev_time = curr_time;
		// routines
		integration_end_time =  ((shared_memory->rt_tasks_data.current_system_time)/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE) *PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
		for (time_ns = integration_start_time; time_ns < integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)   // integrate remaining part in the next task period
		{
			for (i = 0; i < num_of_all_neurons; i++)
			{
				nrn = all_neurons[i];
				if (!evaluate_neuron_dyn(nrn, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, &spike_generated, &spike_time)) {
					print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "! evaluate_neuron_dyn()."); exit(1); }	
				if (spike_generated)
					write_to_spike_data(in_silico_spike_data, nrn->layer, nrn->neuron_group, nrn->neuron_num, spike_time);	
			}
			push_neuron_dynamics_to_neuron_dynamics_buffer_limited(in_silico_network, neuron_dynamics_buffer_limited, time_ns);
		}
		integration_start_time = integration_end_time;
		// routines	
		evaluate_and_save_period_run_time(IZ_PS_NETWORK_SIM_CPU_ID, IZ_PS_NETWORK_SIM_CPU_THREAD_ID, curr_time, rt_get_cpu_time_ns());		
        }
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "rt_task_delete().");	

        return 0;
}
static void *hybrid_net_rl_bmi_blue_spike_rt_handler(void *args)
{
	RT_TASK	*handler;
        RTIME		period;
	unsigned int prev_time, curr_time;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data(); 
	Network		*blue_spike_network = bmi_data->blue_spike_network;
	Neuron		*blue_spike_neuron;
	SpikeTimeStampItem *spike_time_stamp_buff = shared_memory->spike_time_stamp.spike_time_stamp_buff;
	SpikeTimeStampItem *spike_time_stamp_item; 
	unsigned int blue_spike_buff_read_idx, end_idx;
	unsigned int *blue_spike_buff_write_idx = &(shared_memory->spike_time_stamp.buff_idx_write);
	unsigned int blue_spike_buff_size = SPIKE_TIME_STAMP_BUFF_SIZE;
	unsigned int mwa_or_layer, channel_or_neuron_group, unit_or_neuron;
	TimeStamp spike_time;
	SpikeData *blue_spike_spike_data = bmi_data->blue_spike_spike_data;
	if (! check_rt_task_specs_to_init(BLUE_SPIKE_BUFF_HANDLER_CPU_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID, BLUE_SPIKE_BUFF_HANDLER_PERIOD))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_blue_spike_rt_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(BLUE_SPIKE_BUFF_HANDLER_TASK_NAME, BLUE_SPIKE_BUFF_HANDLER_PRIORITY, BLUE_SPIKE_BUFF_HANDLER_STACK_SIZE, BLUE_SPIKE_BUFF_HANDLER_MSG_SIZE, BLUE_SPIKE_BUFF_HANDLER_POLICY, 1 << ((BLUE_SPIKE_BUFF_HANDLER_CPU_ID*MAX_NUM_OF_THREADS_PER_CPU)+BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_blue_spike_rt_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(BLUE_SPIKE_BUFF_HANDLER_CPU_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID, BLUE_SPIKE_BUFF_HANDLER_PERIOD, BLUE_SPIKE_BUFF_HANDLER_POSITIVE_JITTER_THRES, BLUE_SPIKE_BUFF_HANDLER_NEGATIVE_JITTER_THRES))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_blue_spike_rt_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	
        period = nano2count(BLUE_SPIKE_BUFF_HANDLER_PERIOD);
        rt_task_make_periodic(handler, rt_get_time() + period, period);
	prev_time = rt_get_cpu_time_ns();	
        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

	blue_spike_buff_read_idx = *blue_spike_buff_write_idx;
        while (hybrid_net_rl_bmi_rt_tasks_stay_alive) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(BLUE_SPIKE_BUFF_HANDLER_CPU_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID, prev_time, curr_time);
		prev_time = curr_time;
		// routines
		end_idx = *blue_spike_buff_write_idx;
		while (blue_spike_buff_read_idx != end_idx)
		{
			spike_time_stamp_item = &(spike_time_stamp_buff[blue_spike_buff_read_idx]);
			mwa_or_layer = spike_time_stamp_item->mwa_or_layer;
			channel_or_neuron_group = spike_time_stamp_item->channel_or_neuron_group;
			unit_or_neuron = spike_time_stamp_item->unit_or_neuron;
			spike_time = spike_time_stamp_item->peak_time;
			blue_spike_neuron = get_neuron_address(blue_spike_network, mwa_or_layer, channel_or_neuron_group, unit_or_neuron);
			if (!schedule_event(blue_spike_neuron, spike_time)) {
				print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_blue_spike_rt_handler", "! schedule_event()."); exit(1); }
			write_to_spike_data(blue_spike_spike_data, mwa_or_layer, channel_or_neuron_group, unit_or_neuron, spike_time);	
			if ((blue_spike_buff_read_idx+1) == blue_spike_buff_size)
				blue_spike_buff_read_idx = 0;
			else
				blue_spike_buff_read_idx++;
		}
		// routines	
		evaluate_and_save_period_run_time(BLUE_SPIKE_BUFF_HANDLER_CPU_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID, curr_time, rt_get_cpu_time_ns());		
        }
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "rt_task_delete().");		
        return 0;
}

void hybrid_net_rl_bmi_kill_rt_tasks(void)
{
	hybrid_net_rl_bmi_rt_tasks_stay_alive = 0;
}


