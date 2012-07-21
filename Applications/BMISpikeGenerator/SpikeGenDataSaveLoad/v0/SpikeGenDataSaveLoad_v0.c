#include "SpikeGenDataSaveLoad_v0.h"

static bool create_main_meta_file(SpikeGenData *spike_gen_data, char * main_dir_path);

static bool save_notes(char* main_dir_path, GtkWidget *text_view);
static bool save_neuron_params(Network * network, char *main_dir_path);
static bool save_trial_start_available_current_pattern_templates(SpikeGenData *spike_gen_data, char* main_dir_path);
static bool save_in_refractory_current_pattern_templates(SpikeGenData *spike_gen_data, char* main_dir_path);
static bool save_in_trial_current_pattern_templates(SpikeGenData *spike_gen_data, char* main_dir_path);

static bool load_main_meta_file(SpikeGenData *spike_gen_data, char* main_dir_path);
static bool load_notes(char* main_dir_path, GtkWidget *text_view);
static bool load_neuron_params(Network * network, char *main_dir_path);
static bool load_trial_start_available_current_pattern_templates(SpikeGenData *spike_gen_data, char* main_dir_path);
static bool load_in_refractory_current_pattern_templates(SpikeGenData *spike_gen_data, char* main_dir_path);
static bool load_in_trial_current_pattern_templates(SpikeGenData *spike_gen_data, char* main_dir_path);

int create_spike_gen_data_directory_v0(int num, ...)
{
	FILE *fp;
	char *path_chooser;
	SpikeGenData *spike_gen_data;
	DIR	*dir_main_folder;
	char main_dir_path[600];
  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *);
     	spike_gen_data = va_arg ( arguments, SpikeGenData *);   	
	va_end ( arguments );

	strcpy(main_dir_path, path_chooser);	
	if ((fp = fopen("./path_initial_directory", "w")) == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "create_spike_gen_data_directory_v0", "fopen() == NULL: Couldn' t create ./path_initial_directory file.");
	fprintf(fp, "%s/\n", main_dir_path);		
	fclose(fp);	
	strcat(main_dir_path, "/SpikeGenData");
	if ((dir_main_folder = opendir(main_dir_path)) != NULL)
	{
		closedir(dir_main_folder);
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "create_spike_gen_data_directory_v0", "Directory has a SpikeGenData folder already, select another directory.");
	}

	mkdir(main_dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "create_spike_gen_data_directory_v0", "Created SpikeGenData.");
       
	if (!create_main_meta_file(spike_gen_data, main_dir_path))
		return 0;
		
	return 1;
}

static bool create_main_meta_file(SpikeGenData *spike_gen_data, char * main_dir_path)
{
	char  temp_path[600];
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fp;
	unsigned int  i,j;
	Network *network = spike_gen_data->network;
	TrialTypesData *trial_types_data = spike_gen_data->trial_types_data;
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
	
	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "create_main_meta_file", "Saving main meta file...");

 	strcpy(temp_path, main_dir_path);
 	strcat(temp_path, "/meta");
	if ((fp = fopen(temp_path, "w")) == NULL)  		
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "create_main_meta_file", "fopen() == NULL: Couldn' t create main mate file.");
		
	fprintf(fp,"----------SpikeGeneratorData - Main Meta File----------\n");
	fprintf(fp,"DATA_FORMAT_VERSION\t%d\n", 0);	
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"CREATION_DATE\t%s", asctime (timeinfo)); 	
	fprintf(fp,"NUM_OF_LAYERS\t%d\n",	network->layer_count);
	for (i=0; i<network->layer_count; i++)
	{
		ptr_layer = network->layers[i];
		fprintf(fp,"NUM_OF_NEURON_GROUPS_IN_LAYER_%d\t%d\n",	i, ptr_layer->neuron_group_count);	
		for (j=0; j<ptr_layer->neuron_group_count; j++)
		{
			ptr_neuron_group = ptr_layer->neuron_groups[j];
			fprintf(fp,"NUM_OF_NEURONS_IN_LAYER_%d_NEURON_GROUP_%d\t%d\n", i, j, ptr_neuron_group->neuron_count);		
		}			
	}
	fprintf(fp,"NUM_OF_TRIAL_START_AVAILABLE_CURRENTS\t%u\n", spike_gen_data->current_templates->num_of_trial_start_available_currents);
	fprintf(fp,"NUM_OF_IN_REFRACTORY_CURRENTS\t%u\n", spike_gen_data->current_templates->num_of_in_refractory_currents);
	fprintf(fp,"NUM_OF_TRIAL_TYPES\t%u\n", trial_types_data->num_of_trial_types);
	for (i = 0; i < trial_types_data->num_of_trial_types; i ++)
		fprintf(fp,"NUM_OF_IN_TRIAL_CURRENTS_FOR_TRIAL_TYPE_%u\t%u\n", trial_types_data->types[i].type, spike_gen_data->current_templates->num_of_in_trial_currents);  // should be equal for each trialk type, no problem. ( see comment in CurrentTemplate strucy: use OKEK of number of currents for each trial having different numbers of trial currents (for random trial current selection))
	fprintf(fp,"INJECTED_CURRENT_PATTERN_SAMPLING_INTERVAL\t%llu\n", PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
	fprintf(fp,"PARKER_SOCHACKI_ERROR_TOLERANCE\t%.16E\n", get_maximum_parker_sochacki_error_tolerance() );
	fprintf(fp,"PARKER_SOCHACKI_MAX_ORDER\t%d\n", get_maximum_parker_sochacki_order());	
	fprintf(fp,"----------SpikeGeneratorData - End of Main Meta File----------\n");
	fclose(fp);
	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "create_main_meta_file", "Saving main meta file...complete");
	return 1;
}


