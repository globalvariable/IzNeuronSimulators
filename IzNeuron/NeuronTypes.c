#include "NeuronTypes.h"


void get_neuron_type_parameters(int neuron_type, double *v, double *a, double *b, double *c, double *d, double *k, double *C, double *v_resting, double *v_threshold, double *v_peak, bool *inhibitory, double *E_excitatory, double *E_inhibitory, double *tau_excitatory, double *tau_inhibitory)
{
	if (neuron_type == REGULAR_SPIKING) 
	{
		*v = -60;
		*a = 0.03;
		*b = -2;
		*c = -50;
		*d = 100;
		*C = 100;
		*k = 0.7;
		*v_resting = -60;
		*v_threshold = -40;
		*v_peak = 35;
		*inhibitory = 0;
		*E_excitatory = -60;
		*E_inhibitory = -60;
		*tau_excitatory = 5;
		*tau_inhibitory = 10;
	}
	else if (neuron_type == INTRINSICALLY_BURSTING)  
	{
		*v = -75;	
		*a =0.01;
		*b = 5;
		*c = -56;
		*d = 130;
		*C = 150;
		*k = 1.2;
		*v_resting = -75;
		*v_threshold = -45;
		*v_peak = 50;
		*inhibitory = 0;
		*E_excitatory = -75;
		*E_inhibitory = -75;
		*tau_excitatory = 5;
		*tau_inhibitory = 10;				
	}
	else if (neuron_type == CHATTERING) 
	{
		*v = -60;
		*a =0.03;
		*b = 1;
		*c = -40;
		*d = 150;
		*C = 50;
		*k = 1.5;
		*v_resting = -60;
		*v_threshold = -40;
		*v_peak = 25;
		*inhibitory = 0;
		*E_excitatory = -60;
		*E_inhibitory = -60;
		*tau_excitatory = 5;
		*tau_inhibitory = 10;		
	}
	else if (neuron_type == LOW_THRESHOLD) 
	{
		*v = -75;
		*a =0.03;
		*b = 8;
		*c = -53;
		*d = 130;
		*C = 150;
		*k = 1.2;
		*v_resting = -75;
		*v_threshold = -45;
		*v_peak = 50;
		*inhibitory = 1;
		*E_excitatory = -75;
		*E_inhibitory = -75;
		*tau_excitatory = 5;
		*tau_inhibitory = 10;		
	}
	else if (neuron_type == FAST_SPIKING)  
	{
		*v = -60;
		*a =0.03;
		*b = 1;
		*c = -40;
		*d = 150;
		*C = 50;
		*k = 1.5;
		*v_resting = -60;
		*v_threshold = -40;
		*v_peak = 25;
		*inhibitory = 1;
		*E_excitatory = -60;
		*E_inhibitory = -60;
		*tau_excitatory = 5;
		*tau_inhibitory = 10;		
	}
	return;
}

char *get_neuron_type_string(int neuron_type, char *str)
{
	if (neuron_type == REGULAR_SPIKING) 
	{
		str = "Regular Spiking";
	}
	else if (neuron_type == INTRINSICALLY_BURSTING)  
	{
		str = "Intrinsically Bursting";			
	}
	else if (neuron_type == CHATTERING) 
	{
		str = "Chattering";		
	}
	else if (neuron_type == LOW_THRESHOLD) 
	{
		str = "(Inhibitory) Low Threshold";	
	}
	else if (neuron_type == FAST_SPIKING)  
	{
		str = "(Inhibitory) Fast Spikinig";		
	}
	return str;

}

