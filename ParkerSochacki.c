 #include "ParkerSochacki.h"

static neuron *nrn;

static double v;
static double u;
static double a;
static double b;
static double c;
static double d;
static double k;
static double E;	/// 1/C    elastance
static double conductance_excitatory;
static double E_excitatory;
static double decay_rate_excitatory;
static double conductance_inhibitory;	
static double E_inhibitory;
static double decay_rate_inhibitory;
static double v_threshold;
static double v_peak;	// maximum membrane voltage
//static double v_resting;     //unnecessary
static double I_inject;
//static bool inhibitory;     //unnecessary
static double k_v_threshold;
	// Axons	
static neuron **axon_to;
static unsigned int *axonal_delay;
static double *axonal_weight;
static int num_of_output;

	// Sorted Event Buffer
static unsigned int *event_time;
static neuron **event_from;   // interrogate ptrArrayEventFrom->len to reach buffer size from anywhere
static double *event_weight;
static int event_buffer_write_idx;
static int num_of_event;	

static int n_event_inside_step;
static int n_event_outside_step;
static int event_start_idx;

static unsigned int event_time_outside_step[EVENT_BUFF_SIZE_PER_NEURON];
static neuron *event_from_outside_step[EVENT_BUFF_SIZE_PER_NEURON]; 
static double event_weight_outside_step[EVENT_BUFF_SIZE_PER_NEURON];

static unsigned int event_time_inside_step[EVENT_BUFF_SIZE_PER_NEURON];
static neuron *event_from_inside_step[EVENT_BUFF_SIZE_PER_NEURON]; 
static double event_weight_inside_step[EVENT_BUFF_SIZE_PER_NEURON];

static double v_prev_for_iter;
static double u_prev_for_iter;
static double conductance_excitatory_prev_for_iter;
static double conductance_inhibitory_prev_for_iter;
static double chi_prev_for_iter;

static double v_curr_for_iter;
static double u_curr_for_iter;
static double conductance_excitatory_curr_for_iter;
static double conductance_inhibitory_curr_for_iter;
static double chi_curr_for_iter;

static double v_pol_vals[MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON+1];
static double u_pol_vals[MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON+1];
static double conductance_excitatory_pol_vals[MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON+1];
static double conductance_inhibitory_pol_vals[MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON+1];
static double chi_pol_vals[MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON+1];

static double *E_pol_vals;
static double *a_pol_vals;
static double *conductance_decay_rate_excitatory_pol_vals;
static double *conductance_decay_rate_inhibitory_pol_vals;


bool parker_sochacki_set_order_tolerance(int arg_max_ps_order, double arg_ps_error_tolerance, int arg_max_nr_iter,  double arg_nr_error_tolerance)
{
	if (arg_max_ps_order > MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON)
	{
		printf("ERROR: Maximum order for Parker-Sochacki Solution is %d\n", MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON);
		return FALSE;
	}

	parker_sochacki_max_order = arg_max_ps_order;
	parker_sochacki_error_tolerance = arg_ps_error_tolerance;

	newton_raphson_max_iter = arg_max_nr_iter;
	newton_raphson_error_tolerance = arg_nr_error_tolerance;
	return TRUE;
}

