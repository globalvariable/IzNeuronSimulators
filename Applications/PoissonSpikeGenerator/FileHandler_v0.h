#ifndef FILE_HANDLER_V0_H
#define FILE_HANDLER_V0_H


#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../../BlueSpike/Applications/PCIe6259/ConfigDaq/DaqMwaData.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "BMISpikeGenerator.h"


int write_spike_gen_config_data_v0(int num, ...);
int read_spike_gen_config_data_v0(int num, ...);



#endif
