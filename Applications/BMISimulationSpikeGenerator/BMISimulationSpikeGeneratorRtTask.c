#include "BMISimulationSpikeGeneratorRtTask.h"

static int bmi_simulation_spike_generator_rt_thread = 0;
static bool bmi_simulation_spike_generator_rt_task_stay_alive = 1;

static void *bmi_simulation_spike_generator_rt_handler(void *args); 
static bool bmi_simulation_spike_generator_integration_handler(TrialsData *trials_data, Network *network, CurrentTemplate *current_templates, CurrentPatternBuffer *current_pattern_buffer, NeuronDynamicsBuffer	*neuron_dynamics_buffer, TimeStamp *integration_start_time,  TimeStamp *integration_end_time, unsigned int num_of_layers, char *message);


static void *bmi_simulation_spike_generator_rt_handler(void *args)
{
	RT_TASK *handler;
        RTIME period;
	unsigned int prev_time, curr_time;
	unsigned int num_of_layers;
	char message[TRIAL_STATUS_TYPE_MAX_STRING_LENGTH];
	TimeStamp integration_start_time, integration_end_time;
	SpikeGenData *spike_gen_data = get_bmi_simulation_spike_generator_spike_gen_data();
	TrialsData *trials_data = get_bmi_simulation_spike_generator_trials_data();
	Network *network = spike_gen_data->network;
	CurrentTemplate *current_templates =  spike_gen_data->current_templates;
	CurrentPatternBuffer *current_pattern_buffer =  spike_gen_data->current_pattern_buffer;
	NeuronDynamicsBuffer	*neuron_dynamics_pattern_buffer = spike_gen_data->neuron_dynamics_pattern_buffer;
	if (! check_rt_task_specs_to_init(SPIKE_GENERATOR_CPU_ID, SPIKE_GENERATOR_CPU_THREAD_ID, SPIKE_GENERATOR_PERIOD))  {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(SPIKE_GENERATOR_TASK_NAME,SPIKE_GENERATOR_TASK_PRIORITY, SPIKE_GENERATOR_STACK_SIZE, SPIKE_GENERATOR_MSG_SIZE,SPIKE_GENERATOR_POLICY, 1 << ((SPIKE_GENERATOR_CPU_ID*MAX_NUM_OF_THREADS_PER_CPU)+SPIKE_GENERATOR_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(SPIKE_GENERATOR_CPU_ID, SPIKE_GENERATOR_CPU_THREAD_ID, SPIKE_GENERATOR_PERIOD, SPIKE_GENERATOR_POSITIVE_JITTER_THRES, SPIKE_GENERATOR_NEGATIVE_JITTER_THRES))  {
		print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	

        period = nano2count(SPIKE_GENERATOR_PERIOD);
        rt_task_make_periodic(handler, rt_get_time() + period, period);
	prev_time = rt_get_cpu_time_ns();	
	integration_start_time = shared_memory->rt_tasks_data.current_system_time;	
	reset_all_network_neuron_dynamics (network);
	if (!get_num_of_layers_in_network(network, &num_of_layers))
		return (void *)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "Couldn' t retrieve number of layers.");	
        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 
        while (bmi_simulation_spike_generator_rt_task_stay_alive) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(SPIKE_GENERATOR_CPU_ID, SPIKE_GENERATOR_CPU_THREAD_ID, prev_time, curr_time);
		prev_time = curr_time;
		// routines
		integration_end_time =  shared_memory->rt_tasks_data.current_system_time;
		if(! bmi_simulation_spike_generator_integration_handler(trials_data, network, current_templates, current_pattern_buffer, neuron_dynamics_pattern_buffer, &integration_start_time, &integration_end_time, num_of_layers, message))
			return (void *)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "! bmi_simulation_spike_generator_integration_handler().");
		// routines	
		evaluate_and_save_period_run_time(SPIKE_GENERATOR_CPU_ID, SPIKE_GENERATOR_CPU_THREAD_ID, curr_time, rt_get_cpu_time_ns());		
        }
        rt_task_delete(handler);

	print_message(INFO_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "rt_task_delete().");		
        return 0;
}