int parker_sochacki_evaluate_neuron_dyn(neuron *arg_nrn)   
{
/*	unsigned int a,b, c;
	a = 1;	
	b = 4.294967295 * 1000000000;
	c = a-b;      ///    result: c=2  */

	int i, idx;
	parker_sochacki_init_params(arg_nrn);
	parker_sochacki_sort_synaptic_events();
	parker_sochacki_integ_start_time = parker_sochacki_time_prev;   // required to know it to schedule events for outputs  (parker_sochacki_step_start_time+ dt_part)
	if (n_event_inside_step>0)
	{
		idx = event_start_idx;
		parker_sochacki_integration((event_time[idx] - parker_sochacki_integ_start_time)/1000000.0);
		if (event_weight[idx]>0)
			arg_nrn->conductance_excitatory += event_weight[idx];
		else if (event_weight[idx]<0)
			arg_nrn->conductance_inhibitory -= event_weight[idx];
		else
			printf ("BUG: parker_sochacki_evaluate_neuron_dyn - ParkerSochacki.c  -----> Weight cannot be zero\n");
//		printf("integ event_time[idx] : %u\n", event_time[idx]);
		parker_sochacki_integ_start_time = event_time[idx];

		for (i = 1; i<n_event_inside_step; i++)
		{	
			if ((event_start_idx + i) < EVENT_BUFF_SIZE_PER_NEURON)
			{
				idx = event_start_idx +i ;
				parker_sochacki_integration( (event_time[idx] - parker_sochacki_integ_start_time)/1000000.0 );
				if (event_weight[idx]>0)
					arg_nrn->conductance_excitatory += event_weight[idx];
				else if (event_weight[idx]<0)
					arg_nrn->conductance_inhibitory -= event_weight[idx];
				else
					printf ("BUG: parker_sochacki_evaluate_neuron_dyn - ParkerSochacki.c  -----> Weight cannot be zero\n");

				parker_sochacki_integ_start_time = event_time[idx];
//				printf("integ event_time[idx] : %u\n", event_time[idx]);
			}
			else
			{
				idx = event_start_idx +i - EVENT_BUFF_SIZE_PER_NEURON;
				parker_sochacki_integration( (event_time[idx] - parker_sochacki_integ_start_time)/1000000.0 );
				if (event_weight[idx]>0)
					arg_nrn->conductance_excitatory += event_weight[idx];
				else if (event_weight[idx]<0)
					arg_nrn->conductance_inhibitory -= event_weight[idx];
				else
					printf ("BUG: parker_sochacki_evaluate_neuron_dyn - ParkerSochacki.c  -----> Weight cannot be zero\n");

				parker_sochacki_integ_start_time = event_time[idx];
//				printf("integ event_time[idx] : %u\n", event_time[idx]);
			}	
		}
		parker_sochacki_integration((parker_sochacki_time_curr - parker_sochacki_integ_start_time) / 1000000.0);
	}
	else
	{
		parker_sochacki_integration((parker_sochacki_time_curr - parker_sochacki_time_prev)/1000000.0);
	}
	return 0;
}

int parker_sochacki_init_params(neuron *arg_nrn)    // ----> scope of arg_nrn is this function. if you use nrn, it will suppress static neuron *nrn all the time in this function. Then it would be impossible to access static nrn. 
{
	nrn = arg_nrn;

	v=nrn->v;
	u=nrn->u;
	a=nrn->a;
	b=nrn->b;
	c=nrn->c;
	d=nrn->d;
	k=nrn->k;
	E=nrn->E;	/// 1/C    elastance
	conductance_excitatory=nrn->conductance_excitatory;
	E_excitatory=nrn->E_excitatory;
	decay_rate_excitatory=nrn->decay_rate_excitatory;
	conductance_inhibitory=nrn->conductance_inhibitory;	
	E_inhibitory=nrn->E_inhibitory;
	decay_rate_inhibitory =nrn->decay_rate_inhibitory;
	v_threshold=nrn->v_threshold;
	v_peak=nrn->v_peak;	// maximum membrane voltage
//	v_resting=nrn->v_resting;               unnecessary
	I_inject=nrn->I_inject;
//	inhibitory=nrn->inhibitory;              unnecessary
	k_v_threshold=nrn->k_v_threshold;
	// Axons	
	axon_to=nrn->axon_to;
	axonal_delay=nrn->axonal_delay;
	axonal_weight=nrn->axonal_weight;
	num_of_output=nrn->num_of_output;
	// Sorted Event Buffer
	event_time=nrn->event_time;
	event_from=nrn->event_from;   // interrogate ptrArrayEventFrom->len to reach buffer size from anywhere
	event_weight=nrn->event_weight;
	event_buffer_write_idx=nrn->event_buffer_write_idx;
	num_of_event=nrn->num_of_event;	

	E_pol_vals = nrn->E_pol_vals;
	a_pol_vals = nrn->a_pol_vals;
	conductance_decay_rate_excitatory_pol_vals = nrn->conductance_decay_rate_excitatory_pol_vals;
	conductance_decay_rate_inhibitory_pol_vals = nrn->conductance_decay_rate_inhibitory_pol_vals;

	n_event_inside_step = 0; 
	n_event_outside_step = 0;
	return 0;
}

