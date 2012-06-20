#include "HybridNetRLBMIRtTask.h"


static bool hybrid_net_rl_bmi_rt_tasks_stay_alive = 1;

static int *hybrid_net_rl_bmi_internal_network_handling_rt_threads = NULL;
static unsigned int *hybrid_net_rl_bmi_internal_network_handling_rt_thread_ids = NULL;
static int hybrid_net_rl_bmi_blue_spike_handling_rt_thread = 0;



static void *hybrid_net_rl_bmi_internal_network_handler(void *args); 
static void *hybrid_net_rl_bmi_blue_spike_rt_handler(void *args); 

void hybrid_net_rl_bmi_create_rt_threads(void)
{
	static bool first = TRUE;
	unsigned int i, num_of_dedicated_cpu_threads;
	if (IZ_PS_NETWORK_SIM_NUM_OF_DEDICATED_CPUS == 0)
		return (void)print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_create_rt_threads", "IZ_PS_NETWORK_SIM_NUM_OF_DEDICATED_CPUS == 0.");		
	num_of_dedicated_cpu_threads = IZ_PS_NETWORK_SIM_NUM_OF_DEDICATED_CPUS * MAX_NUM_OF_CPU_THREADS_PER_CPU;
	if (first)
	{
		get_hybrid_net_rl_bmi_data()->simulation_in_progress = TRUE;
		hybrid_net_rl_bmi_internal_network_handling_rt_threads = g_new0(int, num_of_dedicated_cpu_threads);
		hybrid_net_rl_bmi_internal_network_handling_rt_thread_ids = g_new0(unsigned int, num_of_dedicated_cpu_threads);

		for (i = 0; i < num_of_dedicated_cpu_threads; i++)
		{
			hybrid_net_rl_bmi_internal_network_handling_rt_thread_ids[i] = i; 
			hybrid_net_rl_bmi_internal_network_handling_rt_threads[i] =  rt_thread_create(hybrid_net_rl_bmi_internal_network_handler, &(hybrid_net_rl_bmi_internal_network_handling_rt_thread_ids[i]), 10000);
		}
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
	RtTasksData *rt_tasks_data = bmi_data->rt_tasks_data;
	Network		*in_silico_network =  bmi_data->in_silico_network;
	MotorOutputs *motor_outputs = bmi_data->motor_outputs;
	NeuralNet2MovObjHandMsg	*msgs_neural_net_2_mov_obj_hand = bmi_data->msgs_neural_net_2_mov_obj_hand;

	Neuron		**all_neurons = in_silico_network->all_neurons;
	Neuron 		*nrn;

	NeuronDynamicsBufferLimited *neuron_dynamics_buffer_limited = bmi_data->neuron_dynamics_limited_buffer;
	SpikeData	*in_silico_spike_data = bmi_data->in_silico_spike_data ;
	unsigned int i, neurons_start_idx, neurons_end_idx; 
	unsigned int num_of_dedicated_cpu_threads;
	unsigned int cpu_id, cpu_thread_id, cpu_thread_task_id;

	char task_name[8];
	char task_num[4];
	sprintf(task_num, "%u", *((unsigned int*)args) );
	strcpy(task_name, IZ_PS_NETWORK_SIM_TASK_NAME);
	strcat(task_name, task_num);
	/// share the neurons in between rt_threads;    ie.g there are 21 neurons and 4 dedicated cpu threads. then the cpu threads will have 6 + 6 + 6 + 3 neurons to handle
	num_of_dedicated_cpu_threads = IZ_PS_NETWORK_SIM_NUM_OF_DEDICATED_CPUS * MAX_NUM_OF_CPU_THREADS_PER_CPU;
	neurons_start_idx = (((unsigned int)(in_silico_network->num_of_neurons/num_of_dedicated_cpu_threads))+1) * ( *((unsigned int*)args) );
	neurons_end_idx = (((unsigned int)(in_silico_network->num_of_neurons/num_of_dedicated_cpu_threads))+1)  * ( ( *((unsigned int*)args) ) + 1);
	if (neurons_end_idx >  in_silico_network->num_of_neurons)		/// when CPU_NUM 4 and num_of_all_neuons = 5    neurons_start_idx will be 6 and neurons_end_idx will be 5
		neurons_end_idx = in_silico_network->num_of_neurons;		/// for (i = 6; i < 5; i++) { printf ("hit")}      it does not hit !!!  SO this code is OK. 

	printf("neurons_start_idx = %u neurons_end_idx = %u\n", neurons_start_idx, neurons_end_idx);

	cpu_id =  ((*((unsigned int*)args)) / MAX_NUM_OF_CPU_THREADS_PER_CPU);   // find the cpu_id and cpu_thread_id to run this rt_thread
	cpu_thread_id = (*((unsigned int*)args)) - (cpu_id * MAX_NUM_OF_CPU_THREADS_PER_CPU);
	cpu_thread_task_id = 0;
	cpu_id =  IZ_PS_NETWORK_SIM_CPU_ID + cpu_id;
	if (! check_rt_task_specs_to_init(rt_tasks_data, cpu_id, cpu_thread_id, cpu_thread_task_id, IZ_PS_NETWORK_SIM_PERIOD))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(nam2num(task_name), IZ_PS_NETWORK_SIM_TASK_PRIORITY, IZ_PS_NETWORK_SIM_STACK_SIZE, IZ_PS_NETWORK_SIM_MSG_SIZE, IZ_PS_NETWORK_SIM_POLICY, 1 << ((cpu_id*MAX_NUM_OF_CPU_THREADS_PER_CPU)+cpu_thread_id)))) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, cpu_id, cpu_thread_id, cpu_thread_task_id, IZ_PS_NETWORK_SIM_PERIOD, IZ_PS_NETWORK_SIM_POSITIVE_JITTER_THRES, IZ_PS_NETWORK_SIM_NEGATIVE_JITTER_THRES, "HybridNetRLBMI"))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	
        period = nano2count(IZ_PS_NETWORK_SIM_PERIOD);
        rt_task_make_periodic(handler, rt_get_time() + period, period);
	prev_time = rt_get_cpu_time_ns();	
	integration_start_time = ((rt_tasks_data->current_system_time)/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE) *PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
	reset_all_network_iz_neuron_dynamics (in_silico_network);
	clear_motor_output_counters(bmi_data->motor_outputs);
        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 
	for (i = neurons_start_idx; i < neurons_end_idx; i++)  // blue spike buffer handler might schedule events earlier.  // only handle the neurons this thread is responsible for
		clear_neuron_event_buffer(all_neurons[i]);
        while (hybrid_net_rl_bmi_rt_tasks_stay_alive) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(rt_tasks_data, cpu_id, cpu_thread_id, cpu_thread_task_id, prev_time, curr_time);
		prev_time = curr_time;
		// routines
		integration_end_time =  ((rt_tasks_data->current_system_time)/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE) *PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
		for (time_ns = integration_start_time; time_ns < integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)   // integrate remaining part in the next task period
		{
			for (i = neurons_start_idx; i < neurons_end_idx; i++)  // simulate the neurons for which this thread is responsible
			{
				nrn = all_neurons[i];
				//nrn->iz_params->I_inject = 21;
				if (!evaluate_neuron_dyn(nrn, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, &spike_generated, &spike_time)) {
					print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "! evaluate_neuron_dyn()."); exit(1); }	
				if (spike_generated)
				{
					write_to_spike_data(in_silico_spike_data, nrn->layer, nrn->neuron_group, nrn->neuron_num, spike_time);
			//		nrn->stats->num_of_firings_for_firing_rate_bin++;
			//		nrn->stats->num_of_firings_for_motor_output++;
					in_silico_network->num_of_spikes++;
				}	
			}
			push_neuron_dynamics_to_neuron_dynamics_buffer_limited(in_silico_network, neuron_dynamics_buffer_limited, time_ns, neurons_start_idx, neurons_end_idx);
