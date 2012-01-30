#include "PatternType.h"




bool get_pattern_type_string(PatternType pattern_type, char *str)
{
	switch (pattern_type)
	{
////            NON_RT_PATTERN_SIMULATIONS (NON REAL-TIME)
		case PATTERN_TYPE_NON_RT_PATTERN_SIMULATION_ROBOT_TO_LEFT:
			str = "PATTERN_TYPE_NON_RT_PATTERN_SIMULATION_ROBOT_TO_LEFT";
			return TRUE;			
		case PATTERN_TYPE_NON_RT_PATTERN_SIMULATION_ROBOT_TO_RIGHT:
			str = "PATTERN_TYPE_NON_RT_PATTERN_SIMULATION_ROBOT_TO_RIGHT";	
			return TRUE;
						
////            RT_PATTERN_SIMULATIONS    (REAL_TIME)
		case PATTERN_TYPE_RT_PATTERN_SIMULATION_ROBOT_TO_LEFT:
			str = "PATTERN_TYPE_RT_PATTERN_SIMULATION_ROBOT_TO_LEFT";
			return TRUE;			
		case PATTERN_TYPE_RT_PATTERN_SIMULATION_ROBOT_TO_LEFT:
			str = "PATTERN_TYPE_RT_PATTERN_SIMULATION_ROBOT_TO_LEFT";	
			return TRUE;				

			
			
			
/////////////////////////	
		case PATTERN_TYPE_NULL:
 			str = "PATTERN_TYPE_NULL";
			return FALSE;							
		default:
 			str = "PATTERN_TYPE_INVALID";
			return FALSE;							
	}
}








}
