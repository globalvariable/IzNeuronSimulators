#ifndef STATUS_MARKER_H
#define STATUS_MARKER_H

#define STATUS_MARKER_GREEN			0
#define STATUS_MARKER_RED			1
#define STATUS_MARKER_YELLOW			2


typedef struct __StatusMarkers StatusMarkers;
typedef struct __StatusMarker StatusMarker;

#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"

struct  __StatusMarkers
{
	StatusMarker *markers;
	unsigned int num_of_markers;
};


struct  __StatusMarker
{
	float x[2];		// x' s will same
	float y[2];		// zero th index is bottom, first index is top.
};


bool get_status_marker_color(GdkColor *color, unsigned int idx);   





#endif
