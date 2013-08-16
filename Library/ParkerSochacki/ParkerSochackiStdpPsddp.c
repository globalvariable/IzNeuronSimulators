#include "ParkerSochackiStdpPsddp.h"

// PS INTEGRATION WITH STDP & ELIGIBILITY & DEPOLARIZATION BASED ELIGIBILITY (WITHOUT DEPENDENCY TO MEMBRANE VOLTAGE)

//stdp: spike timing_dependent plasticity
//psddp: post synaptic depolarization dependent plasticity 

static bool parker_sochacki_integration(Neuron *nrn, TimeStamp integration_start_time, TimeStamp integration_end_time, bool *spike_generated, TimeStamp *spike_time, double *v_pol_vals, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double *chi_pol_vals, double *E_pol_vals, double *a_pol_vals, double *conductance_decay_rate_excitatory_pol_vals , double *conductance_decay_rate_inhibitory_pol_vals, double **stdp_pre_post_pol_vals, double **stdp_post_pre_pol_vals, double **eligibility_pol_vals, double **stdp_pre_post_decay_rate_pol_vals, double **stdp_post_pre_decay_rate_pol_vals, double **eligibility_decay_rate_pol_vals, SynapseIndex num_of_synapses, double *stdp_pre_post, double *stdp_post_pre, double *eligibility, IzNeuronParams *iz_params, double max_eligibility, double eligibility_rate);

static int parker_sochacki_step(double *v_pol_vals, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double *chi_pol_vals, double *E_pol_vals, double *a_pol_vals, double *conductance_decay_rate_excitatory_pol_vals , double *conductance_decay_rate_inhibitory_pol_vals, double dt, IzNeuronParams *iz_params);

static void parker_sochacki_update(double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double dt, int p, IzNeuronParams *iz_params);

static void parker_sochacki_step_stdp (double **arg_stdp_pre_post_pol_vals, double **arg_stdp_post_pre_pol_vals, double **arg_stdp_pre_post_decay_rate_pol_vals, double **arg_stdp_post_pre_decay_rate_pol_vals, SynapseIndex num_of_synapses, double dt, double *stdp_pre_post, double *stdp_post_pre);

static void parker_sochacki_step_psddp_elig (double **arg_eligibility_pol_vals, double **arg_eligibility_decay_rate_pol_vals, SynapseIndex num_of_synapses, double dt, double *eligibility);

