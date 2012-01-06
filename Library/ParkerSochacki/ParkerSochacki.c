 #include "ParkerSochacki.h"

static int parker_sochacki_max_order = 0;
static double parker_sochacki_error_tolerance = 0;

static bool allocate_and_initialize_parker_sochacki_pol_vals_for_all_neurons(void);

bool parker_sochacki_set_order_tolerance(int max_ps_order, double ps_error_tolerance)
{
	if ((max_ps_order < 0) || (ps_error_tolerance < 0))
	{
		printf("ERROR: ParkerSochachki: Invalid computation adjustment.\n");
		printf("ERROR: ParkerSochachki: It should be such that:\nMin Parker-Sochacki Order Limit is 0\nMin Parker-Sochacki Error Tolerance is 0\n");
		return FALSE;
	}

	parker_sochacki_max_order = max_ps_order;
	parker_sochacki_error_tolerance = ps_error_tolerance;

	if (allocate_and_initialize_parker_sochacki_pol_vals_for_all_neurons())
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

static bool allocate_and_initialize_parker_sochacki_pol_vals_for_all_neurons(void)
{
	int i, j, k, m;
	Layer		*ptr_layer = NULL;
	NeuronGroup	*ptr_neuron_group = NULL;
	Neuron		*ptr_neuron = NULL;
	ParkerSochackiPolynomialVals	*ptr_ps_vals;
	
	if (!is_network_allocated())
		return FALSE;
	for (i=0; i<all_network->layer_count; i++)
	{
		ptr_layer = all_network->layers[i];
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
					ptr_ps_vals->E_pol_vals[m] = ptr_neuron->E/(m +1);
					ptr_ps_vals->a_pol_vals[m] = ptr_neuron->a/(m+1);
					ptr_ps_vals->conductance_decay_rate_excitatory_pol_vals[m] = ptr_neuron->decay_rate_excitatory/(m+1);
					ptr_ps_vals->conductance_decay_rate_inhibitory_pol_vals[m] = ptr_neuron->decay_rate_inhibitory/(m+1);	
				}
			}					
		}
	}	
	return TRUE;
}

TimeStamp evaluate_neuron_dyn(Neuron *nrn, TimeStamp start_time, TimeStamp end_time)
{

	TimeStamp 		integration_start_ns;

	NeuronEventBuffer	*neuron_event_buffer;
	
	TimeStamp		*event_times;			
//	Neuron 			**event_from;		no need to handle this to evaluate neuron dynamics	
	SynapticWeight		*event_weights;			
	int 				event_buff_size;			
	int				*ptr_event_buffer_write_idx;  		
	int				*ptr_event_buffer_read_idx; 		

	int				idx, end_idx;	
	TimeStamp		event_time;	
	TimeStamp		spike_time;	
	
	neuron_event_buffer = nrn->event_buff;
	event_times = neuron_event_buffer->time;
	event_weights = neuron_event_buffer->weight;
	event_buff_size = neuron_event_buffer->buff_size;
	ptr_event_buffer_write_idx = &(neuron_event_buffer->write_idx);
	ptr_event_buffer_read_idx = &(neuron_event_buffer->read_idx);
			
	integration_start_ns = start_time;   // required to know it to schedule events for outputs  (parker_sochacki_step_start_time+ dt_part)
	
	idx = *ptr_event_buffer_read_idx;
	end_idx = *ptr_event_buffer_write_idx;

	while (idx != end_idx)		// There is a check for event buffer overflow in schedule_events()
	{
		event_time = event_times[idx];
		if (event_time >= end_time)	// Do not handle if it is at start of next step or later than that
			break;
		if (event_time < start_time)	// this function handles sorted synaptic events. the events are sorted during buffering
		{
			printf ("Simulate: BUG: evaluate_neuron_dyn - Simulate.c\n");
			printf ("Simulate: BUG: event_time is %llu but the step start time is %llu\n", event_time, start_time);		
			printf ("Simulate: BUG: There must be a problem at event sorting\n");	
			return 0;
		}
		spike_time = parker_sochacki_integration(nrn, integration_start_ns, event_time);
		if (event_weights[idx] > 0)
			nrn->conductance_excitatory += event_weights[idx];
		else if (event_weights[idx] < 0)
			nrn->conductance_inhibitory -= event_weights[idx];	
		else
			printf ("BUG: evaluate_neuron_dyn - Simulate.c  -----> Weight cannot be zero\n");	
		integration_start_ns = event_time;
		idx++;
		if (idx == event_buff_size)
			idx = 0;
	}
	spike_time = parker_sochacki_integration(nrn, integration_start_ns, end_time);
	*ptr_event_buffer_read_idx = end_idx;			// Finally set event_buffer_read_idx,	event_buffer_write_idx will be set by  insert_synaptic_event at the end of function so that no corruption will appear even with multithreading
	return spike_time ;	// Assume that only once a spike can be generated per step (0.25 ms).
}



TimeStamp parker_sochacki_integration(Neuron *nrn, TimeStamp integration_start_time, TimeStamp integration_end_time)
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
	TimeStamp		spike_time = MAX_TIME_STAMP;	
	double dt_part;
	double dt;
	int p;
	
	ParkerSochackiPolynomialVals *polynomial_vals;

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
	
	v_pol_vals[0] = nrn->v;	
	u_pol_vals[0] = nrn->u;
	conductance_excitatory_pol_vals[0] =  nrn->conductance_excitatory;
	conductance_inhibitory_pol_vals[0] =  nrn->conductance_inhibitory;
	chi_pol_vals[0] = nrn->k*nrn->v - conductance_excitatory_pol_vals[0] - conductance_inhibitory_pol_vals[0] - nrn->k_v_threshold;
