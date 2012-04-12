#include "SpikeGenDataSaveLoad_v0.h"


int create_spike_gen_data_directory_v0(int num, ...)
{
/*	char *path_chooser;
	Network *network;
	DIR	*dir_main_folder;
	char main_dir_path[600];
  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *);
     	network = va_arg ( arguments, Network *);   	
	va_end ( arguments );
	
	strcpy(main_dir_path, path_chooser);	
	strcat(main_dir_path, "/SpikeGenData");
	if ((dir_main_folder = opendir(main_dir_path)) != NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "create_spike_gen_data_directory_v0", "Directory has a SpikeGenData folder already, select another directory.");

	mkdir(main_dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "create_spike_gen_data_directory_v0", "Created SpikeGenData.");
       
	if (!create_main_meta_file(network, main_dir_path))
		return 0;
*/		
	return 1;
}

int save_spike_gen_data_directory_v0(int num, ...)
{
	return 1;
}
int load_spike_gen_data_directory_v0(int num, ...)
{
	return 1;
}

