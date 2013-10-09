#include "DataFormat_v0.h"


static SpikeData **in_silico_spike_data_for_recording = NULL;
static unsigned int num_of_dedicated_cpu_threads = 0;
static unsigned int *in_silico_spike_data_end_idx = NULL;

static FILE *meta_data_file_ptr;
static FILE *in_silico_spike_data_file_ptr;
static FILE *in_silico_network_data_file_ptr;

static int create_main_meta_file(char *main_directory_path);
static int create_data_files(TimeStamp rec_start, char *data_directory_path);
static int create_meta_data(TimeStamp rec_start, char *data_directory_path);
static int create_in_silico_spike_data(char *data_directory_path);
static int create_in_silico_network_data(char *data_directory_path);
static int write_to_in_silico_spike_data(void);
static int close_meta_data(TimeStamp rec_end);
static int close_in_silico_spike_data(void);
static int close_in_silico_network_data(void);
static int delete_data_files(char *data_directory_path);
static int delete_meta_data(char *data_directory_path);
static int delete_in_silico_spike_data(char *data_directory_path);
static int delete_in_silico_network_data(char *data_directory_path);
static int load_main_meta_file(char *path_chooser, Network *in_silico_network, Network *blue_spike_network, HybridNetRLBMIData *bmi_data);
static int load_data_folder(char *path_chooser, Network *in_silico_network, Network *blue_spike_network, unsigned int data_folder_num);


int create_main_directory_v0(int num, ...)
{
	FILE *fp;
	char *path_chooser;
	DIR	*dir_main_folder;
	char main_directory_path[600];

  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *); 
	va_end ( arguments );
	
	strcpy(main_directory_path, path_chooser);	
	strcat(main_directory_path, "/HybridNetRLBMIRecord");
	if ((dir_main_folder = opendir(main_directory_path)) != NULL)
        {
        	printf ("HybridNetRLBMI: ERROR: path: %s already has HybridNetRLBMIRecord folder.\n", path_chooser);		
        	printf ("HybridNetRLBMI: ERROR: Select another folder or delete HybridNetRLBMIRecord directory.\n\n");	
		closedir(dir_main_folder );	        		
                return 0;
        }
        closedir(dir_main_folder );
        
	mkdir(main_directory_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

        printf ("HybridNetRLBMI: Created /HybridNetRLBMIRecord folder in: %s.\n", path_chooser);
        printf ("HybridNetRLBMI: /HybridNetRLBMIRecord path is: %s.\n", main_directory_path); 
        
	if (!create_main_meta_file(main_directory_path))
		return 0;

	if ((fp = fopen("./path_initial_directory", "w")) == NULL)  { printf ("ERROR: HybridNetRLBMI: Couldn't find directory: %s\n\n", "./path_initial_directory"); return 0; }
	fprintf(fp, "%s", path_chooser);
	fclose (fp);	
	return 1;
}
int create_data_directory_v0(int num, ...)
{
	char *path_chooser;
	TimeStamp *rec_start;
	unsigned int recording_number;

  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *); 
	rec_start = va_arg ( arguments, TimeStamp*); 
	recording_number = va_arg ( arguments, unsigned int); 
	va_end ( arguments );

	char data_directory_name[10];
	char data_directory_num[10];
	DIR	*dir_data_directory;	
	char data_directory_path[600];
	TimeStamp recording_start_time;
	unsigned int i, data_directory_cntr, ret;
	in_silico_spike_data_for_recording = get_hybrid_net_rl_bmi_data()->in_silico_spike_data_for_recording;  // for recording to hdd // each thread writes to one buffer.	
	num_of_dedicated_cpu_threads = get_hybrid_net_rl_bmi_data()->num_of_dedicated_cpu_threads;  // for recording to hdd // each thread writes to one buffer.	
	in_silico_spike_data_end_idx = g_new0(unsigned int, num_of_dedicated_cpu_threads);

	for (i = 0; i < num_of_dedicated_cpu_threads; i++)
	{
		reset_spike_data_read_idx(in_silico_spike_data_for_recording[i]);
	}	
	recording_start_time = *rec_start;		//  ACCORDING TO THIS DESIGN, RECORDING START TIME AND THE CORRESPONDING SPIKE DATA SAMPLE MIGHT NOT COINCODE TO SAME TIME BUT THEY WILL BE ALMOST EQUAL. 

	meta_data_file_ptr = NULL;
	in_silico_spike_data_file_ptr = NULL;
	in_silico_network_data_file_ptr = NULL;

	data_directory_cntr = recording_number;

	if (data_directory_cntr <10)
	{
		strcpy(data_directory_name, "dat0000");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);
	}
	else if (data_directory_cntr <100)
	{
		strcpy(data_directory_name, "dat000");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}
	else if (data_directory_cntr <1000)
	{
		strcpy(data_directory_name, "dat00");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}	
	else if (data_directory_cntr <10000)
	{
		strcpy(data_directory_name, "dat0");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}	
	else if (data_directory_cntr <100000)
	{
		strcpy(data_directory_name, "dat");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);			
	}	
	else
	{
		printf("HybridNetRLBMI: ERROR: data directory counter is %d.\n", data_directory_cntr);
		printf("HybridNetRLBMI: ERROR: Supported range is 0<= x <100000.\n\n");		
		return 0;
	}
	
	strcpy(data_directory_path, path_chooser);	
	strcat(data_directory_path, "/HybridNetRLBMIRecord/");
	strcat(data_directory_path, data_directory_name);	
	if ((dir_data_directory = opendir(data_directory_path)) != NULL)
        {
        	printf ("HybridNetRLBMI: ERROR: path: %s already has %s folder.\n", data_directory_path, data_directory_name);		
		closedir(dir_data_directory);
                return 0;
        }
	closedir(dir_data_directory);	  
      		        
	if ((ret = mkdir(data_directory_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH)) != 0)
	{
		printf("Couldn't makedir: %s\n, Reason: %d\n", data_directory_path, ret);
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "create_data_directory_v0", "mkdir() != 0");
	}	

	if (! create_data_files(recording_start_time, data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "create_data_directory_v0", "! create_data_files.");

	return 1;
}
int fclose_all_data_files_v0(int num, ...)
{
	TimeStamp *rec_end;

  	va_list arguments;
	va_start ( arguments, num );   
	rec_end = va_arg ( arguments, TimeStamp*); 
	va_end ( arguments );

	TimeStamp recording_end_time;
	unsigned int i;

	for (i = 0; i < num_of_dedicated_cpu_threads; i++)
	{
		in_silico_spike_data_end_idx[i] = in_silico_spike_data_for_recording[i]->buff_idx_write;
	}	
	recording_end_time = *rec_end;		//  ACCORDING TO THIS DESIGN, RECORDING END TIME AND THE CORRESPONDING SPIKE DATA SAMPLE MIGHT NOT COINCODE TO SAME TIME BUT THEY WILL BE ALMOST EQUAL. 


	if (! close_meta_data(recording_end_time))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "create_data_directory_v0", "! close_meta_data");

	if (! close_in_silico_spike_data())
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "create_data_directory_v0", "! close_in_silico_spike_data");

	if (! close_in_silico_network_data())
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "create_data_directory_v0", "! close_in_silico_spike_data");

	return 1;
}

