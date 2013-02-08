#include "DataFormat_v0.h"


int create_main_directory_v0(int num, ...)
{

	return 1;
}
int create_data_directory_v0(int num, ...)
{
	char *path_chooser;
	TimeStamp *rec_start;
	unsigned int recording_number;
	SpikeData **in_silico_spike_data_for_recording;
	unsigned int num_of_dedicated_cpu_threads;



  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *); 
	rec_start = va_arg ( arguments, TimeStamp*); 
	recording_number = va_arg ( arguments, unsigned int); 
	in_silico_spike_data_for_recording = va_arg ( arguments, SpikeData **); 
	num_of_dedicated_cpu_threads = va_arg ( arguments, unsigned int); 
	va_end ( arguments );


	return 1;
}
int fclose_all_data_files_v0(int num, ...)
{
	TimeStamp *rec_end;
	SpikeData **in_silico_spike_data_for_recording;
	unsigned int num_of_dedicated_cpu_threads;

  	va_list arguments;
	va_start ( arguments, num );   
	rec_end = va_arg ( arguments, TimeStamp*); 
	in_silico_spike_data_for_recording = va_arg ( arguments, SpikeData **); 
	num_of_dedicated_cpu_threads = va_arg ( arguments, unsigned int); 
	va_end ( arguments );

	return 1;
}

int delete_data_directory_v0(int num, ...)
{

	return 1;
}

int write_to_data_files_v0(int num, ...)
{

	return 1;
}

int write_notes_to_files_v0(int num, ...)
{
	return 1;
}
int write_additional_notes_to_files_v0(int num, ...)
{
	return 1;
}

