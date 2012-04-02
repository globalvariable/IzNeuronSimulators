#ifndef BUFFER_VIEW_HANDLER_H
#define BUFFER_VIEW_HANDLER_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "NeuronDynamicsView.h"



bool buffer_view_handler(void);
void send_pause_request_to_buffer_view_handler(void);

#endif
