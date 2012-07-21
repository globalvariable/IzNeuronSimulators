#ifndef SPIKE_GEN_DATA_SAVE_LOAD_V0_H
#define SPIKE_GEN_DATA_SAVE_LOAD_V0_H


#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <gtk/gtk.h>
#include <string.h>
#include "../../BMISimulationSpikeGenerator.h"


int create_spike_gen_data_directory_v0(int num, ...);
int save_spike_gen_data_directory_v0(int num, ...);
int load_spike_gen_data_directory_v0(int num, ...);








#endif
