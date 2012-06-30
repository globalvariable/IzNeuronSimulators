 #include "ParkerSochacki.h"

static unsigned int parker_sochacki_max_order = 0;
static double parker_sochacki_error_tolerance = 0;

static bool allocate_and_initialize_parker_sochacki_pol_vals_for_all_neurons(Network *network);

bool parker_sochacki_set_order_tolerance(Network *network, unsigned int max_ps_order, double ps_error_tolerance)
{
	if (ps_error_tolerance < 0)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "ParkerSochacki", "parker_sochacki_set_order_tolerance", "ps_error_tolerance < 0");

	parker_sochacki_max_order = max_ps_order;
	parker_sochacki_error_tolerance = ps_error_tolerance;

	if (allocate_and_initialize_parker_sochacki_pol_vals_for_all_neurons(network))
		return TRUE;
	else
		return FALSE;
}

int get_maximum_parker_sochacki_order(void)
{
	return parker_sochacki_max_order;
}

double get_maximum_parker_sochacki_error_tolerance(void)
{
	return parker_sochacki_error_tolerance;
}

static bool allocate_and_initialize_parker_sochacki_pol_vals_for_all_neurons(Network *network)
{
	int i, j, k, m;
	Layer		*ptr_layer = NULL;
	NeuronGroup	*ptr_neuron_group = NULL;
	Neuron		*ptr_neuron = NULL;
	ParkerSochackiPolynomialVals	*ptr_ps_vals;
	
	if (!is_network_allocated(network))
		return FALSE;
	for (i=0; i<network->layer_count; i++)
	{
		ptr_layer = network->layers[i];
		for (j=0; j<ptr_layer->neuron_group_count; j++)
		{
			ptr_neuron_group = ptr_layer->neuron_groups[j];
			for (k=0; k<ptr_neuron_group->neuron_count; k++)
			{
				ptr_neuron = &(ptr_neuron_group->neurons[k]);
				ptr_ps_vals = ptr_neuron->ps_vals;

				g_free(ptr_ps_vals->v_pol_vals);			
				g_free(ptr_ps_vals->u_pol_vals);
				g_free(ptr_ps_vals->conductance_excitatory_pol_vals);
				g_free(ptr_ps_vals->conductance_inhibitory_pol_vals);
				g_free(ptr_ps_vals->chi_pol_vals);
				g_free(ptr_ps_vals->E_pol_vals);		
				g_free(ptr_ps_vals->a_pol_vals);
				g_free(ptr_ps_vals->conductance_decay_rate_excitatory_pol_vals);
				g_free(ptr_ps_vals->conductance_decay_rate_inhibitory_pol_vals);			

				ptr_ps_vals->v_pol_vals = NULL;			
				ptr_ps_vals->u_pol_vals = NULL;
				ptr_ps_vals->conductance_excitatory_pol_vals = NULL;
				ptr_ps_vals->conductance_inhibitory_pol_vals = NULL;
				ptr_ps_vals->chi_pol_vals = NULL;
				ptr_ps_vals->E_pol_vals = NULL;		
				ptr_ps_vals->a_pol_vals = NULL;
				ptr_ps_vals->conductance_decay_rate_excitatory_pol_vals = NULL;
				ptr_ps_vals->conductance_decay_rate_inhibitory_pol_vals = NULL;

					
				// size should be parker_sochacki_max_order + 1
				ptr_ps_vals->v_pol_vals = g_new0(double, parker_sochacki_max_order + 1); if (ptr_ps_vals->v_pol_vals == NULL) { return FALSE; }  		
				ptr_ps_vals->u_pol_vals = g_new0(double, parker_sochacki_max_order + 1); if (ptr_ps_vals->u_pol_vals == NULL) { return FALSE; }  
				ptr_ps_vals->conductance_excitatory_pol_vals = g_new0(double, parker_sochacki_max_order + 1); if (ptr_ps_vals->conductance_excitatory_pol_vals == NULL) { return FALSE; }  
				ptr_ps_vals->conductance_inhibitory_pol_vals = g_new0(double, parker_sochacki_max_order + 1); if (ptr_ps_vals->conductance_inhibitory_pol_vals == NULL) { return FALSE; }  
				ptr_ps_vals->chi_pol_vals = g_new0(double, parker_sochacki_max_order + 1); if (ptr_ps_vals->chi_pol_vals == NULL) { return FALSE; }  
				ptr_ps_vals->E_pol_vals = g_new0(double, parker_sochacki_max_order + 1); if (ptr_ps_vals->E_pol_vals == NULL) { return FALSE; }  		
				ptr_ps_vals->a_pol_vals = g_new0(double, parker_sochacki_max_order + 1); if (ptr_ps_vals->a_pol_vals == NULL) { return FALSE; }  
				ptr_ps_vals->conductance_decay_rate_excitatory_pol_vals = g_new0(double, parker_sochacki_max_order + 1); if (ptr_ps_vals->conductance_decay_rate_excitatory_pol_vals == NULL) { return FALSE; }  
				ptr_ps_vals->conductance_decay_rate_inhibitory_pol_vals = g_new0(double, parker_sochacki_max_order + 1); if (ptr_ps_vals->conductance_decay_rate_inhibitory_pol_vals == NULL) { return FALSE; }  
				for (m = 0; m < parker_sochacki_max_order + 1; m++)
				{
					ptr_ps_vals->E_pol_vals[m] = ptr_neuron->iz_params->E/(m +1);
					ptr_ps_vals->a_pol_vals[m] = ptr_neuron->iz_params->a/(m+1);
					ptr_ps_vals->conductance_decay_rate_excitatory_pol_vals[m] = ptr_neuron->iz_params->decay_rate_excitatory/(m+1);
					ptr_ps_vals->conductance_decay_rate_inhibitory_pol_vals[m] = ptr_neuron->iz_params->decay_rate_inhibitory/(m+1);	
				}
			}					
		}
	}	
	return TRUE;
}

