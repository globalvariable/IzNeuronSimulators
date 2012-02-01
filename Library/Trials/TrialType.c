#include "TrialType.h"




bool get_trial_type_string(TrialType trial_type, char *str)
{
	switch (trial_type)
	{
////            SIMULATION_TRIALS	
		case TRIAL_TYPE_NON_RT_SIMULATION_ROBOT_TO_LEFT:
			str = "TRIAL_TYPE_NON_RT_SIMULATION_ROBOT_TO_LEFT";
			return TRUE;
		case TRIAL_TYPE_NON_RT_SIMULATION_ROBOT_TO_RIGHT:
			str = "TRIAL_TYPE_NON_RT_SIMULATION_ROBOT_TO_RIGHT";	
			return TRUE;
						
////            RT_SIMULATION_TRIALS    (REAL_TIME)
		case TRIAL_TYPE_RT_SIMULATION_ROBOT_TO_LEFT:
			str = "TRIAL_TYPE_RT_SIMULATION_ROBOT_TO_LEFT";
			return TRUE;
		case TRIAL_TYPE_RT_SIMULATION_ROBOT_TO_RIGHT:
			str = "TRIAL_TYPE_RT_SIMULATION_ROBOT_TO_RIGHT";	
			return TRUE;
									
////            IN_VIVO_BMI_TRIALS    (REAL_TIME)
		case TRIAL_TYPE_IN_VIVO_BMI_ROBOT_TARGET_LEFT:
			str = "TRIAL_TYPE_IN_VIVO_BMI_ROBOT_TARGET_LEFT";
			return TRUE;
		case TRIAL_TYPE_IN_VIVO_BMI_ROBOT_TARGET_RIGHT:
			str = "TRIAL_TYPE_IN_VIVO_BMI_ROBOT_TARGET_RIGHT";	
			return TRUE;	
						
			
/////////////////////////		
		case TRIAL_TYPE_NULL:
 			str = "TRIAL_TYPE_NULL";
			return FALSE;
		default:
 			str = "TRIAL_TYPE_INVALID";
			return FALSE;
	}
}




