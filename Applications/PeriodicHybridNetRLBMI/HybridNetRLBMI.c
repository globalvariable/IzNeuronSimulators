#include "HybridNetRLBMI.h"

static HybridNetRLBMIData *hybrid_net_rl_bmi_data = NULL;

static bool connect_to_trial_hand(void);
static bool connect_to_mov_obj_hand(void);

int main( int argc, char *argv[])
{
	cpu_set_t  mask;
	CPU_ZERO(&mask);
	CPU_SET(IZ_PS_NETWORK_SIM_USER_SPACE_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU+IZ_PS_NETWORK_SIM_USER_SPACE_CPU_THREAD_ID, &mask);
	printf ("sched_getcpu() = %d (Before sched_setaffinity)\n", sched_getcpu());
	if (sched_setaffinity(0, sizeof(mask), &mask))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main","! sched_setaffinity(0, sizeof(mask), &mask).");		
	printf ("sched_getcpu() = %d (After sched_setaffinity)\n", sched_getcpu());

	hybrid_net_rl_bmi_data = g_new0(HybridNetRLBMIData, 1);

	hybrid_net_rl_bmi_data->sorted_spike_time_stamp = rtai_malloc(SHM_NUM_KERNEL_SPIKE_SPIKE_TIME_STAMP, 0);
	if (hybrid_net_rl_bmi_data->sorted_spike_time_stamp == NULL) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "hybrid_net_rl_bmi_data->sorted_spike_time_stamp == NULL."); return -1; }
	hybrid_net_rl_bmi_data->rt_tasks_data = rtai_malloc(SHM_NUM_RT_TASKS_DATA, 0);
	if (hybrid_net_rl_bmi_data->rt_tasks_data == NULL) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "hybrid_net_rl_bmi_data->rt_tasks_data == NULL."); return -1; }
	hybrid_net_rl_bmi_data->blue_spike_network = allocate_network(hybrid_net_rl_bmi_data->blue_spike_network);
	hybrid_net_rl_bmi_data->in_silico_network = allocate_network(hybrid_net_rl_bmi_data->in_silico_network);
	hybrid_net_rl_bmi_data->msgs_trial_hand_2_neural_net = allocate_shm_server_trial_hand_2_neural_net_msg_buffer(hybrid_net_rl_bmi_data->msgs_trial_hand_2_neural_net);
	hybrid_net_rl_bmi_data->msgs_neural_net_2_mov_obj_hand_multi_thread = g_new0(NeuralNet2MovObjHandMsgMultiThread, 1); 
	hybrid_net_rl_bmi_data->msgs_mov_obj_hand_2_neural_net_multi_thread = allocate_shm_server_mov_obj_hand_2_neural_net_multi_thread_msg_buffer(hybrid_net_rl_bmi_data->msgs_mov_obj_hand_2_neural_net_multi_thread); 
	hybrid_net_rl_bmi_data->msgs_neural_net_2_gui = allocate_neural_net_2_gui_msg_buffer(hybrid_net_rl_bmi_data->msgs_neural_net_2_gui);
	hybrid_net_rl_bmi_data->trial_status_events = allocate_trial_status_events_buffer(hybrid_net_rl_bmi_data->trial_status_events, 100, 3000000);  //  3 ms latency
	hybrid_net_rl_bmi_data->num_of_dedicated_cpu_threads = IZ_PS_NETWORK_SIM_NUM_OF_DEDICATED_CPUS * MAX_NUM_OF_CPU_THREADS_PER_CPU;
	hybrid_net_rl_bmi_data->in_silico_spike_data_for_graph = g_new0(SpikeData*, hybrid_net_rl_bmi_data->num_of_dedicated_cpu_threads);	// allocate the array of buffers here. 
	hybrid_net_rl_bmi_data->in_silico_spike_data_for_recording = g_new0(SpikeData*, hybrid_net_rl_bmi_data->num_of_dedicated_cpu_threads); // allocate the array of buffers here.  a recording message which is received before "ready_for_simulation" button can lead to segfault. since read_idx and write_idx are zero here, they will not try to access the buffers (refer to get_next_spike_data_item())

	hybrid_net_rl_bmi_data->reward_data.num_of_targets = 2;
	hybrid_net_rl_bmi_data->reward_data.R = g_new0(double, hybrid_net_rl_bmi_data->reward_data.num_of_targets);

	if (! connect_to_mov_obj_hand()) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "connect_to_mov_obj_hand()."); return -1; }

	if (! connect_to_trial_hand()) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "connect_to_trial_hand()."); return -1; }

	if (! add_neurons_for_external_and_in_silico_network(hybrid_net_rl_bmi_data)) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "! prepare_external_and_in_silico_network()."); return -1; }	

	if (! submit_parker_sochacki_integration_precision(hybrid_net_rl_bmi_data)) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "! submit_parker_sochacki_integration_precision()."); return -1; }

	if (! set_output_layers(hybrid_net_rl_bmi_data)) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "! set_output_layers()."); return -1; }

	if (! connect_external_to_in_silico_network(hybrid_net_rl_bmi_data)) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "! connect_external_to_in_silico_network()."); return -1; }

	if (! connect_medium_spiny_neurons(hybrid_net_rl_bmi_data)) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "! connect_medium_spiny_neurons()."); return -1; }

	if (! connect_babling_2_medium_spiny_neurons(hybrid_net_rl_bmi_data)) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "! connect_babling_2_medium_spiny_neurons()."); return -1; }

	initialize_data_read_write_handlers();

	print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "Network setup is successful."); 

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
	TrialHand2NeuralNetMsgItem msg_item;
	char str_trial_hand_2_neural_net_msg[TRIAL_HAND_2_NEURAL_NET_MSG_STRING_LENGTH];

	while (1) 
	{ 
		while (get_next_trial_hand_2_neural_net_msg_buffer_item(hybrid_net_rl_bmi_data->msgs_trial_hand_2_neural_net, &msg_item))
		{
			get_trial_hand_2_neural_net_msg_type_string(msg_item.msg_type, str_trial_hand_2_neural_net_msg);
			print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", str_trial_hand_2_neural_net_msg);	
			switch (msg_item.msg_type)
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
	MovObjHand2NeuralNetMsgItem msg_item;
	char str_mov_obj_hand_2_neural_net_msg[MOV_OBJ_HAND_2_NEURAL_NET_MSG_STRING_LENGTH];
	unsigned int i, num_of_alive_responses = 0;
	while (1) 
	{ 
		for (i = 0; i < (NUM_OF_NEURAL_NET_2_MOV_OBJ_HAND_MSG_BUFFERS); i++)
		{
			while (get_next_mov_obj_hand_2_neural_net_msg_buffer_item((*(hybrid_net_rl_bmi_data->msgs_mov_obj_hand_2_neural_net_multi_thread))[i], &msg_item))
			{
				get_mov_obj_hand_2_neural_net_msg_type_string(msg_item.msg_type, str_mov_obj_hand_2_neural_net_msg);
				print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", str_mov_obj_hand_2_neural_net_msg);	
				switch (msg_item.msg_type)
				{
					case MOV_OBJ_HAND_2_NEURAL_NET_MSG_ARE_YOU_ALIVE:
						(*(hybrid_net_rl_bmi_data->msgs_neural_net_2_mov_obj_hand_multi_thread))[i] = allocate_shm_client_neural_net_2_mov_obj_hand_multi_thread_msg_buffer_item(hybrid_net_rl_bmi_data->msgs_neural_net_2_mov_obj_hand_multi_thread, i, MIN_NEURAL_NET_2_MOV_OBJ_HAND_EVENT_SCHEDULING_DELAY);
						usleep(1000);
						if (((*(hybrid_net_rl_bmi_data->msgs_neural_net_2_mov_obj_hand_multi_thread))[i]) == NULL)
							return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "msgs_neural_net_2_mov_obj_hand == NULL.");	
						if (!write_to_neural_net_2_mov_obj_hand_msg_buffer((*(hybrid_net_rl_bmi_data->msgs_neural_net_2_mov_obj_hand_multi_thread))[i], hybrid_net_rl_bmi_data->rt_tasks_data->current_system_time, NEURAL_NET_2_MOV_OBJ_HAND_MSG_I_AM_ALIVE, 0, 0, 0, 0))
							return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "write_to_neural_net_2_mov_obj_hand_msg_buffer().");	
						num_of_alive_responses++;
						if (num_of_alive_responses == (NUM_OF_NEURAL_NET_2_MOV_OBJ_HAND_MSG_BUFFERS))
						{
							goto EXIT;	// changed it from GET_JOINT_ANGLE_MINS to EXIT according to new design. no robot angle is required only binary reward signal is enough.
						}
						break;		
					default:
						return print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", str_mov_obj_hand_2_neural_net_msg);	
				}
			}
		}
		print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "Waiting for MOV_OBJ_HANDLER to connect.");	
		sleep(1);
	}