bool evaluate_neuron_dyn(Neuron *nrn, TimeStamp start_time, TimeStamp end_time, bool *spike_generated, TimeStamp *spike_time)	// Assume that only once a spike can be generated per step (0.25 ms).
{			
	TimeStamp 			integration_start_ns;
	NeuronEventBuffer		*neuron_event_buffer;
	TimeStamp			*event_times;			
	SynapseIndex			*syn_idx;	
	SynapseIndex			event_from_syn_idx;
	NeuronSynapseList	*syn_list;
	unsigned int			event_buff_size;			
	unsigned int			*ptr_event_buffer_write_idx;  		
	unsigned int			*ptr_event_buffer_read_idx; 		
	unsigned int			idx, end_idx;	
	TimeStamp			event_time;	
	
	neuron_event_buffer = nrn->event_buff;
	syn_list = nrn->syn_list;
	event_times = neuron_event_buffer->time;
	syn_idx = neuron_event_buffer->syn_idx;
	event_from_syn_idx = syn_list->num_of_synapses;	// to be null
	event_buff_size = neuron_event_buffer->buff_size;
	ptr_event_buffer_write_idx = &(neuron_event_buffer->write_idx);
	ptr_event_buffer_read_idx = &(neuron_event_buffer->read_idx);
			
	integration_start_ns = start_time;   // required to know it to schedule events for outputs  (parker_sochacki_step_start_time+ dt_part)

	idx = *ptr_event_buffer_read_idx;
	end_idx = *ptr_event_buffer_write_idx;
	// spikes times were already sorted during scheduling.
	while (idx != end_idx)		// There is a check for event buffer overflow in schedule_events()
	{
		event_time = event_times[idx];
		if (event_time >= end_time)	// Do not handle if it is at start of next step or later than that
			break;
		if (event_time < integration_start_ns)	// this function handles sorted synaptic events. the events are sorted during buffering
		{
			printf ("Simulate: BUG: evaluate_neuron_dyn - Simulate.c\n");
			printf ("Simulate: BUG: event_time is %llu but the step start time is %llu\n", event_time, start_time);		
			printf ("Simulate: BUG: There must be a problem at event sorting\n");	
			return 0;
		}
		if (!parker_sochacki_integration(nrn, integration_start_ns, event_time, spike_generated, spike_time))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "ParkerSochacki", "evaluate_neuron_dyn", "! parker_sochacki_integration().");
		event_from_syn_idx = syn_idx[idx];
		if (syn_list->type[event_from_syn_idx] == EXCITATORY_SYNAPSE)		
			nrn->iz_params->conductance_excitatory += syn_list->weight[event_from_syn_idx];
		else if (syn_list->type[event_from_syn_idx] == INHIBITORY_SYNAPSE)
			nrn->iz_params->conductance_inhibitory -= syn_list->weight[event_from_syn_idx];
		else
			return print_message(BUG_MSG ,"IzNeuronSimulators", "ParkerSochacki", "evaluate_neuron_dyn", "Unknown neuron type(excitatory/inhibitory).");
		nrn->stdp_list->stdp_pre_post[event_from_syn_idx] += nrn->stdp_list->change_stdp_pre_post[event_from_syn_idx];	// increment pre_post
		nrn->eligibility_list->eligibility[event_from_syn_idx] += nrn->stdp_list->stdp_post_pre[event_from_syn_idx];  /// it is already negative. update eligibility according to post pre event
//		printf ("%llu %f\n", event_time, event_weights[idx]);
		integration_start_ns = event_time;
		idx++;
		if (idx == event_buff_size)
			idx = 0;
		*ptr_event_buffer_read_idx = idx;		// Finally set event_buffer_read_idx,	event_buffer_write_idx will be set by  insert_synaptic_event at the end of function so that no corruption will appear even with multithreading
	}
	if (!parker_sochacki_integration(nrn, integration_start_ns, end_time, spike_generated, spike_time))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "ParkerSochacki", "evaluate_neuron_dyn", "! parker_sochacki_integration().");
	return TRUE ;	
}