int save_spike_gen_data_directory_v0(int num, ...)
{
	char *path_chooser;
	SpikeGenData *spike_gen_data;
	DIR	*dir_main_folder;
	char main_dir_path[600];
	GtkWidget *text_view; 
  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *);  
    	spike_gen_data = va_arg ( arguments, SpikeGenData *);  	
    	text_view = va_arg ( arguments, GtkWidget *);   	
	va_end ( arguments );

	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_spike_gen_data_directory_v0", "Saving spike_gen_data files...");

	strcpy(main_dir_path, path_chooser);						// SpikePatternGeneratorData should be selected to save 
	if ((dir_main_folder = opendir(main_dir_path)) == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_spike_gen_data_directory_v0", "opendir() == NULL: Couldn' t open SpikeGenData directory.");
	
	closedir(dir_main_folder);

        if(! save_notes(main_dir_path, text_view))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_spike_gen_data_directory_v0", "! save_notes().");

	if (! save_neuron_params(spike_gen_data->network, main_dir_path))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_spike_gen_data_directory_v0", "! save_neuron_params().");

        if(! save_trial_start_available_current_pattern_templates(spike_gen_data, main_dir_path))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_spike_gen_data_directory_v0", "! save_trial_start_available_current_pattern_templates().");

        if(! save_in_refractory_current_pattern_templates(spike_gen_data, main_dir_path))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_spike_gen_data_directory_v0", "! save_trial_start_available_current_pattern_templates().");

        if(! save_in_trial_current_pattern_templates(spike_gen_data, main_dir_path))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_spike_gen_data_directory_v0", "! save_in_trial_current_pattern_templates().");

	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_spike_gen_data_directory_v0", "Saving spike_gen_data files...complete");

	return 1;
}

static bool save_notes(char* main_dir_path, GtkWidget *text_view)
{
	char *text_buffer;
	GtkTextIter start, end;
	int char_count;
	char  temp_path[600];
	int i;
	FILE *fp;
	
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));	
       	gtk_text_buffer_get_start_iter ( buffer, &start);
        gtk_text_buffer_get_end_iter ( buffer, &end);
        text_buffer = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);
	char_count = gtk_text_buffer_get_char_count(buffer);

 	strcpy(temp_path, main_dir_path);
 	strcat(temp_path, "/notes");
	if ((fp = fopen(temp_path, "w")) == NULL) 
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_notes", "fopen() == NULL: Couldn' t create /notes file.");

	for (i = 0; i < char_count; i++)
	{
		fprintf(fp, "%c", text_buffer[i]);		
	}
	fclose(fp);

	if ((fp = fopen("./initial_note", "w")) == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_notes", "fopen() == NULL: Couldn' t create ./initial_note file.");
	for (i = 0; i < char_count; i++)
	{
		fprintf(fp, "%c", text_buffer[i]);		
	}
	fclose(fp);
					
	return TRUE;
}

static bool save_trial_start_available_current_pattern_templates(SpikeGenData *spike_gen_data, char* main_dir_path)
{
	FILE *fp;
	unsigned int i,j,k,m,n;
	char current_pattern_name[100];
	char current_pattern_num[10];
	char current_pattern_path[700];	
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	CurrentPatternTemplate	*trial_start_available_currents = current_templates->trial_start_available_currents;
	Network *network = spike_gen_data->network;
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
	for (m = 0; m < current_templates->num_of_trial_start_available_currents; m++)
	{
		if (m < 10)
		{
			strcpy(current_pattern_name, "/current_pattern_template_trial_start_available_0");
			sprintf(current_pattern_num, "%u" , m);
			strcat(current_pattern_name, current_pattern_num);			
		}
		else if (m < 100)
		{
			strcpy(current_pattern_name, "/current_pattern_template_trial_start_available_");
			sprintf(current_pattern_num, "%u" , m);
			strcat(current_pattern_name, current_pattern_num);			
		}		
		else
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_trial_start_available_current_pattern_templates", "spike_gen_data->num_of_trial_start_available_currents >100 .");

		strcpy(current_pattern_path, main_dir_path);
	 	strcat(current_pattern_path, current_pattern_name);
		if ((fp = fopen(current_pattern_path, "w")) == NULL) 
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_trial_start_available_current_pattern_templates", "fopen() == NULL: Couldn' t create /current_pattern_template_trial_start_available file.");

		fprintf(fp,"----------SpikeGeneratorData - TrialStartAvailableCurrentPattern File----------\n");
		fprintf(fp,"LENGTH\t%llu\n", trial_start_available_currents[m].template_length);
		fprintf(fp,"NUM_OF_SAMPLES\t%u\n", trial_start_available_currents[m].num_of_template_samples);
		fprintf(fp,"------------NOISE_PARAMS------------\n");
		for (i=0; i < network->layer_count; i++)
		{
			ptr_layer = network->layers[i];	
			for (j=0; j<ptr_layer->neuron_group_count; j++)
			{
				fprintf(fp,"Layer %u NeuronGroup:%u\n", i, j);		
				ptr_neuron_group = ptr_layer->neuron_groups[j];
				for (k=0; k<ptr_neuron_group->neuron_count; k++)
				{
					fprintf(fp, "%llu\t%.16E\n", trial_start_available_currents[m].noise_params[i][j][k].noise_addition_interval, trial_start_available_currents[m].noise_params[i][j][k].noise_variance);
				}
			}
		}	
		fprintf(fp,"------------TEMPLATES------------\n");
		for (i=0; i < network->layer_count; i++)
		{
			ptr_layer = network->layers[i];	
			for (j=0; j<ptr_layer->neuron_group_count; j++)
			{
				ptr_neuron_group = ptr_layer->neuron_groups[j];
				for (k=0; k<ptr_neuron_group->neuron_count; k++)
				{
					for (n = 0; n <  trial_start_available_currents[m].num_of_template_samples; n++)		
						fprintf(fp, "%.16E\n", trial_start_available_currents[m].template_samples[n].current_sample[i][j][k]);
				}
			}
		}	
		fprintf(fp,"----------SpikeGeneratorData - End of TrialStartAvailableCurrentPattern File----------\n");		
		fclose(fp);
	}
	return TRUE;
}

