 #include "ParkerSochacki.h"



static bool allocate_and_initialize_parker_sochacki_pol_vals_for_all_neurons(Network *network);

bool parker_sochacki_set_order_tolerance(Network *network, unsigned int max_ps_order, double ps_error_tolerance)
{
	char temp[300];
	if (ps_error_tolerance < 0)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "ParkerSochacki", "parker_sochacki_set_order_tolerance", "ps_error_tolerance < 0");

	parker_sochacki_max_order = max_ps_order;
	parker_sochacki_error_tolerance = ps_error_tolerance;
	
	sprintf (temp, "PS Max Order: %u, PS Err.Tol: %.15f,  NR Max. Iter: %u NR Err. Tol: %.15f.", parker_sochacki_max_order, parker_sochacki_error_tolerance, NEWTON_RAPHSON_MAX_ITERATION, NEWTON_RAPHSON_ERROR_TOLERANCE);
	print_message(INFO_MSG ,"IzNeuronSimulators", "ParkerSochacki", "parker_sochacki_set_order_tolerance", temp);	

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
				
				if (ptr_ps_vals == NULL)	// it is not a neuron evaluated via PS method. maybe it is a poisson neuron.
					continue;
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
	if ((dt_part > dt) || (dt_part < 0))
	{
		printf("Newton-Raphson divergence\n");
		dt_part = dt/2;
	}
	return dt_part;
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