bool parker_sochacki_integration(Neuron *nrn, TimeStamp integration_start_time, TimeStamp integration_end_time, bool *spike_generated, TimeStamp *spike_time)
{
	ParkerSochackiPolynomialVals *polynomial_vals = nrn->ps_vals;
	IzNeuronParams	*iz_params = nrn->iz_params;
	STDPList		*stdp_list = nrn->stdp_list;
	EligibilityList		*eligibility_list = nrn->eligibility_list;

	double *v_pol_vals = polynomial_vals->v_pol_vals;			
	double *u_pol_vals = polynomial_vals->u_pol_vals;
	double *conductance_excitatory_pol_vals = polynomial_vals->conductance_excitatory_pol_vals;
	double *conductance_inhibitory_pol_vals = polynomial_vals->conductance_inhibitory_pol_vals;
	double *chi_pol_vals = polynomial_vals->chi_pol_vals;
	double *E_pol_vals = polynomial_vals->E_pol_vals;		
	double *a_pol_vals = polynomial_vals->a_pol_vals;
	double *conductance_decay_rate_excitatory_pol_vals = polynomial_vals->conductance_decay_rate_excitatory_pol_vals;
	double *conductance_decay_rate_inhibitory_pol_vals = polynomial_vals->conductance_decay_rate_inhibitory_pol_vals;

	double		*stdp_pre_post = stdp_list->stdp_pre_post; 					
//	double		*change_stdp_pre_post = stdp_list->change_stdp_pre_post;		// not necessary here 	
//	double		*decay_rate_stdp_pre_post = stdp_list->decay_rate_stdp_pre_post;		// not necessary here 	
	double		*stdp_pre_post_iter_prev = stdp_list->stdp_pre_post_iter_prev;	
	double		*stdp_pre_post_iter_curr = stdp_list->stdp_pre_post_iter_curr;	
	double 		**stdp_pre_post_pol_vals = stdp_list->stdp_pre_post_pol_vals;			
	double		**stdp_pre_post_decay_rate_pol_vals = stdp_list->stdp_pre_post_decay_rate_pol_vals;	
	double		*stdp_post_pre = stdp_list->stdp_post_pre;					
	double		*change_stdp_post_pre = stdp_list->change_stdp_post_pre;			
//	double		*decay_rate_stdp_post_pre = stdp_list->decay_rate_stdp_post_pre;	// not necessary here 	
	double		*stdp_post_pre_iter_prev = stdp_list->stdp_post_pre_iter_prev;	
	double		*stdp_post_pre_iter_curr = stdp_list->stdp_post_pre_iter_curr;			
	double 		**stdp_post_pre_pol_vals = stdp_list->stdp_post_pre_pol_vals;			
	double		**stdp_post_pre_decay_rate_pol_vals = stdp_list->stdp_post_pre_decay_rate_pol_vals;	

	double		*eligibility = eligibility_list->eligibility;					
//	double		*eligibility_decay_rate = eligibility_list->eligibility_decay_rate ; 	// not necessary here 	
	double		*eligibility_iter_prev = eligibility_list->eligibility_iter_prev;	
	double		*eligibility_iter_curr = eligibility_list->eligibility_iter_curr;			
	double 		**eligibility_pol_vals = eligibility_list->eligibility_pol_vals;			
	double		**eligibility_decay_rate_pol_vals = eligibility_list->eligibility_decay_rate_pol_vals;

	*spike_generated = FALSE;	
	double dt_part;
	double dt;
	int p;
	unsigned int s, num_of_synapses = nrn->syn_list->num_of_synapses;
		

	v_pol_vals[0] = iz_params->v;	
	u_pol_vals[0] = iz_params->u;
	conductance_excitatory_pol_vals[0] =  iz_params->conductance_excitatory;
	conductance_inhibitory_pol_vals[0] =  iz_params->conductance_inhibitory;
	chi_pol_vals[0] = iz_params->k*iz_params->v - conductance_excitatory_pol_vals[0] - conductance_inhibitory_pol_vals[0] - iz_params->k_v_threshold;

	for (s = 0; s < num_of_synapses; s++)	// update eligibility & post_pre STDP
	{
		stdp_pre_post_pol_vals[s][0] = stdp_pre_post[s];	
 		stdp_post_pre_pol_vals[s][0] = stdp_post_pre[s];	
		eligibility_pol_vals[s][0]= eligibility[s];  /// increment eligibility 
	}	

//	(chi = kv - eta - gamma - k*v_t)
	dt = (integration_end_time - integration_start_time)/PARKER_SOCHACKI_EMBEDDED_STEP_SIZE;     // do not change PARKER_SOCHACKI_EMBEDDED_STEP_SIZE
	p=parker_sochacki_step(nrn, v_pol_vals, u_pol_vals, conductance_excitatory_pol_vals, conductance_inhibitory_pol_vals, chi_pol_vals, E_pol_vals, a_pol_vals, conductance_decay_rate_excitatory_pol_vals, conductance_decay_rate_inhibitory_pol_vals, stdp_pre_post_pol_vals, stdp_post_pre_pol_vals, eligibility_pol_vals, stdp_pre_post_decay_rate_pol_vals, stdp_post_pre_decay_rate_pol_vals, eligibility_decay_rate_pol_vals, stdp_pre_post_iter_prev, stdp_pre_post_iter_curr, stdp_post_pre_iter_prev, stdp_post_pre_iter_curr, eligibility_iter_prev, eligibility_iter_curr, num_of_synapses, dt);
//	printf("%f\t %d\n ", nrn->v, p);
	if (iz_params->v  > iz_params->v_peak)    // updated nrn->v inside parker_sochacki_step(dt)
	{
		dt_part = newton_raphson_peak_detection(iz_params->v_peak, v_pol_vals, p, dt);
		*spike_generated = TRUE;
		*spike_time = integration_start_time+((TimeStamp)((dt_part*PARKER_SOCHACKI_EMBEDDED_STEP_SIZE)+0.5)); // do not change PARKER_SOCHACKI_EMBEDDED_STEP_SIZE
//		printf("---------------->  Spike time %.15f %llu\n", ((integration_start_time)/PARKER_SOCHACKI_EMBEDDED_STEP_SIZE)+dt_part, *spike_time);		
		if (!schedule_event(nrn, *spike_time))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "ParkerSochacki", "parker_sochacki_integration", "! schedule_events().");

		parker_sochacki_update(nrn, u_pol_vals, conductance_excitatory_pol_vals, conductance_inhibitory_pol_vals, stdp_pre_post_pol_vals, stdp_post_pre_pol_vals, eligibility_pol_vals, stdp_pre_post, stdp_post_pre, eligibility, num_of_synapses, dt_part, p);
		for (s = 0; s < num_of_synapses; s++)	// update eligibility & post_pre STDP
		{
			stdp_post_pre[s] += change_stdp_post_pre[s];	// it is already negative. 
			eligibility[s] += stdp_pre_post[s];  /// increment eligibility 
		}

		v_pol_vals[0] = iz_params->c;    ///   v = c
		u_pol_vals[0] = iz_params->u + iz_params->d;  ///  u = u + d
		conductance_excitatory_pol_vals[0] =   iz_params->conductance_excitatory;
		conductance_inhibitory_pol_vals[0] =   iz_params->conductance_inhibitory;
		chi_pol_vals[0] = iz_params->k*iz_params->c - conductance_excitatory_pol_vals[0] - conductance_inhibitory_pol_vals[0] - iz_params->k_v_threshold;    //   used c instead of v since both same but v has not been updated
		for (s = 0; s < num_of_synapses; s++)	// update eligibility & post_pre STDP
		{
			stdp_pre_post_pol_vals[s][0] = stdp_pre_post[s];	
 			stdp_post_pre_pol_vals[s][0] = stdp_post_pre[s];	
			eligibility_pol_vals[s][0]= eligibility[s];  /// increment eligibility 
		}	
		p=parker_sochacki_step(nrn, v_pol_vals, u_pol_vals, conductance_excitatory_pol_vals, conductance_inhibitory_pol_vals, chi_pol_vals, E_pol_vals, a_pol_vals, conductance_decay_rate_excitatory_pol_vals, conductance_decay_rate_inhibitory_pol_vals, stdp_pre_post_pol_vals, stdp_post_pre_pol_vals, eligibility_pol_vals, stdp_pre_post_decay_rate_pol_vals, stdp_post_pre_decay_rate_pol_vals, eligibility_decay_rate_pol_vals, stdp_pre_post_iter_prev, stdp_pre_post_iter_curr, stdp_post_pre_iter_prev, stdp_post_pre_iter_curr, eligibility_iter_prev, eligibility_iter_curr, num_of_synapses, dt-dt_part);
	}
	return TRUE;
}

