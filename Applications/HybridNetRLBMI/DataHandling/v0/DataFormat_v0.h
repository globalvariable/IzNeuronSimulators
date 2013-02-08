#ifndef DATAFORMAT_V0_H
#define DATAFORMAT_V0_H

#include <stdarg.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../../../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../../Library/SpikeData/SpikeData.h"

int create_main_directory_v0(int num, ...);
int create_data_directory_v0(int num, ...);
int fclose_all_data_files_v0(int num, ...);
int delete_data_directory_v0(int num, ...);
int write_to_data_files_v0(int num, ...);

int write_notes_to_files_v0(int num, ...);
int write_additional_notes_to_files_v0(int num, ...);







#endif
