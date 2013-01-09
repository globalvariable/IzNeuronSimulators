#ifndef SPINDLE_H
#define SPINDLE_H

#include <math.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"

typedef struct
{
	double		max_sensi_angle;    ///  applies I_max at this angle
	double		I_max;
	double		I_decay_rate;    ///    I = I_max*exp(-decay*(max_sensi_angle-theta))
} ExponentialPrimaryExtensorSpindle;

typedef struct
{
	double		min_sensi_angle;	///  applies I_max at this angle
	double		I_max; 			
	double		I_decay_rate;	   ///    I = I_max*exp(-decay*(theta-min_sensi_angle))
} ExponentialPrimaryFlexorSpindle;

typedef struct
{
	unsigned int num_of_extensor;
	unsigned int num_of_flexor;
	ExponentialPrimaryExtensorSpindle *extensor_spindles;  //   num_of_extensor
	ExponentialPrimaryFlexorSpindle *flexor_spindles;  //   num_of_flexor
} ExponentialPrimaryExtensorFlexorSpindleGroup;

typedef struct
{
	double		center_angle;	///  applies I_max at this angle
	double		I_max; 			
	double		I_decay_rate;	   ///    I = I_max*exp(-decay*|theta-center_angle|)
} ExponentialAngularSpindle;

typedef struct
{
	unsigned int				num_of_spindles;
	ExponentialAngularSpindle	*spindles;
} ExponentialAngularSpindleGroup;

ExponentialPrimaryExtensorFlexorSpindleGroup *allocate_exponent_extensor_flexor_spindle_group(unsigned int num_of_extensor, unsigned int num_of_flexor);
bool submit_exponent_extensor_flexor_spindle_group_params(ExponentialPrimaryExtensorFlexorSpindleGroup *group, double I_max, double I_min, double servo_angle_max, double servo_angle_min);
bool evaluate_exponential_primary_spindle_decay_rate(double max_joint_angle, double min_joint_angle, double I_max, double I_min, double *decay_rate);
double evaluate_exponential_primary_extensor_spindle_current(ExponentialPrimaryExtensorSpindle *spindle, double joint_angle);
double evaluate_exponential_primary_flexor_spindle_current(ExponentialPrimaryFlexorSpindle *spindle, double joint_angle);

ExponentialAngularSpindleGroup *allocate_exponent_angular_spindle_group(unsigned int num_of_spindles);
bool submit_exponent_angular_spindle_group_params(ExponentialAngularSpindleGroup *group, double I_center, double I_adjacent, double servo_angle_max, double servo_angle_min);
bool evaluate_angular_exponential_spindle_decay_rate(double spindle_angle_center, double spindle_angle_adjacent, double I_center, double I_adjacent, double *decay_rate);
double evaluate_angular_exponential_spindle_current(ExponentialAngularSpindle *spindle, double joint_angle);

#endif