static bool save_in_refractory_current_pattern_templates(SpikeGenData *spike_gen_data, char* main_dir_path)
{
	FILE *fp;
	unsigned int i,j,k,m,n;
	char current_pattern_name[100];
	char current_pattern_num[10];
	char current_pattern_path[700];	
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	CurrentPatternTemplate	*in_refractory_currents = current_templates->in_refractory_currents;
	Network *network = spike_gen_data->network;
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
	for (m = 0; m < current_templates->num_of_in_refractory_currents; m++)
	{
		if (m < 10)
		{
			strcpy(current_pattern_name, "/current_pattern_template_in_refractory_0");
			sprintf(current_pattern_num, "%u" , m);
			strcat(current_pattern_name, current_pattern_num);			
		}
		else if (m < 100)
		{
			strcpy(current_pattern_name, "/current_pattern_template_in_refractory_");
			sprintf(current_pattern_num, "%u" , m);
			strcat(current_pattern_name, current_pattern_num);			
		}		
		else
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_in_refractory_current_pattern_templates", "spike_gen_data->num_of_in_refractory_currents >100 .");

		strcpy(current_pattern_path, main_dir_path);
	 	strcat(current_pattern_path, current_pattern_name);
		if ((fp = fopen(current_pattern_path, "w")) == NULL) 
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_in_refractory_current_pattern_templates", "fopen() == NULL: Couldn' t create /current_pattern_template_in_refractory file.");

		fprintf(fp,"----------SpikeGeneratorData - InRefractoryCurrentPattern File----------\n");
		fprintf(fp,"LENGTH\t%llu\n", in_refractory_currents[m].template_length);
		fprintf(fp,"NUM_OF_SAMPLES\t%u\n", in_refractory_currents[m].num_of_template_samples);
		fprintf(fp,"------------NOISE_PARAMS------------\n");
		for (i=0; i < network->layer_count; i++)
		{
			ptr_layer = network->layers[i];	
			for (j=0; j<ptr_layer->neuron_group_count; j++)
			{
				fprintf(fp,"Layer %u NeuronGroup:%u\n", i, j);		
				ptr_neuron_group = ptr_layer->neuron_groups[j];
				for (k=0; k<ptr_neuron_group->neuron_count; k++)
				{
					fprintf(fp, "%llu\t%.16E\n", in_refractory_currents[m].noise_params[i][j][k].noise_addition_interval, in_refractory_currents[m].noise_params[i][j][k].noise_variance);
				}
			}
		}	
		fprintf(fp,"------------TEMPLATES------------\n");
		for (i=0; i < network->layer_count; i++)
		{
			ptr_layer = network->layers[i];	
			for (j=0; j<ptr_layer->neuron_group_count; j++)
			{
				ptr_neuron_group = ptr_layer->neuron_groups[j];
				for (k=0; k<ptr_neuron_group->neuron_count; k++)
				{
					for (n = 0; n <  in_refractory_currents[m].num_of_template_samples; n++)		
						fprintf(fp, "%.16E\n", in_refractory_currents[m].template_samples[n].current_sample[i][j][k]);
				}
			}
		}	
		fprintf(fp,"----------SpikeGeneratorData - End of InRefractoryCurrentPattern File----------\n");		
		fclose(fp);
	}

	return TRUE;
}

