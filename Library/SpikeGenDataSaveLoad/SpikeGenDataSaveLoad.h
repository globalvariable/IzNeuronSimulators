#ifndef SAVE_LOAD_SPIKE_GEN_DATA_H
#define SAVE_LOAD_SPIKE_GEN_DATA_H


#define SPIKE_GEN_DATA_MAX_NUMBER_OF_DATA_FORMAT_VER	1	// increment this for every new data format handling to be implemented. 

#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "./v0/SpikeGenDataSaveLoad_v0.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"

int (*create_spike_gen_data_directory[SPIKE_GEN_DATA_MAX_NUMBER_OF_DATA_FORMAT_VER]) (int num, ... );
int (*save_spike_gen_data_directory[SPIKE_GEN_DATA_MAX_NUMBER_OF_DATA_FORMAT_VER]) (int num, ... );
int (*load_spike_gen_data_directory[SPIKE_GEN_DATA_MAX_NUMBER_OF_DATA_FORMAT_VER]) (int num, ... );



bool initialize_spike_gen_data_read_write_handlers(void);
bool is_spike_gen_data(char *spike_gen_data_path);
bool get_spike_gen_data_format_version(int *version, char *spike_gen_data_path);

#endif
