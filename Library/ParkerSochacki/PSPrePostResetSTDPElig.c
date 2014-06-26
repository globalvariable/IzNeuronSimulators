#include "PSPrePostResetSTDPElig.h"

static bool parker_sochacki_integration(Neuron *nrn, TimeStamp integration_start_time, TimeStamp integration_end_time, bool *spike_generated, TimeStamp *spike_time);

static int parker_sochacki_step (Neuron *nrn, double *v_pol_vals, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double *chi_pol_vals, double *E_pol_vals, double *a_pol_vals, double *conductance_decay_rate_excitatory_pol_vals , double *conductance_decay_rate_inhibitory_pol_vals, double dt);

static void parker_sochacki_update(Neuron *nrn, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double dt, int p);

static void parker_sochacki_step_for_synapses_w_no_spike (Neuron *nrn, double dt);
static void parker_sochacki_step_for_synapses_w_spike (Neuron *nrn, double dt);
static void parker_sochacki_step_for_stdp_pre_post (PsStdpPrePost *ps_stdp_pre_post, double dt);
static void parker_sochacki_step_for_eligibility (PsEligibility *ps_eligibility, double dt);


bool evaluate_neuron_dyn_pre_post_w_resetting_stdp_elig(Neuron *nrn, TimeStamp start_time, TimeStamp end_time, bool *spike_generated, TimeStamp *spike_time)	// Assume that only once a spike can be generated per step (0.25 ms).
{			
	NeuronSortedEventBuffer		*sorted_event_buffer = nrn->sorted_event_buffer;
	NeuronSortedEventBufferItem	*event_item;	
	SynapseIndex			event_from_syn_idx;
	Synapse				*synapses = nrn->syn_list->synapses;
	Synapse				*synapse;
	TimeStamp 			integration_start_ns, event_time;
	unsigned int			*read_idx, write_idx;


	integration_start_ns = start_time;   // required to know it to schedule events for outputs  (parker_sochacki_step_start_time+ dt_part)

	if (! sort_neuron_events(nrn))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "ParkerSochacki", "evaluate_neuron_dyn", "! sort_neuron_events().");

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
			printf ("Simulate: BUG: evaluate_neuron_dyn - Simulate.c\n");
			printf ("Simulate: BUG: event_time is %llu but the step start time is %llu\n", event_time, start_time);	
			printf ("Simulate: BUG: event_time:%llu event_type: %u synapse idx: %u.\n", event_time, event_item->event_type,  event_item->syn_idx);	
			printf ("Simulate: BUG: There must be a problem at event sorting\n");	
			return 0;
		}
//		printf("event: %llu syn: %u weight: %f type: %u\n", event_time, event_item->syn_idx, synapses[event_item->syn_idx].weight, event_item->event_type); 	
		if (event_item->event_type == NEURON_EVENT_TYPE_SYNAPTIC_EVENT)
		{
			if (!parker_sochacki_integration(nrn, integration_start_ns, event_time, spike_generated, spike_time))
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "ParkerSochacki", "evaluate_neuron_dyn", "! parker_sochacki_integration().");
			
			event_from_syn_idx = event_item->syn_idx;
			synapse = &(synapses[event_from_syn_idx]);
			if (synapse->type == EXCITATORY_SYNAPSE)		
				nrn->iz_params->conductance_excitatory += synapse->weight;
			else if (synapse->type == INHIBITORY_SYNAPSE)
				nrn->iz_params->conductance_inhibitory -= synapse->weight;
			else
				return print_message(BUG_MSG ,"IzNeuronSimulators", "ParkerSochacki", "evaluate_neuron_dyn", "Unknown neuron type(excitatory/inhibitory).");
			if (synapse->plastic)
				synapse->ps_stdp_pre_post->now = synapse->ps_stdp_pre_post->change_amount;	// increment pre_post
		}
		else
		{
			return print_message(BUG_MSG ,"IzNeuronSimulators", "ParkerSochacki", "evaluate_neuron_dyn", "Unknown neuron event type.");	
		}
		integration_start_ns = event_time;
		if ((*read_idx + 1) == sorted_event_buffer->buff_size)
			*read_idx = 0;
		else
			(*read_idx)++;
	}
	if (!parker_sochacki_integration(nrn, integration_start_ns, end_time, spike_generated, spike_time))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "ParkerSochacki", "evaluate_neuron_dyn", "! parker_sochacki_integration().");
	return TRUE ;	
}



