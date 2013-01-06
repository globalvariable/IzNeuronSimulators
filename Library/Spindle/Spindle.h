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


bool evaluate_exponential_primary_spindle_decay_rate(double max_joint_angle, double min_joint_angle, double I_max, double I_min, double *decay_rate);
double evaluate_exponential_primary_extensor_spindle_current(ExponentialPrimaryExtensorSpindle *spindle, double joint_angle);
double evaluate_exponential_primary_flexor_spindle_current(ExponentialPrimaryFlexorSpindle *spindle, double joint_angle);

#endif
