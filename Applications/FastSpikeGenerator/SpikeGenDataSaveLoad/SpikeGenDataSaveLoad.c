#include "SpikeGenDataSaveLoad.h"


bool initialize_spike_gen_data_read_write_handlers(void)
{
	create_spike_gen_data_directory[0] = &create_spike_gen_data_directory_v0;
	save_spike_gen_data_directory[0] = &save_spike_gen_data_directory_v0;
	load_spike_gen_data_directory[0] = &load_spike_gen_data_directory_v0;

	return TRUE;
}
bool is_spike_gen_data(char *spike_gen_data_path)
{
	char path[600];
	char line[200];
	FILE *fp;
	strcpy(path, spike_gen_data_path);
	strcat(path, "/meta");
	if ((fp = fopen(path, "r")) == NULL) 
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "is_spike_gen_data", "fopen() == NULL.");
	if (fgets(line, sizeof line, fp ) == NULL)	{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "is_spike_gen_data", "fgets() == NULL."); }
	fclose(fp);   
	if (strcmp(line, "----------SpikeGeneratorData - Main Meta File----------\n" ) == 0)
		return TRUE;
	else
		return FALSE;
}
bool get_spike_gen_data_format_version(int *version, char *spike_gen_data_path)
{
	char path[600];
	char line[200];
	char word[10];
	FILE *fp;
	if (!is_spike_gen_data(spike_gen_data_path))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "get_spike_gen_data_format_version", "! is_spike_gen_data().");
	strcpy(path, spike_gen_data_path);
	strcat(path, "/meta");		
	if ((fp = fopen(path, "r")) == NULL) 
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "get_spike_gen_data_format_version", "fopen() == NULL.");
	if (fgets(line, sizeof line, fp ) == NULL)	{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "get_spike_gen_data_format_version", "fgets() == NULL."); }
	if (fgets(line, sizeof line, fp ) == NULL)	{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "get_spike_gen_data_format_version", "fgets() == NULL."); }
	if (!get_word_in_line('\t', 1, word, line, TRUE))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "get_spike_gen_data_format_version", "! get_word_in_line().");
	fclose(fp);   	
	*version = atoi(word);
	return TRUE;
}