static bool save_in_trial_current_pattern_templates(SpikeGenData *spike_gen_data, char* main_dir_path)
{
	FILE *fp;
	unsigned int i,j,k,m,n, p;
	char current_pattern_name[100];
	char current_pattern_num[10];
	char current_pattern_path[700];	
	char trial_type_num[10];
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	CurrentPatternTemplate	**in_trial_currents = current_templates->in_trial_currents;
	Network *network = spike_gen_data->network;
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
	TrialTypesData *trial_types_data = spike_gen_data->trial_types_data;

	for (p = 0; p < trial_types_data->num_of_trial_types; p++)
	{
		for (m = 0; m < current_templates->num_of_in_trial_currents; m++)
		{
			sprintf(trial_type_num, "%u_" , trial_types_data->types[p].type);
			strcpy(current_pattern_name, "/current_pattern_template_in_trial_");
			strcat(current_pattern_name, trial_type_num);	
			if (m < 10)
			{
				sprintf(current_pattern_num, "%u" , m);
				strcat(current_pattern_name, current_pattern_num);			
			}
			else if (m < 100)
			{
				sprintf(current_pattern_num, "%u" , m);
				strcat(current_pattern_name, current_pattern_num);			
			}		
			else
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_in_trial_current_pattern_templates", "spike_gen_data->num_of_in_trial_currents >100 .");

			strcpy(current_pattern_path, main_dir_path);
	 		strcat(current_pattern_path, current_pattern_name);
			if ((fp = fopen(current_pattern_path, "w")) == NULL) 
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_in_trial_current_pattern_templates", "fopen() == NULL: Couldn' t create /current_pattern_template_in_trial file.");

			fprintf(fp,"----------SpikeGeneratorData - InTrialCurrentPattern File----------\n");
			fprintf(fp,"LENGTH\t%llu\n", in_trial_currents[p][m].template_length);
			fprintf(fp,"NUM_OF_SAMPLES\t%u\n", in_trial_currents[p][m].num_of_template_samples);
			fprintf(fp,"------------NOISE_PARAMS------------\n");
			for (i=0; i < network->layer_count; i++)
			{
				ptr_layer = network->layers[i];	
				for (j=0; j<ptr_layer->neuron_group_count; j++)
				{
					fprintf(fp,"Layer %u NeuronGroup:%u\n", i, j);		
					ptr_neuron_group = ptr_layer->neuron_groups[j];
					for (k=0; k<ptr_neuron_group->neuron_count; k++)
					{
						fprintf(fp, "%llu\t%.16E\n", in_trial_currents[p][m].noise_params[i][j][k].noise_addition_interval, in_trial_currents[p][m].noise_params[i][j][k].noise_variance);
					}
				}
			}	
			fprintf(fp,"------------TEMPLATES------------\n");
			for (i=0; i < network->layer_count; i++)
			{
				ptr_layer = network->layers[i];	
				for (j=0; j<ptr_layer->neuron_group_count; j++)
				{
					ptr_neuron_group = ptr_layer->neuron_groups[j];
					for (k=0; k<ptr_neuron_group->neuron_count; k++)
					{
						for (n = 0; n <  in_trial_currents[p][m].num_of_template_samples; n++)		
							fprintf(fp, "%.16E\n", in_trial_currents[p][m].template_samples[n].current_sample[i][j][k]);
					}
				}
			}	
			fprintf(fp,"----------SpikeGeneratorData - End of InTrialCurrentPattern File----------\n");		
			fclose(fp);
		}
	}
	return TRUE;
}


static bool save_neuron_params(Network * network, char *main_dir_path)
{
	char  temp_path[600];
	FILE *fp;
	int i, m, n;    // not to confuse with neuron' s k
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
	Neuron		*ptr_neuron;

	//	double v;  load it from inial_neuron_dynamics log
	double a;
	double b;
	double c;
	double d;
	double k;
	double C;
	double v_resting;
	double v_threshold;
	double v_peak;
//	double I_inject;
	bool inhibitory;
	double E_excitatory;
	double E_inhibitory;
	double tau_excitatory;
	double tau_inhibitory;

 	strcpy(temp_path, main_dir_path);
 	strcat(temp_path, "/neuron_parameters");
	if ((fp = fopen(temp_path, "w")) == NULL)  
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "save_notes", "fopen() == NULL: Couldn' t create /NeuronParameters file.");
	
	fprintf(fp,"----------SpikeGeneratorData - NeuronParameters File----------\n");		
	for (i=0; i<network->layer_count; i++)
	{
		ptr_layer = network->layers[i];			
		for (m=0; m<ptr_layer->neuron_group_count; m++)
		{
			ptr_neuron_group = ptr_layer->neuron_groups[m];
			for (n=0; n<ptr_neuron_group->neuron_count; n++)
			{
				ptr_neuron = &(ptr_neuron_group->neurons[n]);	
				a = ptr_neuron->iz_params->a;
				b = ptr_neuron->iz_params->b;
				c = ptr_neuron->iz_params->c + ptr_neuron->iz_params->v_resting;
				d = ptr_neuron->iz_params->d;
				k = ptr_neuron->iz_params->k;
				C = 1.0/ptr_neuron->iz_params->E;
				v_resting = ptr_neuron->iz_params->v_resting;
				v_threshold = ptr_neuron->iz_params->v_threshold + ptr_neuron->iz_params->v_resting;
				v_peak = ptr_neuron->iz_params->v_peak + ptr_neuron->iz_params->v_resting;
				inhibitory = ptr_neuron->inhibitory;
				E_excitatory = ptr_neuron->iz_params->E_excitatory + ptr_neuron->iz_params->v_resting;
				E_inhibitory = ptr_neuron->iz_params->E_inhibitory + ptr_neuron->iz_params->v_resting;
				tau_excitatory = -1.0/ptr_neuron->iz_params->decay_rate_excitatory;
				tau_inhibitory = -1.0/ptr_neuron->iz_params->decay_rate_inhibitory;								
				fprintf(fp,"Layer: %d\tNeuron Group: %d\tNeuron: %d\n", i, m, n);												
				fprintf(fp, "%.16E\n%.16E\n%.16E\n%.16E\n%.16E\n%.16E\n%.16E\n%.16E\n%.16E\n%d\n%.16E\n%.16E\n%.16E\n%.16E\n", a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory);
			}
		}			
	}
	fprintf(fp,"----------SpikeGeneratorData - End of NeuronParameters File----------\n");			
	fclose(fp);

	return TRUE;
}

