#include "StatusMarker.h"



bool get_status_marker_color(GdkColor *color, unsigned int idx)
{
	switch (idx)
	{
		case STATUS_MARKER_GREEN:
			color->red = 0;
			color->green = 65535;
			color->blue = 0;	
			return TRUE;		
		case STATUS_MARKER_RED:
			color->red = 65535;
			color->green = 0;
			color->blue = 0;	
			return TRUE;	
		case STATUS_MARKER_YELLOW:
			color->red = 65535;
			color->green = 65535;
			color->blue = 0;	
			return TRUE;	
		default:
			color->red = 0;
			color->green = 0;
			color->blue = 0;				
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "StatusMarker", "get_status_marker_color", "Unknown color index.");

	}
}