static bool parker_sochacki_integration(Neuron *nrn, TimeStamp integration_start_time, TimeStamp integration_end_time, bool *spike_generated, TimeStamp *spike_time)
{
	double *v_pol_vals;			// size should be parker_sochacki_max_order + 1
	double *u_pol_vals;
	double *conductance_excitatory_pol_vals;
	double *conductance_inhibitory_pol_vals;
	double *chi_pol_vals;
	double *E_pol_vals;		
	double *a_pol_vals;
	double *conductance_decay_rate_excitatory_pol_vals;
	double *conductance_decay_rate_inhibitory_pol_vals;
	double dt_part;
	double dt;
	int p;

	ParkerSochackiPolynomialVals *polynomial_vals;
	IzNeuronParams	*iz_params;

	*spike_generated = FALSE;	

	iz_params = nrn->iz_params;
	polynomial_vals = nrn->ps_vals;

	v_pol_vals = polynomial_vals->v_pol_vals;			
	u_pol_vals = polynomial_vals->u_pol_vals;
	conductance_excitatory_pol_vals = polynomial_vals->conductance_excitatory_pol_vals;
	conductance_inhibitory_pol_vals = polynomial_vals->conductance_inhibitory_pol_vals;
	chi_pol_vals = polynomial_vals->chi_pol_vals;	
	E_pol_vals = polynomial_vals->E_pol_vals;
	a_pol_vals = polynomial_vals->a_pol_vals;
	conductance_decay_rate_excitatory_pol_vals = polynomial_vals->conductance_decay_rate_excitatory_pol_vals;
	conductance_decay_rate_inhibitory_pol_vals = polynomial_vals->conductance_decay_rate_inhibitory_pol_vals;
	
	v_pol_vals[0] = iz_params->v;	
	u_pol_vals[0] = iz_params->u;
	conductance_excitatory_pol_vals[0] =  iz_params->conductance_excitatory;
	conductance_inhibitory_pol_vals[0] =  iz_params->conductance_inhibitory;
	chi_pol_vals[0] = iz_params->k*iz_params->v - conductance_excitatory_pol_vals[0] - conductance_inhibitory_pol_vals[0] - iz_params->k_v_threshold;
//	(chi = kv - eta - gamma - k*v_t)
	dt = (integration_end_time - integration_start_time)/PARKER_SOCHACKI_EMBEDDED_STEP_SIZE;     // do not change PARKER_SOCHACKI_EMBEDDED_STEP_SIZE
	p=parker_sochacki_step(nrn, v_pol_vals, u_pol_vals, conductance_excitatory_pol_vals, conductance_inhibitory_pol_vals, chi_pol_vals, E_pol_vals, a_pol_vals, conductance_decay_rate_excitatory_pol_vals, conductance_decay_rate_inhibitory_pol_vals, dt);
//	printf("%f\t %d\n ", nrn->v, p);
	if (iz_params->v  > iz_params->v_peak)    // updated nrn->v inside parker_sochacki_step(dt)
	{
		dt_part = newton_raphson_peak_detection(iz_params->v_peak, v_pol_vals, p, dt);
		*spike_generated = TRUE;
		*spike_time = integration_start_time+((TimeStamp)((dt_part*PARKER_SOCHACKI_EMBEDDED_STEP_SIZE)+0.5)); // do not change PARKER_SOCHACKI_EMBEDDED_STEP_SIZE
//		printf("---------------->  Spike time %.15f %llu\n", ((integration_start_time)/PARKER_SOCHACKI_EMBEDDED_STEP_SIZE)+dt_part, *spike_time);		
		if (!schedule_synaptic_event(nrn, *spike_time))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "ParkerSochacki", "parker_sochacki_integration", "! schedule_events().");

		parker_sochacki_update(nrn, u_pol_vals, conductance_excitatory_pol_vals, conductance_inhibitory_pol_vals, dt_part, p);	// integrate until dt_part
		parker_sochacki_step_for_synapses_w_spike (nrn, dt_part);  // integrate until dt_part for synapses

		// reset neuron dynamics after spike generation
		v_pol_vals[0] = iz_params->c;    ///   v = c
		u_pol_vals[0] = iz_params->u + iz_params->d;  ///  u = u + d
		conductance_excitatory_pol_vals[0] =   iz_params->conductance_excitatory;
		conductance_inhibitory_pol_vals[0] =   iz_params->conductance_inhibitory;
		chi_pol_vals[0] = iz_params->k*iz_params->c - conductance_excitatory_pol_vals[0] - conductance_inhibitory_pol_vals[0] - iz_params->k_v_threshold;    //   used c instead of v since both same but v has not been updated
		p=parker_sochacki_step(nrn, v_pol_vals, u_pol_vals, conductance_excitatory_pol_vals, conductance_inhibitory_pol_vals, chi_pol_vals, E_pol_vals, a_pol_vals, conductance_decay_rate_excitatory_pol_vals, conductance_decay_rate_inhibitory_pol_vals, dt-dt_part);	// integrate for remaining dt (dt-dt_part)
		parker_sochacki_step_for_synapses_w_no_spike  (nrn, dt-dt_part);
	}
	else
	{
		parker_sochacki_step_for_synapses_w_no_spike  (nrn, dt);
	}
	return TRUE;
}