/*	GET_JOINT_ANGLE_MINS:
	while (1) 
	{ 
		for (i = 0; i < (NUM_OF_NEURAL_NET_2_MOV_OBJ_HAND_MSG_BUFFERS); i++)
		{
			while (get_next_mov_obj_hand_2_neural_net_msg_buffer_item((*(hybrid_net_rl_bmi_data->msgs_mov_obj_hand_2_neural_net_multi_thread))[i], &msg_item))
			{
				get_mov_obj_hand_2_neural_net_msg_type_string(msg_item.msg_type, str_mov_obj_hand_2_neural_net_msg);
				print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", str_mov_obj_hand_2_neural_net_msg);	
				switch (msg_item.msg_type)
				{
					case MOV_OBJ_HAND_2_NEURAL_NET_MSG_JOINT_ANGLE_MIN:
						hybrid_net_rl_bmi_data->servo_angle_min_max[BASE_SERVO].min = msg_item.additional_data.three_dof_robot_min_joint_angles[BASE_SERVO];
						hybrid_net_rl_bmi_data->servo_angle_min_max[SHOULDER_SERVO].min = msg_item.additional_data.three_dof_robot_min_joint_angles[SHOULDER_SERVO];
						hybrid_net_rl_bmi_data->servo_angle_min_max[ELBOW_SERVO].min = msg_item.additional_data.three_dof_robot_min_joint_angles[ELBOW_SERVO];
						printf ("angle mins %f\n", hybrid_net_rl_bmi_data->servo_angle_min_max[BASE_SERVO].min);
						printf ("angle mins %f\n", hybrid_net_rl_bmi_data->servo_angle_min_max[SHOULDER_SERVO].min);
						printf ("angle mins %f\n", hybrid_net_rl_bmi_data->servo_angle_min_max[ELBOW_SERVO].min);
						goto GET_JOINT_ANGLE_MAXS;
					default:
						return print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", str_mov_obj_hand_2_neural_net_msg);	
				}
			}
		}
		print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "Waiting for MOV_OBJ_HANDLER to joint angle minimums");	
		sleep(1);
	}
	GET_JOINT_ANGLE_MAXS:
	while (1) 
	{ 
		for (i = 0; i < (NUM_OF_NEURAL_NET_2_MOV_OBJ_HAND_MSG_BUFFERS); i++)
		{
			while (get_next_mov_obj_hand_2_neural_net_msg_buffer_item((*(hybrid_net_rl_bmi_data->msgs_mov_obj_hand_2_neural_net_multi_thread))[i], &msg_item))
			{
				get_mov_obj_hand_2_neural_net_msg_type_string(msg_item.msg_type, str_mov_obj_hand_2_neural_net_msg);
				print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", str_mov_obj_hand_2_neural_net_msg);	
				switch (msg_item.msg_type)
				{
					case MOV_OBJ_HAND_2_NEURAL_NET_MSG_JOINT_ANGLE_MAX:
						hybrid_net_rl_bmi_data->servo_angle_min_max[BASE_SERVO].max = msg_item.additional_data.three_dof_robot_max_joint_angles[BASE_SERVO];
						hybrid_net_rl_bmi_data->servo_angle_min_max[SHOULDER_SERVO].max = msg_item.additional_data.three_dof_robot_max_joint_angles[SHOULDER_SERVO];
						hybrid_net_rl_bmi_data->servo_angle_min_max[ELBOW_SERVO].max = msg_item.additional_data.three_dof_robot_max_joint_angles[ELBOW_SERVO];
						printf ("angle maxs %f\n", hybrid_net_rl_bmi_data->servo_angle_min_max[BASE_SERVO].max);
						printf ("angle maxs %f\n", hybrid_net_rl_bmi_data->servo_angle_min_max[SHOULDER_SERVO].max);
						printf ("angle maxs %f\n", hybrid_net_rl_bmi_data->servo_angle_min_max[ELBOW_SERVO].max);
						goto EXIT;
					default:
						return print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", str_mov_obj_hand_2_neural_net_msg);	
				}
			}
		}
		print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "Waiting for MOV_OBJ_HANDLER to joint angle maximums");	
		sleep(1);
	}
*/
	EXIT:
	return print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "Connection to MOV_OBJ_HANDLER is successful!!!");
}


