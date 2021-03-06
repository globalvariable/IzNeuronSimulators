#include "BMISpikeGeneratorRtTask.h"

static int bmi_simulation_spike_generator_rt_thread = 0;
static bool bmi_simulation_spike_generator_rt_task_stay_alive = 1;

static void *bmi_simulation_spike_generator_rt_handler(void *args); 
static bool bmi_simulation_spike_generator_integration_handler(TimeStamp integration_start_time, TimeStamp integration_end_time, unsigned int num_of_neurons);

static bool write_generated_spike_to_blue_spike_buffer(SortedSpikes *sorted_spike_time_stamp, unsigned int layer_num, unsigned int neuron_grp_num, unsigned int neuron_num, TimeStamp spike_time);

static SpikeData **generated_spike_data;
static TrialHandParadigmRobotReach *paradigm = NULL;
static TrialStatusEvents *trial_status_events = NULL;
static Network *network = NULL;
static SortedSpikes *spike_time_stamp = NULL;
static CurrentTemplate *current_templates = NULL;
static CurrentPatternBufferLimited *limited_current_pattern_buffer = NULL;
static NeuronDynamicsBufferLimited *limited_neuron_dynamics_buffer = NULL;
static char *message = NULL;

void bmi_simulation_spike_generator_create_rt_thread(void)
{
	static bool first = TRUE;
	if (first)
	{
		bmi_simulation_spike_generator_rt_thread = rt_thread_create( bmi_simulation_spike_generator_rt_handler, NULL, 10000);
		first = FALSE;
	}
	else
	{
		print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_create_rt_thread", "CANNOT create rt_thread again. Should restart the program. ");	//  use of static variables in bmi_simulation_spike_generator_integration_handler are not designed for re-use. Should clear all read or write indexes (current, templates) for restart of rt_thread. 
	}
}

static void *bmi_simulation_spike_generator_rt_handler(void *args)		
{
	RT_TASK *handler;
        RTIME period;
	unsigned int prev_time, curr_time;
	unsigned int num_of_neurons;
	TrialHand2NeuralNetTrialStatusMsgAdd trial_status_change_msg_add;

	TimeStamp integration_start_time, integration_end_time;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_data();
	paradigm = spike_gen_data->trial_hand_paradigm;
	RtTasksData* rt_tasks_data = spike_gen_data->rt_tasks_data;
	network = spike_gen_data->network;
	current_templates =  spike_gen_data->current_templates;
	limited_current_pattern_buffer =  spike_gen_data->limited_current_pattern_buffer;
	limited_neuron_dynamics_buffer = spike_gen_data->limited_neuron_dynamics_buffer;
	generated_spike_data = spike_gen_data->spike_data;
	spike_time_stamp = spike_gen_data->sorted_spike_time_stamp;
	trial_status_events = spike_gen_data->trial_status_events;
	trial_status_change_msg_add.new_trial_status = TRIAL_STATUS_TRIALS_DISABLED;
	trial_status_change_msg_add.new_robot_target_position_idx = 0;
	schedule_trial_status_event(trial_status_events, rt_tasks_data->current_periodic_system_time, trial_status_change_msg_add) ; 


	g_free(message);
	message = g_new0(char, 200);
	if (! check_rt_task_specs_to_init(rt_tasks_data, SPIKE_GENERATOR_CPU_ID, SPIKE_GENERATOR_CPU_THREAD_ID, SPIKE_GENERATOR_CPU_THREAD_TASK_ID, SPIKE_GENERATOR_PERIOD, FALSE))  {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(SPIKE_GENERATOR_TASK_NAME, SPIKE_GENERATOR_TASK_PRIORITY, SPIKE_GENERATOR_STACK_SIZE, SPIKE_GENERATOR_MSG_SIZE,SPIKE_GENERATOR_POLICY, 1 << ((SPIKE_GENERATOR_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+SPIKE_GENERATOR_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, SPIKE_GENERATOR_CPU_ID, SPIKE_GENERATOR_CPU_THREAD_ID, SPIKE_GENERATOR_CPU_THREAD_TASK_ID, SPIKE_GENERATOR_PERIOD, SPIKE_GENERATOR_POSITIVE_JITTER_THRES, SPIKE_GENERATOR_NEGATIVE_JITTER_THRES, "SpikeGenerator", FALSE))  {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	

        period = nano2count(SPIKE_GENERATOR_PERIOD);
        rt_task_make_periodic(handler, rt_get_time() + period, period);
	prev_time = rt_get_cpu_time_ns();	
	integration_start_time = ((rt_tasks_data->current_periodic_system_time)/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE) *PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
	reset_all_network_iz_neuron_dynamics (network);
	num_of_neurons = get_num_of_neurons_in_network(network);
        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 
        while (bmi_simulation_spike_generator_rt_task_stay_alive) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(rt_tasks_data, SPIKE_GENERATOR_CPU_ID, SPIKE_GENERATOR_CPU_THREAD_ID, SPIKE_GENERATOR_CPU_THREAD_TASK_ID, prev_time, curr_time);
		prev_time = curr_time;
		// routines
		integration_end_time =  ((rt_tasks_data->current_periodic_system_time)/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE) *PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
		if(! bmi_simulation_spike_generator_integration_handler(integration_start_time, integration_end_time, num_of_neurons)) {
			print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "! bmi_simulation_spike_generator_integration_handler()."); exit(1); }
		integration_start_time = integration_end_time;
		// routines	
		evaluate_and_save_period_run_time(rt_tasks_data, SPIKE_GENERATOR_CPU_ID, SPIKE_GENERATOR_CPU_THREAD_ID, SPIKE_GENERATOR_CPU_THREAD_TASK_ID, curr_time, rt_get_cpu_time_ns());		
        }
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "rt_task_delete().");		
        return 0;
}