static int parker_sochacki_step (Neuron *nrn, double *v_pol_vals, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double *chi_pol_vals, double *E_pol_vals, double *a_pol_vals, double *conductance_decay_rate_excitatory_pol_vals , double *conductance_decay_rate_inhibitory_pol_vals, double dt)
{
	double dt_pow;
	int i,p;
	double v_x_chi;
	double v_prev_for_iter, u_prev_for_iter, conductance_excitatory_prev_for_iter, conductance_inhibitory_prev_for_iter, chi_prev_for_iter;
	double v_curr_for_iter, u_curr_for_iter, conductance_excitatory_curr_for_iter, conductance_inhibitory_curr_for_iter, chi_curr_for_iter;	
	double b, k, E_excitatory, E_inhibitory;
	IzNeuronParams	*iz_params;

	iz_params = nrn->iz_params;	
	b = iz_params->b;
	k = iz_params->k;	
	E_excitatory = iz_params->E_excitatory;
	E_inhibitory =  iz_params->E_inhibitory;
	
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
		for (i=1; i<p; i++)
		{
			v_x_chi += v_pol_vals[i] * chi_pol_vals[p-i];
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
	*/	if (fabs(v_curr_for_iter - v_prev_for_iter) <= parker_sochacki_error_tolerance)
		{
			if (fabs(u_curr_for_iter - u_prev_for_iter) <= parker_sochacki_error_tolerance)
			{
				if (fabs(conductance_excitatory_curr_for_iter - conductance_excitatory_prev_for_iter) <= parker_sochacki_error_tolerance)
				{
					if (fabs(conductance_inhibitory_curr_for_iter - conductance_inhibitory_prev_for_iter) <= parker_sochacki_error_tolerance)
					{
						break;
					}
				}
			}
		}
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
	p++;
	if (p == parker_sochacki_max_order+1)
		printf("ERROR: No Parker-Sochacki Solution\n");
	return p;
}

static void parker_sochacki_update(Neuron *nrn, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double dt, int p)
{
	int i;
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
	for (i=p-2; i>=0; i--)
	{
		*u = u_pol_vals[i] + (*u) * dt;
		*conductance_excitatory = conductance_excitatory_pol_vals[i] +  (*conductance_excitatory) * dt;
		*conductance_inhibitory = conductance_inhibitory_pol_vals[i] +  (*conductance_inhibitory) * dt;
	}
	return;
}

static void parker_sochacki_step_for_synapses_w_no_spike (Neuron *nrn, double dt)
{
	unsigned int	i, num_of_synapses;
	Synapse		*synapses, *synapse;

	synapses = nrn->syn_list->synapses;
	num_of_synapses = nrn->syn_list->num_of_synapses;

	for (i = 0; i < num_of_synapses; i++)
	{
		if (synapses[i].plastic)
		{
			synapse = &(synapses[i]);
			parker_sochacki_step_for_stdp_pre_post (synapse->ps_stdp_pre_post, dt);
			parker_sochacki_step_for_eligibility (synapse->ps_eligibility, dt);
		}
	}
	return;
}

static void parker_sochacki_step_for_synapses_w_spike (Neuron *nrn, double dt)
{
	unsigned int	i, num_of_synapses;
	Synapse		*synapses, *synapse;

	synapses = nrn->syn_list->synapses;
	num_of_synapses = nrn->syn_list->num_of_synapses;

	for (i = 0; i < num_of_synapses; i++)
	{
		if (synapses[i].plastic)
		{
			synapse = &(synapses[i]);
			parker_sochacki_step_for_stdp_pre_post (synapse->ps_stdp_pre_post, dt);
			parker_sochacki_step_for_eligibility (synapse->ps_eligibility, dt);

			if (synapse->ps_stdp_pre_post->now > (exp(-1.0)*synapse->ps_stdp_pre_post->change_amount))
				synapse->ps_eligibility->now = synapse->ps_eligibility->max_eligibility;
		}
	}
	return;
}


static void parker_sochacki_step_for_stdp_pre_post (PsStdpPrePost *ps_stdp_pre_post, double dt)
{
	double		now; 					
	double 		iter_prev;			
	double 		iter_curr;			
	double 		*ps_pol_vals;			
	double		*decay_rate_ps_pol_vals;	

	double dt_pow;

	int p;

	now = ps_stdp_pre_post->now; 			
	iter_prev = ps_stdp_pre_post->iter_prev;
	iter_curr = ps_stdp_pre_post->iter_curr; 			
	ps_pol_vals = ps_stdp_pre_post->ps_pol_vals; 			
	decay_rate_ps_pol_vals = ps_stdp_pre_post->decay_rate_ps_pol_vals; 	

	// begin integration
	ps_pol_vals[0] = ps_stdp_pre_post->now;

	ps_pol_vals[1] = decay_rate_ps_pol_vals[0] * ps_pol_vals[0];

	iter_prev = ps_pol_vals[0] + ps_pol_vals[1] * dt;
	iter_curr = iter_prev;

	dt_pow= dt*dt;

	for (p=1;p < (parker_sochacki_max_order-1); p++)
	{
		ps_pol_vals[p+1] = decay_rate_ps_pol_vals[p] * ps_pol_vals[p];  
		iter_curr = iter_prev +ps_pol_vals[p+1] * dt_pow;
		dt_pow *= dt;
		if (fabs(iter_curr - iter_prev) > parker_sochacki_error_tolerance)
		{
			iter_prev = iter_curr;
		}
		else
		{
			break;
		}
	}

	ps_stdp_pre_post->now = iter_curr;		// end of integration

	if (p == parker_sochacki_max_order)
		printf("ERROR: No Parker-Sochacki Solution for synapse STDP\n");   

}

static void parker_sochacki_step_for_eligibility (PsEligibility *ps_eligibility, double dt)
{
	double		now; 					
	double 		iter_prev;			
	double 		iter_curr;			
	double 		*ps_pol_vals;			
	double		*decay_rate_ps_pol_vals;	

	double dt_pow;

	int p;

	now = ps_eligibility->now; 			
	iter_prev = ps_eligibility->iter_prev;
	iter_curr = ps_eligibility->iter_curr; 			
	ps_pol_vals = ps_eligibility->ps_pol_vals; 			
	decay_rate_ps_pol_vals = ps_eligibility->decay_rate_ps_pol_vals; 	

	// begin integration
	ps_pol_vals[0] = ps_eligibility->now;

	ps_pol_vals[1] = decay_rate_ps_pol_vals[0] * ps_pol_vals[0];

	iter_prev = ps_pol_vals[0] + ps_pol_vals[1] * dt;
	iter_curr = iter_prev;

	dt_pow= dt*dt;

	for (p=1;p < (parker_sochacki_max_order-1); p++)
	{
		ps_pol_vals[p+1] = decay_rate_ps_pol_vals[p] * ps_pol_vals[p];  
		iter_curr = iter_prev +ps_pol_vals[p+1] * dt_pow;
		dt_pow *= dt;
		if (fabs(iter_curr - iter_prev) > parker_sochacki_error_tolerance)
		{
			iter_prev = iter_curr;
		}
		else
		{
			break;
		}
	}

	ps_eligibility->now = iter_curr;		// end of integration

	if (p == parker_sochacki_max_order)
		printf("ERROR: No Parker-Sochacki Solution for synapse STDP\n");   

}
