#include "Spindle.h"



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

bool evaluate_angular_exponential_spindle_decay_rate(double joint_angle_center, double joint_angle_adjacent, double I_max, double I_min, double *decay_rate)
{
	double angle;  // radian

	if (I_max <= I_min)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Spindle", "evaluate_angular_exponential_spindle_decay_rate", "(I_max <= I_min)."); 

	angle = fabs( joint_angle_center - joint_angle_adjacent);
	*decay_rate = -log(I_min / I_max) / angle;
	if ((*decay_rate) <= 0) 
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "Spindle", "evaluate_angular_exponential_spindle_decay_rate", "(decay_rate) <= 0)."); 
	return true;
}

double evaluate_angular_exponential_spindle_current(ExponentialAngularSpindle *spindle, double joint_angle)
{
	double angle = fabs(spindle->center_angle - joint_angle);
	return spindle->I_max*exp(-spindle->I_decay_rate*angle);
}
