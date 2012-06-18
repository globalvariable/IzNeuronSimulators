#include "HybridNetRLBMI.h"

static HybridNetRLBMIData *hybrid_net_rl_bmi_data = NULL;

static bool connect_to_trial_hand(void);
static bool connect_to_mov_obj_hand(void);

static int trial_hand_2_neural_net_msgs_handler_rt_thread = 0;
static void *trial_hand_2_neural_net_msgs_handler(void *args); 

int main( int argc, char *argv[])
{
	unsigned int i, j;

	hybrid_net_rl_bmi_data = g_new0(HybridNetRLBMIData, 1);

	hybrid_net_rl_bmi_data->sorted_spike_time_stamp = rtai_malloc(nam2num(KERNEL_SPIKE_SPIKE_TIME_STAMP_SHM_NAME), 0);
	if (hybrid_net_rl_bmi_data->sorted_spike_time_stamp == NULL) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "hybrid_net_rl_bmi_data->sorted_spike_time_stamp == NULL."); return -1; }
	hybrid_net_rl_bmi_data->rt_tasks_data = rtai_malloc(nam2num(RT_TASKS_DATA_SHM_NAME), 0);
	if (hybrid_net_rl_bmi_data->rt_tasks_data == NULL) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "hybrid_net_rl_bmi_data->rt_tasks_data == NULL."); return -1; }
	hybrid_net_rl_bmi_data->blue_spike_network = allocate_network(hybrid_net_rl_bmi_data->blue_spike_network);
	hybrid_net_rl_bmi_data->in_silico_network = allocate_network(hybrid_net_rl_bmi_data->in_silico_network);
	hybrid_net_rl_bmi_data->motor_outputs = allocate_motor_outputs(hybrid_net_rl_bmi_data->motor_outputs, 1);
	hybrid_net_rl_bmi_data->msgs_trial_hand_2_neural_net = allocate_shm_server_trial_hand_2_neural_net_msg_buffer(hybrid_net_rl_bmi_data->msgs_trial_hand_2_neural_net);
	hybrid_net_rl_bmi_data->msgs_mov_obj_hand_2_neural_net = allocate_shm_server_mov_obj_hand_2_neural_net_msg_buffer(hybrid_net_rl_bmi_data->msgs_mov_obj_hand_2_neural_net);
	hybrid_net_rl_bmi_data->trial_status_events = allocate_trial_status_events_buffer(hybrid_net_rl_bmi_data->trial_status_events, 100, 3000000);  //  3 ms latency

	if (! connect_to_mov_obj_hand()) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "connect_to_mov_obj_hand()."); return -1; }

	if (! connect_to_trial_hand()) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "connect_to_trial_hand()."); return -1; }

	trial_hand_2_neural_net_msgs_handler_rt_thread =  rt_thread_create(trial_hand_2_neural_net_msgs_handler, NULL, 10000);

	for (i = 0; i < hybrid_net_rl_bmi_data->motor_outputs->num_of_outputs; i++)
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

static bool connect_to_trial_hand(void )
{
	TrialHand2NeuralNetMsgItem *msg_item;
	char str_trial_hand_2_neural_net_msg[TRIAL_HAND_2_NEURAL_NET_MSG_STRING_LENGTH];

	while (1) 
	{ 
		while (get_next_trial_hand_2_neural_net_msg_buffer_item(hybrid_net_rl_bmi_data->msgs_trial_hand_2_neural_net, &msg_item))
		{
			get_trial_hand_2_neural_net_msg_type_string(msg_item->msg_type, str_trial_hand_2_neural_net_msg);
			print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", str_trial_hand_2_neural_net_msg);	
			switch (msg_item->msg_type)
			{
				case TRIAL_HAND_2_NEURAL_NET_MSG_ARE_YOU_ALIVE:
					hybrid_net_rl_bmi_data->msgs_neural_net_2_trial_hand = allocate_shm_client_neural_net_2_trial_hand_msg_buffer(hybrid_net_rl_bmi_data->msgs_neural_net_2_trial_hand);
					sleep(1);
					if (hybrid_net_rl_bmi_data->msgs_neural_net_2_trial_hand == NULL)
						return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", "msgs_neural_net_2_trial_hand == NULL.");	
					if (!write_to_neural_net_2_trial_hand_msg_buffer(hybrid_net_rl_bmi_data->msgs_neural_net_2_trial_hand, hybrid_net_rl_bmi_data->rt_tasks_data->current_system_time, NEURAL_NET_2_TRIAL_HAND_MSG_I_AM_ALIVE, 0))
						return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", "write_to_neural_net_2_trial_hand_msg_buffer().");	
					print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", "Connection to TRIAL_HANDLER is successful!!!");	
					return TRUE;		
				default:
					return print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", str_trial_hand_2_neural_net_msg);	
			}
		}
		print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", "Waiting for TRIAL_HANDLER to connect.");	
		sleep(1);
	}
	return print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", "Wrong hit in the code.");
}

