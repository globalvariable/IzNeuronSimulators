#include "MotorOutputTypes.h"

bool get_motor_output_type_string(MotorOutputType motor_output_type, char *str)
{
	switch (motor_output_type)
	{
		case MOTOR_OUTPUT_NO_MOVE:
			if (str != NULL)
				strcpy(str, "MOTOR_OUTPUT_NO_MOVE");
			return TRUE;
		case MOTOR_OUTPUT_MOVE_TO_LEFT:
			if (str != NULL)
				strcpy(str, "MOTOR_OUTPUT_MOVE_TO_LEFT");
			return TRUE;
		case MOTOR_OUTPUT_MOVE_TO_RIGHT:
			if (str != NULL)
				strcpy(str, "MOTOR_OUTPUT_MOVE_TO_RIGHT");
			return TRUE;

////////////////////////		
		case MOTOR_OUTPUT_NULL:
 			strcpy(str, "MOTOR_OUTPUT_NULL");
			return FALSE;
		default:
 			strcpy(str, "MOTOR_OUTPUT_INVALID");
			return FALSE;
	}
}