bool evaluate_neuron_dyn_stdp_psddp_elig(Neuron *nrn, TimeStamp start_time, TimeStamp end_time, bool *spike_generated, TimeStamp *spike_time)	// Assume that only once a spike can be generated per step (0.25 ms).
{
	ParkerSochackiPolynomialVals	*ps_vals = nrn->ps_vals;
	IzNeuronParams				*iz_params = nrn->iz_params;
	Synapse						*synapses = nrn->syn_list->synapses;
	Synapse						*synapse;
	STDPList					*stdp_list = nrn->stdp_list;
	EligibilityList					*eligibility_list = nrn->eligibility_list;
	NeuronSortedEventBuffer		*sorted_event_buffer = nrn->sorted_event_buffer;
	NeuronSortedEventBufferItem	*event_item;		
	SynapseIndex					num_of_synapses =  nrn->syn_list->num_of_synapses;	
	SynapseIndex					event_from_syn_idx;

	double *v_pol_vals = ps_vals->v_pol_vals;			
	double *u_pol_vals = ps_vals->u_pol_vals;
	double *conductance_excitatory_pol_vals = ps_vals->conductance_excitatory_pol_vals;
	double *conductance_inhibitory_pol_vals = ps_vals->conductance_inhibitory_pol_vals;
	double *chi_pol_vals = ps_vals->chi_pol_vals;
	double *E_pol_vals = ps_vals->E_pol_vals;		
	double *a_pol_vals = ps_vals->a_pol_vals;
	double *conductance_decay_rate_excitatory_pol_vals = ps_vals->conductance_decay_rate_excitatory_pol_vals;
	double *conductance_decay_rate_inhibitory_pol_vals = ps_vals->conductance_decay_rate_inhibitory_pol_vals;

	double	*stdp_pre_post = stdp_list->stdp_pre_post; 					
	double	*change_stdp_pre_post = stdp_list->change_stdp_pre_post;		
	double	**stdp_pre_post_pol_vals = stdp_list->stdp_pre_post_pol_vals;			
	double	**stdp_pre_post_decay_rate_pol_vals = stdp_list->stdp_pre_post_decay_rate_pol_vals;	

	double	*stdp_post_pre = stdp_list->stdp_post_pre;					
//	double	*change_stdp_post_pre = stdp_list->change_stdp_post_pre;		//  it is used when required just after v_peak is exceed in  (parker_sochacki_integration)
	double	**stdp_post_pre_pol_vals = stdp_list->stdp_post_pre_pol_vals;			
	double	**stdp_post_pre_decay_rate_pol_vals = stdp_list->stdp_post_pre_decay_rate_pol_vals;	

	double	*eligibility = eligibility_list->eligibility;					
	double	**eligibility_pol_vals = eligibility_list->eligibility_pol_vals;			
	double	**eligibility_decay_rate_pol_vals = eligibility_list->eligibility_decay_rate_pol_vals;

	double max_eligibility = eligibility_list->max_eligibility;
	double eligibility_rate = eligibility_list->eligibility_rate;
	
	double	eligibility_val;

	TimeStamp 			integration_start_ns, event_time;
	unsigned int			*read_idx, write_idx;

	integration_start_ns = start_time;   // required to know it to schedule events for outputs  (parker_sochacki_step_start_time+ dt_part)

	if (! sort_neuron_events(nrn))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "ParkerSochacki", "evaluate_neuron_dyn_stdp_elig", "! sort_neuron_events().");

	read_idx = &(sorted_event_buffer->read_idx);
	write_idx = sorted_event_buffer->write_idx;
	while ((*read_idx) != write_idx)		
	{
		event_item = &(sorted_event_buffer->buff[*read_idx]);
		event_time = event_item->time;
		if (event_time >= end_time)	// Do not handle if it is at start of next step or later than that
			break;
		if (event_time < integration_start_ns)	// this function handles sorted synaptic events. the events are sorted during buffering
		{
			printf ("Simulate: BUG: evaluate_neuron_dyn_stdp_psddp_elig - Simulate.c\n");
			printf ("Simulate: BUG: event_time is %llu but the step start time is %llu\n", event_time, start_time);	
			printf ("Simulate: BUG: event_time:%llu event_type: %u synapse idx: %u.\n", event_time, event_item->event_type,  event_item->syn_idx);		
			printf ("Simulate: BUG: There must be a problem at event sorting or task run time has been larger than task period\n");	
			if ((*read_idx + 1) == sorted_event_buffer->buff_size)
				*read_idx = 0;
			else
				(*read_idx)++;
			if (event_item->event_type == NEURON_EVENT_TYPE_TRIAL_START_EVENT)
			{
				clear_eligibility_for_neuron(nrn);
			}
			else if (event_item->event_type == NEURON_EVENT_TYPE_TRIAL_END_WITH_REWARD)
			{
				save_eligibility_for_neuron(nrn);	
			}
			else if (event_item->event_type == NEURON_EVENT_TYPE_TRIAL_END_WITH_PUNISHMENT)
			{
				save_eligibility_for_neuron(nrn);	
			}
			else
			{
				return print_message(BUG_MSG ,"IzNeuronSimulators", "ParkerSochacki", "evaluate_neuron_dyn_stdp_elig", "Unknown neuron event type.");				
			}
			continue;
		}
//		printf("event: %llu syn: %u weight: %f type: %u\n", event_time, event_item->syn_idx, synapses[event_item->syn_idx].weight, event_item->event_type); 	

		if (! parker_sochacki_integration(nrn, integration_start_ns, event_time, spike_generated, spike_time, v_pol_vals, u_pol_vals, conductance_excitatory_pol_vals, conductance_inhibitory_pol_vals, chi_pol_vals, E_pol_vals, a_pol_vals, conductance_decay_rate_excitatory_pol_vals, conductance_decay_rate_inhibitory_pol_vals, stdp_pre_post_pol_vals, stdp_post_pre_pol_vals, eligibility_pol_vals, stdp_pre_post_decay_rate_pol_vals, stdp_post_pre_decay_rate_pol_vals, eligibility_decay_rate_pol_vals, num_of_synapses, stdp_pre_post, stdp_post_pre, eligibility, iz_params, max_eligibility, eligibility_rate))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "ParkerSochacki", "evaluate_neuron_dyn_stdp_elig", "! parker_sochacki_integration_stdp_psddp_elig().");
		if (event_item->event_type == NEURON_EVENT_TYPE_SYNAPTIC_EVENT)
		{
			event_from_syn_idx = event_item->syn_idx;
			synapse = &(synapses[event_from_syn_idx]);
			if (synapse->type == EXCITATORY_SYNAPSE)
			{		
				iz_params->conductance_excitatory += synapse->weight;
				stdp_pre_post[event_from_syn_idx] += change_stdp_pre_post[event_from_syn_idx];	// increment pre_post
				eligibility_val = eligibility[event_from_syn_idx];
				if (eligibility_val >= 0)
				{
					eligibility_val = eligibility_val + (((max_eligibility - eligibility_val) / max_eligibility)*stdp_post_pre[event_from_syn_idx]*eligibility_rate); /// it is already negative. update eligibility according to post pre event
				}
				else
				{
					eligibility_val = eligibility_val + (((max_eligibility + eligibility_val) / max_eligibility)*stdp_post_pre[event_from_syn_idx]*eligibility_rate); /// it is already negative. update eligibility according to post pre event
				}
				if (eligibility_val >= 0)
				{
		//			if (nrn->iz_params->v > eligibility_list->depol_eligibility_memb_volt[event_from_syn_idx]) 
		//			{
						eligibility_val = eligibility_val - (((max_eligibility - eligibility_val) / max_eligibility)*eligibility_list->depol_eligibility_change[event_from_syn_idx]*eligibility_rate);
		//			}
				}
				else
				{
		//			if (nrn->iz_params->v > eligibility_list->depol_eligibility_memb_volt[event_from_syn_idx]) 
		//			{
						eligibility_val = eligibility_val - (((max_eligibility + eligibility_val) / max_eligibility)*eligibility_list->depol_eligibility_change[event_from_syn_idx]*eligibility_rate);
		//			}
				}
				eligibility[event_from_syn_idx] = eligibility_val; 
			}
			else if (synapse->type == INHIBITORY_SYNAPSE)
			{
				iz_params->conductance_inhibitory -= synapse->weight;  
				// CURRENTLY NO DEVELOPMENT FOR INHIBITORY PLASTICITY
/*				stdp_pre_post[event_from_syn_idx] += change_stdp_pre_post[event_from_syn_idx];	// increment pre_post // curentlly no development for inhibitory neuron plasticity. it is here just to be compatible with excitatory.
				eligibility[event_from_syn_idx] += stdp_post_pre[event_from_syn_idx];  /// it is already negative. update eligibility according to post pre event   //				stdp_pre_post[event_from_syn_idx] += change_stdp_pre_post[event_from_syn_idx];	// increment pre_post
				eligibility[event_from_syn_idx] += stdp_post_pre[event_from_syn_idx];  /// it is already negative. update eligibility according to post pre event
				eligibility[event_from_syn_idx] +=  eligibility_list->depol_eligibility_change[event_from_syn_idx];   // curentlly no development for inhibitory neuron plasticity. it is here just to be compatible with excitatory.
*/			}
			else
				return print_message(BUG_MSG ,"IzNeuronSimulators", "ParkerSochacki", "evaluate_neuron_dyn_stdp_elig", "Unknown neuron type(excitatory/inhibitory).");

		}
		else if (event_item->event_type == NEURON_EVENT_TYPE_TRIAL_START_EVENT)
		{
			clear_eligibility_for_neuron(nrn);
		}
		else if (event_item->event_type == NEURON_EVENT_TYPE_TRIAL_END_WITH_REWARD)
		{
			save_eligibility_for_neuron(nrn);	
		}
		else if (event_item->event_type == NEURON_EVENT_TYPE_TRIAL_END_WITH_PUNISHMENT)
		{
			save_eligibility_for_neuron(nrn);	
		}
		else
		{
			return print_message(BUG_MSG ,"IzNeuronSimulators", "ParkerSochacki", "evaluate_neuron_dyn_stdp_elig", "Unknown neuron event type.");				
		}		
		integration_start_ns = event_time;
		if ((*read_idx + 1) == sorted_event_buffer->buff_size)
			*read_idx = 0;
		else
			(*read_idx)++;
	}
	if (! parker_sochacki_integration(nrn, integration_start_ns, end_time, spike_generated, spike_time, v_pol_vals, u_pol_vals, conductance_excitatory_pol_vals, conductance_inhibitory_pol_vals, chi_pol_vals, E_pol_vals, a_pol_vals, conductance_decay_rate_excitatory_pol_vals, conductance_decay_rate_inhibitory_pol_vals, stdp_pre_post_pol_vals, stdp_post_pre_pol_vals, eligibility_pol_vals, stdp_pre_post_decay_rate_pol_vals, stdp_post_pre_decay_rate_pol_vals, eligibility_decay_rate_pol_vals, num_of_synapses, stdp_pre_post, stdp_post_pre, eligibility, iz_params, max_eligibility, eligibility_rate))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "ParkerSochacki", "evaluate_neuron_dyn_stdp_elig", "! parker_sochacki_integration_stdp_psddp_elig().");
	return TRUE ;	
}