static bool connect_to_mov_obj_hand(void )
{
	MovObjHand2NeuralNetMsgItem *msg_item;
	char str_mov_obj_hand_2_neural_net_msg[MOV_OBJ_HAND_2_NEURAL_NET_MSG_STRING_LENGTH];

	while (1) 
	{ 
		while (get_next_mov_obj_hand_2_neural_net_msg_buffer_item(hybrid_net_rl_bmi_data->msgs_mov_obj_hand_2_neural_net, &msg_item))
		{
			get_mov_obj_hand_2_neural_net_msg_type_string(msg_item->msg_type, str_mov_obj_hand_2_neural_net_msg);
			print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", str_mov_obj_hand_2_neural_net_msg);	
			switch (msg_item->msg_type)
			{
				case MOV_OBJ_HAND_2_NEURAL_NET_MSG_ARE_YOU_ALIVE:
					hybrid_net_rl_bmi_data->msgs_neural_net_2_mov_obj_hand = allocate_shm_client_neural_net_2_mov_obj_hand_msg_buffer(hybrid_net_rl_bmi_data->msgs_neural_net_2_mov_obj_hand);
					sleep(1);
					if (hybrid_net_rl_bmi_data->msgs_neural_net_2_mov_obj_hand == NULL)
						return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "msgs_neural_net_2_mov_obj_hand == NULL.");	
					if (!write_to_neural_net_2_mov_obj_hand_msg_buffer(hybrid_net_rl_bmi_data->msgs_neural_net_2_mov_obj_hand, hybrid_net_rl_bmi_data->rt_tasks_data->current_system_time, NEURAL_NET_2_MOV_OBJ_HAND_MSG_I_AM_ALIVE, MOV_OBJ_COMPONENT_NUM_NULL, MOV_OBJ_DIRECTION_NULL, MOV_OBJ_SPEED_NULL, MOV_OBJ_LOCATION_NULL))
						return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "write_to_neural_net_2_mov_obj_hand_msg_buffer().");	
					print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "Connection to MOV_OBJ_HANDLER is successful!!!");	
					return TRUE;		
				default:
					return print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", str_mov_obj_hand_2_neural_net_msg);	
			}
		}
		print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "Waiting for MOV_OBJ_HANDLER to connect.");	
		sleep(1);
	}
	return print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "Wrong hit in the code.");
}

static void *trial_hand_2_neural_net_msgs_handler(void *args)
{
	RT_TASK *handler;
        RTIME period;
	unsigned int prev_time, curr_time;
	char str_trial_hand_msg[TRIAL_HAND_2_NEURAL_NET_MSG_STRING_LENGTH];

	TrialHand2NeuralNetMsg *msgs_trial_hand_2_neural_net = NULL;
	TrialHand2NeuralNetMsgItem *msg_item = NULL;
	TrialStatusEvents* trial_status_events = NULL;
	RtTasksData *rt_tasks_data = NULL;

	msgs_trial_hand_2_neural_net = hybrid_net_rl_bmi_data->msgs_trial_hand_2_neural_net;
	trial_status_events = hybrid_net_rl_bmi_data->trial_status_events;
	rt_tasks_data = hybrid_net_rl_bmi_data->rt_tasks_data;

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

        while (1) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(rt_tasks_data, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, prev_time, curr_time);
		prev_time = curr_time;
		// routines
		while (get_next_trial_hand_2_neural_net_msg_buffer_item(msgs_trial_hand_2_neural_net, &msg_item))
		{
			switch (msg_item->msg_type)
			{
				case TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_STATUS_CHANGED:
					schedule_trial_status_event(trial_status_events, rt_tasks_data->current_system_time, msg_item->additional_data_0, msg_item->additional_data_1) ; 
					break;	
				case TRIAL_HAND_2_NEURAL_NET_MSG_REWARD_GIVEN:   // not implemeted yet, for RL it will be required.
					get_trial_hand_2_neural_net_msg_type_string(msg_item->msg_type, str_trial_hand_msg);  
					print_message(WARNING_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", str_trial_hand_msg);
					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_PUNISHMENT_GIVEN:  // not implemeted yet, for RL it will be required.
					get_trial_hand_2_neural_net_msg_type_string(msg_item->msg_type, str_trial_hand_msg);  
					print_message(WARNING_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", str_trial_hand_msg);
					break;
				default: 
					get_trial_hand_2_neural_net_msg_type_string(msg_item->msg_type, str_trial_hand_msg);  
					print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", str_trial_hand_msg);
					rt_make_soft_real_time();
        				rt_task_delete(handler);
					print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "rt_task_delete().");	
				        return 0;
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
