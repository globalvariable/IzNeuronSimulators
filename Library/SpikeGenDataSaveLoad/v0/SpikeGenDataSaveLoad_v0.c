#include "SpikeGenDataSaveLoad_v0.h"

static bool create_main_meta_file(SpikeGenData *spike_gen_data, char * main_dir_path);
static bool save_notes(char* main_dir_path, GtkWidget *text_view);
static bool save_current_pattern_templates(SpikeGenData *spike_gen_data, char* main_dir_path);
static bool save_neuron_params(Network * network, char *main_dir_path);

int create_spike_gen_data_directory_v0(int num, ...)
{
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
	strcat(main_dir_path, "/SpikeGenData");
	if ((dir_main_folder = opendir(main_dir_path)) != NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "create_spike_gen_data_directory_v0", "Directory has a SpikeGenData folder already, select another directory.");

	mkdir(main_dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "create_spike_gen_data_directory_v0", "Created SpikeGenData.");
       
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
	TrialsData *trials_data = spike_gen_data->trials_data;
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
	
	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "create_main_meta_file", "Saving main meta file...");

 	strcpy(temp_path, main_dir_path);
 	strcat(temp_path, "/meta");
	if ((fp = fopen(temp_path, "w")) == NULL)  		
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "create_main_meta_file", "fopen() == NULL: Couldn' t create main mate file.");
		
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
	fprintf(fp,"NUM_OF_TRIAL_TYPES\t%u\n", trials_data->trial_types_data.num_of_types);
	for (i = 0; i < trials_data->trial_types_data.num_of_types; i ++)
		fprintf(fp,"NUM_OF_IN_TRIAL_CURRENTS_FOR_TRIAL_TYPE_%u\t%u\n", trials_data->trial_types_data.type_data[i].type, spike_gen_data->current_templates->num_of_in_trial_currents);  // should be equal for each trialk type, no problem. ( see comment in CurrentTemplate strucy: use OKEK of number of currents for each trial having different numbers of trial currents (for random trial current selection))
	fprintf(fp,"INJECTED_CURRENT_PATTERN_SAMPLING_INTERVAL\t%llu\n", PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
	fprintf(fp,"PARKER_SOCHACKI_ERROR_TOLERANCE\t%.16E\n", get_maximum_parker_sochacki_error_tolerance() );
	fprintf(fp,"PARKER_SOCHACKI_MAX_ORDER\t%d\n", get_maximum_parker_sochacki_order());	
	fprintf(fp,"----------SpikeGeneratorData - End of Main Meta File----------\n");
	fclose(fp);
	print_message(INFO_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "create_main_meta_file", "Saving main meta file...complete");
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

	strcpy(main_dir_path, path_chooser);						// SpikePatternGeneratorData should be selected to save 
	if ((dir_main_folder = opendir(main_dir_path)) == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "save_spike_gen_data_directory_v0", "opendir() == NULL: Couldn' t open SpikeGenData directory.");

        if(! save_notes(main_dir_path, text_view))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "save_spike_gen_data_directory_v0", "! save_notes().");

        if(! save_current_pattern_templates(spike_gen_data, main_dir_path))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "save_spike_gen_data_directory_v0", "! save_current_pattern_templates().");

	if (! save_neuron_params(spike_gen_data->network, main_dir_path))
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "save_spike_gen_data_directory_v0", "! save_neuron_params().");

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
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "save_notes", "fopen() == NULL: Couldn' t create /notes file.");

	for (i = 0; i < char_count; i++)
	{
		fprintf(fp, "%c", text_buffer[i]);		
	}
	fclose(fp);

	if ((fp = fopen("./initial_note", "w")) == NULL)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "save_notes", "fopen() == NULL: Couldn' t create ./initial_note file.");
	for (i = 0; i < char_count; i++)
	{
		fprintf(fp, "%c", text_buffer[i]);		
	}
	fclose(fp);
					
	return TRUE;
}

static bool save_current_pattern_templates(SpikeGenData *spike_gen_data, char* main_dir_path)
{
current_pattern_template_in_refractory_0001
current_pattern_template_trial_start_available_0001
current_pattern_template_in_trial_trial_type_11_0001
current_pattern_template_in_trial_trial_type_12_0001

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
 	strcat(temp_path, "/NeuronParameters");
	if ((fp = fopen(temp_path, "w")) == NULL)  
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad", "save_notes", "fopen() == NULL: Couldn' t create /NeuronParameters file.");
	
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
	return 1;
}