static bool bmi_simulation_spike_generator_integration_handler(TimeStamp integration_start_time, TimeStamp integration_end_time, unsigned int num_of_neurons)
{
	static unsigned int trial_status_events_write_idx_prev = 0;	// bmi_simulation_spike_generator_rt_handler writes trials disabled at the beginning. 
	static TrialStatusEventItem last_trial_status_event;
	static CurrentPatternTemplate* current_template_in_use;
	static unsigned int current_template_read_idx;
	unsigned int current_template_num_in_use;
	unsigned int i;
	Neuron		**all_neurons = network->all_neurons;
	Neuron *neuron;
	TimeStamp time_ns;
	TimeStamp spike_time;
	bool spike_generated;

	if (trial_status_events->buff_write_idx != trial_status_events_write_idx_prev) // there is a change in trial_status  // do not take care of trial status change scheduled time for now. 
	{
		trial_status_events_write_idx_prev = trial_status_events->buff_write_idx;
		last_trial_status_event = get_last_trial_status_events_buffer_item(trial_status_events);
		if (!get_trial_status_type_string(last_trial_status_event.trial_status, message))  
			return print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_integration_handler", "! get_trial_status_type_string()");	
		print_message(INFO_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_integration_handler", message);								
		switch (last_trial_status_event.trial_status)
		{
			case TRIAL_STATUS_TRIALS_DISABLED:	// ignore handling
				break;
			case TRIAL_STATUS_IN_TRIAL:	
				switch (last_trial_status_event.selected_robot_target_idx_in_trial_hand_paradigm)
				{
					case LEFT_ROBOT_TARGET:   // left target
						determine_in_trial_current_number_randomly(current_templates, &current_template_num_in_use);
						get_in_trial_current_pattern_template(current_templates, &current_template_in_use, LEFT_ROBOT_TARGET , current_template_num_in_use);
						current_template_read_idx = 0;
						current_template_in_use->template_start_time = integration_start_time;   // it is useless for in trial currents but used for out of trial currents to determine current injection duration.
						reset_prev_noise_addition_times_for_current_template(network, current_template_in_use);
						for (time_ns = integration_start_time; time_ns < integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)   // integrate remaining part in the next task period
						{
							load_current_template_sample_to_neurons_with_noise(network, current_template_in_use, current_template_read_idx, time_ns);
							current_template_read_idx++;
							if (current_template_read_idx == current_template_in_use->num_of_template_samples)
								current_template_read_idx--; 	// read last idx again and again until trial_status_changes. it might appear since trialcontroller period is larger than this task	
							for (i = 0; i < num_of_neurons; i++)
							{	
								neuron = all_neurons[i];
								if (!evaluate_neuron_dyn(neuron, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, &spike_generated, &spike_time))
									return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_integration_handler", "! evaluate_neuron_dyn().");
								if (spike_generated)
								{
									write_generated_spike_to_blue_spike_buffer(spike_time_stamp, neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
									write_to_spike_data(generated_spike_data[0], neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
								}
								push_neuron_dynamics_to_neuron_dynamics_buffer_limited(network, limited_neuron_dynamics_buffer, time_ns, i);	
							}
							push_neuron_currents_to_neuron_current_buffer_limited(network, limited_current_pattern_buffer, time_ns);

						}
						break;									
					case RIGHT_ROBOT_TARGET:	 // right target
						determine_in_trial_current_number_randomly(current_templates, &current_template_num_in_use);
						get_in_trial_current_pattern_template(current_templates, &current_template_in_use, RIGHT_ROBOT_TARGET , current_template_num_in_use);
						current_template_read_idx = 0;
						current_template_in_use->template_start_time = integration_start_time;   // it is useless for in trial currents but used for out of trial currents to determine current injection duration.
						reset_prev_noise_addition_times_for_current_template(network, current_template_in_use);
						for (time_ns = integration_start_time; time_ns < integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)   // integrate remaining part in the next task period
						{
							load_current_template_sample_to_neurons_with_noise(network, current_template_in_use, current_template_read_idx, time_ns);
							current_template_read_idx++;
							if (current_template_read_idx == current_template_in_use->num_of_template_samples)
								current_template_read_idx--; 	// read last idx again and again until trial_status_changes. it might appear since trialcontroller period is larger than this task	
							for (i = 0; i < num_of_neurons; i++)
							{	
								neuron = all_neurons[i];
								if (!evaluate_neuron_dyn(neuron, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, &spike_generated, &spike_time))
									return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_integration_handler", "! evaluate_neuron_dyn().");
								if (spike_generated)
								{
									write_generated_spike_to_blue_spike_buffer(spike_time_stamp, neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
									write_to_spike_data(generated_spike_data[0], neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
								}
								push_neuron_dynamics_to_neuron_dynamics_buffer_limited(network, limited_neuron_dynamics_buffer, time_ns, i);	
							}
							push_neuron_currents_to_neuron_current_buffer_limited(network, limited_current_pattern_buffer, time_ns);
						}
						break;	
					default:
						return print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_integration_handler", "switch 			(last_trial_status_event.selected_robot_target_idx_in_trial_hand_paradigm)");	
				}				
				break;	
			case TRIAL_STATUS_IN_REFRACTORY:	
				determine_in_refractory_current_number_randomly(current_templates, &current_template_num_in_use);
				get_in_refractory_current_pattern_template(current_templates, &current_template_in_use, current_template_num_in_use);
				current_template_read_idx = 0;
				current_template_in_use->template_start_time = integration_start_time;   
				reset_prev_noise_addition_times_for_current_template(network, current_template_in_use);
				for (time_ns = integration_start_time; time_ns < integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)   // integrate remaining part in the next task period
				{
					if (time_ns >= (current_template_in_use->template_start_time + current_template_in_use->template_length))
					{
						determine_in_refractory_current_number_randomly(current_templates, &current_template_num_in_use);
						get_in_refractory_current_pattern_template(current_templates, &current_template_in_use, current_template_num_in_use);
						current_template_read_idx = 0;
						current_template_in_use->template_start_time = time_ns;   
						reset_prev_noise_addition_times_for_current_template(network, current_template_in_use);					
					}
					load_current_template_sample_to_neurons_with_noise(network, current_template_in_use, current_template_read_idx, time_ns);
					current_template_read_idx++;
					if (current_template_read_idx == current_template_in_use->num_of_template_samples)
						current_template_read_idx--; 	// read last idx again and again until trial_status_changes. it might appear since trialcontroller period is larger than this task	
					for (i = 0; i < num_of_neurons; i++)
					{	
						neuron = all_neurons[i];
						if (!evaluate_neuron_dyn(neuron, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, &spike_generated, &spike_time))
							return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_integration_handler", "! evaluate_neuron_dyn().");
						if (spike_generated)
						{
							write_generated_spike_to_blue_spike_buffer(spike_time_stamp, neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
							write_to_spike_data(generated_spike_data[0], neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
						}
						push_neuron_dynamics_to_neuron_dynamics_buffer_limited(network, limited_neuron_dynamics_buffer, time_ns, i);	
					}
					push_neuron_currents_to_neuron_current_buffer_limited(network, limited_current_pattern_buffer, time_ns);
				}
				break;
			case TRIAL_STATUS_START_TRIAL_AVAILABLE:  // ignore handling any duration.
				determine_trial_start_available_current_number_randomly(current_templates, &current_template_num_in_use);
				get_trial_start_available_current_pattern_template(current_templates, &current_template_in_use, current_template_num_in_use);
				current_template_read_idx = 0;
				current_template_in_use->template_start_time = integration_start_time;   
				reset_prev_noise_addition_times_for_current_template(network, current_template_in_use);
				for (time_ns = integration_start_time; time_ns < integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)   // integrate remaining part in the next task period
				{
					if (time_ns >= (current_template_in_use->template_start_time + current_template_in_use->template_length))
					{
						determine_trial_start_available_current_number_randomly(current_templates, &current_template_num_in_use);
						get_trial_start_available_current_pattern_template(current_templates, &current_template_in_use, current_template_num_in_use);
						current_template_read_idx = 0;
						current_template_in_use->template_start_time = time_ns;   
						reset_prev_noise_addition_times_for_current_template(network, current_template_in_use);			
					}
					load_current_template_sample_to_neurons_with_noise(network, current_template_in_use, current_template_read_idx, time_ns);
					current_template_read_idx++;
					if (current_template_read_idx == current_template_in_use->num_of_template_samples)
						current_template_read_idx--; 	// read last idx again and again until trial_status_changes. it might appear since trialcontroller period is larger than this task	
					for (i = 0; i < num_of_neurons; i++)
					{	
						neuron = all_neurons[i];
						if (!evaluate_neuron_dyn(neuron, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, &spike_generated, &spike_time))
							return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_integration_handler", "! evaluate_neuron_dyn().");
						if (spike_generated)
						{
							write_generated_spike_to_blue_spike_buffer(spike_time_stamp, neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
							write_to_spike_data(generated_spike_data[0], neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
						}
						push_neuron_dynamics_to_neuron_dynamics_buffer_limited(network, limited_neuron_dynamics_buffer, time_ns, i);	
					}
					push_neuron_currents_to_neuron_current_buffer_limited(network, limited_current_pattern_buffer, time_ns);
				}
				break;
			default: 
				return print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_rt_handler", "switch (last_trial_status_event.trial_status) - default");	
		}
	}
	else // no change in trial status
	{
		switch (last_trial_status_event.trial_status)
		{
			case TRIAL_STATUS_TRIALS_DISABLED:	// ignore handling
				break;
			case TRIAL_STATUS_IN_TRIAL:	
				switch (last_trial_status_event.selected_robot_target_idx_in_trial_hand_paradigm)
				{
					case LEFT_ROBOT_TARGET:   // left target
						for (time_ns = integration_start_time; time_ns < integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)   // integrate remaining part in the next task period
						{
							load_current_template_sample_to_neurons_with_noise(network, current_template_in_use, current_template_read_idx, time_ns);
							current_template_read_idx++;
							if (current_template_read_idx == current_template_in_use->num_of_template_samples)
								current_template_read_idx--; 	// read last idx again and again until trial_status_changes. it might appear since trialcontroller period is larger than this task	
							for (i = 0; i < num_of_neurons; i++)
							{	
								neuron = all_neurons[i];
								if (!evaluate_neuron_dyn(neuron, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, &spike_generated, &spike_time))
									return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_integration_handler", "! evaluate_neuron_dyn().");
								if (spike_generated)
								{
									write_generated_spike_to_blue_spike_buffer(spike_time_stamp, neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
									write_to_spike_data(generated_spike_data[0], neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
								}
								push_neuron_dynamics_to_neuron_dynamics_buffer_limited(network, limited_neuron_dynamics_buffer, time_ns, i);	
							}
							push_neuron_currents_to_neuron_current_buffer_limited(network, limited_current_pattern_buffer, time_ns);
						}

						break;									
					case RIGHT_ROBOT_TARGET:	// right target
						for (time_ns = integration_start_time; time_ns < integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)   // integrate remaining part in the next task period
						{
							load_current_template_sample_to_neurons_with_noise(network, current_template_in_use, current_template_read_idx, time_ns);
							current_template_read_idx++;
							if (current_template_read_idx == current_template_in_use->num_of_template_samples)
								current_template_read_idx--; 	// read last idx again and again until trial_status_changes. it might appear since trialcontroller period is larger than this task	
							for (i = 0; i < num_of_neurons; i++)
							{	
								neuron = all_neurons[i];
								if (!evaluate_neuron_dyn(neuron, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, &spike_generated, &spike_time))
									return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_integration_handler", "! evaluate_neuron_dyn().");
								if (spike_generated)
								{
									write_generated_spike_to_blue_spike_buffer(spike_time_stamp, neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
									write_to_spike_data(generated_spike_data[0], neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
								}
								push_neuron_dynamics_to_neuron_dynamics_buffer_limited(network, limited_neuron_dynamics_buffer, time_ns, i);	
							}
							push_neuron_currents_to_neuron_current_buffer_limited(network, limited_current_pattern_buffer, time_ns);
						}
						break;	
					default:
						return print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_integration_handler", "switch (last_trial_status_event.selected_robot_target_idx_in_trial_hand_paradigm)");	
				}				
				break;	
			case TRIAL_STATUS_IN_REFRACTORY:	
				for (time_ns = integration_start_time; time_ns < integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)   // integrate remaining part in the next task period
				{
					if (time_ns >= (current_template_in_use->template_start_time + current_template_in_use->template_length))  // restart current injection with new template since duration of this template is completed. 
					{
						determine_in_refractory_current_number_randomly(current_templates, &current_template_num_in_use);
						get_in_refractory_current_pattern_template(current_templates, &current_template_in_use, current_template_num_in_use);
						current_template_read_idx = 0;
						current_template_in_use->template_start_time = time_ns;   	
						reset_prev_noise_addition_times_for_current_template(network, current_template_in_use);					
					}
					load_current_template_sample_to_neurons_with_noise(network, current_template_in_use, current_template_read_idx, time_ns);
					current_template_read_idx++;
					if (current_template_read_idx == current_template_in_use->num_of_template_samples)
						current_template_read_idx--; 	// read last idx again and again until trial_status_changes. it might appear since trialcontroller period is larger than this task	
					for (i = 0; i < num_of_neurons; i++)
					{	
						neuron = all_neurons[i];
						if (!evaluate_neuron_dyn(neuron, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, &spike_generated, &spike_time))
							return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_integration_handler", "! evaluate_neuron_dyn().");
						if (spike_generated)
						{
							write_generated_spike_to_blue_spike_buffer(spike_time_stamp, neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
							write_to_spike_data(generated_spike_data[0], neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
						}
						push_neuron_dynamics_to_neuron_dynamics_buffer_limited(network, limited_neuron_dynamics_buffer, time_ns, i);	
					}
					push_neuron_currents_to_neuron_current_buffer_limited(network, limited_current_pattern_buffer, time_ns);
				}
				break;
			case TRIAL_STATUS_START_TRIAL_AVAILABLE:  // ignore handling any duration.
				for (time_ns = integration_start_time; time_ns < integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)   // integrate remaining part in the next task period
				{
					if (time_ns >= (current_template_in_use->template_start_time + current_template_in_use->template_length))
					{
						determine_trial_start_available_current_number_randomly(current_templates, &current_template_num_in_use);
						get_trial_start_available_current_pattern_template(current_templates, &current_template_in_use, current_template_num_in_use);
						current_template_read_idx = 0;
						current_template_in_use->template_start_time = time_ns;   
						reset_prev_noise_addition_times_for_current_template(network, current_template_in_use);			
					}
					load_current_template_sample_to_neurons_with_noise(network, current_template_in_use, current_template_read_idx, time_ns);
					current_template_read_idx++;
					if (current_template_read_idx == current_template_in_use->num_of_template_samples)
						current_template_read_idx--; 	// read last idx again and again until trial_status_changes. it might appear since trialcontroller period is larger than this task	
					for (i = 0; i < num_of_neurons; i++)
					{	
						neuron = all_neurons[i];
						if (!evaluate_neuron_dyn(neuron, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, &spike_generated, &spike_time))
							return print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_integration_handler", "! evaluate_neuron_dyn().");
						if (spike_generated)
						{
							write_generated_spike_to_blue_spike_buffer(spike_time_stamp, neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
							write_to_spike_data(generated_spike_data[0], neuron->layer, neuron->neuron_group, neuron->neuron_num, spike_time);
						}
						push_neuron_dynamics_to_neuron_dynamics_buffer_limited(network, limited_neuron_dynamics_buffer, time_ns, i);	
					}
					push_neuron_currents_to_neuron_current_buffer_limited(network, limited_current_pattern_buffer, time_ns);
				}
				break;
			default: 
				get_trial_status_type_string(last_trial_status_event.trial_status, message);
				print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_rt_handler", message);	
				return print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_rt_handler", "default - switch.");
		}		
	}
	return TRUE;
}

void bmi_simulation_spike_generator_kill_rt_task(void)
{
	bmi_simulation_spike_generator_rt_task_stay_alive = 0;
}

static bool write_generated_spike_to_blue_spike_buffer(SortedSpikes *sorted_spike_time_stamp, unsigned int layer_num, unsigned int neuron_grp_num, unsigned int neuron_num, TimeStamp spike_time)
{
	SortedSpikeItem	*sorted_spike_chan_buff;
	int sorted_spike_buff_idx_write;

	sorted_spike_chan_buff = (*sorted_spike_time_stamp)[layer_num][neuron_grp_num].buffer;
	sorted_spike_buff_idx_write = (*sorted_spike_time_stamp)[layer_num][neuron_grp_num].buff_idx_write;

	sorted_spike_chan_buff[sorted_spike_buff_idx_write].peak_time = spike_time;
	sorted_spike_chan_buff[sorted_spike_buff_idx_write].unit = neuron_num;

	if ((sorted_spike_buff_idx_write +1) ==  BLUESPIKE_SORTED_SPIKE_BUFF_SIZE )	   // first check then increment. if first increment and check end of buffer might lead to problem during reading.
		(*sorted_spike_time_stamp)[layer_num][neuron_grp_num].buff_idx_write = 0;
	else
		((*sorted_spike_time_stamp)[layer_num][neuron_grp_num].buff_idx_write)++;	
	
	return TRUE;			
}