static bool bmi_simulation_spike_generator_integration_handler(TrialsData *trials_data, Network *network, CurrentTemplate *current_templates, CurrentPatternBuffer *current_pattern_buffer, NeuronDynamicsBuffer	*neuron_dynamics_buffer, TimeStamp *integration_start_time,  TimeStamp *integration_end_time, unsigned int num_of_layers, char *message)
{
	if (status_change)
		select current according to trial status
		reset_current_template_read_idx
	else 
		handle_current_duration according to trial status
		if end of template
			if in trial
				write last current sample to buffer
			else
				select new current template according to trial status
		else
			write next current sample to buffer.


	current_duration_handler
	if change

	static unsigned int trial_status_prev = TRIAL_STATUS_TRIALS_DISABLED;
	static unsigned int current_template_num_in_use = 0;
	static unsigned int current_template_read_idx = 0;
	TimeStamp time_ns;
	TimeStamp spike_time;
	unsigned int i, j, k;
	unsigned int num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	Neuron *neuron;
	switch (trial_status)
	{
		case TRIAL_STATUS_TRIALS_DISABLED:	// ignore handling
			break;
		case TRIAL_STATUS_IN_TRIAL:	
			for (time_ns = *integration_start_time; time_ns < *integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)
			{
				current_templates->
				for (i = 0; i < num_of_layers; i++)
				{	
					if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
						return (CurrentTemplate*)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "Couldn' t retrieve number of neuron groups.");
					for (j=0; j<num_of_neuron_groups_in_layer; j++)
					{
						if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
							return (CurrentTemplate*)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "Couldn' t retrieve number of neurons.");
						for (k = 0; k < num_of_neurons_in_neuron_group; k++)
						{
							neuron = get_neuron_address(network, i, j, k);
							spike_time = evaluate_neuron_dyn(neuron, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
						}
					}
				}					
			}
			break;
		case TRIAL_STATUS_IN_REFRACTORY:	// 
			for (time_ns = *integration_start_time; time_ns < *integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)
			{
				for (i = 0; i < num_of_layers; i++)
				{	
					if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
						return (CurrentTemplate*)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "Couldn' t retrieve number of neuron groups.");
					for (j=0; j<num_of_neuron_groups_in_layer; j++)
					{
						if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
							return (CurrentTemplate*)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "Couldn' t retrieve number of neurons.");
						for (k = 0; k < num_of_neurons_in_neuron_group; k++)
						{
							neuron = get_neuron_address(network, i, j, k);
							spike_time = evaluate_neuron_dyn(neuron, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
						}
					}
				}					
			}
			break;
		case TRIAL_STATUS_START_TRIAL_AVAILABLE:  // ignore handling any duration.
			for (time_ns = *integration_start_time; time_ns < *integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)
			{
				for (i = 0; i < num_of_layers; i++)
				{	
					if(!get_num_of_neuron_groups_in_layer(network, i, &num_of_neuron_groups_in_layer))
						return (CurrentTemplate*)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "Couldn' t retrieve number of neuron groups.");
					for (j=0; j<num_of_neuron_groups_in_layer; j++)
					{
						if (!get_num_of_neurons_in_neuron_group(network, i, j, &num_of_neurons_in_neuron_group))
							return (CurrentTemplate*)print_message(ERROR_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGenerator", "bmi_simulation_spike_generator_rt_handler", "Couldn' t retrieve number of neurons.");
						for (k = 0; k < num_of_neurons_in_neuron_group; k++)
						{
							neuron = get_neuron_address(network, i, j, k);
							spike_time = evaluate_neuron_dyn(neuron, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
						}
					}
				}					
			}
			break;
		default: 
			get_trial_status_type_string(trial_status, message);
			print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_rt_handler", message);	
			return print_message(BUG_MSG ,"BMISimulationSpikeGenerator", "BMISimulationSpikeGeneratorRtTask", "bmi_simulation_spike_generator_rt_handler", "default - switch.");	
	}
	*integration_start_time = time_ns - PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
	trial_status_prev = trial_status;
}

void bmi_simulation_spike_generator_kill_rt_task(void)
{
	bmi_simulation_spike_generator_rt_task_stay_alive = 0;
}


