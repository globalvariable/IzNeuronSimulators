#include "HybridNetRLBMI.h"

static HybridNetRLBMIData *hybrid_net_rl_bmi_data = NULL;

static bool connect_to_trial_hand(void);
static bool connect_to_mov_obj_hand(void);
static bool prepare_external_and_in_silico_network(HybridNetRLBMIData *bmi_data);

int main( int argc, char *argv[])
{
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

	hybrid_net_rl_bmi_data->angle_sensitive_spindles[BASE_SERVO] = allocate_exponent_angular_spindle_group(NUM_OF_ANGULAR_SPINDLES);
	hybrid_net_rl_bmi_data->angle_sensitive_spindles[SHOULDER_SERVO] = allocate_exponent_angular_spindle_group(NUM_OF_ANGULAR_SPINDLES);
	hybrid_net_rl_bmi_data->angle_sensitive_spindles[ELBOW_SERVO] = allocate_exponent_angular_spindle_group(NUM_OF_ANGULAR_SPINDLES);

	if (! connect_to_mov_obj_hand()) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "connect_to_mov_obj_hand()."); return -1; }

	if (! connect_to_trial_hand()) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "connect_to_trial_hand()."); return -1; }

	if (! prepare_external_and_in_silico_network(hybrid_net_rl_bmi_data)) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "! prepare_external_and_in_silico_network()."); return -1; }	

	initialize_data_read_write_handlers();

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
							goto GET_JOINT_ANGLE_MINS;
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

	GET_JOINT_ANGLE_MINS:
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

	EXIT:
	return print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "Connection to MOV_OBJ_HANDLER is successful!!!");
}

static bool prepare_external_and_in_silico_network(HybridNetRLBMIData *bmi_data)
{
	double v;
	double a;
	double b;
	double c;
	double d;
	double k;
	double C;
	double v_resting;
	double v_threshold;
	double v_peak;
//	double I_inject;
	bool inhibitory;
	double E_excitatory;
	double E_inhibitory;
	double tau_excitatory;
	double tau_inhibitory;
	unsigned int randomize_params = 0;
	unsigned int i, j;
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j = 0; j < MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			if (!add_neuron_nodes_to_layer(bmi_data->blue_spike_network, MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE ,i, FALSE)) {
				return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "main", "add_neuron_nodes_to_layer()."); return -1; }				
		}
	}

	// BASE SERVO
	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_BASE_SERVO_EXTENSOR_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_BASE_SERVO_EXTENSOR_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, NUM_OF_EXTENSOR_IA_SPINDLES, LAYER_BASE_SERVO_EXTENSOR_SPINDLE_IA, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_BASE_SERVO_FLEXOR_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_BASE_SERVO_FLEXOR_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, NUM_OF_FLEXOR_IA_SPINDLES, LAYER_BASE_SERVO_FLEXOR_SPINDLE_IA, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	C = 50.0; 
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, NUM_OF_ANGULAR_SPINDLES, LAYER_BASE_SERVO_ANGLE_SENS_SPINDLE, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	// SHOULDER SERVO
	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_SHOULDER_SERVO_EXTENSOR_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_SHOULDER_SERVO_EXTENSOR_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, NUM_OF_EXTENSOR_IA_SPINDLES, LAYER_SHOULDER_SERVO_EXTENSOR_SPINDLE_IA, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_SHOULDER_SERVO_FLEXOR_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_SHOULDER_SERVO_FLEXOR_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, NUM_OF_FLEXOR_IA_SPINDLES, LAYER_SHOULDER_SERVO_FLEXOR_SPINDLE_IA, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	C = 50.0; 
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, NUM_OF_ANGULAR_SPINDLES, LAYER_SHOULDER_SERVO_ANGLE_SENS_SPINDLE, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	// ELBOW SERVO
	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_ELBOW_SERVO_EXTENSOR_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_ELBOW_SERVO_EXTENSOR_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, NUM_OF_EXTENSOR_IA_SPINDLES, LAYER_ELBOW_SERVO_EXTENSOR_SPINDLE_IA, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_ELBOW_SERVO_FLEXOR_MOTOR, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, 5, LAYER_ELBOW_SERVO_FLEXOR_INTER, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_LOW_THRESHOLD, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, NUM_OF_FLEXOR_IA_SPINDLES, LAYER_ELBOW_SERVO_FLEXOR_SPINDLE_IA, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_REGULAR_SPIKING, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	C = 50.0; 
	if (!add_iz_neurons_to_layer(bmi_data->in_silico_network, NUM_OF_ANGULAR_SPINDLES, LAYER_ELBOW_SERVO_ANGLE_SENS_SPINDLE, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "NetworkView", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	return TRUE;
}