int parker_sochacki_step (Neuron *nrn, double *v_pol_vals, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double *chi_pol_vals, double *E_pol_vals, double *a_pol_vals, double *conductance_decay_rate_excitatory_pol_vals , double *conductance_decay_rate_inhibitory_pol_vals, double **stdp_pre_post_pol_vals, double **stdp_post_pre_pol_vals, double **eligibility_pol_vals, double **stdp_pre_post_decay_rate_pol_vals, double **stdp_post_pre_decay_rate_pol_vals, double **eligibility_decay_rate_pol_vals, double *stdp_pre_post_iter_prev, double *stdp_pre_post_iter_curr, double *stdp_post_pre_iter_prev, double *stdp_post_pre_iter_curr, double *eligibility_iter_prev, double *eligibility_iter_curr, unsigned int num_of_synapses, double dt)
{
	double dt_pow;
	int m, p, s;
	double v_x_chi;
	double v_prev_for_iter, u_prev_for_iter, conductance_excitatory_prev_for_iter, conductance_inhibitory_prev_for_iter, chi_prev_for_iter;
	double v_curr_for_iter, u_curr_for_iter, conductance_excitatory_curr_for_iter, conductance_inhibitory_curr_for_iter, chi_curr_for_iter;	
	double b, k, E_excitatory, E_inhibitory;
	IzNeuronParams	*iz_params = nrn->iz_params;

	double		*stdp_pre_post = nrn->stdp_list->stdp_pre_post; 
	double		*stdp_post_pre = nrn->stdp_list->stdp_post_pre;
 	double		*eligibility = nrn->eligibility_list->eligibility;		

	b = iz_params->b;
	k = iz_params->k;	
	E_excitatory = iz_params->E_excitatory;
	E_inhibitory =  iz_params->E_inhibitory;
	
	v_pol_vals[1] =iz_params->E*( v_pol_vals[0] * chi_pol_vals[0] + E_excitatory * conductance_excitatory_pol_vals[0] + E_inhibitory * conductance_inhibitory_pol_vals[0]  - u_pol_vals[0]+ iz_params->I_inject);  // eqtn 27
	u_pol_vals[1] = iz_params->a * (b * v_pol_vals[0] - u_pol_vals[0]);
	conductance_excitatory_pol_vals[1] = iz_params->decay_rate_excitatory * conductance_excitatory_pol_vals[0];  // decay_rate_excitatory already negatized during initialization
	conductance_inhibitory_pol_vals[1] = iz_params->decay_rate_inhibitory * conductance_inhibitory_pol_vals[0];   //decay_rate_inhibitory already negatized during initialization
	chi_pol_vals[1] = k * v_pol_vals[1] - conductance_excitatory_pol_vals[1] - conductance_inhibitory_pol_vals[1];

	for (s = 0; s < num_of_synapses; s++)
	{
		stdp_pre_post_pol_vals[s][1] = stdp_pre_post_decay_rate_pol_vals[s][0] * stdp_pre_post_pol_vals[s][0];   // stdp_pre_post_decay_rate_pol_vals[i][0] = decay_rate_stdp_pre_post[i]
 		stdp_post_pre_pol_vals[s][1] = stdp_post_pre_decay_rate_pol_vals[s][0] * stdp_post_pre_pol_vals[s][0];	
		eligibility_pol_vals[s][1] = eligibility_decay_rate_pol_vals[s][0] * eligibility_pol_vals[s][0];  
	}

	v_prev_for_iter = v_pol_vals[0] + v_pol_vals[1] * dt;
	u_prev_for_iter = u_pol_vals[0] + u_pol_vals[1] * dt;
	conductance_excitatory_prev_for_iter = conductance_excitatory_pol_vals[0] + conductance_excitatory_pol_vals[1] * dt;
	conductance_inhibitory_prev_for_iter = conductance_inhibitory_pol_vals[0] + conductance_inhibitory_pol_vals[1] * dt;
	chi_prev_for_iter = chi_pol_vals[0] + chi_pol_vals[1] * dt;

	for (s = 0; s < num_of_synapses; s++)
	{
		stdp_pre_post_iter_prev[s] = stdp_pre_post_pol_vals[s][0] + stdp_pre_post_pol_vals[s][1] * dt;   
		stdp_post_pre_iter_prev[s] = stdp_post_pre_pol_vals[s][0] + stdp_post_pre_pol_vals[s][1] * dt;   
		eligibility_iter_prev[s] = eligibility_pol_vals[s][0] + eligibility_pol_vals[s][1] * dt;   
	}	
	
	v_curr_for_iter = v_prev_for_iter;
	u_curr_for_iter = v_prev_for_iter;
	conductance_excitatory_curr_for_iter = conductance_excitatory_prev_for_iter;
	conductance_inhibitory_curr_for_iter = conductance_inhibitory_prev_for_iter;
	chi_curr_for_iter = chi_prev_for_iter; 

	for (s = 0; s < num_of_synapses; s++)
	{
		stdp_pre_post_iter_curr[s] = stdp_pre_post_iter_prev[s];
		stdp_post_pre_iter_curr[s] = stdp_post_pre_iter_prev[s];   
		eligibility_iter_curr[s] = eligibility_iter_prev[s];   
	}	
	
	dt_pow= dt*dt;

	for (p=1;p < (parker_sochacki_max_order-1); p++)
	{
		v_x_chi = v_pol_vals[0] * chi_pol_vals[p] + v_pol_vals[p] * chi_pol_vals[0];   ///  Cauchy product
		for (m=1; m<p; m++)
		{
			v_x_chi += v_pol_vals[m] * chi_pol_vals[p-m];
		}
		v_pol_vals[p+1] = E_pol_vals[p] * (v_x_chi + E_excitatory * conductance_excitatory_pol_vals[p] + E_inhibitory * conductance_inhibitory_pol_vals[p] - u_pol_vals[p]); 
		u_pol_vals[p+1] =  a_pol_vals[p] * (b * v_pol_vals[p] - u_pol_vals[p]);
		conductance_excitatory_pol_vals[p+1] = conductance_decay_rate_excitatory_pol_vals[p] * conductance_excitatory_pol_vals[p]; // decay_rate_excitatory already negatized
 		conductance_inhibitory_pol_vals[p+1] = conductance_decay_rate_inhibitory_pol_vals[p] * conductance_inhibitory_pol_vals[p];// decay_rate_inhibitory already negatized
		chi_pol_vals[p+1] = k * v_pol_vals[p+1] - conductance_excitatory_pol_vals[p+1] - conductance_inhibitory_pol_vals[p+1];

		for (s = 0; s < num_of_synapses; s++)
		{
			stdp_pre_post_pol_vals[s][p+1] = stdp_pre_post_decay_rate_pol_vals[s][p] * stdp_pre_post_pol_vals[s][p];   
			stdp_post_pre_pol_vals[s][p+1] = stdp_post_pre_decay_rate_pol_vals[s][p] * stdp_post_pre_pol_vals[s][p];   
			eligibility_pol_vals[s][p+1] = eligibility_decay_rate_pol_vals[s][p] * eligibility_pol_vals[s][p];   
		}	

		v_curr_for_iter = v_prev_for_iter + v_pol_vals[p+1] * dt_pow;
		u_curr_for_iter = u_prev_for_iter  + u_pol_vals[p+1] * dt_pow;
		conductance_excitatory_curr_for_iter = conductance_excitatory_prev_for_iter + conductance_excitatory_pol_vals[p+1] * dt_pow;
		conductance_inhibitory_curr_for_iter = conductance_inhibitory_prev_for_iter  + conductance_inhibitory_pol_vals[p+1] * dt_pow;
		chi_curr_for_iter = chi_prev_for_iter + chi_pol_vals[p+1] * dt_pow;

		for (s = 0; s < num_of_synapses; s++)
		{
			stdp_pre_post_iter_curr[s] = stdp_pre_post_iter_prev[s] + stdp_pre_post_pol_vals[s][p+1] * dt_pow;
			stdp_post_pre_iter_curr[s] = stdp_post_pre_iter_prev[s] + stdp_post_pre_pol_vals[s][p+1] * dt_pow;   
			eligibility_iter_curr[s] = eligibility_iter_prev[s] + eligibility_pol_vals[s][p+1] * dt_pow;   
		}	

		dt_pow *= dt;

//		printf("v_pol_vals[p+1] = %f\t v_curr_for_iter=%f\n", v_pol_vals[p+1], v_curr_for_iter);
/*		if (fabs(v_pol_vals[p+1]) > 50000.0)  ///  Different from Stewart Bair 2009 since they scale E_pol_vals[p]*1/4 
		{
			printf("ERROR: Parker Sochacki Solution Divergence!!!\n");
			return 0;			
		}
	*/	if (fabs(v_curr_for_iter - v_prev_for_iter) > parker_sochacki_error_tolerance)
			goto GO_ON_ITER;
		if (fabs(u_curr_for_iter - u_prev_for_iter) > parker_sochacki_error_tolerance)
			goto GO_ON_ITER;
		if (fabs(conductance_excitatory_curr_for_iter - conductance_excitatory_prev_for_iter) > parker_sochacki_error_tolerance)
			goto GO_ON_ITER;
		if (fabs(conductance_inhibitory_curr_for_iter - conductance_inhibitory_prev_for_iter) > parker_sochacki_error_tolerance)
			goto GO_ON_ITER;
		for (s = 0; s < num_of_synapses; s++)
		{
			if (fabs(stdp_pre_post_iter_curr[s] - stdp_pre_post_iter_prev[s]) > parker_sochacki_error_tolerance)
				goto GO_ON_ITER;		
			if (fabs(stdp_post_pre_iter_curr[s] - stdp_post_pre_iter_prev[s]) > parker_sochacki_error_tolerance)
				goto GO_ON_ITER;	
			if (fabs(eligibility_iter_curr[s] - eligibility_iter_prev[s]) > parker_sochacki_error_tolerance)
				goto GO_ON_ITER;	
		}
		break;	// break iteration since error tolerance achieved 

		GO_ON_ITER:
		v_prev_for_iter = v_curr_for_iter;
		u_prev_for_iter = u_curr_for_iter;
		conductance_excitatory_prev_for_iter = conductance_excitatory_curr_for_iter; 
		conductance_inhibitory_prev_for_iter = conductance_inhibitory_curr_for_iter; 
		chi_prev_for_iter = chi_curr_for_iter;	
		for (s = 0; s < num_of_synapses; s++)
		{
			stdp_pre_post_iter_prev[s] = stdp_pre_post_iter_curr[s];
			stdp_post_pre_iter_prev[s] = stdp_post_pre_iter_curr[s];   
			eligibility_iter_prev[s] = eligibility_iter_curr[s];   
		}	
	}
	iz_params->v = v_curr_for_iter;
	iz_params->u = u_curr_for_iter;
	iz_params->conductance_excitatory = conductance_excitatory_curr_for_iter;
	iz_params->conductance_inhibitory = conductance_inhibitory_curr_for_iter;
	for (s = 0; s < num_of_synapses; s++)
	{
		stdp_pre_post[s] = stdp_pre_post_iter_curr[s];
		stdp_post_pre[s] = stdp_post_pre_iter_curr[s];   
		eligibility[s] = eligibility_iter_curr[s];   
	}	
	p++;
	if (p == parker_sochacki_max_order+1)
		printf("ERROR: No Parker-Sochacki Solution\n");
	return p;
}