static bool parker_sochacki_integration(Neuron *nrn, TimeStamp integration_start_time, TimeStamp integration_end_time, bool *spike_generated, TimeStamp *spike_time, double *v_pol_vals, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double *chi_pol_vals, double *E_pol_vals, double *a_pol_vals, double *conductance_decay_rate_excitatory_pol_vals , double *conductance_decay_rate_inhibitory_pol_vals, double **stdp_pre_post_pol_vals, double **stdp_post_pre_pol_vals, double **eligibility_pol_vals, double **stdp_pre_post_decay_rate_pol_vals, double **stdp_post_pre_decay_rate_pol_vals, double **eligibility_decay_rate_pol_vals, SynapseIndex num_of_synapses, double *stdp_pre_post, double *stdp_post_pre, double *eligibility, IzNeuronParams *iz_params, double max_eligibility, double eligibility_rate)
{

	*spike_generated = FALSE;	
	double dt_part, dt;
	int p;
	unsigned int s;
	double	*change_stdp_post_pre;	
	double	eligibility_val;

//	(chi = kv - eta - gamma - k*v_t)
	dt = (integration_end_time - integration_start_time)/PARKER_SOCHACKI_EMBEDDED_STEP_SIZE;     // do not change PARKER_SOCHACKI_EMBEDDED_STEP_SIZE
	p = parker_sochacki_step(v_pol_vals, u_pol_vals, conductance_excitatory_pol_vals, conductance_inhibitory_pol_vals, chi_pol_vals, E_pol_vals, a_pol_vals, conductance_decay_rate_excitatory_pol_vals, conductance_decay_rate_inhibitory_pol_vals, dt, iz_params);
//	printf("%f\t %d\n ", nrn->v, p);
	if (iz_params->v  > iz_params->v_peak)    // updated nrn->v inside parker_sochacki_step(dt)
	{
		dt_part = newton_raphson_peak_detection(iz_params->v_peak, v_pol_vals, p, dt);
		*spike_generated = TRUE;
		*spike_time = integration_start_time+((TimeStamp)((dt_part*PARKER_SOCHACKI_EMBEDDED_STEP_SIZE)+0.5)); // do not change PARKER_SOCHACKI_EMBEDDED_STEP_SIZE
//		printf("---------------->  Spike time %.15f %llu\n", ((integration_start_time)/PARKER_SOCHACKI_EMBEDDED_STEP_SIZE)+dt_part, *spike_time);		
		if (!schedule_synaptic_event(nrn, *spike_time))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "ParkerSochacki", "parker_sochacki_integration", "! schedule_events().");

		parker_sochacki_update(u_pol_vals, conductance_excitatory_pol_vals, conductance_inhibitory_pol_vals, dt_part, p, iz_params);   

		parker_sochacki_step_stdp (stdp_pre_post_pol_vals, stdp_post_pre_pol_vals, stdp_pre_post_decay_rate_pol_vals, stdp_post_pre_decay_rate_pol_vals, num_of_synapses, dt_part, stdp_pre_post, stdp_post_pre);
		parker_sochacki_step_psddp_elig (eligibility_pol_vals, eligibility_decay_rate_pol_vals, num_of_synapses, dt_part, eligibility);

		change_stdp_post_pre = nrn->stdp_list->change_stdp_post_pre;
		for (s = 0; s < num_of_synapses; s++)	// update eligibility & post_pre STDP
		{
			stdp_post_pre[s] += change_stdp_post_pre[s];	// it is already negative. 
			eligibility_val = eligibility[s];
 			if (eligibility_val >= 0)
			{
				eligibility_val = eligibility_val + (((max_eligibility - eligibility_val) / max_eligibility)*stdp_pre_post[s]*eligibility_rate); /// it is already negative. update eligibility according to post pre event
			}
			else
			{
				eligibility_val = eligibility_val + (((max_eligibility + eligibility_val) / max_eligibility)*stdp_pre_post[s]*eligibility_rate); /// it is already negative. update eligibility according to post pre event
			}
			eligibility[s] = eligibility_val;
		}
		iz_params->v = iz_params->c;    ///   v = c
		iz_params->u = iz_params->u + iz_params->d;  ///  u = u + d
		p = parker_sochacki_step(v_pol_vals, u_pol_vals, conductance_excitatory_pol_vals, conductance_inhibitory_pol_vals, chi_pol_vals, E_pol_vals, a_pol_vals, conductance_decay_rate_excitatory_pol_vals, conductance_decay_rate_inhibitory_pol_vals, dt-dt_part, iz_params);
		parker_sochacki_step_stdp (stdp_pre_post_pol_vals, stdp_post_pre_pol_vals, stdp_pre_post_decay_rate_pol_vals, stdp_post_pre_decay_rate_pol_vals, num_of_synapses, dt-dt_part, stdp_pre_post, stdp_post_pre);
		parker_sochacki_step_psddp_elig (eligibility_pol_vals, eligibility_decay_rate_pol_vals, num_of_synapses, dt-dt_part, eligibility);
	}
	else
	{
		parker_sochacki_step_stdp (stdp_pre_post_pol_vals, stdp_post_pre_pol_vals, stdp_pre_post_decay_rate_pol_vals, stdp_post_pre_decay_rate_pol_vals, num_of_synapses, dt, stdp_pre_post, stdp_post_pre);
		parker_sochacki_step_psddp_elig (eligibility_pol_vals, eligibility_decay_rate_pol_vals, num_of_synapses, dt, eligibility);
	}
	return TRUE;
}