int delete_data_directory_v0(int num, ...)
{
	char data_directory_name[10];
	char data_directory_num[10];
	DIR	*dir_data_directory;	
	char *path_chooser;
	char data_directory_path[600];

	unsigned int data_directory_cntr, recording_number;
 
  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *); 
	recording_number = va_arg ( arguments, unsigned int); 
	va_end ( arguments );

	data_directory_cntr = recording_number;
	if (data_directory_cntr <10)
	{
		strcpy(data_directory_name, "dat0000");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);
	}
	else if (data_directory_cntr <100)
	{
		strcpy(data_directory_name, "dat000");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}
	else if (data_directory_cntr <1000)
	{
		strcpy(data_directory_name, "dat00");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}	
	else if (data_directory_cntr <10000)
	{
		strcpy(data_directory_name, "dat0");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}	
	else if (data_directory_cntr <100000)
	{
		strcpy(data_directory_name, "dat");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);			
	}	
	else
	{
		printf("HybridNetRLBMI: ERROR: data directory counter is %d.\n", data_directory_cntr);
		printf("HybridNetRLBMI: ERROR: Supported range is 0<= x <100000.\n\n");		
		return 0;
	}
	
	strcpy(data_directory_path, path_chooser);	
	strcat(data_directory_path, "/HybridNetRLBMIRecord/");
	strcat(data_directory_path, data_directory_name);	
	if ((dir_data_directory = opendir(data_directory_path)) == NULL)
        {
        	printf ("HybridNetRLBMI: ERROR: path: %s does not have %s folder.\n", data_directory_path, data_directory_name);		
		closedir(dir_data_directory);
                return 0;
        }
	closedir(dir_data_directory);	 

	if (! delete_data_files(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "delete_data_directory_v0", "! delete_data_files(data_directory_path)");

	if (rmdir(data_directory_path) != 0) 
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "delete_data_directory_v0", "! remove(data_directory_path)");

	return 1;
}

int write_to_data_files_v0(int num, ...)
{
  	va_list arguments;
	va_start ( arguments, num );   
	va_end ( arguments );

	if (! write_to_in_silico_spike_data())
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "write_to_data_files_v0", "! write_to_in_silico_spike_data");		

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

int load_data_directory_v0(int num, ...)
{

	Network *in_silico_network, *blue_spike_network;
	char *path_chooser;
	unsigned int data_folder_num;
	HybridNetRLBMIData *bmi_data;
  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *); 
	in_silico_network = va_arg ( arguments, Network *); 
	blue_spike_network = va_arg ( arguments, Network *); 
	data_folder_num = va_arg ( arguments, unsigned int); 
	bmi_data = va_arg ( arguments, HybridNetRLBMIData*);
	va_end ( arguments );

	if (! load_main_meta_file(path_chooser, in_silico_network, blue_spike_network, bmi_data))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_data_directory_v0", "! load_main_meta_file");		

	if (! load_data_folder(path_chooser, in_silico_network, blue_spike_network, data_folder_num)) 
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_data_directory_v0", "! load_data_folder");

	return 1;	
}





