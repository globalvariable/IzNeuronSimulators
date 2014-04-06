#include "HybridNetRLBMIRtTask.h"


static bool hybrid_net_rl_bmi_rt_tasks_stay_alive = 1;

static int *hybrid_net_rl_bmi_internal_network_handling_rt_threads = NULL;
static unsigned int *hybrid_net_rl_bmi_internal_network_handling_rt_thread_ids = NULL;
static int hybrid_net_rl_bmi_blue_spike_handling_rt_thread = 0;
static int trial_hand_2_neural_net_msgs_handler_rt_thread = 0;
//static int mov_obj_hand_2_neural_net_msgs_handler_rt_thread = 0;

static void *hybrid_net_rl_bmi_internal_network_handler(void *args); 
static void *hybrid_net_rl_bmi_blue_spike_rt_handler(void *args); 
static void *trial_hand_2_neural_net_msgs_handler(void *args); 

static void update_synaptic_weights_all_neurons(Neuron	**all_neurons, unsigned int num_of_all_neurons, unsigned int task_num, unsigned int num_of_dedicated_cpu_threads, double reward, HybridNetRLBMIRewardData *reward_data);


void hybrid_net_rl_bmi_create_rt_threads(void)
{
	static bool first = TRUE;
	unsigned int i;
	HybridNetRLBMIData *bmi_data = get_hybrid_net_rl_bmi_data(); 

	if (bmi_data->in_silico_network->layer_count != NUM_OF_IN_SILICO_NETWORK_LAYERS)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_create_rt_threads", "in_silico_network->layer_count != NUM_OF_IN_SILICO_NETWORK_LAYERS.");			
	
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

		first = FALSE;
	}
	else
	{
		print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_create_rt_threads", "CANNOT create rt_thread again. Should restart the program. ");
		exit(1);	
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
	SpikeData	**in_silico_spike_data_for_graph = bmi_data->in_silico_spike_data_for_graph ;
	SpikeData	**in_silico_spike_data_for_recording = bmi_data->in_silico_spike_data_for_recording ;
	unsigned int i; 
	unsigned int num_of_dedicated_cpu_threads;
	unsigned int cpu_id, cpu_thread_id, cpu_thread_task_id;
	char task_name[10];
	char task_num_name[4];

	MovObjHand2NeuralNetMsg *msgs_mov_obj_hand_2_neural_net;
	MovObjHand2NeuralNetMsgItem msg_item;

	HybridNetRLBMIRewardData *reward_data = &(bmi_data->reward_data);
	unsigned int target_idx;
	double reward;

	TimeStamp *sys_time_ptr = bmi_data->sys_time_ptr;

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
	if (! check_rt_task_specs_to_init(rt_tasks_data, cpu_id, cpu_thread_id, cpu_thread_task_id, IZ_PS_NETWORK_SIM_PERIOD, FALSE))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(nam2num(task_name), IZ_PS_NETWORK_SIM_TASK_PRIORITY, IZ_PS_NETWORK_SIM_STACK_SIZE, IZ_PS_NETWORK_SIM_MSG_SIZE, IZ_PS_NETWORK_SIM_POLICY, 1 << ((cpu_id*MAX_NUM_OF_CPU_THREADS_PER_CPU)+cpu_thread_id)))) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, cpu_id, cpu_thread_id, cpu_thread_task_id, IZ_PS_NETWORK_SIM_PERIOD, IZ_PS_NETWORK_SIM_POSITIVE_JITTER_THRES, IZ_PS_NETWORK_SIM_NEGATIVE_JITTER_THRES, "HybridNetRLBMI", FALSE))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	
        period = nano2count(IZ_PS_NETWORK_SIM_PERIOD);
        rt_task_make_periodic(handler, rt_get_time() + period, period);
	prev_time = rt_get_cpu_time_ns();	
	integration_start_time = ((*sys_time_ptr)/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE) *PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
	reset_all_network_iz_neuron_dynamics (in_silico_network);

        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 
	for (i = task_num; i < num_of_all_neurons; i+=num_of_dedicated_cpu_threads)  // blue spike buffer handler might schedule events earlier.  // only handle the neurons this thread is responsible for
		reset_neuron_event_buffer(all_neurons[i]);

	msgs_mov_obj_hand_2_neural_net = (*msgs_mov_obj_hand_2_neural_net_multi_thread)[task_num];

	msgs_mov_obj_hand_2_neural_net->buff_read_idx = msgs_mov_obj_hand_2_neural_net->buff_write_idx;   // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,
	

        while (hybrid_net_rl_bmi_rt_tasks_stay_alive) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(rt_tasks_data, cpu_id, cpu_thread_id, cpu_thread_task_id, prev_time, curr_time);
		prev_time = curr_time;
		// routines

		// handle mov obj hand 2 neural net msgs.
		while (get_next_mov_obj_hand_2_neural_net_msg_buffer_item(msgs_mov_obj_hand_2_neural_net, &msg_item))		
		{
			switch (msg_item.msg_type)
			{
				case MOV_OBJ_HAND_2_NEURAL_NET_MSG_REINFORCEMENT:
					printf("reinf %d\n", msg_item.additional_data.binary_reward_add.reward);
					target_idx = msg_item.additional_data.binary_reward_add.target_idx;	// target idx is sent to neural net throughout the trial.
					if (msg_item.additional_data.binary_reward_add.reward > 0)  // LTP
					{					
						reward_data->R[target_idx] = (1.0/REWARD_WINDOW_SIZE) + (reward_data->R[target_idx]*((REWARD_WINDOW_SIZE-1)/REWARD_WINDOW_SIZE));
						reward = reward_data->R[target_idx] * reward_data->R_prediction_reward[target_idx];
						update_synaptic_weights_all_neurons(all_neurons, num_of_all_neurons, task_num, num_of_dedicated_cpu_threads, reward, reward_data);
					}
					else	if (msg_item.additional_data.binary_reward_add.reward < 0)	// LTD
					{
						reward_data->R[target_idx] = (-1.0/REWARD_WINDOW_SIZE) + (reward_data->R[target_idx]*((REWARD_WINDOW_SIZE-1)/REWARD_WINDOW_SIZE));		
						reward = reward_data->R[target_idx] * reward_data->R_prediction_reward[target_idx];
						update_synaptic_weights_all_neurons(all_neurons, num_of_all_neurons, task_num, num_of_dedicated_cpu_threads, reward, reward_data);
					}		
					else
					{
						// do nothing, no significant change in robot position.
					}	
					break;	
				case MOV_OBJ_HAND_2_NEURAL_NET_MSG_END_TRIAL_W_REWARD:
					printf("end w rew\n");
					target_idx = msg_item.additional_data.binary_reward_add.target_idx;	// target idx is sent to neural net throughout the trial.
					reward_data->R[0] = 0;
					reward_data->R[1] = 0;
					reward_data->R_prediction_error[target_idx] = (1.0/PREDICTION_ERROR_WINDOW_SIZE) + (reward_data->R_prediction_error[target_idx]*((PREDICTION_ERROR_WINDOW_SIZE-1)/PREDICTION_ERROR_WINDOW_SIZE));
					reward_data->R_prediction_reward[target_idx] = 1- reward_data->R_prediction_error[target_idx];
					break;	
				case MOV_OBJ_HAND_2_NEURAL_NET_MSG_END_TRIAL_W_PUNISH:
					printf("end w pnsh\n");
					target_idx = msg_item.additional_data.binary_reward_add.target_idx;	// target idx is sent to neural net throughout the trial.
					reward_data->R[0] = 0;
					reward_data->R[1] = 0;
					reward_data->R_prediction_error[target_idx] = reward_data->R_prediction_error[target_idx]*((PREDICTION_ERROR_WINDOW_SIZE-1)/PREDICTION_ERROR_WINDOW_SIZE);
					reward_data->R_prediction_reward[target_idx] = 1- reward_data->R_prediction_error[target_idx];
					break;	
				default:
					print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "mov_obj_hand_2_neural_net_msgs_handler", "Invalid message.");	
					exit(1);
			}
		}		

		// start integrations
		integration_end_time =  ((*sys_time_ptr)/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE) *PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
		for (time_ns = integration_start_time; time_ns < integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)   // integrate remaining part in the next task period
		{
			for (i = task_num; i < num_of_all_neurons; i+=num_of_dedicated_cpu_threads)  // simulate the neurons for which this thread is responsible
			{
				nrn = all_neurons[i];
				if (nrn->iz_params == NULL)
				{
					if (! evaluate_poisson_neuron(nrn, &spike_generated, &spike_time, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)) {
						print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "! evaluate_poisson_neuron()."); exit(1); }	
					if (spike_generated)
					{
						write_to_spike_data(in_silico_spike_data_for_graph[task_num], nrn->layer, nrn->neuron_group, nrn->neuron_num, spike_time);  // since exluding poisson neurons via using allocate_network_spike_pattern_graph_scroll_exclude_poisson
						write_to_spike_data(in_silico_spike_data_for_recording[task_num], nrn->layer, nrn->neuron_group, nrn->neuron_num, spike_time);
					}
				}
				else
				{
					if (! evaluate_neuron_dyn_pre_post_stdp_elig(nrn, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, &spike_generated, &spike_time)) {
						print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "! evaluate_neuron_dyn_stdp_psddp_elig()."); exit(1); }	
					if (spike_generated)
					{
						write_to_spike_data(in_silico_spike_data_for_graph[task_num], nrn->layer, nrn->neuron_group, nrn->neuron_num, spike_time);
						write_to_spike_data(in_silico_spike_data_for_recording[task_num], nrn->layer, nrn->neuron_group, nrn->neuron_num, spike_time);
						if (nrn->layer_type == NEURON_LAYER_TYPE_OUTPUT)
						{
							if (! write_to_neural_net_2_mov_obj_hand_msg_buffer((*msgs_neural_net_2_mov_obj_hand_multi_thread)[task_num], integration_start_time, NEURAL_NET_2_MOV_OBJ_HAND_MSG_SPIKE_OUTPUT, nrn->layer, nrn->neuron_group, nrn->neuron_num, spike_time)) {
								print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_internal_network_handler", "! write_to_neural_net_2_mov_obj_hand_msg_buffer()."); exit(1); }	
						}
					}	
					push_neuron_dynamics_to_neuron_dynamics_buffer_limited(in_silico_network, neuron_dynamics_buffer_limited, time_ns, i);
					push_stdp_to_stdp_buffer_limited(in_silico_network, stdp_buffer_limited, time_ns, i);
					push_eligibility_to_eligibility_buffer_limited(in_silico_network, eligibility_buffer_limited, time_ns, i);
				}
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

	SortedSpikes 	*sorted_spike_time_stamp = bmi_data->sorted_spike_time_stamp;  

	SortedSpikeItem *sorted_spike_buffer;
	SortedSpikeItem *sorted_spike_item; 

	unsigned int blue_spike_buff_read_idx[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA], end_idx, idx;
	unsigned int i, j;

	SpikeData **blue_spike_spike_data_for_graph = bmi_data->blue_spike_spike_data_for_graph;

	if (! check_rt_task_specs_to_init(rt_tasks_data, BLUE_SPIKE_BUFF_HANDLER_CPU_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_TASK_ID, BLUE_SPIKE_BUFF_HANDLER_PERIOD, FALSE))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_blue_spike_rt_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(BLUE_SPIKE_BUFF_HANDLER_TASK_NAME, BLUE_SPIKE_BUFF_HANDLER_TASK_PRIORITY, BLUE_SPIKE_BUFF_HANDLER_STACK_SIZE, BLUE_SPIKE_BUFF_HANDLER_MSG_SIZE, BLUE_SPIKE_BUFF_HANDLER_POLICY, 1 << ((BLUE_SPIKE_BUFF_HANDLER_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_blue_spike_rt_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, BLUE_SPIKE_BUFF_HANDLER_CPU_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_TASK_ID, BLUE_SPIKE_BUFF_HANDLER_PERIOD, BLUE_SPIKE_BUFF_HANDLER_POSITIVE_JITTER_THRES, BLUE_SPIKE_BUFF_HANDLER_NEGATIVE_JITTER_THRES, "BlueSpike_Buff_Handler", FALSE))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_blue_spike_rt_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	
        period = nano2count(BLUE_SPIKE_BUFF_HANDLER_PERIOD);
        rt_task_make_periodic(handler, rt_get_time() + period, period);
	prev_time = rt_get_cpu_time_ns();	
        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

	for (i = 0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j = 0; j < MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			blue_spike_buff_read_idx[i][j] = (*sorted_spike_time_stamp)[i][j].buff_idx_write;
		}
	}

        while (hybrid_net_rl_bmi_rt_tasks_stay_alive) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(rt_tasks_data, BLUE_SPIKE_BUFF_HANDLER_CPU_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID, BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_TASK_ID, prev_time, curr_time);
		prev_time = curr_time;
		// routines
		for (i = 0; i < MAX_NUM_OF_MWA; i++)
		{
			for (j = 0; j < MAX_NUM_OF_CHAN_PER_MWA; j++)
			{
				sorted_spike_buffer = (*sorted_spike_time_stamp)[i][j].buffer;
				end_idx = (*sorted_spike_time_stamp)[i][j].buff_idx_write;
				idx = blue_spike_buff_read_idx[i][j];
				while (idx != end_idx)
				{
					sorted_spike_item = &(sorted_spike_buffer[idx]);
					blue_spike_neuron = get_neuron_address(blue_spike_network, i, j, sorted_spike_item->unit);

					if (!schedule_synaptic_event(blue_spike_neuron, sorted_spike_item->peak_time)) {
						print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_blue_spike_rt_handler", "! schedule_event()."); exit(1); }

					write_to_spike_data(blue_spike_spike_data_for_graph[0], i, j, sorted_spike_item->unit, sorted_spike_item->peak_time);	
					if ((idx+1) == BLUESPIKE_SORTED_SPIKE_BUFF_SIZE)
						idx = 0;
					else
						idx++;
				}
				blue_spike_buff_read_idx[i][j] = end_idx;

			}
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
	unsigned int prev_time, curr_time, num_of_cpu_threads; 
	HybridNetRLBMIData *hybrid_net_rl_bmi_data = get_hybrid_net_rl_bmi_data(); 
	char str_trial_hand_msg[TRIAL_HAND_2_NEURAL_NET_MSG_STRING_LENGTH];
//	Network		*in_silico_network = hybrid_net_rl_bmi_data->in_silico_network;
//	unsigned int 		num_of_neurons = in_silico_network->num_of_neurons;
//	Neuron		**all_neurons = in_silico_network->all_neurons;
	TrialHand2NeuralNetMsg *msgs_trial_hand_2_neural_net = NULL;
	TrialHand2NeuralNetMsgItem msg_item;
	TrialStatusEvents* trial_status_events = NULL;
	RtTasksData *rt_tasks_data = NULL;
	NeuralNet2GuiMsgAdditional neural_net_2_gui_msg_add;

	NeuralNet2GuiMsg *msgs_neural_net_2_gui = NULL;
	msgs_neural_net_2_gui = hybrid_net_rl_bmi_data->msgs_neural_net_2_gui;
	msgs_trial_hand_2_neural_net = hybrid_net_rl_bmi_data->msgs_trial_hand_2_neural_net;
	trial_status_events = hybrid_net_rl_bmi_data->trial_status_events;
	rt_tasks_data = hybrid_net_rl_bmi_data->rt_tasks_data;
	num_of_cpu_threads = hybrid_net_rl_bmi_data->num_of_dedicated_cpu_threads;

	if (! check_rt_task_specs_to_init(rt_tasks_data, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD, FALSE))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_NAME, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_PRIORITY, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_STACK_SIZE, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_MSG_SIZE, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_POLICY, 1 << ((TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_POSITIVE_JITTER_THRES, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_NEGATIVE_JITTER_THRES, "TrialHand2NeuralNetMsgHandler", FALSE))  {
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
					schedule_trial_status_event(trial_status_events, *(hybrid_net_rl_bmi_data->sys_time_ptr), msg_item.additional_data.trial_status_change_msg_add) ; 
					break;	
				case TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_START:   // not implemeted yet, for RL it will be required.
/*					current_sys_time = rt_tasks_data->current_system_time;
					for (i = 0; i < num_of_neurons; i++)
					{
						if (all_neurons[i]->trial_event_buffer == NULL)   // maybe not an izhikevich neuron (or a neuron that can accept events, e.g. poisson neuron)
							continue;
						if (! write_to_neuron_trial_event_buffer(all_neurons[i], current_sys_time, NEURON_EVENT_TYPE_TRIAL_START_EVENT)) {
							print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neuron_trial_event_buffer()."); exit(1); }	
					}					
*/					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_REWARD_GIVEN:   
/*					current_sys_time = rt_tasks_data->current_system_time;
					for (i = 0; i < num_of_neurons; i++)
					{
						if (all_neurons[i]->trial_event_buffer == NULL)   // maybe not an izhikevich neuron (or a neuron that can accept events, e.g. poisson neuron)
							continue;
						if (! write_to_neuron_trial_event_buffer(all_neurons[i], current_sys_time, NEURON_EVENT_TYPE_TRIAL_END_WITH_REWARD)) {
							print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neuron_trial_event_buffer()."); exit(1); }	
					}
					neural_net_2_gui_msg_add.reward = msg_item.additional_data.reward;
*/

					if (! write_to_neural_net_2_gui_msg_buffer(msgs_neural_net_2_gui, *(hybrid_net_rl_bmi_data->sys_time_ptr), NEURAL_NET_2_GUI_MSG_UPDATE_SYNAPTIC_WEIGHTS, neural_net_2_gui_msg_add)) {
						print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neural_net_2_gui_msg_buffer()."); exit(1); }	
					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_PUNISHMENT_GIVEN:   
/*					current_sys_time = rt_tasks_data->current_system_time;
					for (i = 0; i < num_of_neurons; i++)
					{
						if (all_neurons[i]->trial_event_buffer == NULL)   // maybe not an izhikevich neuron (or a neuron that can accept events, e.g. poisson neuron)
							continue;
						if (! write_to_neuron_trial_event_buffer(all_neurons[i], current_sys_time, NEURON_EVENT_TYPE_TRIAL_END_WITH_PUNISHMENT)) {
							print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neuron_trial_event_buffer()."); exit(1); }	
					}
					neural_net_2_gui_msg_add.reward = msg_item.additional_data.reward;	/// THIS MESSAGE SHOULD BE SENT BEFORE TRIAL_HAND_2_NEURAL_NET_MSG_STOP_RECORDING which saves the synaptic weights.
*/

					if (! write_to_neural_net_2_gui_msg_buffer(msgs_neural_net_2_gui, *(hybrid_net_rl_bmi_data->sys_time_ptr), NEURAL_NET_2_GUI_MSG_UPDATE_SYNAPTIC_WEIGHTS, neural_net_2_gui_msg_add)) {
						print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neural_net_2_gui_msg_buffer()."); exit(1); }	
					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_END_TRIAL_WITH_NOTHING:	// do nothing. this message received due to invalid behavior of rat in / during the trial (i.e. nose retract) 

					neural_net_2_gui_msg_add.reward = 0;	// zero reward will not update synaptic weights. but the weight history will be kept for this trial in gui timeout callback (NetworkView.c)
					// no need to send any message to neurons such as NEURON_EVENT_TYPE_TRIAL_END_WITH_REWARD etc since the weights will not be updated at the end of this trial. so no eligiility saving required for the neurons.			
					if (! write_to_neural_net_2_gui_msg_buffer(msgs_neural_net_2_gui, *(hybrid_net_rl_bmi_data->sys_time_ptr), NEURAL_NET_2_GUI_MSG_UPDATE_SYNAPTIC_WEIGHTS, neural_net_2_gui_msg_add)) {
						print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neural_net_2_gui_msg_buffer()."); exit(1); }	
					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_START_RECORDING:	
					neural_net_2_gui_msg_add.recording_number = msg_item.additional_data.recording_number;
					if (! write_to_neural_net_2_gui_msg_buffer(msgs_neural_net_2_gui, *(hybrid_net_rl_bmi_data->sys_time_ptr), NEURAL_NET_2_GUI_MSG_START_RECORDING, neural_net_2_gui_msg_add)) {
						print_message(BUG_MSG ,"NeuralNetler", "HandleTrialHand2NeuralNetMsgs", "write_to_neural_net_2_gui_msg_buffer", "! write_to_neural_net_2_gui_msg_buffer(().");exit(1); }	
					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_STOP_RECORDING:	
					neural_net_2_gui_msg_add.recording_number = msg_item.additional_data.recording_number;
					if (! write_to_neural_net_2_gui_msg_buffer(msgs_neural_net_2_gui, *(hybrid_net_rl_bmi_data->sys_time_ptr), NEURAL_NET_2_GUI_MSG_STOP_RECORDING, neural_net_2_gui_msg_add)) {
						print_message(BUG_MSG ,"NeuralNetler", "HandleTrialHand2NeuralNetMsgs", "write_to_neural_net_2_gui_msg_buffer", "! write_to_neural_net_2_gui_msg_buffer(().");exit(1); }	
					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_CANCEL_RECORDING:	
					neural_net_2_gui_msg_add.recording_number = msg_item.additional_data.recording_number;
					if (! write_to_neural_net_2_gui_msg_buffer(msgs_neural_net_2_gui, *(hybrid_net_rl_bmi_data->sys_time_ptr), NEURAL_NET_2_GUI_MSG_CANCEL_RECORDING, neural_net_2_gui_msg_add)) {
						print_message(BUG_MSG ,"NeuralNetler", "HandleTrialHand2NeuralNetMsgs", "write_to_neural_net_2_gui_msg_buffer", "! write_to_neural_net_2_gui_msg_buffer(().");exit(1); }	
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

void hybrid_net_rl_bmi_kill_rt_tasks(void)
{
	hybrid_net_rl_bmi_rt_tasks_stay_alive = 0;
}
/*
static void *mov_obj_hand_2_neural_net_msgs_handler(void *args)
{
	RT_TASK *handler;
        RTIME period;
	unsigned int i, j, prev_time, curr_time, num_of_cpu_threads; 
	HybridNetRLBMIData *hybrid_net_rl_bmi_data = get_hybrid_net_rl_bmi_data(); 
	MovObjHand2NeuralNetMsgMultiThread		*msgs_mov_obj_hand_2_neural_net_multi_thread;
	MovObjHand2NeuralNetMsgItem msg_item;
	RtTasksData *rt_tasks_data = NULL;

	static double robot_angles[THREE_DOF_ROBOT_NUM_OF_SERVOS];
	static double robot_angles_prev[THREE_DOF_ROBOT_NUM_OF_SERVOS];

	
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

	for (i = 0; i < THREE_DOF_ROBOT_NUM_OF_SERVOS; i++)
	{
		robot_angles[i] = M_PI_2;
		robot_angles_prev[i] = M_PI_2;
	}

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
					case MOV_OBJ_HAND_2_NEURAL_NET_MSG_3_DOF_JOINT_ANGLE:
						for (j = 0; j < THREE_DOF_ROBOT_NUM_OF_SERVOS; j++)
						{
							robot_angles[j] = msg_item.additional_data.three_dof_robot_joint_angles[j];
						}
						break;	
					default:
						print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "mov_obj_hand_2_neural_net_msgs_handler", "Invalid message.");	
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
*/

static void update_synaptic_weights_all_neurons(Neuron	**all_neurons, unsigned int num_of_all_neurons, unsigned int task_num, unsigned int num_of_dedicated_cpu_threads, double reward, HybridNetRLBMIRewardData *reward_data)
{
	unsigned int i, j;
	Neuron *nrn;
	Synapse			*synapses;
	SynapseIndex		num_of_synapses; 
	double	E, dw, E_max, E_ltp, E_ltd;  // eligibility
	double weight;
	for (i = task_num; i < num_of_all_neurons; i+=num_of_dedicated_cpu_threads)  // simulate the neurons for which this thread is responsible
	{
		nrn = all_neurons[i];
		if (nrn->iz_params == NULL)
			continue;
		synapses = nrn->syn_list->synapses;
		num_of_synapses = nrn->syn_list->num_of_synapses;
		for (j = 0; j < num_of_synapses; j++)
		{
			if (! synapses[j].plastic)
				continue;
			E = synapses[j].ps_eligibility->now;
			E_max = synapses[j].ps_eligibility->max_eligibility;
			if (reward > 0)    // LTP
			{
				E_ltp = E;
				dw = (reward * E_ltp);
				dw = synapses[j].weight * dw * reward_data->learning_rate;
				synapses[j].weight += dw; 
			}
			else
			{
				E_ltd = pow (E, 3.0);
				dw = (reward * E_ltd);		// E_ltd is always positive.  reward <=0 for ltd then dw is always negative.
				dw = synapses[j].weight * dw * reward_data->learning_rate;
				weight = synapses[j].weight + dw;
				if (weight < 0.1)
					synapses[j].weight = 0.1; 
				else
					synapses[j].weight = weight; 
			}
		}

	}
	return;
}
