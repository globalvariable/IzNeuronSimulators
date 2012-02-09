#ifndef CURRENT_PATTERN_GRAPH_H
#define CURRENT_PATTERN_GRAPH_H

typedef struct 	__CurrentPatternGraph 	CurrentPatternGraph;

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include "../../../BlueSpike/TimeStamp.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Patterns/CurrentPatterns.h"

struct  __CurrentPatternGraph
{
	CurrentPatterns* current_patterns;
	GtkWidget *box;
	GtkWidget *databox;
	GtkDataboxGraph *graph;
	float *x;
	float *y;
};

CurrentPatternGraph* allocate_current_pattern_graph(GtkWidget *hbox, CurrentPatternGraph *graph);


#endif
