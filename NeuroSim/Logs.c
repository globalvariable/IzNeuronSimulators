 #include "Logs.h"


void log_neuron_dynamics(neuron *nrn)
{
	int write_idx = nrn->dynamics_log_buffer_write_idx;
	nrn->dynamics_log[V_LOG][write_idx] = nrn->v;  // significant error during type casting. Use double for good precision in logging. For visualization no big problem.
	nrn->dynamics_log[U_LOG][write_idx] = nrn->u;
	nrn->dynamics_log[CONDUCTANCE_EXCITATORY_LOG][write_idx] = nrn->conductance_excitatory;
	nrn->dynamics_log[CONDUCTANCE_INHIBITORY_LOG][write_idx] = nrn->conductance_inhibitory;
//	printf("%.20f\t %f\t %d\n", nrn->v, nrn->u, write_idx);
	nrn->dynamics_log_buffer_write_idx++;
	if (nrn->dynamics_log_buffer_write_idx == neuron_dynamics_log_buffer_size)
		nrn->dynamics_log_buffer_write_idx = 0;
	return;
}

void set_buffer_size_for_neuron_dynamics_logging(int buffer_size)
{
	neuron_dynamics_log_buffer_size = buffer_size;
	return;
}

void turn_neuron_dynamics_logging_on(void)
{
	neuron_dynamics_logging_on = 1;
	return;
}

void turn_neuron_dynamics_logging_off(void)
{
	neuron_dynamics_logging_on = 0;
	return;
}

int get_buffer_size_for_neuron_dynamics_logging(void)
{
	return neuron_dynamics_log_buffer_size;
}

void clear_neuron_dynamics_log(neuron *nrn)
{
	int i, j;
	for (i=0;i<MAX_NUM_OF_VALUE_PER_NEURON_TO_LOG; i++)
	{
		for (j=0;j<neuron_dynamics_log_buffer_size; j++)
		{
			nrn->dynamics_log[i][j] = 0;
		}
	}  
	nrn->dynamics_log_buffer_write_idx = 0;
	return;
}