static int parker_sochacki_step(double *v_pol_vals, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double *chi_pol_vals, double *E_pol_vals, double *a_pol_vals, double *conductance_decay_rate_excitatory_pol_vals , double *conductance_decay_rate_inhibitory_pol_vals, double dt, IzNeuronParams *iz_params)
{
	double dt_pow;
	int m, p;
	double v_x_chi;
	double v_prev_for_iter, u_prev_for_iter, conductance_excitatory_prev_for_iter, conductance_inhibitory_prev_for_iter, chi_prev_for_iter;
	double v_curr_for_iter, u_curr_for_iter, conductance_excitatory_curr_for_iter, conductance_inhibitory_curr_for_iter, chi_curr_for_iter;	
	double b, k, E_excitatory, E_inhibitory;

	b = iz_params->b;
	k = iz_params->k;	
	E_excitatory = iz_params->E_excitatory;
	E_inhibitory =  iz_params->E_inhibitory;

	v_pol_vals[0] = iz_params->v;	
	u_pol_vals[0] = iz_params->u;
	conductance_excitatory_pol_vals[0] =  iz_params->conductance_excitatory;
	conductance_inhibitory_pol_vals[0] =  iz_params->conductance_inhibitory;
	chi_pol_vals[0] = k*iz_params->v - conductance_excitatory_pol_vals[0] - conductance_inhibitory_pol_vals[0] - iz_params->k_v_threshold;
	
	v_pol_vals[1] =iz_params->E*( v_pol_vals[0] * chi_pol_vals[0] + E_excitatory * conductance_excitatory_pol_vals[0] + E_inhibitory * conductance_inhibitory_pol_vals[0]  - u_pol_vals[0]+ iz_params->I_inject);  // eqtn 27
	u_pol_vals[1] = iz_params->a * (b * v_pol_vals[0] - u_pol_vals[0]);
	conductance_excitatory_pol_vals[1] = iz_params->decay_rate_excitatory * conductance_excitatory_pol_vals[0];  // decay_rate_excitatory already negatized during initialization
	conductance_inhibitory_pol_vals[1] = iz_params->decay_rate_inhibitory * conductance_inhibitory_pol_vals[0];   //decay_rate_inhibitory already negatized during initialization
	chi_pol_vals[1] = k * v_pol_vals[1] - conductance_excitatory_pol_vals[1] - conductance_inhibitory_pol_vals[1];

	v_prev_for_iter = v_pol_vals[0] + v_pol_vals[1] * dt;
	u_prev_for_iter = u_pol_vals[0] + u_pol_vals[1] * dt;
	conductance_excitatory_prev_for_iter = conductance_excitatory_pol_vals[0] + conductance_excitatory_pol_vals[1] * dt;
	conductance_inhibitory_prev_for_iter = conductance_inhibitory_pol_vals[0] + conductance_inhibitory_pol_vals[1] * dt;
	chi_prev_for_iter = chi_pol_vals[0] + chi_pol_vals[1] * dt;

	v_curr_for_iter = v_prev_for_iter;
	u_curr_for_iter = v_prev_for_iter;
	conductance_excitatory_curr_for_iter = conductance_excitatory_prev_for_iter;
	conductance_inhibitory_curr_for_iter = conductance_inhibitory_prev_for_iter;
	chi_curr_for_iter = chi_prev_for_iter; 

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

		v_curr_for_iter = v_prev_for_iter + v_pol_vals[p+1] * dt_pow;
		u_curr_for_iter = u_prev_for_iter  + u_pol_vals[p+1] * dt_pow;
		conductance_excitatory_curr_for_iter = conductance_excitatory_prev_for_iter + conductance_excitatory_pol_vals[p+1] * dt_pow;
		conductance_inhibitory_curr_for_iter = conductance_inhibitory_prev_for_iter  + conductance_inhibitory_pol_vals[p+1] * dt_pow;
		chi_curr_for_iter = chi_prev_for_iter + chi_pol_vals[p+1] * dt_pow;

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

		break;	// break iteration since error tolerance achieved 

		GO_ON_ITER:
		v_prev_for_iter = v_curr_for_iter;
		u_prev_for_iter = u_curr_for_iter;
		conductance_excitatory_prev_for_iter = conductance_excitatory_curr_for_iter; 
		conductance_inhibitory_prev_for_iter = conductance_inhibitory_curr_for_iter; 
		chi_prev_for_iter = chi_curr_for_iter;	
	}
	iz_params->v = v_curr_for_iter;
	iz_params->u = u_curr_for_iter;
	iz_params->conductance_excitatory = conductance_excitatory_curr_for_iter;
	iz_params->conductance_inhibitory = conductance_inhibitory_curr_for_iter;

	if (p == parker_sochacki_max_order)
		printf("ERROR: No Parker-Sochacki Solution\n");
	return p;
}

