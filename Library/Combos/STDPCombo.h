#ifndef STDP_COMBO_H
#define STDP_COMBO_H

typedef struct 	__STDPCombo 		STDPCombo;
#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"

struct  __STDPCombo
{
	GtkWidget *combo;
};

STDPCombo* allocate_stdp_combo(GtkWidget *hbox, STDPCombo *combo);
bool stdp_combo_get_selected(STDPCombo *combo, int *idx);


#endif