int parker_sochacki_sort_synaptic_events(void)
{
	int idx;

	if (num_of_event == 0)
		return 0;
	// There is a check for event buffer overflow in schedule_events()
	if (event_buffer_write_idx < num_of_event)
		event_start_idx = EVENT_BUFF_SIZE_PER_NEURON + (event_buffer_write_idx - num_of_event);
	else
		event_start_idx = event_buffer_write_idx - num_of_event;
	
	idx = event_start_idx;
//	printf("time_prev: %u time_curr: %u\n ", parker_sochacki_time_prev, parker_sochacki_time_curr);
//	printf("event_start_idx: %d\n", event_start_idx);
	while (idx != event_buffer_write_idx)
	{
//		printf("unclassed event_time[idx] : %u %f\n", event_time[idx], event_weight[idx]);
		if (  
			( 
				(parker_sochacki_time_prev < parker_sochacki_time_curr) && (parker_sochacki_time_prev <= event_time[idx] ) && (event_time[idx] < parker_sochacki_time_curr ) 
			)    
		||
			( 
				( parker_sochacki_time_prev > parker_sochacki_time_curr) && 
				( 
					(parker_sochacki_time_prev <= event_time[idx] ) || (event_time[idx] < parker_sochacki_time_curr )
				) 
			) 
		)
		{
			event_time_inside_step[n_event_inside_step] = event_time[idx]-parker_sochacki_time_prev;
			event_from_inside_step[n_event_inside_step] = event_from[idx]; 
			event_weight_inside_step[n_event_inside_step] = event_weight[idx];
			n_event_inside_step++;
			if (n_event_inside_step >= MAX_BUFF_FOR_EVENT_SORTING)
			{
				n_event_inside_step--;
				printf ("ERROR:Local buffer size for event sorting for events inside step exceeded\n");
			}		
		}
		else 
		{
			event_time_outside_step[n_event_outside_step] = event_time[idx];
			event_from_outside_step[n_event_outside_step] = event_from[idx]; 
			event_weight_outside_step[n_event_outside_step] = event_weight[idx];
			n_event_outside_step++;	
			if (n_event_outside_step >= MAX_BUFF_FOR_EVENT_SORTING)
			{
				n_event_outside_step--;
				printf ("ERROR:Local buffer size for event sorting for events outside step exceeded\n");
			}			
		}		
		idx++;
		if (idx == EVENT_BUFF_SIZE_PER_NEURON)
			idx = 0;
	} 
	
	int i,j; 		///  insertion sorting
	unsigned int temp_t; 
	neuron * temp_from; 
	double temp_w;
	for(i=1; i<n_event_inside_step; i++)
	{
		temp_t = event_time_inside_step[ i ];
		temp_from = event_from_inside_step[ i ];
		temp_w = event_weight_inside_step[ i ];
		j = i - 1;
		while( temp_t < event_time_inside_step[ j ] && j >=0 )
		{
			event_time_inside_step[ j+1] = event_time_inside_step[ j ];
			event_from_inside_step[ j+1] = event_from_inside_step[ j ];
			event_weight_inside_step[ j+1] = event_weight_inside_step[ j ];
			--j;
		}
		event_time_inside_step[ j+1] = temp_t;
		event_from_inside_step[ j+1] = temp_from;
		event_weight_inside_step[ j+1] = temp_w;		
	}

	idx = event_start_idx; 

	for(i=0; i<n_event_inside_step; i++)
	{
		event_time[idx] = event_time_inside_step[i]+parker_sochacki_time_prev;
		event_from[idx] = event_from_inside_step[i]; 
		event_weight[idx] = event_weight_inside_step[i];
//		printf("inside event_time[idx] : %u %f %u\n", event_time[idx], event_weight[idx], (unsigned int)event_from[idx]);
		idx++;
		if (idx == EVENT_BUFF_SIZE_PER_NEURON)
			idx = 0;

	}
	for(i=0; i<n_event_outside_step; i++)   ///  forward these events to be handled in next step
	{
		event_time[idx] = event_time_outside_step[i];  
		event_from[idx] = event_from_outside_step[i]; 
		event_weight[idx] = event_weight_outside_step[i];
//		printf("outside event_time[idx] : %u %f %u\n", event_time[idx], event_weight[idx], (unsigned int)event_from[idx]);
		idx++;
		if (idx == EVENT_BUFF_SIZE_PER_NEURON)
			idx = 0;

	}
	nrn->num_of_event = n_event_outside_step;
//	printf ("%u %d\n", (unsigned int)nrn, nrn->num_of_event);
	return 0;
}



