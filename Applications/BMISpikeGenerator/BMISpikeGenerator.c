#include "BMISpikeGenerator.h"


SpikeGenData *bmi_simulation_spike_gen_data = NULL;

static bool connect_to_trial_hand(void);

static int trial_hand_2_spike_gen_msgs_handler_rt_thread = 0;
static void *trial_hand_2_spike_gen_msgs_handler(void *args); 

int main( int argc, char *argv[])
{

	bmi_simulation_spike_gen_data = g_new0(SpikeGenData, 1);

	bmi_simulation_spike_gen_data->sorted_spike_time_stamp = rtai_malloc(nam2num(KERNEL_SPIKE_SPIKE_TIME_STAMP_SHM_NAME), 0);
	if (bmi_simulation_spike_gen_data->sorted_spike_time_stamp == NULL) {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "main", "bmi_simulation_spike_gen_data->blue_spike_data == NULL."); return -1; }

	bmi_simulation_spike_gen_data->rt_tasks_data = rtai_malloc(nam2num(RT_TASKS_DATA_SHM_NAME), 0);
	if (bmi_simulation_spike_gen_data->rt_tasks_data == NULL) {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "main", "bmi_simulation_spike_gen_data->rt_tasks_data == NULL."); return -1; }

	bmi_simulation_spike_gen_data->trial_types_data = allocate_trial_types_data(bmi_simulation_spike_gen_data->trial_types_data);
	if (! add_trial_type_to_trial_types_data(bmi_simulation_spike_gen_data->trial_types_data, TRIAL_TYPE_IN_VIVO_BMI_LEFT_TARGET, 5000000000, 4000000000)) 
		return print_message(ERROR_MSG ,"BMIExpController", "TrialHandler", "main", "! add_trial_type_to_trial_types_data().");
	if (! add_trial_type_to_trial_types_data(bmi_simulation_spike_gen_data->trial_types_data, TRIAL_TYPE_IN_VIVO_BMI_RIGHT_TARGET, 5000000000, 4000000000)) 
		return print_message(ERROR_MSG ,"BMIExpController", "TrialHandler", "main", "! add_trial_type_to_trial_types_data().");
	bmi_simulation_spike_gen_data->network = allocate_network(bmi_simulation_spike_gen_data->network);
	bmi_simulation_spike_gen_data->msgs_trial_hand_2_spike_gen = allocate_shm_server_trial_hand_2_spike_gen_msg_buffer(bmi_simulation_spike_gen_data->msgs_trial_hand_2_spike_gen);
	bmi_simulation_spike_gen_data->trial_status_events = allocate_trial_status_events_buffer(bmi_simulation_spike_gen_data->trial_status_events, 100, 3000000);  //  3 ms latency
/*
	if (! connect_to_trial_hand()) {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "main", "connect_to_trial_hand()."); return -1; }
*/
	trial_hand_2_spike_gen_msgs_handler_rt_thread =  rt_thread_create(trial_hand_2_spike_gen_msgs_handler, NULL, 10000);

//	initialize_spike_gen_data_read_write_handlers();
	gtk_init(&argc, &argv);
	create_gui();
	gtk_main();
	return 0;
}


SpikeGenData* get_bmi_simulation_spike_generator_data(void)
{
	return bmi_simulation_spike_gen_data;
}