double newton_raphson_peak_detection(double v_peak, double *v_pol_vals, int p, double dt)
{
	double dx, dx_old, dv, dt_part, v_curr_for_iter;
	int i, j;

	v_pol_vals[0] = v_pol_vals[0]  - v_peak;      // shift for root finding, the point the curve cuts x-axis
	dt_part = -v_pol_vals[0]/v_pol_vals[1];	//
	dx_old = 100.0;
	
	for(i=0; i < NEWTON_RAPHSON_MAX_ITERATION; i++)
	{
		v_curr_for_iter = v_pol_vals[p] * dt_part + v_pol_vals[p-1];
		dv = v_pol_vals[p];
		for (j = p-2; j >= 0; j--)
		{
			dv = v_curr_for_iter + dv*dt_part;
			v_curr_for_iter = v_pol_vals[j] + v_curr_for_iter * dt_part;
		}
		dx = v_curr_for_iter / dv;
		dt_part -=dx;
		if (fabs(dx) < NEWTON_RAPHSON_ERROR_TOLERANCE)
			break;
		if (fabs(dx+dx_old) < NEWTON_RAPHSON_ERROR_TOLERANCE)	
			break;
		dx_old =dx;	// For oscillation		
	}
	if (i == NEWTON_RAPHSON_MAX_ITERATION)
		printf ("Newton-Raphson error tolerance failure\n");
	if (dt_part > dt)
	{
		printf("Newton-Raphson divergence\n");
		dt_part = dt/2;
	}
	return dt_part;
}