static int create_main_meta_file(char *main_directory_path)
{
	char  temp_path[600];
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fp;

	unsigned int i, j, k, m;
	Network *in_silico_network =  get_hybrid_net_rl_bmi_data()->in_silico_network;
	Network *blue_spike_network =  get_hybrid_net_rl_bmi_data()->blue_spike_network;
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/meta");
	if ((fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: HybridNetRLBMI: Couldn't create file: %s\n\n", temp_path); return 0; }
		
	fprintf(fp,"----------HybridNetRLBMI - Main Meta File----------\n");
	fprintf(fp,"DATA_FORMAT_VERSION\t%d\n", 0);	
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"CREATION_DATE\t%s", asctime (timeinfo)); 	

/*	fprintf(fp,"LAYER_BASE_SERVO_EXTENSOR_MOTOR\t%u\n",  LAYER_BASE_SERVO_EXTENSOR_MOTOR);
	fprintf(fp,"LAYER_BASE_SERVO_EXTENSOR_INTER\t%u\n", LAYER_BASE_SERVO_EXTENSOR_INTER);
//	fprintf(fp,"LAYER_BASE_SERVO_EXTENSOR_SPINDLE_IA\t%u\n", LAYER_BASE_SERVO_EXTENSOR_SPINDLE_IA);
	fprintf(fp,"LAYER_BASE_SERVO_FLEXOR_MOTOR\t%u\n", LAYER_BASE_SERVO_FLEXOR_MOTOR);
	fprintf(fp,"LAYER_BASE_SERVO_FLEXOR_INTER\t%u\n", LAYER_BASE_SERVO_FLEXOR_INTER);
//	fprintf(fp,"LAYER_BASE_SERVO_FLEXOR_SPINDLE_IA\t%u\n", LAYER_BASE_SERVO_FLEXOR_SPINDLE_IA);
	fprintf(fp,"LAYER_BASE_SERVO_ANGLE_SENS_SPINDLE\t%u\n", LAYER_BASE_SERVO_ANGLE_SENS_SPINDLE);

	fprintf(fp,"LAYER_SHOULDER_SERVO_EXTENSOR_MOTOR\t%u\n",  LAYER_SHOULDER_SERVO_EXTENSOR_MOTOR);
	fprintf(fp,"LAYER_SHOULDER_SERVO_EXTENSOR_INTER\t%u\n", LAYER_SHOULDER_SERVO_EXTENSOR_INTER);
//	fprintf(fp,"LAYER_SHOULDER_SERVO_EXTENSOR_SPINDLE_IA\t%u\n", LAYER_SHOULDER_SERVO_EXTENSOR_SPINDLE_IA);
	fprintf(fp,"LAYER_SHOULDER_SERVO_FLEXOR_MOTOR\t%u\n", LAYER_SHOULDER_SERVO_FLEXOR_MOTOR);
	fprintf(fp,"LAYER_SHOULDER_SERVO_FLEXOR_INTER\t%u\n", LAYER_SHOULDER_SERVO_FLEXOR_INTER);
//	fprintf(fp,"LAYER_SHOULDER_SERVO_FLEXOR_SPINDLE_IA\t%u\n", LAYER_SHOULDER_SERVO_FLEXOR_SPINDLE_IA);
	fprintf(fp,"LAYER_SHOULDER_SERVO_ANGLE_SENS_SPINDLE\t%u\n", LAYER_SHOULDER_SERVO_ANGLE_SENS_SPINDLE);

	fprintf(fp,"LAYER_ELBOW_SERVO_EXTENSOR_MOTOR\t%u\n",  LAYER_ELBOW_SERVO_EXTENSOR_MOTOR);
	fprintf(fp,"LAYER_ELBOW_SERVO_EXTENSOR_INTER\t%u\n", LAYER_ELBOW_SERVO_EXTENSOR_INTER);
//	fprintf(fp,"LAYER_ELBOW_SERVO_EXTENSOR_SPINDLE_IA\t%u\n", LAYER_ELBOW_SERVO_EXTENSOR_SPINDLE_IA);
	fprintf(fp,"LAYER_ELBOW_SERVO_FLEXOR_MOTOR\t%u\n", LAYER_ELBOW_SERVO_FLEXOR_MOTOR);
	fprintf(fp,"LAYER_ELBOW_SERVO_FLEXOR_INTER\t%u\n", LAYER_ELBOW_SERVO_FLEXOR_INTER);
//	fprintf(fp,"LAYER_ELBOW_SERVO_FLEXOR_SPINDLE_IA\t%u\n", LAYER_ELBOW_SERVO_FLEXOR_SPINDLE_IA);
	fprintf(fp,"LAYER_ELBOW_SERVO_ANGLE_SENS_SPINDLE\t%u\n", LAYER_ELBOW_SERVO_ANGLE_SENS_SPINDLE);

	fprintf(fp,"NUM_OF_IN_SILICO_NETWORK_LAYERS\t%u\n",  NUM_OF_IN_SILICO_NETWORK_LAYERS);

	fprintf(fp,"NUM_OF_ANGULAR_SPINDLES\t%u\n", NUM_OF_ANGULAR_SPINDLES);

	fprintf(fp,"NUM_OF_FLEXOR_IA_SPINDLES\t%u\n", NUM_OF_FLEXOR_IA_SPINDLES);
	fprintf(fp,"NUM_OF_EXTENSOR_IA_SPINDLES\t%u\n", NUM_OF_EXTENSOR_IA_SPINDLES);
*/
	fprintf(fp,"learning_rate\t%.15f\n", get_hybrid_net_rl_bmi_data()->learning_rate);
	fprintf(fp,"servo_angle_min_max[BASE_SERVO].min\t%.15f\n", get_hybrid_net_rl_bmi_data()->servo_angle_min_max[BASE_SERVO].min);
	fprintf(fp,"servo_angle_min_max[BASE_SERVO].max\t%.15f\n", get_hybrid_net_rl_bmi_data()->servo_angle_min_max[BASE_SERVO].max);
	fprintf(fp,"servo_angle_min_max[SHOULDER_SERVO].min\t%.15f\n", get_hybrid_net_rl_bmi_data()->servo_angle_min_max[SHOULDER_SERVO].min);
	fprintf(fp,"servo_angle_min_max[SHOULDER_SERVO].max\t%.15f\n", get_hybrid_net_rl_bmi_data()->servo_angle_min_max[SHOULDER_SERVO].max);
	fprintf(fp,"servo_angle_min_max[ELBOW_SERVO].min\t%.15f\n", get_hybrid_net_rl_bmi_data()->servo_angle_min_max[ELBOW_SERVO].min);
	fprintf(fp,"servo_angle_min_max[ELBOW_SERVO].max\t%.15f\n", get_hybrid_net_rl_bmi_data()->servo_angle_min_max[ELBOW_SERVO].max);

/*	fprintf(fp,"angle_sensitive_spindles..........\n");
	for (i = 0; i < NUM_OF_ANGULAR_SPINDLES; i++)
	{
		fprintf(fp,"angle_sensitive_spindles[BASE_SERVO]->spindles[%u]->center_angle\t%.15f\n", i, get_hybrid_net_rl_bmi_data()->angle_sensitive_spindles[BASE_SERVO]->spindles[i].center_angle);
		fprintf(fp,"angle_sensitive_spindles[BASE_SERVO]->spindles[%u]->I_max\t%.15f\n", i, get_hybrid_net_rl_bmi_data()->angle_sensitive_spindles[BASE_SERVO]->spindles[i].I_max);
		fprintf(fp,"angle_sensitive_spindles[BASE_SERVO]->spindles[%u]->I_decay_rate\t%.15f\n", i, get_hybrid_net_rl_bmi_data()->angle_sensitive_spindles[BASE_SERVO]->spindles[i].I_decay_rate);
	}
	for (i = 0; i < NUM_OF_ANGULAR_SPINDLES; i++)
	{
		fprintf(fp,"angle_sensitive_spindles[SHOULDER_SERVO]->spindles[%u]->center_angle\t%.15f\n", i, get_hybrid_net_rl_bmi_data()->angle_sensitive_spindles[SHOULDER_SERVO]->spindles[i].center_angle);
		fprintf(fp,"angle_sensitive_spindles[SHOULDER_SERVO]->spindles[%u]->I_max\t%.15f\n", i, get_hybrid_net_rl_bmi_data()->angle_sensitive_spindles[SHOULDER_SERVO]->spindles[i].I_max);
		fprintf(fp,"angle_sensitive_spindles[SHOULDER_SERVO]->spindles[%u]->I_decay_rate\t%.15f\n", i, get_hybrid_net_rl_bmi_data()->angle_sensitive_spindles[SHOULDER_SERVO]->spindles[i].I_decay_rate);
	}
	for (i = 0; i < NUM_OF_ANGULAR_SPINDLES; i++)
	{
		fprintf(fp,"angle_sensitive_spindles[ELBOW_SERVO]->spindles[%u]->center_angle\t%.15f\n", i, get_hybrid_net_rl_bmi_data()->angle_sensitive_spindles[ELBOW_SERVO]->spindles[i].center_angle);
		fprintf(fp,"angle_sensitive_spindles[ELBOW_SERVO]->spindles[%u]->I_max\t%.15f\n", i, get_hybrid_net_rl_bmi_data()->angle_sensitive_spindles[ELBOW_SERVO]->spindles[i].I_max);
		fprintf(fp,"angle_sensitive_spindles[ELBOW_SERVO]->spindles[%u]->I_decay_rate\t%.15f\n", i, get_hybrid_net_rl_bmi_data()->angle_sensitive_spindles[ELBOW_SERVO]->spindles[i].I_decay_rate);
	}
*/
	fprintf(fp,".PARKER_SOCHACKI_ERROR_TOLERANCE\t%E\n", get_maximum_parker_sochacki_error_tolerance());
	fprintf(fp,".PARKER_SOCHACKI_MAX_ORDER\t%d\n", get_maximum_parker_sochacki_order());

	fprintf(fp,"............blue_spike_network.............\n");
	fprintf(fp,"blue_spike_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n", blue_spike_network->num_of_neurons);
	fprintf(fp,"blue_spike_network->layer_count\t%u\n", blue_spike_network->layer_count);
	for(i = 0; i < blue_spike_network->layer_count; i++)
	{
		fprintf(fp,"layer_num\t%u\n", i);
		fprintf(fp,"address\t%llu\n", (unsigned long long int) blue_spike_network->layers[i]);
		fprintf(fp,"blue_spike_network->layers[%u]->num_of_connections\t%u\n", i, blue_spike_network->layers[i]->num_of_connections);
		for(j = 0; j < blue_spike_network->layers[i]->num_of_connections; j++)
		{
			fprintf(fp,"%u_th_connection_to_layer(address)\t%llu\n", j, (unsigned long long int) blue_spike_network->layers[i]->connected_to_layers[j]);
		}
		fprintf(fp,"neuron_group_count\t%u\n", blue_spike_network->layers[i]->neuron_group_count);	
		for(j = 0; j < blue_spike_network->layers[i]->neuron_group_count; j++)	
		{
			fprintf(fp,"neuron_group_num\t%u\n", j);	
			fprintf(fp,"neuron_count\t%u\n", blue_spike_network->layers[i]->neuron_groups[j]->neuron_count);		
			for (k = 0; k < blue_spike_network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				fprintf(fp,"neuron_num\t%u\n", k);			
				fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n", i, j, k, blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].inhibitory);	
			}
		}			
	}


	fprintf(fp,"............in_silico_network.............\n");
	fprintf(fp,"in_silico_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n", in_silico_network->num_of_neurons);
	fprintf(fp,"in_silico_network->layer_count\t%u\n", in_silico_network->layer_count);
	for(i = 0; i < in_silico_network->layer_count; i++)
	{
		fprintf(fp,"layer_num\t%u\n", i);
		fprintf(fp,"address\t%llu\n", (unsigned long long int) in_silico_network->layers[i]);
		fprintf(fp,"in_silico_network->layers[%u]->num_of_connections\t%u\n", i, in_silico_network->layers[i]->num_of_connections);
		for(j = 0; j < in_silico_network->layers[i]->num_of_connections; j++)
		{
			fprintf(fp,"%u_th_connection_to_layer(address)\t%llu\n", j, (unsigned long long int) in_silico_network->layers[i]->connected_to_layers[j]);
		}
		fprintf(fp,"neuron_group_count\t%u\n", in_silico_network->layers[i]->neuron_group_count);	
		for(j = 0; j < in_silico_network->layers[i]->neuron_group_count; j++)	
		{
			fprintf(fp,"neuron_group_num\t%u\n", j);	
			fprintf(fp,"neuron_count\t%u\n", in_silico_network->layers[i]->neuron_groups[j]->neuron_count);
			for (k = 0; k < in_silico_network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				fprintf(fp,"neuron_num\t%u\n", k);			
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].inhibitory);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params.............\n", i, j, k);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->v);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->u\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->u);
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->a\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->a);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->b\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->b);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->c\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->c);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->d\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->d);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->k\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->k);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->E);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_resting\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->v_resting);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_threshold\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->v_threshold);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_peak\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->v_peak);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->I_inject\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->I_inject);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E_excitatory\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->E_excitatory);
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E_inhibitory\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->E_inhibitory);
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->decay_rate_excitatory\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->decay_rate_excitatory);
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->decay_rate_inhibitory\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->decay_rate_inhibitory);
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->conductance_excitatory\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->conductance_excitatory);
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->conductance_inhibitory\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->conductance_inhibitory);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->k_v_threshold\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->k_v_threshold);	
			}		
		}
	}

	fprintf(fp,"............blue_spike_network axon list.............\n");
	fprintf(fp,"blue_spike_network->layer_count\t%u\n", blue_spike_network->layer_count);
	for(i = 0; i < blue_spike_network->layer_count; i++)
	{
		fprintf(fp,"neuron_group_count\t%u\n", blue_spike_network->layers[i]->neuron_group_count);	
		for(j = 0; j < blue_spike_network->layers[i]->neuron_group_count; j++)	
		{
			fprintf(fp,"neuron_group_num\t%u\n", j);	
			fprintf(fp,"neuron_count\t%u\n", blue_spike_network->layers[i]->neuron_groups[j]->neuron_count);		
			for (k = 0; k < blue_spike_network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				fprintf(fp,"neuron_num\t%u\n", k);			
				fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list.............\n", i, j, k);	
				fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->num_of_axons\t%u\n", i, j, k, blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->num_of_axons);
				for (m = 0; m < blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->num_of_axons; m++)
				{
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]\t%llu\n", i, j, k, m, (unsigned long long int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->to[m]);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->delay[%u]\t%u\n", i, j, k, m, (unsigned int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->delay[m]);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->syn_idx[%u]\t%u\n", i, j, k, m, (unsigned int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m]);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_layer[%u]\t%u\n", i, j, k, m, (unsigned int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->target_layer[m]);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_group[%u]\t%u\n", i, j, k, m, (unsigned int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->target_neuron_group[m]);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_num[%u]\t%u\n", i, j, k, m, (unsigned int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->target_neuron_num[m]);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]->syn_list->synapses[%u].weight\t%.15f\n", i, j, k, m, blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m], blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->to[m]->syn_list->synapses[blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m]].weight);
				}
			}
		}
	}

	fprintf(fp,"............in_silico_network axon list.............\n");
	fprintf(fp,"in_silico_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n", in_silico_network->num_of_neurons);
	fprintf(fp,"in_silico_network->layer_count\t%u\n", in_silico_network->layer_count);
	for(i = 0; i < in_silico_network->layer_count; i++)
	{
		fprintf(fp,"layer_num\t%u\n", i);
		fprintf(fp,"address\t%llu\n", (unsigned long long int) in_silico_network->layers[i]);
		fprintf(fp,"neuron_group_count\t%u\n", in_silico_network->layers[i]->neuron_group_count);	
		for(j = 0; j < in_silico_network->layers[i]->neuron_group_count; j++)	
		{
			fprintf(fp,"neuron_group_num\t%u\n", j);	
			fprintf(fp,"neuron_count\t%u\n", in_silico_network->layers[i]->neuron_groups[j]->neuron_count);
			for (k = 0; k < in_silico_network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				fprintf(fp,"neuron_num\t%u\n", k);			

				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list.............\n", i, j, k);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->num_of_axons\t%u\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->num_of_axons);
				for (m = 0; m < in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->num_of_axons; m++)
				{
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]\t%llu\n", i, j, k, m, (unsigned long long int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->to[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->delay[%u]\t%u\n", i, j, k, m, (unsigned int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->delay[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->syn_idx[%u]\t%u\n", i, j, k, m, (unsigned int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_layer[%u]\t%u\n", i, j, k, m, (unsigned int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->target_layer[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_group[%u]\t%u\n", i, j, k, m, (unsigned int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->target_neuron_group[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_num[%u]\t%u\n", i, j, k, m, (unsigned int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->target_neuron_num[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]->syn_list->synapses[%u].weight\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m], in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->to[m]->syn_list->synapses[in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m]].weight);
				}
			}		
		}
	}

	fprintf(fp,"............in_silico_network synapse list.............\n");
	fprintf(fp,"in_silico_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n", in_silico_network->num_of_neurons);
	fprintf(fp,"in_silico_network->layer_count\t%u\n", in_silico_network->layer_count);
	for(i = 0; i < in_silico_network->layer_count; i++)
	{
		fprintf(fp,"layer_num\t%u\n", i);
		fprintf(fp,"neuron_group_count\t%u\n", in_silico_network->layers[i]->neuron_group_count);	
		for(j = 0; j < in_silico_network->layers[i]->neuron_group_count; j++)	
		{
			fprintf(fp,"neuron_group_num\t%u\n", j);	
			fprintf(fp,"neuron_count\t%u\n", in_silico_network->layers[i]->neuron_groups[j]->neuron_count);
			for (k = 0; k < in_silico_network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				fprintf(fp,"neuron_num\t%u\n", k);			

				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list.............\n", i, j, k);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->num_of_synapses\t%u\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->num_of_synapses);
				for (m = 0; m < in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->num_of_synapses; m++)
				{
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].weight\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->synapses[m].weight);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].type\t%u\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->synapses[m].type);
				}
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].stdp_list.............(as_many_as_num_of_synapses)\n", i, j, k);	
				for (m = 0; m < in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->num_of_synapses; m++)
				{
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].stdp_list->change_stdp_pre_post[%u]\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].stdp_list->change_stdp_pre_post[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].stdp_list->decay_rate_stdp_pre_post[%u]\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].stdp_list->decay_rate_stdp_pre_post[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].stdp_list->change_stdp_post_pre[%u]\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].stdp_list->change_stdp_post_pre[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].stdp_list->decay_rate_stdp_post_pre[%u]\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].stdp_list->decay_rate_stdp_post_pre[m]);
				}
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].eligibility_list.............(as_many_as_num_of_synapses)\n", i, j, k);	
				for (m = 0; m < in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->num_of_synapses; m++)
				{
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].eligibility_list->eligibility_decay_rate[%u]\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].eligibility_list->eligibility_decay_rate[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].eligibility_list->depol_eligibility_change[%u]\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].eligibility_list->depol_eligibility_change[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].eligibility_list->depol_eligibility_memb_volt[%u]\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].eligibility_list->depol_eligibility_memb_volt[m] + in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->v_resting);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].eligibility_list->max_eligibility\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].eligibility_list->max_eligibility);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].eligibility_list->eligibility_rate\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].eligibility_list->eligibility_rate);
				}
			}		
		}
	}

	fprintf(fp,"............initial synaptic weights.............\n");
	for(i = 0; i < in_silico_network->layer_count; i++)
	{
		for(j = 0; j < in_silico_network->layers[i]->neuron_group_count; j++)	
		{
			for (k = 0; k < in_silico_network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				for (m = 0; m < in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->num_of_synapses; m++)
				{
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].weight\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->synapses[m].weight);
				}
			}		
		}
	}	

	fprintf(fp,"............HybridNetRLBMISynapseData.............\n");
