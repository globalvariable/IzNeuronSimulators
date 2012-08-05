#include "HybridNetRLBMIRtTask.h"


static bool hybrid_net_rl_bmi_rt_tasks_stay_alive = 1;

static int *hybrid_net_rl_bmi_internal_network_handling_rt_threads = NULL;
static unsigned int *hybrid_net_rl_bmi_internal_network_handling_rt_thread_ids = NULL;
static int hybrid_net_rl_bmi_blue_spike_handling_rt_thread = 0;
static int trial_hand_2_neural_net_msgs_handler_rt_thread = 0;
static int neural_net_2_post_trial_hand_msgs_handler_thread = 0;
static int mov_obj_hand_2_neural_net_msgs_handler_rt_thread = 0;
static pthread_t post_trial_hand_thread;

static void *hybrid_net_rl_bmi_internal_network_handler(void *args); 
static void *hybrid_net_rl_bmi_blue_spike_rt_handler(void *args); 
static void *trial_hand_2_neural_net_msgs_handler(void *args); 
static void *mov_obj_hand_2_neural_net_msgs_handler(void *args); 
static void *neural_net_2_post_trial_hand_msgs_handler(void *args); 

void hybrid_net_rl_bmi_create_rt_threads(void)
{
	static bool first = TRUE;
	unsigned int i;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data(); 
	
	if (first)
	{
		get_hybrid_net_rl_bmi_data()->simulation_in_progress = TRUE;
		hybrid_net_rl_bmi_internal_network_handling_rt_threads = g_new0(int, bmi_data->num_of_dedicated_cpu_threads);
		hybrid_net_rl_bmi_internal_network_handling_rt_thread_ids = g_new0(unsigned int, bmi_data->num_of_dedicated_cpu_threads);

		for (i = 0; i < bmi_data->num_of_dedicated_cpu_threads; i++)
		{
			hybrid_net_rl_bmi_internal_network_handling_rt_thread_ids[i] = i; 
			hybrid_net_rl_bmi_internal_network_handling_rt_threads[i] =  rt_thread_create(hybrid_net_rl_bmi_internal_network_handler, &(hybrid_net_rl_bmi_internal_network_handling_rt_thread_ids[i]), 10000);
		}
		hybrid_net_rl_bmi_blue_spike_handling_rt_thread = rt_thread_create( hybrid_net_rl_bmi_blue_spike_rt_handler, NULL, 10000);
		trial_hand_2_neural_net_msgs_handler_rt_thread =  rt_thread_create(trial_hand_2_neural_net_msgs_handler, NULL, 10000);
		mov_obj_hand_2_neural_net_msgs_handler_rt_thread =  rt_thread_create(mov_obj_hand_2_neural_net_msgs_handler, NULL, 10000);
		neural_net_2_post_trial_hand_msgs_handler_thread = pthread_create( &post_trial_hand_thread, NULL, neural_net_2_post_trial_hand_msgs_handler, NULL);

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
	unsigned int task_num = *((unsigned int*)args);
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data(); 
	RtTasksData *rt_tasks_data = bmi_data->rt_tasks_data;
	Network		*in_silico_network =  bmi_data->in_silico_network;
	NeuralNet2MovObjHandMsgMultiThread *msgs_neural_net_2_mov_obj_hand_multi_thread = bmi_data->msgs_neural_net_2_mov_obj_hand_multi_thread;
	MovObjHand2NeuralNetMsgMultiThread *msgs_mov_obj_hand_2_neural_net_multi_thread = bmi_data->msgs_mov_obj_hand_2_neural_net_multi_thread;
	Neuron		**all_neurons = in_silico_network->all_neurons;
	unsigned int	num_of_all_neurons =  in_silico_network->num_of_neurons;
	Neuron 		*nrn;

	NeuronDynamicsBufferLimited *neuron_dynamics_buffer_limited = bmi_data->neuron_dynamics_limited_buffer;
	STDPBufferLimited *stdp_buffer_limited = bmi_data->stdp_limited_buffer;
	EligibilityBufferLimited *eligibility_buffer_limited = bmi_data->eligibility_limited_buffer;
	DepolEligibilityBufferLimited *depol_eligibility_buffer_limited = bmi_data->depol_eligibility_limited_buffer;
	SpikeData	*in_silico_spike_data = bmi_data->in_silico_spike_data ;
	FiringCount	*num_of_firing =  bmi_data->num_of_firing_of_neurons_in_trial;
	unsigned int i; 
	unsigned int num_of_dedicated_cpu_threads;
	unsigned int cpu_id, cpu_thread_id, cpu_thread_task_id;
	char task_name[10];
	char task_num_name[4];

	/// share the neurons in between rt_threads;    ie.g there are 21 neurons and 4 dedicated cpu threads. then the cpu threads will have 6 + 6 + 6 + 3 neurons to handle
	num_of_dedicated_cpu_threads = IZ_PS_NETWORK_SIM_NUM_OF_DEDICATED_CPUS * MAX_NUM_OF_CPU_THREADS_PER_CPU;
	if (num_of_dedicated_cpu_threads != (NUM_OF_NEURAL_NET_2_MOV_OBJ_HAND_MSG_BUFFERS)) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "num_of_dedicated_cpu_threads != (NUM_OF_NEURAL_NET_2_MOV_OBJ_HAND_MSG_BUFFERS."); exit(1); }	
	if (num_of_dedicated_cpu_threads != (NUM_OF_MOV_OBJ_HAND_2_NEURAL_NET_MSG_BUFFERS)) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "num_of_dedicated_cpu_threads != (NUM_OF_NEURAL_NET_2_MOV_OBJ_HAND_MSG_BUFFERS."); exit(1); }	

	for (i = task_num; i < num_of_all_neurons; i+=num_of_dedicated_cpu_threads)  // blue spike buffer handler might schedule events earlier.  // only handle the neurons this thread is responsible for
		printf ("Task Num: %u\t Neuron: %u\n", task_num, i);

	sprintf(task_num_name, "%u", task_num );
	strcpy(task_name, IZ_PS_NETWORK_SIM_TASK_NAME);
	strcat(task_name, task_num_name);

	cpu_id =  task_num / MAX_NUM_OF_CPU_THREADS_PER_CPU;   // find the cpu_id and cpu_thread_id to run this rt_thread
	cpu_thread_id = task_num - (cpu_id * MAX_NUM_OF_CPU_THREADS_PER_CPU);
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

        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 
	for (i = task_num; i < num_of_all_neurons; i+=num_of_dedicated_cpu_threads)  // blue spike buffer handler might schedule events earlier.  // only handle the neurons this thread is responsible for
		reset_neuron_event_buffer(all_neurons[i]);

	for (i = 0; i < num_of_dedicated_cpu_threads; i++)
	{
		((*msgs_mov_obj_hand_2_neural_net_multi_thread)[i])->buff_read_idx = ((*msgs_mov_obj_hand_2_neural_net_multi_thread)[i])->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,
	}
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
			for (i = task_num; i < num_of_all_neurons; i+=num_of_dedicated_cpu_threads)  // simulate the neurons for which this thread is responsible
			{
				nrn = all_neurons[i];
				if (!evaluate_neuron_dyn_stdp_elig_depol_elig(nrn, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, &spike_generated, &spike_time)) {
					print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "! evaluate_neuron_dyn_stdp_elig_depol_elig()."); exit(1); }	
				if (spike_generated)
				{
//					write_to_spike_data(in_silico_spike_data, nrn->layer, nrn->neuron_group, nrn->neuron_num, spike_time);
					if (nrn->layer_type == NEURON_LAYER_TYPE_OUTPUT)
					{
						if (! write_to_neural_net_2_mov_obj_hand_msg_buffer((*msgs_neural_net_2_mov_obj_hand_multi_thread)[task_num], integration_start_time, NEURAL_NET_2_MOV_OBJ_HAND_MSG_SPIKE_OUTPUT, nrn->layer, nrn->neuron_group, nrn->neuron_num, spike_time)) {
							print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "! write_to_neural_net_2_mov_obj_hand_msg_buffer()."); exit(1); }	
					}
					num_of_firing[i]++;
				}	
				push_neuron_dynamics_to_neuron_dynamics_buffer_limited(in_silico_network, neuron_dynamics_buffer_limited, time_ns, i);
				push_stdp_to_stdp_buffer_limited(in_silico_network, stdp_buffer_limited, time_ns, i);
				push_eligibility_to_eligibility_buffer_limited(in_silico_network, eligibility_buffer_limited, time_ns, i);
				push_depol_eligibility_to_depol_eligibility_buffer_limited(in_silico_network, depol_eligibility_buffer_limited, time_ns, i);
			}

		}
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
			if (!schedule_synaptic_event(blue_spike_neuron, spike_time)) {
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

static void *trial_hand_2_neural_net_msgs_handler(void *args)
{
	RT_TASK *handler;
        RTIME period;
	unsigned int i, prev_time, curr_time, num_of_cpu_threads; 
	HybridNetRLBMIData *hybrid_net_rl_bmi_data = get_hybrid_net_rl_bmi_data(); 
	char str_trial_hand_msg[TRIAL_HAND_2_NEURAL_NET_MSG_STRING_LENGTH];
	Network		*in_silico_network = hybrid_net_rl_bmi_data->in_silico_network;
	unsigned int 		num_of_neurons = in_silico_network->num_of_neurons;
	Neuron		**all_neurons = in_silico_network->all_neurons;
	TrialHand2NeuralNetMsg *msgs_trial_hand_2_neural_net = NULL;
	TrialHand2NeuralNetMsgItem msg_item;
	TrialStatusEvents* trial_status_events = NULL;
	RtTasksData *rt_tasks_data = NULL;
	TimeStamp current_sys_time;
	double trajectory_success_ratio = 0;
	NeuralNet2PostTrialHandMsg *msgs_neural_net_2_post_trial_hand = NULL;
	msgs_neural_net_2_post_trial_hand = hybrid_net_rl_bmi_data->msgs_neural_net_2_post_trial_hand;
	msgs_trial_hand_2_neural_net = hybrid_net_rl_bmi_data->msgs_trial_hand_2_neural_net;
	trial_status_events = hybrid_net_rl_bmi_data->trial_status_events;
	rt_tasks_data = hybrid_net_rl_bmi_data->rt_tasks_data;
	num_of_cpu_threads = hybrid_net_rl_bmi_data->num_of_dedicated_cpu_threads;

	if (! check_rt_task_specs_to_init(rt_tasks_data, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_NAME, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_PRIORITY, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_STACK_SIZE, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_MSG_SIZE, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_POLICY, 1 << ((TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_POSITIVE_JITTER_THRES, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_NEGATIVE_JITTER_THRES, "TrialHand2NeuralNetMsgHandler"))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	
        period = nano2count(TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD);

        rt_task_make_periodic(handler, rt_get_time() + period, period);
	prev_time = rt_get_cpu_time_ns();	
        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

	msgs_trial_hand_2_neural_net->buff_read_idx = msgs_trial_hand_2_neural_net->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,

        while (1) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(rt_tasks_data, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, prev_time, curr_time);
		prev_time = curr_time;
		// routines

		while (get_next_trial_hand_2_neural_net_msg_buffer_item(msgs_trial_hand_2_neural_net, &msg_item))
		{
			get_trial_hand_2_neural_net_msg_type_string(msg_item.msg_type, str_trial_hand_msg);  
			print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", str_trial_hand_msg);
			switch (msg_item.msg_type)
			{
				case TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_STATUS_CHANGED:   // current system time might be different from the one TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_START. it can change during processing the message buffer but would not lead to problem.  TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_STATUS_CHANGED is used by the graphs.
					schedule_trial_status_event(trial_status_events, rt_tasks_data->current_system_time, msg_item.additional_data_0, msg_item.additional_data_1) ; 
					break;	
				case TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_START:   // not implemeted yet, for RL it will be required.
					current_sys_time = rt_tasks_data->current_system_time;
					for (i = 0; i < num_of_neurons; i++)
					{
						if (! write_to_neuron_trial_event_buffer(all_neurons[i], current_sys_time, NEURON_EVENT_TYPE_TRIAL_START_EVENT)) {
							print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neuron_trial_event_buffer()."); exit(1); }	
					}					
					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_MOV_OBJ_REACHED_THRESHOLD:  
					current_sys_time = rt_tasks_data->current_system_time;
					for (i = 0; i < num_of_neurons; i++)
					{
						if (! write_to_neuron_trial_event_buffer(all_neurons[i], current_sys_time, NEURON_EVENT_TYPE_TRIAL_END_WITH_REWARD)) {
							print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neuron_trial_event_buffer()."); exit(1); }	
					}
					if (! write_to_neural_net_2_post_trial_hand_msg_buffer(msgs_neural_net_2_post_trial_hand, current_sys_time, NEURAL_NET_2_POST_TRIAL_HAND_MSG_TRIAL_END, trajectory_success_ratio)) {
						print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neural_net_2_post_trial_hand_msg_buffer()."); exit(1); }	
					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_TIMEOUT_BEFORE_THRESHOLD_REACH:
					current_sys_time = rt_tasks_data->current_system_time;
					for (i = 0; i < num_of_neurons; i++)
					{
						if (! write_to_neuron_trial_event_buffer(all_neurons[i], current_sys_time, NEURON_EVENT_TYPE_TRIAL_END_WITH_PUNISHMENT)) {
							print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neuron_trial_event_buffer()."); exit(1); }	
					}
					if (! write_to_neural_net_2_post_trial_hand_msg_buffer(msgs_neural_net_2_post_trial_hand, current_sys_time, NEURAL_NET_2_POST_TRIAL_HAND_MSG_TRIAL_END, trajectory_success_ratio)) {
						print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neural_net_2_post_trial_hand_msg_buffer()."); exit(1); }	
					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_TRAJECTORY_SUCCESS_RATIO:  // this message is received before 
					trajectory_success_ratio = msg_item.additional_data_0;
					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_REWARD_GIVEN:   // it is sent here just before trial ends.   // since threshold reached before this message, the neurons handles NEURON_EVENT_TYPE_TRIAL_END_WITH_REWARD previously. then, this message is useless and it is not sent. it is here just to be template for future applicaitons.
					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_PUNISHMENT_GIVEN:   // it is sent here just before trial ends.   // since trial timed out before this message, the neurons handles NEURON_EVENT_TYPE_TRIAL_END_WITH_PUNISHMENT previously. then, this message is useless and it is not sent. it is here just to be template for future applicaitons.
					break;
				default: 
					print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", str_trial_hand_msg);
					rt_make_soft_real_time();
        				rt_task_delete(handler);
					print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "rt_task_delete().");	
				        exit(1); 
			}
		}
		// routines	
		evaluate_and_save_period_run_time(rt_tasks_data, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, curr_time, rt_get_cpu_time_ns());		
        }
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "rt_task_delete().");	

        return 0;
}

static void *neural_net_2_post_trial_hand_msgs_handler(void *args)
{
	char str_neural_net_msg[NEURAL_NET_2_POST_TRIAL_HAND_MSG_STRING_LENGTH];
	NeuralNet2PostTrialHandMsg *msgs_neural_net_2_post_trial_hand = NULL;
	NeuralNet2PostTrialHandMsgItem msg_item;
	HybridNetRLBMIData *hybrid_net_rl_bmi_data = get_hybrid_net_rl_bmi_data(); 
	Neuron		**all_neurons = hybrid_net_rl_bmi_data->in_silico_network->all_neurons;
	unsigned int 		i, num_of_neurons = hybrid_net_rl_bmi_data->in_silico_network->num_of_neurons;
	msgs_neural_net_2_post_trial_hand = get_hybrid_net_rl_bmi_data()->msgs_neural_net_2_post_trial_hand;
	double change_rate;
        while (1) 
	{
		sleep(1);
		while (get_next_neural_net_2_post_trial_hand_msg_buffer_item(msgs_neural_net_2_post_trial_hand, &msg_item))
		{
			get_neural_net_2_post_trial_hand_msg_type_string(msg_item.msg_type, str_neural_net_msg);  
			print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "neural_net_2_post_trial_hand_msgs_handler", str_neural_net_msg);
			switch (msg_item.msg_type)
			{
				case NEURAL_NET_2_POST_TRIAL_HAND_MSG_TRIAL_END: // update synaptic weights 
					sleep(1);	// wait one second to ensure trial is really ended.
					change_rate = msg_item.additional_data;
					printf ("trajectory success: %f\n", change_rate);
					for (i = 0; i < num_of_neurons; i++)
					{
						if (! update_neuron_synaptic_weights_with_history(all_neurons[i], change_rate)) {
							print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "neural_net_2_post_trial_hand_msgs_handler", "! update_neuron_synaptic_weights()"); exit(1); }
					}
					break;	
				default: 
					print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "neural_net_2_post_trial_hand_msgs_handler", str_neural_net_msg);
				        exit(1); 
			}
		}		
	}
	return 0;
}

void hybrid_net_rl_bmi_kill_rt_tasks(void)
{
	hybrid_net_rl_bmi_rt_tasks_stay_alive = 0;
}

static void *mov_obj_hand_2_neural_net_msgs_handler(void *args)
{
	RT_TASK *handler;
        RTIME period;
	unsigned int i, prev_time, curr_time, num_of_cpu_threads; 
	HybridNetRLBMIData *hybrid_net_rl_bmi_data = get_hybrid_net_rl_bmi_data(); 
	MovObjHand2NeuralNetMsgMultiThread		*msgs_mov_obj_hand_2_neural_net_multi_thread;
	MovObjHand2NeuralNetMsgItem msg_item;
	RtTasksData *rt_tasks_data = NULL;
	Network *in_silico_network = hybrid_net_rl_bmi_data->in_silico_network;
	Neuron *place_cell = get_neuron_address(in_silico_network, 2, 0, 0);   /// it holds the address of last place cell fired according to location. At startup location is 0
	InjectionCurrent	***baseline_injection_current = hybrid_net_rl_bmi_data->baseline_injection_current->current;
	InjectionCurrent	***location_injection_current = hybrid_net_rl_bmi_data->location_injection_current->current;
	double location;

	msgs_mov_obj_hand_2_neural_net_multi_thread = hybrid_net_rl_bmi_data->msgs_mov_obj_hand_2_neural_net_multi_thread;
	rt_tasks_data = hybrid_net_rl_bmi_data->rt_tasks_data;
	num_of_cpu_threads = hybrid_net_rl_bmi_data->num_of_dedicated_cpu_threads;

	if (! check_rt_task_specs_to_init(rt_tasks_data, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "mov_obj_hand_2_neural_net_msgs_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_NAME, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_PRIORITY, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_STACK_SIZE, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_MSG_SIZE, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_POLICY, 1 << ((MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "mov_obj_hand_2_neural_net_msgs_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_POSITIVE_JITTER_THRES, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_NEGATIVE_JITTER_THRES, "MovObjHand2NeuralNetMsgHandler"))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "mov_obj_hand_2_neural_net_msgs_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	
        period = nano2count(MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD);

        rt_task_make_periodic(handler, rt_get_time() + period, period);
	prev_time = rt_get_cpu_time_ns();	
        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

	for (i = 0; i < (NUM_OF_NEURAL_NET_2_MOV_OBJ_HAND_MSG_BUFFERS); i++)
	{
		(*(hybrid_net_rl_bmi_data->msgs_mov_obj_hand_2_neural_net_multi_thread))[i]->buff_read_idx =(*(hybrid_net_rl_bmi_data->msgs_mov_obj_hand_2_neural_net_multi_thread))[i]->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,
	}
        while (1) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(rt_tasks_data, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, prev_time, curr_time);
		prev_time = curr_time;
		// routines
		for (i = 0; i < (NUM_OF_NEURAL_NET_2_MOV_OBJ_HAND_MSG_BUFFERS); i++)
		{
			while (get_next_mov_obj_hand_2_neural_net_msg_buffer_item((*(hybrid_net_rl_bmi_data->msgs_mov_obj_hand_2_neural_net_multi_thread))[i], &msg_item))
			{
				switch (msg_item.msg_type)
				{
					case MOV_OBJ_HAND_2_NEURAL_NET_MSG_LOCATION:
						place_cell->iz_params->I_inject = baseline_injection_current[place_cell->layer][place_cell->neuron_group][place_cell->neuron_num];
						location = msg_item.additional_data;
						if (location == 0)
						{
							place_cell = get_neuron_address(in_silico_network, 2, 0, 0);
							place_cell->iz_params->I_inject = location_injection_current[place_cell->layer][place_cell->neuron_group][place_cell->neuron_num];
						}			
						else if (location > 0)   /// left locations
						{
							place_cell = get_neuron_address(in_silico_network, 3, 0, (unsigned int)(location/10));
							place_cell->iz_params->I_inject = location_injection_current[place_cell->layer][place_cell->neuron_group][place_cell->neuron_num];
						}
						else ///  (location < 0)  /// right locations
						{
							place_cell = get_neuron_address(in_silico_network, 4, 0, (unsigned int)(-location/10));
							place_cell->iz_params->I_inject = location_injection_current[place_cell->layer][place_cell->neuron_group][place_cell->neuron_num];
						}
						break;		
					default:
						print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "Invalid message.");	
						exit(1);
				}
			}
		}
		// routines	
		evaluate_and_save_period_run_time(rt_tasks_data, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, curr_time, rt_get_cpu_time_ns());		
        }
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "mov_obj_hand_2_neural_net_msgs_handler", "rt_task_delete().");	

        return 0;
}