static void parker_sochacki_update(double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double dt, int p, IzNeuronParams *iz_params)
{
	int i;
	double *u;
	double *conductance_excitatory;
	double *conductance_inhibitory;

	u = &(iz_params->u);
	conductance_excitatory = &(iz_params->conductance_excitatory);
	conductance_inhibitory = &(iz_params->conductance_inhibitory);	
				// Horner' s method
	*u = u_pol_vals[p] * dt + u_pol_vals[p-1];       
	*conductance_excitatory = conductance_excitatory_pol_vals[p] * dt +  conductance_excitatory_pol_vals[p-1];
	*conductance_inhibitory = conductance_inhibitory_pol_vals[p] * dt +  conductance_inhibitory_pol_vals[p-1];

	for (i=p-2; i>=0; i--)
	{
		*u = u_pol_vals[i] + (*u) * dt;
		*conductance_excitatory = conductance_excitatory_pol_vals[i] +  (*conductance_excitatory) * dt;
		*conductance_inhibitory = conductance_inhibitory_pol_vals[i] +  (*conductance_inhibitory) * dt;
	}
	return;
}

static void parker_sochacki_step_stdp (double **arg_stdp_pre_post_pol_vals, double **arg_stdp_post_pre_pol_vals, double **arg_stdp_pre_post_decay_rate_pol_vals, double **arg_stdp_post_pre_decay_rate_pol_vals, SynapseIndex num_of_synapses, double dt, double *stdp_pre_post, double *stdp_post_pre)
{
	double dt_pow;
	int p, s;
	double stdp_pre_post_iter_curr, stdp_pre_post_iter_prev;
	double stdp_post_pre_iter_curr, stdp_post_pre_iter_prev;
	double *stdp_pre_post_pol_vals, *stdp_post_pre_pol_vals, *stdp_pre_post_decay_rate_pol_vals, *stdp_post_pre_decay_rate_pol_vals;


	for (s = 0; s < num_of_synapses; s++)
	{
		stdp_pre_post_pol_vals = arg_stdp_pre_post_pol_vals[s];
		stdp_post_pre_pol_vals = arg_stdp_post_pre_pol_vals[s];
		stdp_pre_post_decay_rate_pol_vals = arg_stdp_pre_post_decay_rate_pol_vals[s];
		stdp_post_pre_decay_rate_pol_vals = arg_stdp_post_pre_decay_rate_pol_vals[s];

		stdp_pre_post_pol_vals[0] = stdp_pre_post[s];
		stdp_post_pre_pol_vals[0] = stdp_post_pre[s];

		stdp_pre_post_pol_vals[1] = stdp_pre_post_decay_rate_pol_vals[0] * stdp_pre_post_pol_vals[0];   // stdp_pre_post_decay_rate_pol_vals[i][0] = decay_rate_stdp_pre_post[i]
 		stdp_post_pre_pol_vals[1] = stdp_post_pre_decay_rate_pol_vals[0] * stdp_post_pre_pol_vals[0];	

		stdp_pre_post_iter_prev = stdp_pre_post_pol_vals[0] + stdp_pre_post_pol_vals[1] * dt;   
		stdp_post_pre_iter_prev = stdp_post_pre_pol_vals[0] + stdp_post_pre_pol_vals[1] * dt;   

		stdp_pre_post_iter_curr = stdp_pre_post_iter_prev;
		stdp_post_pre_iter_curr = stdp_post_pre_iter_prev;   

		dt_pow= dt*dt;

		for (p=1;p < (parker_sochacki_max_order-1); p++)
		{

			stdp_pre_post_pol_vals[p+1] = stdp_pre_post_decay_rate_pol_vals[p] * stdp_pre_post_pol_vals[p];   
			stdp_post_pre_pol_vals[p+1] = stdp_post_pre_decay_rate_pol_vals[p] * stdp_post_pre_pol_vals[p];   

			stdp_pre_post_iter_curr = stdp_pre_post_iter_prev + stdp_pre_post_pol_vals[p+1] * dt_pow;
			stdp_post_pre_iter_curr = stdp_post_pre_iter_prev + stdp_post_pre_pol_vals[p+1] * dt_pow;   

			dt_pow *= dt;

			if (fabs(stdp_pre_post_iter_curr - stdp_pre_post_iter_prev) > parker_sochacki_error_tolerance)
				goto GO_ON_ITER;		
			if (fabs(stdp_post_pre_iter_curr - stdp_post_pre_iter_prev) > parker_sochacki_error_tolerance)
				goto GO_ON_ITER;	

			break;	// break iteration since error tolerance achieved 

			GO_ON_ITER:

			stdp_pre_post_iter_prev = stdp_pre_post_iter_curr;
			stdp_post_pre_iter_prev = stdp_post_pre_iter_curr;   

		}

		stdp_pre_post[s] = stdp_pre_post_iter_curr;
		stdp_post_pre[s] = stdp_post_pre_iter_curr;   

		if (p == parker_sochacki_max_order)
			printf("ERROR: No Parker-Sochacki Solution\n");   
	}	


	return;
}