int load_spike_gen_data_directory_v0(int num, ...)
{
	char *path_chooser;
	SpikeGenData *spike_gen_data;
	DIR	*dir_main_folder;
	char main_dir_path[600];
	GtkWidget *text_view; 
  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *);  
    	spike_gen_data = va_arg ( arguments, SpikeGenData *);  	
    	text_view = va_arg ( arguments, GtkWidget *);   	
	va_end ( arguments );

	strcpy(main_dir_path, path_chooser);						// SpikePatternGeneratorData should be selected to load 
	if ((dir_main_folder = opendir(main_dir_path)) == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_spike_gen_data_directory_v0", "opendir() == NULL: Couldn' t open SpikeGenData directory.");
	closedir (dir_main_folder);

	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_spike_gen_data_directory_v0", "Loading spike_gen_data files...");

        if(! load_main_meta_file(spike_gen_data, main_dir_path))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_spike_gen_data_directory_v0", "! load_main_meta_file().");
        if(! load_notes(main_dir_path, text_view))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_spike_gen_data_directory_v0", "! load_notes().");
        if(! load_neuron_params(spike_gen_data->network, main_dir_path))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_spike_gen_data_directory_v0", "! load_neuron_params().");
        if(! load_trial_start_available_current_pattern_templates(spike_gen_data, main_dir_path))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_spike_gen_data_directory_v0", "! load_trial_start_available_current_pattern_templates().");
        if(! load_in_refractory_current_pattern_templates(spike_gen_data, main_dir_path))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_spike_gen_data_directory_v0", "! load_trial_start_available_current_pattern_templates().");
        if(! load_in_trial_current_pattern_templates(spike_gen_data, main_dir_path))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_spike_gen_data_directory_v0", "! load_in_trial_current_pattern_templates().");

	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_spike_gen_data_directory_v0", "Loading spike_gen_data files...complete.");

	return 1;
}

static bool load_main_meta_file(SpikeGenData *spike_gen_data, char* main_dir_path)
{
	char  temp_path[600];
	char line[1000];
	char word[100];
	char *end_ptr;	
	FILE *fp;
	unsigned int num_of_layers, num_of_neuron_groups_in_layer, num_of_neurons_in_neuron_group;
	unsigned int i, j;	
	unsigned int num_of_trial_start_available_currents, num_of_in_refractory_currents, num_of_in_trial_currents;
	unsigned int parker_sochacki_max_order;
	double parker_sochacki_error_tolerance;
 	strcpy(temp_path, main_dir_path);
 	strcat(temp_path, "/meta");
	if ((fp = fopen(temp_path, "r")) == NULL)  
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fopen() == NULL: Couldn' t read /meta file.");

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }
	if (strcmp(line, "----------SpikeGeneratorData - Main Meta File----------\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "Not a SpikeGeneratorData meta file."); }
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }   // "DATA_FORMAT_VERSION\t%d\n"
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }   // 	"CREATION_DATE\t%s" 
	spike_gen_data->network = allocate_network(spike_gen_data->network);	 // deallocates previously allocated network and brings a new one.
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }   //	"NUM_OF_LAYERS\t%d\n",
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "!get_word_in_line."); }
	num_of_layers = strtoull(word, &end_ptr, 10);
	for (i=0; i < num_of_layers; i++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }   //NUM_OF_NEURON_GRPS
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "!get_word_in_line."); }
		num_of_neuron_groups_in_layer = strtoull(word, &end_ptr, 10);
		for (j=0; j<num_of_neuron_groups_in_layer; j++)
		{
			if (fgets(line, sizeof line, fp ) == NULL) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }   //	"NUM_OF_NEURONS
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "!get_word_in_line."); }
			num_of_neurons_in_neuron_group = strtoull(word, &end_ptr, 10);
			if (!add_iz_neurons_to_layer(spike_gen_data->network, num_of_neurons_in_neuron_group, i, 0, 0, 0, 0, 0, 100.0, 0, 0, 0, 0, 0, 0, 1.0, 1.0, 0))	// create neurons and neuron groups with no parameter.
				{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "! add_iz_neurons_to_layer."); }
		}			
	}
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }   //NUM_OF_TRIAL_START_AVAI",
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "!get_word_in_line."); }
	num_of_trial_start_available_currents = strtoull(word, &end_ptr, 10);
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }   //IN_REFRACTORY_CURRENTS",
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "!get_word_in_line."); }
	num_of_in_refractory_currents = strtoull(word, &end_ptr, 10);
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }   //NUM_OF_TRIAL_TYPES",
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "!get_word_in_line."); }
	if (spike_gen_data->trial_types_data->num_of_trial_types != strtoull(word, &end_ptr, 10))
		{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "Number of trials types in shared memory trials data is not same the one read from meta."); }
	for (i = 0; i < spike_gen_data->trial_types_data->num_of_trial_types; i++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }   //NUM_OF_IN_TRIAL_CURRENTS",
		if(!get_word_in_line('_', 8, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "!get_word_in_line."); }	
		if (spike_gen_data->trial_types_data->types[i].type != strtoull(word, &end_ptr, 10))
			{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "trial type in shared memory trials data does not match the one read from meta."); }
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "!get_word_in_line."); }	
		num_of_in_trial_currents = strtoull(word, &end_ptr, 10);
	}
	spike_gen_data->current_templates = allocate_current_templates(spike_gen_data->network, spike_gen_data->trial_types_data, spike_gen_data->current_templates , num_of_trial_start_available_currents, num_of_in_refractory_currents, num_of_in_trial_currents);
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }   //INJECTED_CURRENT_PATTERN_SAMPLING_INTERVAL
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "!get_word_in_line."); }
	if (strtoull(word, &end_ptr, 10) != PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)
		{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "INJECTED_CURRENT_PATTERN_SAMPLING_INTERVAL != PARKER_SOCHACKI_INTEGRATION_STEP_SIZE."); }
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }   // PARKER_SOCHACKI_ERROR_TOLERANCE
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "!get_word_in_line."); }
	parker_sochacki_error_tolerance = atof(word);
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }   // PARKER_SOCHACKI_MAX_ORDER  // Handle after initialization of neurons
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "!get_word_in_line."); }
	parker_sochacki_max_order = strtoull(word, &end_ptr, 10);
	if (!parker_sochacki_set_order_tolerance(spike_gen_data->network, parker_sochacki_max_order, parker_sochacki_error_tolerance))
		{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "!parker_sochacki_set_order_tolerance()."); }		
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }   
	if (strcmp(line, "----------SpikeGeneratorData - End of Main Meta File----------\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "Not a SpikeGeneratorData meta file - End did not match."); }
	fclose(fp);
	return TRUE;
}

static bool load_notes(char* main_dir_path, GtkWidget *text_view)
{
	char line[1000];
	char  temp_path[600];	
	FILE *fp;
	unsigned int i = 0;
	GtkTextIter start, end;

 	strcpy(temp_path, main_dir_path);
 	strcat(temp_path, "/notes");	
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));	
       	gtk_text_buffer_get_start_iter ( buffer, &start);
        gtk_text_buffer_get_end_iter ( buffer, &end);
	gtk_text_buffer_delete (buffer, &start, &end);
       	gtk_text_buffer_get_start_iter ( buffer, &start);
        gtk_text_buffer_get_end_iter ( buffer, &end);
                
        if ((fp = fopen(temp_path, "r")) == NULL)  
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_notes", "fopen() == NULL: Couldn' t read /notes file.");

	while (fgets(line, sizeof line, fp ) != NULL) 
	{ 
       		gtk_text_buffer_get_end_iter ( buffer, &end);   				
		gtk_text_buffer_insert(buffer, &end, line, -1);
		i++;
		if (i == 200)
		{
			print_message(WARNING_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_notes", "/notes file has more than 200 lines(upper limit)");
       			break; 				 				
		}
	}
	fclose(fp);       		
		
	return TRUE;
}