void parker_sochacki_update(Neuron *nrn, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double **stdp_pre_post_pol_vals, double **stdp_post_pre_pol_vals, double **eligibility_pol_vals, double *stdp_pre_post, double *stdp_post_pre, double *eligibility, unsigned int num_of_synapses, double dt, int p)
{
	int i;
	unsigned int s;
	double *u;
	double *conductance_excitatory;
	double *conductance_inhibitory;
	IzNeuronParams	*iz_params;

	iz_params = nrn->iz_params;	
	u = &(iz_params->u);
	conductance_excitatory = &(iz_params->conductance_excitatory);
	conductance_inhibitory = &(iz_params->conductance_inhibitory);	
				// Horner' s method
	*u = u_pol_vals[p] * dt + u_pol_vals[p-1];       
	*conductance_excitatory = conductance_excitatory_pol_vals[p] * dt +  conductance_excitatory_pol_vals[p-1];
	*conductance_inhibitory = conductance_inhibitory_pol_vals[p] * dt +  conductance_inhibitory_pol_vals[p-1];
	for (s = 0; s < num_of_synapses; s++)
	{
		stdp_pre_post[s] = stdp_pre_post_pol_vals[s][p] * dt + stdp_pre_post_pol_vals[s][p-1];
		stdp_post_pre[s] = stdp_post_pre_pol_vals[s][p] * dt + stdp_post_pre_pol_vals[s][p-1];
		eligibility[s] = eligibility_pol_vals[s][p] * dt + eligibility_pol_vals[s][p-1];
	}	
	for (i=p-2; i>=0; i--)
	{
		*u = u_pol_vals[i] + (*u) * dt;
		*conductance_excitatory = conductance_excitatory_pol_vals[i] +  (*conductance_excitatory) * dt;
		*conductance_inhibitory = conductance_inhibitory_pol_vals[i] +  (*conductance_inhibitory) * dt;
		for (s = 0; s < num_of_synapses; s++)
		{
			stdp_pre_post[s] = stdp_pre_post_pol_vals[s][i] + stdp_pre_post[s] * dt;
			stdp_post_pre[s] = stdp_post_pre_pol_vals[s][i] + stdp_post_pre[s] * dt;
			eligibility[s] = eligibility_pol_vals[s][i] + eligibility[s] * dt;
		}	
	}
	return;
}

