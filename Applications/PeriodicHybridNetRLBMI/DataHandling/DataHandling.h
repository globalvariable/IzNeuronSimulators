#ifndef DATAHANDLING_H
#define DATAHANDLING_H

#include "./v0/DataFormat_v0.h"

// Includes handling of HybridNetRLBMI Format versions starting from v0. The following versions shoudl be v1, v2, v3 and so on. 

#define MAX_NUMBER_OF_DATA_FORMAT_VER	1	// increment this for every new data format handling to be implemented. 

#define DATA_FORMAT_VERSION	0	// Change this according to the intended data format/   Upper limit is MAX_NUMBER_OF_DATA_FORMAT_VER-1

int (*create_main_directory[MAX_NUMBER_OF_DATA_FORMAT_VER]) (int num, ... );
int (*create_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER]) (int num, ... );
int (*fclose_all_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER]) (int num, ... );
int (*delete_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER]) (int num, ... );
int (*write_to_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER]) (int num, ... );

int (*load_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER]) (int num, ... );

int (*write_notes_to_files[MAX_NUMBER_OF_DATA_FORMAT_VER]) (int num, ... );
int (*write_additional_notes_to_files[MAX_NUMBER_OF_DATA_FORMAT_VER]) (int num, ... );



int initialize_data_read_write_handlers(void);
int is_hybrid_net_rl_bmi_data(char *hybrid_net_rl_bmi_data_path);
int get_format_version(int *version, char *hybrid_net_rl_bmi_data_path);

#endif