static bool load_neuron_params(Network * network, char *main_dir_path)
{
	char  temp_path[600];
	FILE *fp;
	unsigned int i, m, n;
	char  line[1000];	
	int line_cntr = 0;	
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
	Neuron		*ptr_neuron;

	double a;
	double b;
	double c;
	double d;
	double k;
	double C;
	double v_resting;
	double v_threshold;
	double v_peak;
	bool inhibitory;
	double E_excitatory;
	double E_inhibitory;
	double tau_excitatory;
	double tau_inhibitory;

 	strcpy(temp_path, main_dir_path);
 	strcat(temp_path, "/neuron_parameters");
        if ((fp = fopen(temp_path, "r")) == NULL)  
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_neuron_params", "fopen() == NULL: Couldn' t read /neuron_parameters file.");
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_neuron_params", "fgets() == NULL."); }
	if (strcmp(line, "----------SpikeGeneratorData - NeuronParameters File----------\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_neuron_params", "Not a NeuronParameters file."); }

	for (i=0; i<network->layer_count; i++)
	{
		ptr_layer = network->layers[i];			
		for (m=0; m<ptr_layer->neuron_group_count; m++)
		{
			ptr_neuron_group = ptr_layer->neuron_groups[m];
			for (n=0; n<ptr_neuron_group->neuron_count; n++)
			{
				ptr_neuron = &(ptr_neuron_group->neurons[n]);	
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}  
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}  
				a = atof(line);	
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}
				b = atof(line);
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}  
				c = atof(line);	
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}
				d = atof(line);			
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}
				k = atof(line);				
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}
				C = atof(line);					
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}
				v_resting = atof(line);		
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}
				v_threshold = atof(line);						
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}
				v_peak = atof(line);						
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}
				inhibitory = (bool)atof(line);		
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}
				E_excitatory = atof(line);		
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}
				E_inhibitory = atof(line);						
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}
				tau_excitatory = atof(line);						
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, temp_path);  fclose(fp); return 0; } else {line_cntr++;}
				tau_inhibitory = atof(line);																															
				if (!submit_new_iz_neuron_params(network, ptr_neuron, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory))
					{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_neuron_params", "!submit_new_iz_neuron_param()."); }
			}
		}			
	}
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_neuron_params", "fgets() == NULL."); }
	if (strcmp(line, "----------SpikeGeneratorData - End of NeuronParameters File----------\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_neuron_params", "Not a NeuronParameters file - End of file mismatch."); }
	fclose(fp); 
      	
	return TRUE;	
}