void clear_parker_sochacki_polynomials(Network *network, int num_of_layers, int *num_of_neuron_groups,  int **num_of_neurons_in_group) /// For debugging, delete when testing complete
{
	int i, j, k, m;
	Layer		*ptr_layer = NULL;
	NeuronGroup	*ptr_neuron_group = NULL;
	Neuron		*ptr_neuron = NULL;
	ParkerSochackiPolynomialVals	*ptr_ps_vals;
	for (i=0; i<network->layer_count; i++)
	{
		ptr_layer = network->layers[i];
		for (j=0; j<ptr_layer->neuron_group_count; j++)
		{
			ptr_neuron_group = ptr_layer->neuron_groups[j];
			for (k=0; k<ptr_neuron_group->neuron_count; k++)
			{
				ptr_neuron = &(ptr_neuron_group->neurons[k]);
				ptr_ps_vals = ptr_neuron->ps_vals;
				for (m = 0; m < parker_sochacki_max_order + 1; m++)
				{
					ptr_ps_vals->v_pol_vals[m] = 0;
					ptr_ps_vals->u_pol_vals[m] = 0;
					ptr_ps_vals->conductance_excitatory_pol_vals[m] = 0;
					ptr_ps_vals->conductance_inhibitory_pol_vals[m] = 0;
					ptr_ps_vals->chi_pol_vals[m] = 0;
				}
			}					
		}
	}	
	return;
}

void destroy_neuron_parker_sochacki_pol_vals(Neuron *neuron)
{
	ParkerSochackiPolynomialVals	*ptr_ps_vals;

	ptr_ps_vals = neuron->ps_vals;

	g_free(ptr_ps_vals->v_pol_vals);			
	g_free(ptr_ps_vals->u_pol_vals);
	g_free(ptr_ps_vals->conductance_excitatory_pol_vals);
	g_free(ptr_ps_vals->conductance_inhibitory_pol_vals);
	g_free(ptr_ps_vals->chi_pol_vals);
	g_free(ptr_ps_vals->E_pol_vals);		
	g_free(ptr_ps_vals->a_pol_vals);
	g_free(ptr_ps_vals->conductance_decay_rate_excitatory_pol_vals);
	g_free(ptr_ps_vals->conductance_decay_rate_inhibitory_pol_vals);
	
	g_free(ptr_ps_vals);
	
	return;
}		

