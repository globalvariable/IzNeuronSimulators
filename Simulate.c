 #include "Simulate.h"

void simulate_parker_sochacki(void)
{
	int i, j, k;
	GPtrArray *ptrArrayLayer, *ptrArrayNeuronGroup; 
	neuron *nrn;
	for (i=0; i<ptrArrayAllNetwork->len; i++)
	{	
		ptrArrayLayer = g_ptr_array_index(ptrArrayAllNetwork,i);	
		for (j=0; j<ptrArrayLayer->len; j++)
		{
			ptrArrayNeuronGroup = g_ptr_array_index(ptrArrayLayer,j);
			for (k=0; k<ptrArrayNeuronGroup->len; k++)
			{
				nrn = g_ptr_array_index(ptrArrayNeuronGroup, k);
				parker_sochacki_evaluate_neuron_dyn(nrn);
				if (neuron_dynamics_logging_on)
					log_neuron_dynamics(nrn);
			}
		}
	}
	return;
}