int parker_sochacki_integration(double  dt)
{
	double dt_part;
	int p;
	v = nrn->v;
	u = nrn->u;
	conductance_excitatory = nrn->conductance_excitatory;
	conductance_inhibitory = nrn->conductance_inhibitory;
	v_pol_vals[0] = v;	// init' d with parker_sochacki_init_params(nrn)
	u_pol_vals[0] = u;
	conductance_excitatory_pol_vals[0] =  conductance_excitatory;
	conductance_inhibitory_pol_vals[0] =  conductance_inhibitory;
	chi_pol_vals[0] = k*v - conductance_excitatory - conductance_inhibitory - k_v_threshold;
//	(chi = kv - eta - gamma - k*v_t)

	p=parker_sochacki_step(dt);
//	printf("%f\t %d\n ", nrn->v, p);
	if (nrn->v  > v_peak)    // updated nrn->v inside parker_sochacki_step(dt)
	{
		dt_part = newton_raphson_peak_detection(p, dt);
		schedule_events(dt_part);

		parker_sochacki_update(dt_part, p);
		
		v_pol_vals[0] = c;    ///   v = c
		u_pol_vals[0] = u + d;  ///  u = u + d
		conductance_excitatory_pol_vals[0] =  conductance_excitatory;
		conductance_inhibitory_pol_vals[0] =  conductance_inhibitory;
		chi_pol_vals[0] = k*c - conductance_excitatory - conductance_inhibitory - k_v_threshold;    //   used c instead of v since both same but v has not been updated
		p=parker_sochacki_step(dt-dt_part);
	}
	return 0;
}

