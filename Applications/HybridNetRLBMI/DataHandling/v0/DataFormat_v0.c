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

	fprintf(fp,"LAYER_BASE_SERVO_EXTENSOR_MOTOR\t%u\n",  LAYER_BASE_SERVO_EXTENSOR_MOTOR);
	fprintf(fp,"LAYER_BASE_SERVO_EXTENSOR_INTER\t%u\n", LAYER_BASE_SERVO_EXTENSOR_INTER);
	fprintf(fp,"LAYER_BASE_SERVO_EXTENSOR_SPINDLE_IA\t%u\n", LAYER_BASE_SERVO_EXTENSOR_SPINDLE_IA);
	fprintf(fp,"LAYER_BASE_SERVO_FLEXOR_MOTOR\t%u\n", LAYER_BASE_SERVO_FLEXOR_MOTOR);
	fprintf(fp,"LAYER_BASE_SERVO_FLEXOR_INTER\t%u\n", LAYER_BASE_SERVO_FLEXOR_INTER);
	fprintf(fp,"LAYER_BASE_SERVO_FLEXOR_SPINDLE_IA\t%u\n", LAYER_BASE_SERVO_FLEXOR_SPINDLE_IA);
	fprintf(fp,"LAYER_BASE_SERVO_ANGLE_SENS_SPINDLE\t%u\n", LAYER_BASE_SERVO_ANGLE_SENS_SPINDLE);

	fprintf(fp,"LAYER_SHOULDER_SERVO_EXTENSOR_MOTOR\t%u\n",  LAYER_SHOULDER_SERVO_EXTENSOR_MOTOR);
	fprintf(fp,"LAYER_SHOULDER_SERVO_EXTENSOR_INTER\t%u\n", LAYER_SHOULDER_SERVO_EXTENSOR_INTER);
	fprintf(fp,"LAYER_SHOULDER_SERVO_EXTENSOR_SPINDLE_IA\t%u\n", LAYER_SHOULDER_SERVO_EXTENSOR_SPINDLE_IA);
	fprintf(fp,"LAYER_SHOULDER_SERVO_FLEXOR_MOTOR\t%u\n", LAYER_SHOULDER_SERVO_FLEXOR_MOTOR);
	fprintf(fp,"LAYER_SHOULDER_SERVO_FLEXOR_INTER\t%u\n", LAYER_SHOULDER_SERVO_FLEXOR_INTER);
	fprintf(fp,"LAYER_SHOULDER_SERVO_FLEXOR_SPINDLE_IA\t%u\n", LAYER_SHOULDER_SERVO_FLEXOR_SPINDLE_IA);
	fprintf(fp,"LAYER_SHOULDER_SERVO_ANGLE_SENS_SPINDLE\t%u\n", LAYER_SHOULDER_SERVO_ANGLE_SENS_SPINDLE);

	fprintf(fp,"LAYER_ELBOW_SERVO_EXTENSOR_MOTOR\t%u\n",  LAYER_ELBOW_SERVO_EXTENSOR_MOTOR);
	fprintf(fp,"LAYER_ELBOW_SERVO_EXTENSOR_INTER\t%u\n", LAYER_ELBOW_SERVO_EXTENSOR_INTER);
	fprintf(fp,"LAYER_ELBOW_SERVO_EXTENSOR_SPINDLE_IA\t%u\n", LAYER_ELBOW_SERVO_EXTENSOR_SPINDLE_IA);
	fprintf(fp,"LAYER_ELBOW_SERVO_FLEXOR_MOTOR\t%u\n", LAYER_ELBOW_SERVO_FLEXOR_MOTOR);
	fprintf(fp,"LAYER_ELBOW_SERVO_FLEXOR_INTER\t%u\n", LAYER_ELBOW_SERVO_FLEXOR_INTER);
	fprintf(fp,"LAYER_ELBOW_SERVO_FLEXOR_SPINDLE_IA\t%u\n", LAYER_ELBOW_SERVO_FLEXOR_SPINDLE_IA);
	fprintf(fp,"LAYER_ELBOW_SERVO_ANGLE_SENS_SPINDLE\t%u\n", LAYER_ELBOW_SERVO_ANGLE_SENS_SPINDLE);

	fprintf(fp,"NUM_OF_IN_SILICO_NETWORK_LAYERS\t%u\n",  NUM_OF_IN_SILICO_NETWORK_LAYERS);

	fprintf(fp,"NUM_OF_ANGULAR_SPINDLES\t%u\n", NUM_OF_ANGULAR_SPINDLES);

	fprintf(fp,"NUM_OF_FLEXOR_IA_SPINDLES\t%u\n", NUM_OF_FLEXOR_IA_SPINDLES);
	fprintf(fp,"NUM_OF_EXTENSOR_IA_SPINDLES\t%u\n", NUM_OF_EXTENSOR_IA_SPINDLES);

	fprintf(fp,"learning_rate\t%.15f\n", get_hybrid_net_rl_bmi_data()->learning_rate);
	fprintf(fp,"servo_angle_min_max[BASE_SERVO].min\t%.15f\n", get_hybrid_net_rl_bmi_data()->servo_angle_min_max[BASE_SERVO].min);
	fprintf(fp,"servo_angle_min_max[BASE_SERVO].max\t%.15f\n", get_hybrid_net_rl_bmi_data()->servo_angle_min_max[BASE_SERVO].max);
	fprintf(fp,"servo_angle_min_max[SHOULDER_SERVO].min\t%.15f\n", get_hybrid_net_rl_bmi_data()->servo_angle_min_max[SHOULDER_SERVO].min);
	fprintf(fp,"servo_angle_min_max[SHOULDER_SERVO].max\t%.15f\n", get_hybrid_net_rl_bmi_data()->servo_angle_min_max[SHOULDER_SERVO].max);
	fprintf(fp,"servo_angle_min_max[ELBOW_SERVO].min\t%.15f\n", get_hybrid_net_rl_bmi_data()->servo_angle_min_max[ELBOW_SERVO].min);
	fprintf(fp,"servo_angle_min_max[ELBOW_SERVO].max\t%.15f\n", get_hybrid_net_rl_bmi_data()->servo_angle_min_max[ELBOW_SERVO].max);

	fprintf(fp,"angle_sensitive_spindles..........\n");
	for (i = 0; i < NUM_OF_ANGULAR_SPINDLES; i++)
	{
		fprintf(fp,"angle_sensitive_spindles[BASE_SERVO]->spindles[%u]->center_angle\t%.15f\n", i, get_hybrid_net_rl_bmi_data()->angle_sensitive_spindles[BASE_SERVO]->spindles[i].center_angle);
		fprintf(fp,"angle_sensitive_spindles[BASE_SERVO]->spindles[%u]->center_angle\t%.15f\n", i, get_hybrid_net_rl_bmi_data()->angle_sensitive_spindles[BASE_SERVO]->spindles[i].I_max);
		fprintf(fp,"angle_sensitive_spindles[BASE_SERVO]->spindles[%u]->center_angle\t%.15f\n", i, get_hybrid_net_rl_bmi_data()->angle_sensitive_spindles[BASE_SERVO]->spindles[i].I_decay_rate);
	}

	fprintf(fp,"............blue_spike_network.............\n");
	fprintf(fp,"blue_spike_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n", blue_spike_network->num_of_neurons);
	fprintf(fp,"blue_spike_network->layer_count\t%u\n", blue_spike_network->layer_count);
	for(i = 0; i < blue_spike_network->layer_count; i++)
	{
		fprintf(fp,"layer_num\t%u\n", i);
		fprintf(fp,"address\t%llu\n", (unsigned long long int) blue_spike_network->layers[i]);
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
				fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list.............\n", i, j, k);	
				fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->num_of_axons\t%u\n", i, j, k, blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->num_of_axons);
				for (m = 0; m < blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->num_of_axons; m++)
				{
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]\t%llu\n", i, j, k, m, (unsigned long long int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->to[m]);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->delay[%u]\t%u\n", i, j, k, m, (unsigned int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->delay[m]);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->syn_idx[%u]\t%u\n", i, j, k, m, (unsigned int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m]);
				}
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

				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list.............\n", i, j, k);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->num_of_axons\t%u\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->num_of_axons);
				for (m = 0; m < in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->num_of_axons; m++)
				{
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]\t%llu\n", i, j, k, m, (unsigned long long int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->to[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->delay[%u]\t%u\n", i, j, k, m, (unsigned int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->delay[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->syn_idx[%u]\t%u\n", i, j, k, m, (unsigned int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m]);
				}
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
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].eligibility_list->depol_eligibility_change_scaler[%u]\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].eligibility_list->depol_eligibility_change_scaler[m]);
				}
			}		
		}
	}


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