/*	fprintf(fp,"blue_spike_2_in_silico_excitatory_connection_probability\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.blue_spike_2_in_silico_excitatory_connection_probability);
	fprintf(fp,"blue_spike_2_in_silico_excitatory_connection_weight_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.blue_spike_2_in_silico_excitatory_connection_weight_max);
	fprintf(fp,"blue_spike_2_in_silico_excitatory_connection_weight_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.blue_spike_2_in_silico_excitatory_connection_weight_min);
	fprintf(fp,"blue_spike_2_in_silico_excitatory_connection_delay_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.blue_spike_2_in_silico_excitatory_connection_delay_max);
	fprintf(fp,"blue_spike_2_in_silico_excitatory_connection_delay_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.blue_spike_2_in_silico_excitatory_connection_delay_min);

	fprintf(fp,"blue_spike_2_in_silico_inhibitory_connection_probability\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.blue_spike_2_in_silico_inhibitory_connection_probability);
	fprintf(fp,"blue_spike_2_in_silico_inhibitory_connection_weight_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.blue_spike_2_in_silico_inhibitory_connection_weight_max);
	fprintf(fp,"blue_spike_2_in_silico_inhibitory_connection_weight_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.blue_spike_2_in_silico_inhibitory_connection_weight_min);
	fprintf(fp,"blue_spike_2_in_silico_inhibitory_connection_delay_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.blue_spike_2_in_silico_inhibitory_connection_delay_max);
	fprintf(fp,"blue_spike_2_in_silico_inhibitory_connection_delay_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.blue_spike_2_in_silico_inhibitory_connection_delay_min);

	fprintf(fp,"extensor_motor_2_flexor_inter_neuron_excitatory_connection_probability\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_motor_2_flexor_inter_neuron_excitatory_connection_probability);
	fprintf(fp,"extensor_motor_2_flexor_inter_neuron_excitatory_connection_weight_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_motor_2_flexor_inter_neuron_excitatory_connection_weight_max);
	fprintf(fp,"extensor_motor_2_flexor_inter_neuron_excitatory_connection_weight_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_motor_2_flexor_inter_neuron_excitatory_connection_weight_min);
	fprintf(fp,"extensor_motor_2_flexor_inter_neuron_excitatory_connection_delay_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_motor_2_flexor_inter_neuron_excitatory_connection_delay_max);
	fprintf(fp,"extensor_motor_2_flexor_inter_neuron_excitatory_connection_delay_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_motor_2_flexor_inter_neuron_excitatory_connection_delay_min);

	fprintf(fp,"extensor_motor_2_flexor_inter_neuron_inhibitory_connection_probability\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_motor_2_flexor_inter_neuron_inhibitory_connection_probability);
	fprintf(fp,"extensor_motor_2_flexor_inter_neuron_inhibitory_connection_weight_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_motor_2_flexor_inter_neuron_inhibitory_connection_weight_max);
	fprintf(fp,"extensor_motor_2_flexor_inter_neuron_inhibitory_connection_weight_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_motor_2_flexor_inter_neuron_inhibitory_connection_weight_min);
	fprintf(fp,"extensor_motor_2_flexor_inter_neuron_inhibitory_connection_delay_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_motor_2_flexor_inter_neuron_inhibitory_connection_delay_max);
	fprintf(fp,"extensor_motor_2_flexor_inter_neuron_inhibitory_connection_delay_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_motor_2_flexor_inter_neuron_inhibitory_connection_delay_min);

	fprintf(fp,"flexor_motor_2_extensor_inter_neuron_excitatory_connection_probability\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_motor_2_extensor_inter_neuron_excitatory_connection_probability);
	fprintf(fp,"flexor_motor_2_extensor_inter_neuron_excitatory_connection_weight_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_motor_2_extensor_inter_neuron_excitatory_connection_weight_max);
	fprintf(fp,"flexor_motor_2_extensor_inter_neuron_excitatory_connection_weight_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_motor_2_extensor_inter_neuron_excitatory_connection_weight_min);
	fprintf(fp,"flexor_motor_2_extensor_inter_neuron_excitatory_connection_delay_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_motor_2_extensor_inter_neuron_excitatory_connection_delay_max);
	fprintf(fp,"flexor_motor_2_extensor_inter_neuron_excitatory_connection_delay_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_motor_2_extensor_inter_neuron_excitatory_connection_delay_min);

	fprintf(fp,"flexor_motor_2_extensor_inter_neuron_inhibitory_connection_probability\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_motor_2_extensor_inter_neuron_inhibitory_connection_probability);
	fprintf(fp,"flexor_motor_2_extensor_inter_neuron_inhibitory_connection_weight_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_motor_2_extensor_inter_neuron_inhibitory_connection_weight_max);
	fprintf(fp,"flexor_motor_2_extensor_inter_neuron_inhibitory_connection_weight_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_motor_2_extensor_inter_neuron_inhibitory_connection_weight_min);
	fprintf(fp,"flexor_motor_2_extensor_inter_neuron_inhibitory_connection_delay_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_motor_2_extensor_inter_neuron_inhibitory_connection_delay_max);
	fprintf(fp,"flexor_motor_2_extensor_inter_neuron_inhibitory_connection_delay_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_motor_2_extensor_inter_neuron_inhibitory_connection_delay_min);

	fprintf(fp,"extensor_inter_2_extensor_motor_neuron_excitatory_connection_probability\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_inter_2_extensor_motor_neuron_excitatory_connection_probability);
	fprintf(fp,"extensor_inter_2_extensor_motor_neuron_excitatory_connection_weight_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_inter_2_extensor_motor_neuron_excitatory_connection_weight_max);
	fprintf(fp,"extensor_inter_2_extensor_motor_neuron_excitatory_connection_weight_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_inter_2_extensor_motor_neuron_excitatory_connection_weight_min);
	fprintf(fp,"extensor_inter_2_extensor_motor_neuron_excitatory_connection_delay_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_inter_2_extensor_motor_neuron_excitatory_connection_delay_max);
	fprintf(fp,"extensor_inter_2_extensor_motor_neuron_excitatory_connection_delay_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_inter_2_extensor_motor_neuron_excitatory_connection_delay_min);

	fprintf(fp,"extensor_inter_2_extensor_motor_neuron_inhibitory_connection_probability\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_inter_2_extensor_motor_neuron_inhibitory_connection_probability);
	fprintf(fp,"extensor_inter_2_extensor_motor_neuron_inhibitory_connection_weight_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_inter_2_extensor_motor_neuron_inhibitory_connection_weight_max);
	fprintf(fp,"extensor_inter_2_extensor_motor_neuron_inhibitory_connection_weight_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_inter_2_extensor_motor_neuron_inhibitory_connection_weight_min);
	fprintf(fp,"extensor_inter_2_extensor_motor_neuron_inhibitory_connection_delay_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_inter_2_extensor_motor_neuron_inhibitory_connection_delay_max);
	fprintf(fp,"extensor_inter_2_extensor_motor_neuron_inhibitory_connection_delay_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.extensor_inter_2_extensor_motor_neuron_inhibitory_connection_delay_min);

	fprintf(fp,"flexor_inter_2_flexor_motor_neuron_excitatory_connection_probability\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_inter_2_flexor_motor_neuron_excitatory_connection_probability);
	fprintf(fp,"flexor_inter_2_flexor_motor_neuron_excitatory_connection_weight_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_inter_2_flexor_motor_neuron_excitatory_connection_weight_max);
	fprintf(fp,"flexor_inter_2_flexor_motor_neuron_excitatory_connection_weight_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_inter_2_flexor_motor_neuron_excitatory_connection_weight_min);
	fprintf(fp,"flexor_inter_2_flexor_motor_neuron_excitatory_connection_delay_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_inter_2_flexor_motor_neuron_excitatory_connection_delay_max);
	fprintf(fp,"flexor_inter_2_flexor_motor_neuron_excitatory_connection_delay_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_inter_2_flexor_motor_neuron_excitatory_connection_delay_min);

	fprintf(fp,"flexor_inter_2_flexor_motor_neuron_inhibitory_connection_probability\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_inter_2_flexor_motor_neuron_inhibitory_connection_probability);
	fprintf(fp,"flexor_inter_2_flexor_motor_neuron_inhibitory_connection_weight_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_inter_2_flexor_motor_neuron_inhibitory_connection_weight_max);
	fprintf(fp,"flexor_inter_2_flexor_motor_neuron_inhibitory_connection_weight_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_inter_2_flexor_motor_neuron_inhibitory_connection_weight_min);
	fprintf(fp,"flexor_inter_2_flexor_motor_neuron_inhibitory_connection_delay_max\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_inter_2_flexor_motor_neuron_inhibitory_connection_delay_max);
	fprintf(fp,"flexor_inter_2_flexor_motor_neuron_inhibitory_connection_delay_min\t%.15f\n", get_hybrid_net_rl_bmi_data()->synapse_data.flexor_inter_2_flexor_motor_neuron_inhibitory_connection_delay_min);
*/
	fprintf(fp,"----------HybridNetRLBMI - End of Main Meta File----------\n");
	fclose(fp);
	return 1;
}


