#ifndef MOTOR_OUTPUT_H
#define MOTOR_OUTPUT_H


#define 	NEURONS_MOTOR_OUTPUT_CLASS_LEFT_MOVE	0
#define 	NEURONS_MOTOR_OUTPUT_CLASS_RIGHT_MOVE	1

#define 	NEURONS_MOTOR_OUTPUT_MOTOR_SPEED		3

typedef struct __MotorOutputClass MotorOutputClass;
typedef struct __MotorOutput MotorOutput;
typedef struct __MotorOutputs MotorOutputs;


#include "../Neuron/Neuron.h"
#include "MotorOutputTimeStamp.h"

struct __MotorOutputClass
{
	Neuron		**neuron_list;
	unsigned int 	num_of_neurons; // in class
	unsigned int	total_firing_in_class;
	Layer 		*layer;
};


struct __MotorOutput
{
	MotorOutputClass	*classes;			// clock_wise, counter clock wise   OR  up down left right   (4 classes)
	unsigned int		num_of_classes;
	TimeStamp		bin_start_time;
	TimeStamp		bin_size;
};

struct __MotorOutputs
{
	unsigned int		num_of_outputs;  // 	each DOF or arm  // each output has classes to decide the selected class of movement
	MotorOutput		*outputs;
};

MotorOutputs* allocate_motor_outputs(MotorOutputs* motor_outputs, unsigned int num_of_outputs);
MotorOutputs* deallocate_motor_outputs(MotorOutputs* motor_outputs);
bool increment_num_of_classes_in_motor_output(MotorOutputs* motor_outputs, unsigned int output_num, TimeStamp bin_size);
bool add_neurons_in_layer_to_motor_output_class(MotorOutputs* motor_outputs, Network *network, unsigned int layer_num, unsigned int output_num, unsigned int class_num);
bool handle_motor_outputs(MotorOutputs* motor_outputs, TimeStamp current_time);
void clear_motor_output_counters(MotorOutputs* motor_outputs);
bool any_unused_classes_for_motor_outputs(MotorOutputs* motor_outputs);


#endif