static bool load_trial_start_available_current_pattern_templates(SpikeGenData *spike_gen_data, char* main_dir_path)
{
	FILE *fp;
	unsigned int i,j,k,m,n;
	char current_pattern_name[100];
	char current_pattern_num[10];
	char current_pattern_path[700];	
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	CurrentPatternTemplate	*trial_start_available_currents = current_templates->trial_start_available_currents;
	Network *network = spike_gen_data->network;
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
	unsigned int				num_of_template_samples;
	TimeStamp				template_length;
	bool has_unallocated_current_template;
	char  line[1000];	
	char word[100];
	char *end_ptr;
	for (m = 0; m < current_templates->num_of_trial_start_available_currents; m++)
	{
		if (m < 10)
		{
			strcpy(current_pattern_name, "/current_pattern_template_trial_start_available_0");
			sprintf(current_pattern_num, "%u" , m);
			strcat(current_pattern_name, current_pattern_num);			
		}
		else if (m < 100)
		{
			strcpy(current_pattern_name, "/current_pattern_template_trial_start_available_");
			sprintf(current_pattern_num, "%u" , m);
			strcat(current_pattern_name, current_pattern_num);			
		}		
		else
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "spike_gen_data->num_of_trial_start_available_currents >100 .");

		strcpy(current_pattern_path, main_dir_path);
	 	strcat(current_pattern_path, current_pattern_name);
		if ((fp = fopen(current_pattern_path, "r")) == NULL) 
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fopen() == NULL: Couldn' t read /current_pattern_template_trial_start_available file.");

		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
		if (strcmp(line, "----------SpikeGeneratorData - TrialStartAvailableCurrentPattern File----------\n") != 0) 
			{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "Not a TrialStartAvailableCurrentPattern file."); }

		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "!get_word_in_line."); }
		template_length = strtoull(word, &end_ptr, 10);
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "!get_word_in_line."); }
		num_of_template_samples = strtoull(word, &end_ptr, 10);
		if (!submit_current_length_trial_start_available_status(spike_gen_data->network, current_templates, m, template_length, &has_unallocated_current_template))
			{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "!submit_current_length_trial_start_available_status."); }
		//------------NOISE_PARAMS------------
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
		for (i=0; i < network->layer_count; i++)
		{
			ptr_layer = network->layers[i];	
			for (j=0; j<ptr_layer->neuron_group_count; j++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
				ptr_neuron_group = ptr_layer->neuron_groups[j];
				for (k=0; k<ptr_neuron_group->neuron_count; k++)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
					if(!get_word_in_line('\t', 0, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "!get_word_in_line."); }	
					trial_start_available_currents[m].noise_params[i][j][k].noise_addition_interval = strtoull(word, &end_ptr, 10);
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "!get_word_in_line."); }	
					trial_start_available_currents[m].noise_params[i][j][k].noise_variance = atof(word);
				}
			}
		}	
		//------------TEMPLATES------------
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
		for (i=0; i < network->layer_count; i++)
		{
			ptr_layer = network->layers[i];	
			for (j=0; j<ptr_layer->neuron_group_count; j++)
			{
				ptr_neuron_group = ptr_layer->neuron_groups[j];
				for (k=0; k<ptr_neuron_group->neuron_count; k++)
				{
					for (n = 0; n <  trial_start_available_currents[m].num_of_template_samples; n++)	
					{
						if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
						trial_start_available_currents[m].template_samples[n].current_sample[i][j][k] = atof(line);
					}
				}
			}
		}	
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
		if (strcmp(line, "----------SpikeGeneratorData - End of TrialStartAvailableCurrentPattern File----------\n") != 0) 
			{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "No end of file match at TrialStartAvailableCurrentPattern."); }
		fclose(fp);
	}
	if (has_unallocated_current_template)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "has_unallocated_current_template."); 
	return TRUE;
}