static int create_data_files(TimeStamp rec_start, char *data_directory_path)
{

	if (!create_meta_data(rec_start, data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "create_data_files", "! create_meta_data");
	
	if (!create_in_silico_spike_data(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "create_data_files", "! create_in_silico_spike_data");

	if (! create_in_silico_network_data(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "create_data_files", "! create_in_silico_network_data");

	return 1;
}

static int create_meta_data(TimeStamp rec_start, char *data_directory_path)
{
	char temp[600];
	FILE *fp;
	time_t rawtime;
	struct tm * timeinfo;
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/meta");
	if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: HybridNetRLBMI: Couldn't create file: %s\n\n", temp); return 0; }
	fprintf(fp,"----------HybridNetRLBMI - Meta File----------\n");
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"DATE\t%s", asctime (timeinfo)); 	
	fprintf(fp,"RECORDING START\t%llu\n", rec_start);		
	meta_data_file_ptr = fp;

	return 1;	
}

static int create_in_silico_spike_data(char *data_directory_path)
{
	char temp[600];
	FILE *fp;
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/in_silico_spikes");
	if ((fp = fopen(temp, "wb")) == NULL)  { printf ("ERROR: HybridNetRLBMI: Couldn't create file: %s\n\n", temp); return 0; }

	in_silico_spike_data_file_ptr =  fp;

	return 1;	
}
static int create_in_silico_network_data(char *data_directory_path)
{
	char temp[600];
	FILE *fp;

	strcpy(temp, data_directory_path);
	strcat(temp, "/in_silico_network");
	if ((fp = fopen(temp, "wb")) == NULL)  { printf ("ERROR: HybridNetRLBMI: Couldn't create file: %s\n\n", temp); return 0; }

	in_silico_network_data_file_ptr =  fp;

	return 1;	
}


static int write_to_in_silico_spike_data(void)
{
	unsigned int i;
	SpikeTimeStampItem item;
	SpikeData *spike_data;
	for (i = 0; i < num_of_dedicated_cpu_threads; i++)
	{
		spike_data = in_silico_spike_data_for_recording[i];
		while (get_next_spike_data_item(spike_data, &item))
		{
			fwrite(&(item), sizeof(SpikeTimeStampItem), 1, in_silico_spike_data_file_ptr);
		}
	}	
	return 1;
}

static int close_meta_data(TimeStamp rec_end)
{
	fprintf(meta_data_file_ptr,"RECORDING END\t%llu\n", rec_end);	
	fprintf(meta_data_file_ptr,"----------HybridNetRLBMI - End of Meta File----------\n");
	fclose(meta_data_file_ptr);

	return 1;
}
static int close_in_silico_spike_data(void)
{
	unsigned int i, end_idx;
	SpikeTimeStampItem item;
	SpikeData *spike_data;
	for (i = 0; i < num_of_dedicated_cpu_threads; i++)
	{
		spike_data = in_silico_spike_data_for_recording[i];
		end_idx = in_silico_spike_data_end_idx[i];
		while (get_next_spike_data_item(spike_data, &item))
		{
			if (spike_data->buff_idx_read == end_idx)
				break;
			fwrite(&(item), sizeof(SpikeTimeStampItem), 1, in_silico_spike_data_file_ptr);
		}
		reset_spike_data_read_idx(spike_data);
	}
	fclose(in_silico_spike_data_file_ptr);	
	return 1;
}
static int close_in_silico_network_data(void)
{
	Network *in_silico_network =  get_hybrid_net_rl_bmi_data()->in_silico_network;
	NeuronGroup	*neuron_group;
	Neuron *neuron;
	SynapseList *syn_list;	
	Synapse	*synapses;
	unsigned int i, j, k, m;

	for(i = 0; i < in_silico_network->layer_count; i++)
	{
		for(j = 0; j < in_silico_network->layers[i]->neuron_group_count; j++)	
		{
			neuron_group = in_silico_network->layers[i]->neuron_groups[j];
			for (k = 0; k < neuron_group->neuron_count; k++)
			{
				neuron = &(neuron_group->neurons[k]);
				syn_list = neuron->syn_list;
				synapses = syn_list->synapses;
				for (m = 0; m < syn_list->num_of_synapses; m++)
				{
					fwrite(&(synapses[m].weight), sizeof(SynapticWeight), 1, in_silico_network_data_file_ptr);
				}
				fwrite(neuron->eligibility_list->eligibility_saved, sizeof(double), syn_list->num_of_synapses, in_silico_network_data_file_ptr);
			}		
		}
	}	
	fclose(in_silico_network_data_file_ptr);	
	return 1;
}

static int delete_data_files(char *data_directory_path)
{
	if (! delete_meta_data(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "delete_data_files", "! delete_meta_data(data_directory_path)");
	
	if (! delete_in_silico_spike_data(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "delete_data_files", "! delete_in_silico_spike_data(data_directory_path)");

	if (! delete_in_silico_network_data(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "delete_data_files", "!  delete_in_silico_network_data(data_directory_path)");

	return 1;
}

static int delete_meta_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/meta");
	
	if (remove(temp) != 0)  { printf ("ERROR: HybridNetRLBMI: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;
}

static int delete_in_silico_spike_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/in_silico_spikes");
	
	if (remove(temp) != 0)  { printf ("ERROR: HybridNetRLBMI: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;
}

static int delete_in_silico_network_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/in_silico_network");
	
	if (remove(temp) != 0)  { printf ("ERROR: HybridNetRLBMI: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;
}

static int load_main_meta_file(char *path_chooser, Network *in_silico_network, Network *blue_spike_network, HybridNetRLBMIData *bmi_data)
{
	char temp[1000];
	char line[1000];
	char word[100];
	char *end_ptr;	
	FILE *fp;
	unsigned int i, j, k_iter, m;
	double learning_rate;	
	ServoAngularLimit				servo_angle_min_max[THREE_DOF_ROBOT_NUM_OF_SERVOS];
	double parker_sochacki_error_tolerance;
	double parker_sochacki_max_order;
	unsigned int 		layer_count, neuron_group_count, neuron_count, num_of_connections;
	bool inhibitory;
	double v, u, a, b, c, d, k, E, v_resting, v_threshold, v_peak, I_inject, E_excitatory, E_inhibitory, decay_rate_excitatory, decay_rate_inhibitory, conductance_excitatory, conductance_inhibitory, k_v_threshold;
	Neuron 			*to;
	AxonalDelay		delay;
	SynapseIndex		syn_idx;
	unsigned int		num_of_axons;	
	unsigned int		target_layer;		
	unsigned int		target_neuron_group;	
	unsigned int		target_neuron_num;	

	unsigned int		num_of_synapses;	
	double weight;
	bool syn_type;
	double change_stdp_pre_post, decay_rate_stdp_pre_post, change_stdp_post_pre, decay_rate_stdp_post_pre;
	double eligibility_decay_rate, depol_eligibility_change, max_eligibility, eligibility_rate, depol_eligibility_memb_volt;
	bool first;
	unsigned int randomize_params = 0;
	
	Neuron *this_neuron, *target_neuron;

	strcpy(temp, path_chooser);
	strcat(temp, "/meta");

	if ((fp = fopen(temp, "r")) == NULL) 
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fopen() == NULL.");

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }
	if (strcmp(line, "----------HybridNetRLBMI - Main Meta File----------\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "Not a HybridNetRLBMI - Main Meta File."); }

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // DATA_FORMAT_VERSION	
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // CREATION_DATE	

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // learning_rate
	strcpy(word, line);
	word[13] = 0;
	if (strcmp(word, "learning_rate") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "Not learning_rate line."); }
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	learning_rate = atof(word);

	bmi_data->learning_rate = learning_rate;

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	servo_angle_min_max[BASE_SERVO].min = atof(word);

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	servo_angle_min_max[BASE_SERVO].max = atof(word);

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	servo_angle_min_max[SHOULDER_SERVO].min = atof(word);

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	servo_angle_min_max[SHOULDER_SERVO].max = atof(word);

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	servo_angle_min_max[ELBOW_SERVO].min = atof(word);

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	servo_angle_min_max[ELBOW_SERVO].max = atof(word);

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // PARKER_SOCHACKI_ERROR_TOLERANCE	
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	parker_sochacki_error_tolerance = atof(word);

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // PARKER_SOCHACKI_MAX_ORDER	
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	parker_sochacki_max_order = (int)atof(word);

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // ............blue_spike_network.............
	if (strcmp(line, "............blue_spike_network.............\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "Not a .............in_silico_network............ line."); }
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layer_count\t%u\n
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	layer_count = strtoull(word, &end_ptr, 10);
	for(i = 0; i < layer_count; i++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // layer_num\t%u\n
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // address\t%llu\n
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  //blue_spike_network->layers[%u]->num_of_connections\t%u\n
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
		num_of_connections = strtoull(word, &end_ptr, 10);
		for(j = 0; j < num_of_connections; j++)
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // %u_th_connection_to_layer(address)\t%llu\n
		}
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_count\t%u\n
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
		neuron_group_count = strtoull(word, &end_ptr, 10);
		for(j = 0; j < neuron_group_count; j++)	
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_num\t%u\n
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_count\t%u\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			neuron_count = strtoull(word, &end_ptr, 10);
			if (!add_neuron_nodes_to_layer(blue_spike_network, neuron_count ,i, FALSE)) {
				return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "main", "add_neuron_nodes_to_layer()."); return -1; }	
			for (k_iter = 0; k_iter < neuron_count; k_iter++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_num\t%u\n
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }   // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n
				inhibitory = strtoull(word, &end_ptr, 10);
			}
		}			
	}

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // "............in_silico_network.............\n
	if (strcmp(line, "............in_silico_network.............\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "Not a .............in_silico_network............ line."); }
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // "in_silico_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layer_count\t%u\n
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	layer_count = strtoull(word, &end_ptr, 10);
	for(i = 0; i < layer_count; i++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // layer_num\t%u\n
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // address\t%llu\n
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  //blue_spike_network->layers[%u]->num_of_connections\t%u\n
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
		num_of_connections = strtoull(word, &end_ptr, 10);
		for(j = 0; j < num_of_connections; j++)
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // %u_th_connection_to_layer(address)\t%llu\n
		}
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_count\t%u\n
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
		neuron_group_count = strtoull(word, &end_ptr, 10);

		for(j = 0; j < neuron_group_count; j++)	
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_num\t%u\n
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_count\t%u\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			neuron_count = strtoull(word, &end_ptr, 10);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_num\t%u\n
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }   // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n
			inhibitory = strtoull(word, &end_ptr, 10);
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params.
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			v = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->u\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			u = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->a\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			a = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->b\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			b = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->c\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			c = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->d\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			d = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->k\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			k = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			E = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_resting\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			v_resting = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_threshold\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			v_threshold = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_peak\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			v_peak = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->I_inject\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			I_inject = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E_excitatory\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			E_excitatory = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E_inhibitory\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			E_inhibitory = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->decay_rate_excitatory\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			decay_rate_excitatory = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->decay_rate_inhibitory\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			decay_rate_inhibitory = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->conductance_excitatory\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			conductance_excitatory = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->conductance_inhibitory\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			conductance_inhibitory = atof(word);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->k_v_threshold\t%.15f\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			k_v_threshold = atof(word);

			if (!add_iz_neurons_to_layer(in_silico_network, neuron_count, i, a, b, c+v_resting, d, k, 1.0/E, v_resting, v_threshold+v_resting, v_peak+v_resting, inhibitory, E_excitatory+v_resting, E_inhibitory+v_resting, -1.0/decay_rate_excitatory, -1.0/decay_rate_inhibitory, randomize_params))
				return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	
			first = TRUE;
			for (k_iter = 0; k_iter < neuron_count; k_iter++)
			{
				if (first)
				{
					first = FALSE;
					continue;
				}
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_num\t%u\n
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }   // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n
				inhibitory = strtoull(word, &end_ptr, 10);
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params.
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				v = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->u\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				u = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->a\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				a = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->b\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				b = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->c\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				c = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->d\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				d = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->k\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				k = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				E = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_resting\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				v_resting = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_threshold\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				v_threshold = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_peak\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				v_peak = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->I_inject\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				I_inject = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E_excitatory\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				E_excitatory = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E_inhibitory\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				E_inhibitory = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->decay_rate_excitatory\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				decay_rate_excitatory = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->decay_rate_inhibitory\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				decay_rate_inhibitory = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->conductance_excitatory\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				conductance_excitatory = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->conductance_inhibitory\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				conductance_inhibitory = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->k_v_threshold\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				k_v_threshold = atof(word);

			}		
		}
	}

	if (! parker_sochacki_set_order_tolerance(in_silico_network, parker_sochacki_max_order, parker_sochacki_error_tolerance))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "! parker_sochacki_set_order_tolerance().");

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // ............blue_spike_network axon list.............\n
	if (strcmp(line, "............blue_spike_network axon list.............\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "Not a ............blue_spike_network axon list............. line."); }
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layer_count\t%u\n
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	layer_count = strtoull(word, &end_ptr, 10);
	for(i = 0; i < layer_count; i++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_count\t%u\n
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
		neuron_group_count = strtoull(word, &end_ptr, 10);
		for(j = 0; j < neuron_group_count; j++)	
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_num\t%u\n
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_count\t%u\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			neuron_count = strtoull(word, &end_ptr, 10);	
			for (k_iter = 0; k_iter < neuron_count; k_iter++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_num\t%u\n	
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list.............\n
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->num_of_axons\t%u\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				this_neuron = get_neuron_address(blue_spike_network, i, j, k_iter);					
				num_of_axons = strtoull(word, &end_ptr, 10);	
				for (m = 0; m < num_of_axons; m++)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					to = (Neuron*)strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->delay[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					delay = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->syn_idx[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					syn_idx = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_layer[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					target_layer = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_group[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					target_neuron_group = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_num[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					target_neuron_num = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]->syn_list->synapses[%u].weight\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					weight = atof(word);	

					target_neuron = get_neuron_address(in_silico_network, target_layer, target_neuron_group, target_neuron_num);	
					if (! create_axon_with_values(this_neuron, target_neuron, weight, delay, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY))
 						{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "! create_axon_with_values."); }
				}
			}
		}
	}

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // ............in_silico_network axon list.............\n
	if (strcmp(line, "............in_silico_network axon list.............\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "Not a ............in_silico_network axon list............. line."); }
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layer_count\t%u\n
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	layer_count = strtoull(word, &end_ptr, 10);
	for(i = 0; i < layer_count; i++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // layer_num\t%u\n
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // address\t%llu\n
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_count\t%u\n
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
		neuron_group_count = strtoull(word, &end_ptr, 10);
		for(j = 0; j < neuron_group_count; j++)	
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_num\t%u\n
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_count
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			neuron_count = strtoull(word, &end_ptr, 10);
			for (k_iter = 0; k_iter < neuron_count; k_iter++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_num\t%u\n

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list.............\n
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->num_of_axons\t%u\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				num_of_axons = strtoull(word, &end_ptr, 10);
				this_neuron = get_neuron_address(in_silico_network, i, j, k_iter);					
				for (m = 0; m < num_of_axons; m++)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					to = (Neuron*)strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->delay[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					delay = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->syn_idx[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					syn_idx = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_layer[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					target_layer = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_group[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					target_neuron_group = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_num[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					target_neuron_num = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]->syn_list->synapses[%u].weight\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					weight = atof(word);	

					target_neuron = get_neuron_address(in_silico_network, target_layer, target_neuron_group, target_neuron_num);	
					if (! create_axon_with_values(this_neuron, target_neuron, weight, delay, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY))
 						{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "! create_axon_with_values."); }
				}
			}		
		}
	}

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // ............in_silico_network synapse list.............\n
	if (strcmp(line, "............in_silico_network synapse list.............\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "Not a ............in_silico_network synapse list............. line."); }
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layer_count\t%u\n
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	layer_count = strtoull(word, &end_ptr, 10);	
	for(i = 0; i < layer_count; i++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // layer_num\t%u\n
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->neuron_group_count\t%u\n
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
		neuron_group_count = strtoull(word, &end_ptr, 10);	
		for(j = 0; j < neuron_group_count; j++)	
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_num\t%u\n
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->neuron_count\t%u\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			neuron_count = strtoull(word, &end_ptr, 10);	
			for (k_iter = 0; k_iter < neuron_count; k_iter++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_num\t%u\n

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list.............\n

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->num_of_synapses\t%u\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				num_of_synapses = strtoull(word, &end_ptr, 10);	
				for (m = 0; m < num_of_synapses; m++)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].weight\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					weight = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].type\t%u\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					syn_type = strtoull(word, &end_ptr, 10);	
				}

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].stdp_list.............(as_many_as_num_of_synapses)\n
				this_neuron = get_neuron_address(in_silico_network, i, j, k_iter);	

				allocate_ps_stdp_for_neuron(this_neuron , parker_sochacki_max_order);				

				for (m = 0; m < num_of_synapses; m++)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].stdp_list->change_stdp_pre_post[%u]\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					change_stdp_pre_post = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].stdp_list->decay_rate_stdp_pre_post[%u]\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					decay_rate_stdp_pre_post = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].stdp_list->change_stdp_post_pre[%u]\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					change_stdp_post_pre = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].stdp_list->decay_rate_stdp_post_pre[%u]\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					decay_rate_stdp_post_pre = atof(word);

					if (! submit_ps_stdp_for_synapse(this_neuron , m, change_stdp_pre_post, -1.0 / decay_rate_stdp_pre_post, -1.0 * change_stdp_post_pre, -1.0 / decay_rate_stdp_post_pre))
						{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "! submit_ps_stdp_for_synapse."); }
				}
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].eligibility_list.............(as_many_as_num_of_synapses)\n


				allocate_ps_eligibility_for_neuron(this_neuron , parker_sochacki_max_order);
				for (m = 0; m < num_of_synapses; m++)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].stdp_list->eligibility_decay_rate[%u]\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					eligibility_decay_rate = atof(word);	
				
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].stdp_list->depol_eligibility_change[%u]\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					depol_eligibility_change = atof(word);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].eligibility_list->depol_eligibility_memb_volt[%u]\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					depol_eligibility_memb_volt = atof(word);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].eligibility_list->max_eligibility\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					max_eligibility = atof(word);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].eligibility_list->eligibility_rate\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					eligibility_rate = atof(word);	

					if (! submit_ps_eligibility_for_synapse(this_neuron , m, -1.0 / eligibility_decay_rate, depol_eligibility_change, max_eligibility, eligibility_rate, depol_eligibility_memb_volt))
						{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "! submit_ps_eligibility_for_synapse."); }
				}
			}		
		}
	}

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // "............initial synaptic weights.............\n"
	if (strcmp(line, "............initial synaptic weights.............\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "Not a ..............initial synaptic weights.............. line."); }
	for(i = 0; i < in_silico_network->layer_count; i++)
	{
		for(j = 0; j < in_silico_network->layers[i]->neuron_group_count; j++)	
		{
			for (k_iter = 0; k_iter < in_silico_network->layers[i]->neuron_groups[j]->neuron_count; k_iter++)
			{
				for (m = 0; m < in_silico_network->layers[i]->neuron_groups[j]->neurons[k_iter].syn_list->num_of_synapses; m++)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].weight\t%.15f\n
				}
			}		
		}
	}	

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // ............HybridNetRLBMISynapseData......
/*	for (i = 0; i < 50; i++)    // HybridNetRLBMISynapseData
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  
	}
*/
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }
	if (strcmp(line, "----------HybridNetRLBMI - End of Main Meta File----------\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetRLBMI", "DataFormat_v0", "load_main_meta_file", "Not a HybridNetRLBMI - Main Meta File. Not end of meta file."); }

	set_layer_type_of_the_neurons_in_layer(in_silico_network, LAYER_BASE_SERVO_EXTENSOR_SPINY, NEURON_LAYER_TYPE_OUTPUT);
	set_layer_type_of_the_neurons_in_layer(in_silico_network, LAYER_BASE_SERVO_FLEXOR_SPINY, NEURON_LAYER_TYPE_OUTPUT);

	

	fclose(fp);

	return 1;
}
static int load_data_folder(char *path_chooser, Network *in_silico_network, Network *blue_spike_network, unsigned int data_folder_num)
{
	char data_directory_name[10];
	char data_directory_num[10];
	char data_directory_path[600];
	unsigned int data_directory_cntr, i, j, k, m, count_size = 0, size;
	FILE *fp;
	NeuronGroup	*neuron_group = NULL;
	Neuron			*neuron;	
	SynapseList		*syn_list;
	Synapse			*synapses;
	double dummy;
	
	data_directory_cntr = data_folder_num;

	if (data_directory_cntr <10)
	{
		strcpy(data_directory_name, "dat0000");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);
	}
	else if (data_directory_cntr <100)
	{
		strcpy(data_directory_name, "dat000");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}
	else if (data_directory_cntr <1000)
	{
		strcpy(data_directory_name, "dat00");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}	
	else if (data_directory_cntr <10000)
	{
		strcpy(data_directory_name, "dat0");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}	
	else if (data_directory_cntr <100000)
	{
		strcpy(data_directory_name, "dat");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);			
	}	
	else
	{
		printf("HybridNetRLBMI: ERROR: data directory counter is %d.\n", data_directory_cntr);
		printf("HybridNetRLBMI: ERROR: Supported range is 0<= x <100000.\n\n");		
		return 0;
	}

	strcpy(data_directory_path, path_chooser);	
	strcat(data_directory_path, "/");
	strcat(data_directory_path, data_directory_name);	
	strcat(data_directory_path, "/in_silico_network");	

	if ((fp = fopen(data_directory_path, "rb")) == NULL)  { printf ("ERROR: HybridNetRLBMI: Couldn't read file: %s\n\n", data_directory_path); return 0; }

	fseek (fp , 0 , SEEK_END);
	size = ftell (fp);
	rewind (fp);

	for(i = 0; i < in_silico_network->layer_count; i++)
	{
		for(j = 0; j < in_silico_network->layers[i]->neuron_group_count; j++)	
		{
			neuron_group = in_silico_network->layers[i]->neuron_groups[j];
			for (k = 0; k < neuron_group->neuron_count; k++)
			{
				neuron = &(neuron_group->neurons[k]);
				syn_list = neuron->syn_list;
				synapses = syn_list->synapses;
				for (m = 0; m < syn_list->num_of_synapses; m++)
				{
					if (! fread(&(synapses[m].weight), sizeof(SynapticWeight), 1, fp))   { printf ("ERROR: HybridNetRLBMI: Couldn't read weight info from file: %s\n\n", data_directory_path); fclose(fp); return 0; } 
					printf("%f\n", synapses[m].weight);
					count_size += sizeof(SynapticWeight);
				}
				for (m = 0; m < syn_list->num_of_synapses; m++)
				{
					if (! fread(&dummy, sizeof(double), 1, fp))   { printf ("ERROR: HybridNetRLBMI: Couldn't read eligibility info from file: %s\n\n", data_directory_path); fclose(fp); return 0; } // for eligibility which is not used to load.
					count_size += sizeof(double);
				}
			}		
		}
	}	

	if (count_size != size) { printf ("ERROR: HybridNetRLBMI: Invalid formatted file: %s\n\n", data_directory_path); return 0; } 
		
	fclose(fp);


	return 1;
}