static bool connect_to_trial_hand(void )
{
	TrialHand2SpikeGenMsgItem msg_item;
	char str_trial_hand_2_spike_gen_msg[TRIAL_HAND_2_SPIKE_GEN_MSG_STRING_LENGTH];

	while (1) 
	{ 
		while (get_next_trial_hand_2_spike_gen_msg_buffer_item(bmi_simulation_spike_gen_data->msgs_trial_hand_2_spike_gen, &msg_item))
		{
			get_trial_hand_2_spike_gen_msg_type_string(msg_item.msg_type, str_trial_hand_2_spike_gen_msg);
			print_message(INFO_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "connect_to_trial_hand", str_trial_hand_2_spike_gen_msg);	
			switch (msg_item.msg_type)
			{
				case TRIAL_HAND_2_SPIKE_GEN_MSG_ARE_YOU_ALIVE:
					bmi_simulation_spike_gen_data->msgs_spike_gen_2_trial_hand = allocate_shm_client_spike_gen_2_trial_hand_msg_buffer(bmi_simulation_spike_gen_data->msgs_spike_gen_2_trial_hand);
					sleep(1);
					if (bmi_simulation_spike_gen_data->msgs_spike_gen_2_trial_hand == NULL)
						return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "connect_to_trial_hand", "msgs_spike_gen_2_trial_hand == NULL.");	
					if (!write_to_spike_gen_2_trial_hand_msg_buffer(bmi_simulation_spike_gen_data->msgs_spike_gen_2_trial_hand, bmi_simulation_spike_gen_data->rt_tasks_data->current_system_time, SPIKE_GEN_2_TRIAL_HAND_MSG_I_AM_ALIVE, 0))
						return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "connect_to_trial_hand", "write_to_spike_gen_2_trial_hand_msg_buffer().");	
					print_message(INFO_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "connect_to_trial_hand", "Connection to TRIAL_HANDLER is successful!!!");	
					return TRUE;		
				default:
					return print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "connect_to_trial_hand", str_trial_hand_2_spike_gen_msg);	
			}
		}
		print_message(INFO_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "connect_to_trial_hand", "Waiting for TRIAL_HANDLER to connect.");	
		sleep(1);
	}
	return print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "connect_to_trial_hand", "Wrong hit in the code.");
}

static void *trial_hand_2_spike_gen_msgs_handler(void *args)
{
	RT_TASK *handler;
        RTIME period;
	unsigned int prev_time, curr_time;
	char str_trial_hand_msg[TRIAL_HAND_2_SPIKE_GEN_MSG_STRING_LENGTH];

	TrialHand2SpikeGenMsg *msgs_trial_hand_2_spike_gen = NULL;
	TrialHand2SpikeGenMsgItem msg_item;
	TrialStatusEvents* trial_status_events = NULL;
	RtTasksData *rt_tasks_data = NULL;

	msgs_trial_hand_2_spike_gen = bmi_simulation_spike_gen_data->msgs_trial_hand_2_spike_gen;
	trial_status_events = bmi_simulation_spike_gen_data->trial_status_events;
	rt_tasks_data = bmi_simulation_spike_gen_data->rt_tasks_data;

	if (! check_rt_task_specs_to_init(rt_tasks_data, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_TASK_ID, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_PERIOD))  {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "trial_hand_2_spike_gen_msgs_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_TASK_NAME, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_TASK_PRIORITY, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_STACK_SIZE, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_MSG_SIZE, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_POLICY, 1 << ((TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "trial_hand_2_spike_gen_msgs_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_TASK_ID, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_PERIOD, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_POSITIVE_JITTER_THRES, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_NEGATIVE_JITTER_THRES, "TrialHand2SpikeGenMssHand"))  {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "trial_hand_2_spike_gen_msgs_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	
        period = nano2count(TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_PERIOD);

        rt_task_make_periodic(handler, rt_get_time() + period, period);
	prev_time = rt_get_cpu_time_ns();	
        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

	msgs_trial_hand_2_spike_gen->buff_read_idx = msgs_trial_hand_2_spike_gen->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,

        while (1) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(rt_tasks_data, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_TASK_ID, prev_time, curr_time);
		prev_time = curr_time;
		// routines
		while (get_next_trial_hand_2_spike_gen_msg_buffer_item(msgs_trial_hand_2_spike_gen, &msg_item))
		{
			switch (msg_item.msg_type)
			{
				case TRIAL_HAND_2_SPIKE_GEN_MSG_TRIAL_STATUS_CHANGED:
					schedule_trial_status_event(trial_status_events, rt_tasks_data->current_system_time, msg_item.additional_data_0, msg_item.additional_data_1) ; 
					break;	
				default: 
					get_trial_hand_2_spike_gen_msg_type_string(msg_item.msg_type, str_trial_hand_msg);  
					print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "trial_hand_2_spike_gen_msgs_handler", str_trial_hand_msg);
					rt_make_soft_real_time();
        				rt_task_delete(handler);
					print_message(INFO_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "trial_hand_2_spike_gen_msgs_handler", "rt_task_delete().");	
				        return 0;
			}
		}
		// routines	
		evaluate_and_save_period_run_time(rt_tasks_data, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_TASK_ID, curr_time, rt_get_cpu_time_ns());		
        }
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_spike_gen_msgs_handler", "rt_task_delete().");	

        return 0;
}
