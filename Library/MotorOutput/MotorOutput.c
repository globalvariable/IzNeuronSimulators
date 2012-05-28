#include "MotorOutput.h"



MotorOutputs* allocate_motor_outputs(MotorOutputs* motor_outputs, unsigned int num_of_outputs)
{
	if (motor_outputs != NULL)
	{
		motor_outputs = deallocate_motor_outputs(motor_outputs);
		motor_outputs = allocate_motor_outputs(motor_outputs, num_of_outputs);
		return motor_outputs;
	}
	motor_outputs = g_new0(MotorOutputs,1);
	motor_outputs->outputs = g_new0(MotorOutput, num_of_outputs);
	motor_outputs->num_of_outputs = num_of_outputs;
	print_message(INFO_MSG ,"IzNeuronSimulators", "MotorOutput", "allocate_motor_outputs", "Created motor_outputs.");
	return motor_outputs;
}

MotorOutputs* deallocate_motor_outputs(MotorOutputs* motor_outputs)
{
	unsigned int i,j;
	if (motor_outputs == NULL)
		return (MotorOutputs*)print_message(WARNING_MSG ,"IzNeuronSimulators", "MotorOutput", "deallocate_motor_outputs", "motor_outputs == NULL.");

	for (i = 0; i < motor_outputs->num_of_outputs; i++)
	{	
		for(j = 0; j < motor_outputs->outputs[i].num_of_classes; j ++)
		{
			g_free(motor_outputs->outputs[i].classes[j].neuron_list);
		}
		g_free(motor_outputs->outputs[i].classes);
	}
	g_free(motor_outputs->outputs);
	g_free(motor_outputs);
	return NULL;
}
bool increment_num_of_classes_in_motor_output(MotorOutputs* motor_outputs, unsigned int output_num, TimeStamp bin_size)
{
	MotorOutputClass	*lcl_classes;	
	unsigned int i;		
	if (motor_outputs == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "MotorOutput", "increment_num_of_calsses_in_motor_output", "motor_outputs == NULL.");	
	if (output_num >= motor_outputs->num_of_outputs)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "MotorOutput", "increment_num_of_calsses_in_motor_output", "output_num >= motor_outputs->num_of_outputs.");	
	lcl_classes = g_new0(MotorOutputClass, motor_outputs->outputs[output_num].num_of_classes + 1);
	for (i = 0; i < motor_outputs->outputs[output_num].num_of_classes; i++)
		lcl_classes[i] = motor_outputs->outputs[output_num].classes[i];
	g_free(motor_outputs->outputs[output_num].classes);
	if ((motor_outputs->outputs[output_num].bin_size != 0) && (motor_outputs->outputs[output_num].bin_size != bin_size))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "MotorOutput", "increment_num_of_calsses_in_motor_output", "submitted bin size is not same the one submitted for other class(es) of the ouput.");	
	motor_outputs->outputs[output_num].bin_size = bin_size;
	motor_outputs->outputs[output_num].classes = lcl_classes;
	motor_outputs->outputs[output_num].num_of_classes++;
	return TRUE;
}
bool add_neurons_in_layer_to_motor_output_class(MotorOutputs* motor_outputs, Network *network, unsigned int layer_num, unsigned int output_num, unsigned int class_num)
{
	Layer *layer;
	unsigned int i, j, k = 0, num_of_neurons_in_layer = 0;
	char msg[200];
	if (motor_outputs == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "MotorOutput", "add_neurons_in_layer_to_motor_output_class", "motor_outputs == NULL.");		
	if (output_num >= motor_outputs->num_of_outputs)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "MotorOutput", "add_neurons_in_layer_to_motor_output_class", "output_num >= motor_outputs->num_of_outputs.");	
	if (class_num >= motor_outputs->outputs[output_num].num_of_classes)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "MotorOutput", "add_neurons_in_layer_to_motor_output_class", "class_num >= motor_outputs->outputs[output_num].num_of_classes.");
	if (is_layer_free (network, layer_num))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "MotorOutput", "add_neurons_in_layer_to_motor_output_class", "is_layer_free().");
	if (motor_outputs->outputs[output_num].classes[class_num].layer != NULL)	
	{
		sprintf(msg, "A network layer has already been assigned to Motor Output: %u Class: %u", output_num, class_num);		
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "MotorOutput", "add_neurons_in_layer_to_motor_output_class", msg);
	}

	layer = network->layers[layer_num];

	for (i = 0; i < layer->neuron_group_count; i++)
		num_of_neurons_in_layer = num_of_neurons_in_layer + layer->neuron_groups[i]->neuron_count;
 
	motor_outputs->outputs[output_num].classes[class_num].neuron_list = g_new0(Neuron*, num_of_neurons_in_layer);
	for (i = 0; i < layer->neuron_group_count; i++)
	{
		for (j = 0; j < layer->neuron_groups[i]->neuron_count; j++)
		{
			motor_outputs->outputs[output_num].classes[class_num].neuron_list[k] = &(layer->neuron_groups[i]->neurons[j]);
			k++;
		}
	} 
	motor_outputs->outputs[output_num].classes[class_num].num_of_neurons = num_of_neurons_in_layer;
	motor_outputs->outputs[output_num].classes[class_num].layer = layer;
	sprintf(msg, "Connection of Layer: %u to Motor Output: %u Class: %u is successful !!!", layer_num, output_num, class_num);	
	print_message(INFO_MSG ,"IzNeuronSimulators", "MotorOutput", "add_neurons_in_layer_to_motor_output_class", msg);

	return TRUE;
}

void clear_motor_output_counters(MotorOutputs* motor_outputs)
{
	unsigned int i, j, k;
	unsigned int num_of_outputs, num_of_classes, num_of_neurons; // in class
	Neuron **neuron_list;
	MotorOutput *output;
	MotorOutputClass *class;			// clock_wise, counter clock wise   OR  up down left right   (4 classes)

	num_of_outputs = motor_outputs->num_of_outputs;
	for (i = 0; i < num_of_outputs; i++)
	{	
		output = &(motor_outputs->outputs[i]);
		output->bin_start_time = 0;
		num_of_classes = output->num_of_classes;
		for(j = 0; j < num_of_classes; j ++)
		{
			class = &(output->classes[j]);
			num_of_neurons = class->num_of_neurons;
			neuron_list = class->neuron_list;
			class->total_firing_in_class = 0;
			for(k = 0; k < num_of_neurons; k++)
				neuron_list[k]->stats->num_of_firings_for_motor_output = 0;
		}
	}
	return;
}

bool any_unused_classes_for_motor_outputs(MotorOutputs* motor_outputs)
{
	unsigned int i, j;
	unsigned int num_of_outputs, num_of_classes; 
	MotorOutput *output;

	num_of_outputs = motor_outputs->num_of_outputs;
	for (i = 0; i < num_of_outputs; i++)
	{	
		output = &(motor_outputs->outputs[i]);
		num_of_classes = output->num_of_classes;
		for(j = 0; j < num_of_classes; j ++)
		{
			if (motor_outputs->outputs[i].classes[j].layer == NULL)	
				return TRUE;
		}
	}
	return FALSE;
}
