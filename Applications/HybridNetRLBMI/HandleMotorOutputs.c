#include "HandleMotorOutputs.h"


bool handle_motor_outputs(MotorOutputs* motor_outputs, TimeStamp current_time, NeuralNet2MovObjHandMsg	*msgs_neural_net_2_mov_obj_hand)
{
	unsigned int i, j, k;
	unsigned int num_of_outputs, num_of_classes, num_of_neurons; // in class
	Neuron **neuron_list;
	unsigned int	*total_firing_in_class;
	unsigned int largest_firing = 0, second_largest_firing = 0;
	unsigned int largest_class = 0;
	MotorOutput *output;
	MotorOutputClass *class;			// clock_wise, counter clock wise   OR  up down left right   (4 classes)
	num_of_outputs = motor_outputs->num_of_outputs;
	for (i = 0; i < num_of_outputs; i++)
	{	
		output = &(motor_outputs->outputs[i]);
		if (current_time >= (output->bin_start_time + output->bin_size))
		{
			num_of_classes = output->num_of_classes;
			for(j = 0; j < num_of_classes; j ++)
			{
				class = &(output->classes[j]);
				num_of_neurons = class->num_of_neurons;
				total_firing_in_class = &(class->total_firing_in_class);
				neuron_list = class->neuron_list;
				for(k = 0; k < num_of_neurons; k++)
					*total_firing_in_class += neuron_list[k]->stats->num_of_firings_for_motor_output;
				if (*total_firing_in_class >= largest_firing)
				{	
					second_largest_firing = largest_firing;	
					largest_firing = *total_firing_in_class;
					largest_class = j;
				} 
			}
			if (second_largest_firing == largest_firing)   // there is an equavalence at firing rate of classes, stay stationary
			{
				printf ("No move %u\n", largest_firing);
				if (!write_to_neural_net_2_mov_obj_hand_msg_buffer(msgs_neural_net_2_mov_obj_hand, current_time, NEURAL_NET_2_MOV_OBJ_HAND_MSG_DIRECTION_SPEED_LOCATION, 0, MOV_OBJ_DIRECTION_STATIONARY, MOV_OBJ_SPEED_NULL, MOV_OBJ_LOCATION_NULL))
					return print_message(ERROR_MSG ,"IzNeuronSimulators", "MotorOutput", "handle_motor_outputs", "! write_to_neural_net_2_mov_obj_hand_msg_buffer().");
			}
			else   		//		MOVE ROBOT TO LEFT OR RIGHT ACOORDING TO LARGEST CLASS
			{
				switch (largest_class)  
				{
					case NEURONS_MOTOR_OUTPUT_CLASS_LEFT_MOVE:
						printf ("Left move %u\n", largest_firing);
						if (!write_to_neural_net_2_mov_obj_hand_msg_buffer(msgs_neural_net_2_mov_obj_hand, current_time, NEURAL_NET_2_MOV_OBJ_HAND_MSG_DIRECTION_SPEED_LOCATION, 0, MOV_OBJ_DIRECTION_LEFT, NEURONS_MOTOR_OUTPUT_MOTOR_SPEED, MOV_OBJ_LOCATION_NULL))
							return print_message(ERROR_MSG ,"IzNeuronSimulators", "MotorOutput", "handle_motor_outputs", "! write_to_neural_net_2_mov_obj_hand_msg_buffer().");
						break;
					case NEURONS_MOTOR_OUTPUT_CLASS_RIGHT_MOVE:
						printf ("Right move %u\n", largest_firing);
						if (!write_to_neural_net_2_mov_obj_hand_msg_buffer(msgs_neural_net_2_mov_obj_hand, current_time, NEURAL_NET_2_MOV_OBJ_HAND_MSG_DIRECTION_SPEED_LOCATION, 0, MOV_OBJ_DIRECTION_RIGHT, NEURONS_MOTOR_OUTPUT_MOTOR_SPEED, MOV_OBJ_LOCATION_NULL))
							return print_message(ERROR_MSG ,"IzNeuronSimulators", "MotorOutput", "handle_motor_outputs", "! write_to_neural_net_2_mov_obj_hand_msg_buffer().");
						break;
					default:
						return print_message(ERROR_MSG ,"IzNeuronSimulators", "MotorOutput", "handle_motor_outputs", "Invalid neuron motor output class !!!.");
				}
			}
			/// RESET COUNTERS
			output->bin_start_time = current_time;
			for(j = 0; j < num_of_classes; j ++)
			{
				class = &(output->classes[j]);
				num_of_neurons = class->num_of_neurons;
				class->total_firing_in_class = 0;
				neuron_list = class->neuron_list;
				for(k = 0; k < num_of_neurons; k++)
					neuron_list[k]->stats->num_of_firings_for_motor_output = 0;
			}
			largest_firing = 0;
			second_largest_firing = 0;
		}
	}
	return TRUE;
}
