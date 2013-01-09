#include "Spindle.h"


ExponentialPrimaryExtensorFlexorSpindleGroup *allocate_exponent_extensor_flexor_spindle_group(unsigned int num_of_extensor, unsigned int num_of_flexor)
{
	ExponentialPrimaryExtensorFlexorSpindleGroup *group;

	group = g_new0(ExponentialPrimaryExtensorFlexorSpindleGroup, 1);
	group->num_of_extensor = num_of_extensor;
	group->num_of_flexor = num_of_flexor;
		
	group->extensor_spindles = g_new0(ExponentialPrimaryExtensorSpindle, num_of_extensor);
	group->flexor_spindles = g_new0(ExponentialPrimaryFlexorSpindle, num_of_flexor);

	return group;
}

bool submit_exponent_extensor_flexor_spindle_group_params(ExponentialPrimaryExtensorFlexorSpindleGroup *group, double I_max, double I_min, double servo_angle_max, double servo_angle_min)
{
	unsigned int i;	
	double decay_rate, min_sensitive_angle, max_sensitive_angle;

	if (servo_angle_max <= servo_angle_min)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Spindle", "submit_exponent_extensor_flexor_spindle_group_params", "(servo_angle_max <= servo_angle_min)."); 

	for (i = 0; i < group->num_of_extensor; i++)
	{
		min_sensitive_angle = servo_angle_min + (i * ((servo_angle_max - servo_angle_min) / ((double)group->num_of_extensor)));
		if (! evaluate_exponential_primary_spindle_decay_rate(servo_angle_max, min_sensitive_angle, I_max, I_min, &decay_rate))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "Spindle", "submit_exponent_extensor_flexor_spindle_group_params", "! evaluate_exponential_primary_spindle_decay_rate().");
		group->extensor_spindles[i].I_decay_rate = decay_rate;
		group->extensor_spindles[i].I_max = I_max;
		group->extensor_spindles[i].max_sensi_angle = servo_angle_max;		
	}

	for (i = 0; i < group->num_of_flexor; i++)
	{
		max_sensitive_angle = servo_angle_max - (i * ((servo_angle_max - servo_angle_min) / ((double)group->num_of_flexor)));
		if (! evaluate_exponential_primary_spindle_decay_rate(max_sensitive_angle, servo_angle_min, I_max, I_min, &decay_rate))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "Spindle", "submit_exponent_extensor_flexor_spindle_group_params", "! evaluate_exponential_primary_spindle_decay_rate().");
		group->flexor_spindles[i].I_decay_rate = decay_rate;
		group->flexor_spindles[i].I_max = I_max;
		group->flexor_spindles[i].min_sensi_angle = servo_angle_min;
	}
	return true;
}


bool evaluate_exponential_primary_spindle_decay_rate(double max_joint_angle, double min_joint_angle, double I_max, double I_min, double *decay_rate)
{
	double angle;  // radian
	if (max_joint_angle <= min_joint_angle)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Spindle", "evaluate_exponential_primary_spindle_decay_rate", "(max_joint_angle <= min_joint_angle)."); 
	if (I_max <= I_min)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Spindle", "evaluate_exponential_primary_spindle_decay_rate", "(I_max <= I_min)."); 
	angle = max_joint_angle - min_joint_angle;
	*decay_rate = -log(I_min / I_max) / angle;
	if ((*decay_rate) <= 0) 
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Spindle", "evaluate_exponential_primary_spindle_decay_rate", "(decay_rate) <= 0)."); 
	return true;
}

double evaluate_exponential_primary_extensor_spindle_current(ExponentialPrimaryExtensorSpindle *spindle, double joint_angle)
{
	double angle = spindle->max_sensi_angle - joint_angle;
	
	if (angle < 0)
		angle = 0;

	return spindle->I_max*exp(-spindle->I_decay_rate*angle);
}

double evaluate_exponential_primary_flexor_spindle_current(ExponentialPrimaryFlexorSpindle *spindle, double joint_angle)
{
	double angle = joint_angle - spindle->min_sensi_angle;
	
	if (angle < 0)
		angle = 0;

	return spindle->I_max*exp(-spindle->I_decay_rate*angle);
}

ExponentialAngularSpindleGroup *allocate_exponent_angular_spindle_group(unsigned int num_of_spindles)
{
	ExponentialAngularSpindleGroup *group;

	group = g_new0(ExponentialAngularSpindleGroup, 1);	
	group->spindles = g_new0(ExponentialAngularSpindle, num_of_spindles);	
	group->num_of_spindles = num_of_spindles;
	
	return group;
}

bool submit_exponent_angular_spindle_group_params(ExponentialAngularSpindleGroup *group, double I_center, double I_adjacent, double servo_angle_max, double servo_angle_min)
{
	double spindle_angle_range, decay_rate;
	unsigned int i;
	if (servo_angle_max <= servo_angle_min)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Spindle", "submit_exponent_angular_spindle_group_params", "(servo_angle_max <= servo_angle_min)."); 

	spindle_angle_range = (servo_angle_max - servo_angle_min)/((double)group->num_of_spindles);
	
	for (i = 0; i < group->num_of_spindles; i++)
	{
		group->spindles[i].center_angle = servo_angle_min + (spindle_angle_range/2.0) + (i*spindle_angle_range);
		group->spindles[i].I_max = I_center;
		if (! evaluate_angular_exponential_spindle_decay_rate(spindle_angle_range/2.0, 0.0, I_center, I_adjacent, &decay_rate))
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "Spindle", "submit_exponent_angular_spindle_group_params", "! evaluate_angular_exponential_spindle_decay_rate().");
		group->spindles[i].I_decay_rate = decay_rate;
	}
	return true;
}

bool evaluate_angular_exponential_spindle_decay_rate(double spindle_angle_center, double spindle_angle_adjacent, double I_center, double I_adjacent, double *decay_rate)
{
	double angle;  // radian

	if (I_center <= I_adjacent)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Spindle", "evaluate_angular_exponential_spindle_decay_rate", "(I_max <= I_min)."); 

	angle = fabs( spindle_angle_center - spindle_angle_adjacent);
	*decay_rate = -log(I_adjacent / I_center) / angle;
	if ((*decay_rate) <= 0) 
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Spindle", "evaluate_angular_exponential_spindle_decay_rate", "(decay_rate) <= 0)."); 
	return true;
}

double evaluate_angular_exponential_spindle_current(ExponentialAngularSpindle *spindle, double joint_angle)
{
	double angle = fabs(spindle->center_angle - joint_angle);
	return spindle->I_max*exp(-spindle->I_decay_rate*angle);
}
