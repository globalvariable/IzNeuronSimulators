#ifndef MOTOR_OUTPUT_TYPES_H
#define MOTOR_OUTPUT_TYPES_H



// Outputs to MovObjControllers

#include <stdbool.h>
#include <gtk/gtk.h>
#include <string.h>


typedef unsigned int MotorOutputType;					

#define	MOTOR_OUTPUT_MAX_STRING_LENGTH							200

#define	MOTOR_OUTPUT_NULL											0

#define	MOTOR_OUTPUT_NO_MOVE										1	
#define	MOTOR_OUTPUT_MOVE_TO_LEFT									2	// send motor speed as additional data
#define	MOTOR_OUTPUT_MOVE_TO_RIGHT								3	// send motor speed as additional data



bool get_motor_output_type_string(MotorOutputType motor_output_type, char *str);







#endif