static bool load_in_refractory_current_pattern_templates(SpikeGenData *spike_gen_data, char* main_dir_path)
{
	FILE *fp;
	unsigned int i,j,k,m,n;
	char current_pattern_name[100];
	char current_pattern_num[10];
	char current_pattern_path[700];	
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	CurrentPatternTemplate	*in_refractory_currents = current_templates->in_refractory_currents;
	Network *network = spike_gen_data->network;
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
	unsigned int				num_of_template_samples;
	TimeStamp				template_length;
	bool has_unallocated_current_template;
	char  line[1000];	
	char word[100];
	char *end_ptr;
	for (m = 0; m < current_templates->num_of_in_refractory_currents; m++)
	{
		if (m < 10)
		{
			strcpy(current_pattern_name, "/current_pattern_template_in_refractory_0");
			sprintf(current_pattern_num, "%u" , m);
			strcat(current_pattern_name, current_pattern_num);			
		}
		else if (m < 100)
		{
			strcpy(current_pattern_name, "/current_pattern_template_in_refractory_");
			sprintf(current_pattern_num, "%u" , m);
			strcat(current_pattern_name, current_pattern_num);			
		}		
		else
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "spike_gen_data->num_of_in_refractory_currents >100 .");

		strcpy(current_pattern_path, main_dir_path);
	 	strcat(current_pattern_path, current_pattern_name);
		if ((fp = fopen(current_pattern_path, "r")) == NULL) 
			return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "fopen() == NULL: Couldn' t read /current_pattern_template_in_refractory file.");

		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "fgets() == NULL."); }
		if (strcmp(line, "----------SpikeGeneratorData - InRefractoryCurrentPattern File----------\n") != 0) 
			{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "Not a InRefractoryCurrentPattern file."); }

		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "fgets() == NULL."); }
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "!get_word_in_line."); }
		template_length = strtoull(word, &end_ptr, 10);
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "fgets() == NULL."); }
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "!get_word_in_line."); }
		num_of_template_samples = strtoull(word, &end_ptr, 10);
		if (!submit_current_length_in_refractory_status(spike_gen_data->network, current_templates, m, template_length, &has_unallocated_current_template))
			{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "!submit_current_length_in_refractory_status."); }
		//------------NOISE_PARAMS------------
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "fgets() == NULL."); }
		for (i=0; i < network->layer_count; i++)
		{
			ptr_layer = network->layers[i];	
			for (j=0; j<ptr_layer->neuron_group_count; j++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "fgets() == NULL."); }
				ptr_neuron_group = ptr_layer->neuron_groups[j];
				for (k=0; k<ptr_neuron_group->neuron_count; k++)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "fgets() == NULL."); }
					if(!get_word_in_line('\t', 0, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "!get_word_in_line."); }	
					in_refractory_currents[m].noise_params[i][j][k].noise_addition_interval = strtoull(word, &end_ptr, 10);
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "!get_word_in_line."); }	
					in_refractory_currents[m].noise_params[i][j][k].noise_variance = atof(word);
				}
			}
		}	
		//------------TEMPLATES------------
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "fgets() == NULL."); }
		for (i=0; i < network->layer_count; i++)
		{
			ptr_layer = network->layers[i];	
			for (j=0; j<ptr_layer->neuron_group_count; j++)
			{
				ptr_neuron_group = ptr_layer->neuron_groups[j];
				for (k=0; k<ptr_neuron_group->neuron_count; k++)
				{
					for (n = 0; n <  in_refractory_currents[m].num_of_template_samples; n++)	
					{
						if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "fgets() == NULL."); }
						in_refractory_currents[m].template_samples[n].current_sample[i][j][k] = atof(line);
					}
				}
			}
		}	
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "fgets() == NULL."); }
		if (strcmp(line, "----------SpikeGeneratorData - End of InRefractoryCurrentPattern File----------\n") != 0) 
			{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "No end of file match at TrialStartAvailableCurrentPattern."); }
		fclose(fp);
	}
	if (has_unallocated_current_template)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_refractory_current_pattern_templates", "has_unallocated_current_template."); 
	return TRUE;
}
static bool load_in_trial_current_pattern_templates(SpikeGenData *spike_gen_data, char* main_dir_path)
{
	FILE *fp;
	unsigned int i,j,k,m,n, p;
	char current_pattern_name[100];
	char current_pattern_num[10];
	char current_pattern_path[700];	
	char trial_type_num[10];
	char  line[1000];	
	char word[100];
	char *end_ptr;
	CurrentTemplate *current_templates = spike_gen_data->current_templates;
	CurrentPatternTemplate	**in_trial_currents = current_templates->in_trial_currents;
	Network *network = spike_gen_data->network;
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
	TrialTypesData *trial_types_data = spike_gen_data->trial_types_data;

	for (p = 0; p < trial_types_data->num_of_trial_types; p++)
	{
		for (m = 0; m < current_templates->num_of_in_trial_currents; m++)
		{
			sprintf(trial_type_num, "%u_" , trial_types_data->types[p].type);
			strcpy(current_pattern_name, "/current_pattern_template_in_trial_");
			strcat(current_pattern_name, trial_type_num);	
			if (m < 10)
			{
				sprintf(current_pattern_num, "%u" , m);
				strcat(current_pattern_name, current_pattern_num);			
			}
			else if (m < 100)
			{
				sprintf(current_pattern_num, "%u" , m);
				strcat(current_pattern_name, current_pattern_num);			
			}		
			else
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "spike_gen_data->num_of_in_trial_currents >100 .");

			strcpy(current_pattern_path, main_dir_path);
	 		strcat(current_pattern_path, current_pattern_name);
			if ((fp = fopen(current_pattern_path, "r")) == NULL) 
				return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fopen() == NULL: Couldn' t load /current_pattern_template_in_trial file.");

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
			if (strcmp(line, "----------SpikeGeneratorData - InTrialCurrentPattern File----------\n") != 0) 
				{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "Not a InTrialCurrentPattern file."); }

			//LENGTH was loaded by allocate_current_templates
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
			//NUM_OF_SAMPLES was loaded by allocate_current_templates
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
			///------------NOISE_PARAMS------------
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
			for (i=0; i < network->layer_count; i++)
			{
				ptr_layer = network->layers[i];	
				for (j=0; j<ptr_layer->neuron_group_count; j++)
				{
					//fprintf(fp,"Layer %u NeuronGroup:%u\n", i, j);	
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }	
					ptr_neuron_group = ptr_layer->neuron_groups[j];
					for (k=0; k<ptr_neuron_group->neuron_count; k++)
					{
						if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
						if(!get_word_in_line('\t', 0, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "!get_word_in_line."); }		
						in_trial_currents[p][m].noise_params[i][j][k].noise_addition_interval = strtoull(word, &end_ptr, 10);
						if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "!get_word_in_line."); }	
						in_trial_currents[p][m].noise_params[i][j][k].noise_variance = atof(word);
					}
				}
			}	
			//------------TEMPLATES------------\n");
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
			for (i=0; i < network->layer_count; i++)
			{
				ptr_layer = network->layers[i];	
				for (j=0; j<ptr_layer->neuron_group_count; j++)
				{
					ptr_neuron_group = ptr_layer->neuron_groups[j];
					for (k=0; k<ptr_neuron_group->neuron_count; k++)
					{
						for (n = 0; n <  in_trial_currents[p][m].num_of_template_samples; n++)	
						{
							if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
							in_trial_currents[p][m].template_samples[n].current_sample[i][j][k] = atof(line);
						}	
					}
				}
			}	
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
			if (strcmp(line, "----------SpikeGeneratorData - End of InTrialCurrentPattern File----------\n") != 0) 
				{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "End of InTrialCurrentPattern file mismatch."); }
			fclose(fp);
		}
	}
	return TRUE;
}