static void parker_sochacki_step_psddp_elig (double **arg_eligibility_pol_vals, double **arg_eligibility_decay_rate_pol_vals, SynapseIndex num_of_synapses, double dt, double *eligibility)
{
	double dt_pow;
	int p, s;

	double *eligibility_pol_vals, *eligibility_decay_rate_pol_vals;
	double eligibility_iter_curr, eligibility_iter_prev;

	for (s = 0; s < num_of_synapses; s++)
	{
		eligibility_pol_vals = arg_eligibility_pol_vals[s];
		eligibility_decay_rate_pol_vals = arg_eligibility_decay_rate_pol_vals[s];

		eligibility_pol_vals[0] = eligibility[s];

		eligibility_pol_vals[1] = eligibility_decay_rate_pol_vals[0] * eligibility_pol_vals[0];  

		eligibility_iter_prev = eligibility_pol_vals[0] + eligibility_pol_vals[1] * dt;   

		eligibility_iter_curr = eligibility_iter_prev; 

		dt_pow= dt*dt;

		for (p=1;p < (parker_sochacki_max_order-1); p++)
		{
			eligibility_pol_vals[p+1] = eligibility_decay_rate_pol_vals[p] * eligibility_pol_vals[p];   
	
			eligibility_iter_curr = eligibility_iter_prev + eligibility_pol_vals[p+1] * dt_pow;   

			dt_pow *= dt;

			if (fabs(eligibility_iter_curr - eligibility_iter_prev) > parker_sochacki_error_tolerance)
				goto GO_ON_ITER;
	
			break;	// break iteration since error tolerance achieved 

			GO_ON_ITER:

			eligibility_iter_prev = eligibility_iter_curr;   
		}	

		eligibility[s] = eligibility_iter_curr;

		if (p == parker_sochacki_max_order)
			printf("ERROR: No Parker-Sochacki Solution\n");   
	}

	return;
}
