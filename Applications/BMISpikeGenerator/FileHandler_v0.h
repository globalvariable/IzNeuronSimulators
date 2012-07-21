#ifndef FILE_HANDLER_V0_H
#define FILE_HANDLER_V0_H


#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../../BlueSpike/DaqMwaData.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../BlueSpike/TemplateMatchingData.h"
#include "BMISpikeGenerator.h"


int write_spike_gen_config_data_v0(int num, ...);
int read_spike_gen_config_data_v0(int num, ...);



#endif