int parker_sochacki_step(double dt)
{
	double dt_pow;
	int i,p;
	double v_x_chi;

	v_pol_vals[1] =E*( v_pol_vals[0] * chi_pol_vals[0] + E_excitatory * conductance_excitatory_pol_vals[0] +  E_inhibitory * conductance_inhibitory_pol_vals[0]  - u_pol_vals[0]+ I_inject);  // eqtn 27
	u_pol_vals[1] = a * (b*v_pol_vals[0] - u_pol_vals[0]);
	conductance_excitatory_pol_vals[1] = decay_rate_excitatory * conductance_excitatory_pol_vals[0];  // decay_rate_excitatory already negatized during initialization
	conductance_inhibitory_pol_vals[1] = decay_rate_inhibitory * conductance_inhibitory_pol_vals[0];   //decay_rate_inhibitory already negatized during initialization
	chi_pol_vals[1] = k * v_pol_vals[1] - conductance_excitatory_pol_vals[1] - conductance_inhibitory_pol_vals[1];

	v_prev_for_iter = v_pol_vals[0] + v_pol_vals[1] * dt;
	u_prev_for_iter = u_pol_vals[0] + u_pol_vals[1] * dt;
	conductance_excitatory_prev_for_iter = conductance_excitatory_pol_vals[0] + conductance_excitatory_pol_vals[1] * dt;
	conductance_inhibitory_prev_for_iter = conductance_inhibitory_pol_vals[0] + conductance_inhibitory_pol_vals[1] * dt;
	chi_prev_for_iter = chi_pol_vals[0] + chi_pol_vals[1] * dt;
	
	dt_pow= dt*dt;
//	printf("\n");
	for (p=1;p < (parker_sochacki_max_order - 1); p++)
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
	*/	if (fabs(v_curr_for_iter - v_prev_for_iter) < parker_sochacki_error_tolerance)
		{
			if (fabs(u_curr_for_iter - u_prev_for_iter) < parker_sochacki_error_tolerance)
			{
				if (fabs(conductance_excitatory_curr_for_iter - conductance_excitatory_prev_for_iter) < parker_sochacki_error_tolerance)
				{
					if (fabs(conductance_inhibitory_curr_for_iter - conductance_inhibitory_prev_for_iter) < parker_sochacki_error_tolerance)
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
	if (p == parker_sochacki_max_order)
		printf("ERROR: No Parker-Sochacki Solution\n");
	return p;
}

double newton_raphson_peak_detection(int p, double dt)
{
	double dx, dx_old, dv, dt_part;
	int i, j;

	v_pol_vals[0] = v_pol_vals[0]  - v_peak;      // shift for root finding, the point the curve cuts x-axis
	dt_part = -v_pol_vals[0]/v_pol_vals[1];	//
	dx_old = 100.0;

	for(i=0; i < newton_raphson_max_iter; i++)
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
		if (fabs(dx) < newton_raphson_error_tolerance)
			break;
		if (fabs(dx+dx_old) < newton_raphson_error_tolerance)	
			break;
		dx_old =dx;	// For oscillation		
	}
	if (i == newton_raphson_max_iter)
		printf ("Newton-Raphson error tolerance failure\n");
	if (dt_part > dt)
	{
		printf("Newton-Raphson divergence\n");
		dt_part = dt/2;
	}
//	printf("---------------->  Spike time%.15f\n", ((parker_sochacki_integ_start_time)/1000000.0)+dt_part);
	return dt_part;
}

int parker_sochacki_update(double dt, int p)
{
	int i;
				// Horner' s method
	u = u_pol_vals[p] * dt + u_pol_vals[p-1];       
	conductance_excitatory = conductance_excitatory_pol_vals[p] * dt +  conductance_excitatory_pol_vals[p-1];
	conductance_inhibitory = conductance_inhibitory_pol_vals[p] * dt +  conductance_inhibitory_pol_vals[p-1];
	for (i=p-2; i>=0; i--)
	{
		u = u_pol_vals[i] + u * dt;
		conductance_excitatory = conductance_excitatory_pol_vals[i] +  conductance_excitatory * dt;
		conductance_inhibitory = conductance_inhibitory_pol_vals[i] +  conductance_inhibitory * dt;
	}
	return 0;
}

int schedule_events(double dt)
{
	int i, write_idx;
	neuron * post_syn_nrn;
	for (i=0; i < num_of_output; i++)
	{
		post_syn_nrn = axon_to[i];
		if (post_syn_nrn->event_buffer_write_idx > EVENT_BUFF_SIZE_PER_NEURON) //for bug checking in runtime. Remove if there is no bug in tests.
		{
			post_syn_nrn->event_buffer_write_idx = 0;
			printf ("BUG: in schedule_events(), ParkerSochacki.c\n");
		}
		write_idx = post_syn_nrn->event_buffer_write_idx;
		post_syn_nrn->event_time[write_idx] = parker_sochacki_integ_start_time + ((unsigned int) (dt*1000000.0)) + axonal_delay[i];
		post_syn_nrn->event_from[write_idx] = nrn;
		post_syn_nrn->event_weight[write_idx] = axonal_weight[i];
		post_syn_nrn->event_buffer_write_idx++;
		post_syn_nrn->num_of_event++;
		if (post_syn_nrn->num_of_event == EVENT_BUFF_SIZE_PER_NEURON)
		{
			printf("*************WARNING: Buffer for synaptic events is full!!!*******************\n");
			printf("Neuron address: %u\n", (unsigned int) post_syn_nrn);
			printf("Event Time: %u will be overwritten\n ", post_syn_nrn->event_time[write_idx]);
			post_syn_nrn->num_of_event--; /// Discard last event.
			post_syn_nrn->event_buffer_write_idx--;
		}
		if (post_syn_nrn->event_buffer_write_idx== EVENT_BUFF_SIZE_PER_NEURON) 
			post_syn_nrn->event_buffer_write_idx = 0;
	}
	return 0;
}

void clear_parker_sochacki_polynomials(void) /// For debugging, delete when testing complete
{
	int i = 0;
	v_prev_for_iter = 0;
	u_prev_for_iter = 0;
	conductance_excitatory_prev_for_iter = 0;
	conductance_inhibitory_prev_for_iter = 0;
	chi_prev_for_iter = 0;

	v_curr_for_iter = 0;
	u_curr_for_iter = 0;
	conductance_excitatory_curr_for_iter = 0;
	conductance_inhibitory_curr_for_iter = 0;
	chi_curr_for_iter = 0;


	for (i=0; i<MAX_PARKER_SOCHACKI_ALLOCATION_PER_NEURON+1; i++)
	{
		v_pol_vals[i] = 0;
		u_pol_vals[i] = 0;
		conductance_excitatory_pol_vals[i] = 0;
		conductance_inhibitory_pol_vals[i] = 0;
		chi_pol_vals[i] = 0;
	}

	E_pol_vals = NULL;
	a_pol_vals = NULL;
	conductance_decay_rate_excitatory_pol_vals = NULL;
	conductance_decay_rate_inhibitory_pol_vals = NULL;
	return;
}