//	(chi = kv - eta - gamma - k*v_t)
	dt = (integration_end_time - integration_start_time)/PARKER_SOCHACKI_EMBEDDED_STEP_SIZE;     // for 1ms step size
	p=parker_sochacki_step(nrn, v_pol_vals, u_pol_vals, conductance_excitatory_pol_vals, conductance_inhibitory_pol_vals, chi_pol_vals, E_pol_vals, a_pol_vals, conductance_decay_rate_excitatory_pol_vals, conductance_decay_rate_inhibitory_pol_vals, dt);
//	printf("%f\t %d\n ", nrn->v, p);
	if (nrn->v  > nrn->v_peak)    // updated nrn->v inside parker_sochacki_step(dt)
	{
		dt_part = newton_raphson_peak_detection(nrn->v_peak, v_pol_vals, p, dt);
		spike_time = integration_start_time+((TimeStamp)((dt_part*1000000)+0.5));
		printf("---------------->  Spike time%.15f\n", ((integration_start_time)/1000000.0)+dt_part);		
		if (!schedule_events(nrn, dt_part, integration_start_time))
			return 0;

		parker_sochacki_update(nrn, u_pol_vals, conductance_excitatory_pol_vals, conductance_inhibitory_pol_vals, dt_part, p);
		v_pol_vals[0] = nrn->c;    ///   v = c
		u_pol_vals[0] = nrn->u + nrn->d;  ///  u = u + d
		conductance_excitatory_pol_vals[0] =   nrn->conductance_excitatory;
		conductance_inhibitory_pol_vals[0] =   nrn->conductance_inhibitory;
		chi_pol_vals[0] = nrn->k*nrn->c - conductance_excitatory_pol_vals[0] - conductance_inhibitory_pol_vals[0] - nrn->k_v_threshold;    //   used c instead of v since both same but v has not been updated
		p=parker_sochacki_step(nrn, v_pol_vals, u_pol_vals, conductance_excitatory_pol_vals, conductance_inhibitory_pol_vals, chi_pol_vals, E_pol_vals, a_pol_vals, conductance_decay_rate_excitatory_pol_vals, conductance_decay_rate_inhibitory_pol_vals, dt-dt_part);
	}
	return spike_time;
}

int parker_sochacki_step (Neuron *nrn, double *v_pol_vals, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double *chi_pol_vals, double *E_pol_vals, double *a_pol_vals, double *conductance_decay_rate_excitatory_pol_vals , double *conductance_decay_rate_inhibitory_pol_vals, double dt)
{
	double dt_pow;
	int i,p;
	double v_x_chi;
	double v_prev_for_iter, u_prev_for_iter, conductance_excitatory_prev_for_iter, conductance_inhibitory_prev_for_iter, chi_prev_for_iter;
	double v_curr_for_iter, u_curr_for_iter, conductance_excitatory_curr_for_iter, conductance_inhibitory_curr_for_iter, chi_curr_for_iter;	
	double b, k, E_excitatory, E_inhibitory;
	
	b = nrn->b;
	k = nrn->k;	
	E_excitatory = nrn->E_excitatory;
	E_inhibitory =  nrn->E_inhibitory;
	
	v_pol_vals[1] =nrn->E*( v_pol_vals[0] * chi_pol_vals[0] + E_excitatory * conductance_excitatory_pol_vals[0] + E_inhibitory * conductance_inhibitory_pol_vals[0]  - u_pol_vals[0]+ nrn->I_inject);  // eqtn 27
	u_pol_vals[1] = nrn->a * (b * v_pol_vals[0] - u_pol_vals[0]);
	conductance_excitatory_pol_vals[1] = nrn->decay_rate_excitatory * conductance_excitatory_pol_vals[0];  // decay_rate_excitatory already negatized during initialization
	conductance_inhibitory_pol_vals[1] = nrn->decay_rate_inhibitory * conductance_inhibitory_pol_vals[0];   //decay_rate_inhibitory already negatized during initialization
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
	nrn->v = v_curr_for_iter;
	nrn->u = u_curr_for_iter;
	nrn->conductance_excitatory = conductance_excitatory_curr_for_iter;
	nrn->conductance_inhibitory = conductance_inhibitory_curr_for_iter;
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

int parker_sochacki_update(Neuron *nrn, double *u_pol_vals, double *conductance_excitatory_pol_vals, double *conductance_inhibitory_pol_vals, double dt, int p)
{
	int i;
	double *u;
	double *conductance_excitatory;
	double *conductance_inhibitory;
	
	u = &(nrn->u);
	conductance_excitatory = &(nrn->conductance_excitatory);
	conductance_inhibitory = &(nrn->conductance_inhibitory);	
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
	return 0;
}

void clear_parker_sochacki_polynomials(int num_of_layers, int *num_of_neuron_groups,  int **num_of_neurons_in_group) /// For debugging, delete when testing complete
{
	int i, j, k, m;
	Layer		*ptr_layer = NULL;
	NeuronGroup	*ptr_neuron_group = NULL;
	Neuron		*ptr_neuron = NULL;
	ParkerSochackiPolynomialVals	*ptr_ps_vals;
	for (i=0; i<all_network->layer_count; i++)
	{
		ptr_layer = all_network->layers[i];
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