/*			if (!handle_motor_outputs(motor_outputs, time_ns, msgs_neural_net_2_mov_obj_hand)) {
				print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "! handle_motor_outputs()."); exit(1); }	
*/		}
		integration_start_time = integration_end_time;
		// routines	
		evaluate_and_save_period_run_time(rt_tasks_data, cpu_id, cpu_thread_id, cpu_thread_task_id, curr_time, rt_get_cpu_time_ns());		
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
	RtTasksData *rt_tasks_data = bmi_data->rt_tasks_data;

	Network		*blue_spike_network = bmi_data->blue_spike_network;
	Neuron		*blue_spike_neuron;
	SpikeTimeStampItem *spike_time_stamp_buff = bmi_data->sorted_spike_time_stamp->spike_time_stamp_buff;
	SpikeTimeStampItem *spike_time_stamp_item; 
	unsigned int blue_spike_buff_read_idx, end_idx;
	unsigned int *blue_spike_buff_write_idx = &(bmi_data->sorted_spike_time_stamp->buff_idx_write);
	unsigned int blue_spike_buff_size = SPIKE_TIME_STAMP_BUFF_SIZE;
	unsigned int mwa_or_layer, channel_or_neuron_group, unit_or_neuron;
	TimeStamp spike_time;
	SpikeData *blue_spike_spike_data = bmi_data->blue_spike_spike_data;
	if (! check_rt_task_specs_to_init(rt_tasks_data, BLUE_SPIKE_BUFF_HANDLER_CPU_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_TASK_ID, BLUE_SPIKE_BUFF_HANDLER_PERIOD))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_blue_spike_rt_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(BLUE_SPIKE_BUFF_HANDLER_TASK_NAME, BLUE_SPIKE_BUFF_HANDLER_TASK_PRIORITY, BLUE_SPIKE_BUFF_HANDLER_STACK_SIZE, BLUE_SPIKE_BUFF_HANDLER_MSG_SIZE, BLUE_SPIKE_BUFF_HANDLER_POLICY, 1 << ((BLUE_SPIKE_BUFF_HANDLER_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_blue_spike_rt_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, BLUE_SPIKE_BUFF_HANDLER_CPU_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_TASK_ID, BLUE_SPIKE_BUFF_HANDLER_PERIOD, BLUE_SPIKE_BUFF_HANDLER_POSITIVE_JITTER_THRES, BLUE_SPIKE_BUFF_HANDLER_NEGATIVE_JITTER_THRES, "BlueSpike_Buff_Handler"))  {
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
		evaluate_and_save_jitter(rt_tasks_data, BLUE_SPIKE_BUFF_HANDLER_CPU_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_TASK_ID, prev_time, curr_time);
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
		evaluate_and_save_period_run_time(rt_tasks_data, BLUE_SPIKE_BUFF_HANDLER_CPU_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_TASK_ID, curr_time, rt_get_cpu_time_ns());		
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


