#include "FileHandler_v0.h"




int write_spike_gen_config_data_v0(int num, ...)
{
	FILE *fp;	
	char  *path_folder;
	char  path[600];
	SpikeGenData *spike_gen_data = NULL;
	TrialTypesData *trial_types_data = NULL;
	GtkWidget *text_view = NULL;
	GtkTextBuffer *buffer = NULL;
	Network *network = NULL;
	CurrentTemplate *current_templates = NULL;
	CurrentPatternTemplate	*trial_start_available_currents = NULL;
	CurrentPatternTemplate	*in_refractory_currents = NULL;
	CurrentPatternTemplate	**in_trial_currents = NULL;
	Neuron		*ptr_neuron = NULL;
	double a, b, c, d, k_np, C, v_resting, v_threshold, v_peak, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory;
	bool inhibitory;

	char *text_buffer;
	GtkTextIter start, end;
	char message[600];
	int i, j, k, m, n, p, char_count;
  	va_list arguments;
	va_start ( arguments, num );   
    	path_folder = va_arg ( arguments, char*); 
      	spike_gen_data = va_arg ( arguments, SpikeGenData*); 	
	text_view = va_arg ( arguments, GtkWidget*); 	
	va_end ( arguments );
	trial_types_data = spike_gen_data->trial_types_data;
	network = spike_gen_data->network;
	current_templates = spike_gen_data->current_templates;
	trial_start_available_currents = current_templates->trial_start_available_currents;
	in_refractory_currents = current_templates->in_refractory_currents;
	in_trial_currents = current_templates->in_trial_currents;
	strcpy(path, path_folder);
	strcat(path, "/SpikeGenConfig");

	if ((fp = fopen(path, "w")) == NULL)  { sprintf(message, "Couldn' t create %s.", path); print_message(ERROR_MSG ,"BMISpikeGenerator", "FileHandler", "write_spike_gen_config_data_v0", message); return 0; }

	fprintf(fp, "----------SpikeGenConfig Data----------\n");	
	fprintf(fp,"VERSION\t%u\n", 0);
	fprintf(fp,"MAX_NUM_OF_DAQ_CARD\t%u\n",MAX_NUM_OF_DAQ_CARD);
	fprintf(fp,"MAX_NUM_OF_CHANNEL_PER_DAQ_CARD\t%u\n",MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);		
	fprintf(fp,"MAX_NUM_OF_MWA\t%u\n", MAX_NUM_OF_MWA);
	fprintf(fp,"MAX_NUM_OF_CHAN_PER_MWA\t%u\n",MAX_NUM_OF_CHAN_PER_MWA);
	fprintf(fp,"MAX_NUM_OF_UNIT_PER_CHAN\t%u\n",MAX_NUM_OF_UNIT_PER_CHAN);
	fprintf(fp,"MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE\t%u\n",MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE);
	fprintf(fp,"NUM_OF_TRIAL_START_AVAILABLE_CURRENTS\t%u\n", spike_gen_data->current_templates->num_of_trial_start_available_currents);
	fprintf(fp,"NUM_OF_IN_REFRACTORY_CURRENTS\t%u\n", spike_gen_data->current_templates->num_of_in_refractory_currents);
	fprintf(fp,"NUM_OF_TRIAL_TYPES\t%u\n", trial_types_data->num_of_trial_types);
	for (i = 0; i < trial_types_data->num_of_trial_types; i ++)
		fprintf(fp,"NUM_OF_IN_TRIAL_CURRENTS_FOR_TRIAL_TYPE_%u\t%u\n", trial_types_data->types[i].type, spike_gen_data->current_templates->num_of_in_trial_currents);  // should be equal for each trialk type, no problem. ( see comment in CurrentTemplate strucy: use OKEK of number of currents for each trial having different numbers of trial currents (for random trial current selection))
	fprintf(fp,"INJECTED_CURRENT_PATTERN_SAMPLING_INTERVAL\t%llu\n", PARKER_SOCHACKI_INTEGRATION_STEP_SIZE);
	fprintf(fp,"PARKER_SOCHACKI_ERROR_TOLERANCE\t%.16E\n", get_maximum_parker_sochacki_error_tolerance() );
	fprintf(fp,"PARKER_SOCHACKI_MAX_ORDER\t%d\n", get_maximum_parker_sochacki_order());	
	fprintf(fp, "------------------Notes---------------------\n");	
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));	
       	gtk_text_buffer_get_start_iter ( buffer, &start);
        gtk_text_buffer_get_end_iter ( buffer, &end);
        text_buffer = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);
	char_count = gtk_text_buffer_get_char_count(buffer);
	for (i = 0; i < char_count; i++)
	{
		fprintf(fp, "%c", text_buffer[i]);		
	}
	fprintf(fp,"------------------Bottom of Notes---------------------\n");
	fprintf(fp, "------------------End of Notes---------------------\n");	
	fprintf(fp, "------------------TrialStartAvailableCurrents---------------------\n");
	for (m = 0; m < current_templates->num_of_trial_start_available_currents; m++)
	{
		fprintf(fp, "------------------TrialStartAvailableCurrent %d---------------------\n", m);
		fprintf(fp,"LENGTH\t%llu\n", trial_start_available_currents[m].template_length);
		fprintf(fp,"NUM_OF_SAMPLES\t%u\n", trial_start_available_currents[m].num_of_template_samples);	
		fprintf(fp,"------------NOISE_PARAMS------------\n");
		for (i=0; i < MAX_NUM_OF_MWA; i++)
		{
			for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
			{
				fprintf(fp,"Layer %u NeuronGroup:%u\n", i, j);		
				for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE; k++)
				{
					fprintf(fp, "%llu\t%.16E\n", trial_start_available_currents[m].noise_params[i][j][k].noise_addition_interval, trial_start_available_currents[m].noise_params[i][j][k].noise_variance);
				}
			}
		}
		fprintf(fp,"------------TEMPLATES------------\n");
		for (i=0; i < MAX_NUM_OF_MWA; i++)
		{
			for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
			{
				for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE; k++)
				{
					for (n = 0; n <  trial_start_available_currents[m].num_of_template_samples; n++)		
						fprintf(fp, "%.16E\n", trial_start_available_currents[m].template_samples[n].current_sample[i][j][k]);
				}
			}
		}
	}
	fprintf(fp, "------------------End of TrialStartAvailableCurrents---------------------\n");
	fprintf(fp, "------------------InRefractoryCurrents---------------------\n");
	for (m = 0; m < current_templates->num_of_in_refractory_currents; m++)
	{
		fprintf(fp, "------------------InRefractoryCurrent %d---------------------\n", m);
		fprintf(fp,"LENGTH\t%llu\n", in_refractory_currents[m].template_length);
		fprintf(fp,"NUM_OF_SAMPLES\t%u\n", in_refractory_currents[m].num_of_template_samples);
		fprintf(fp,"------------NOISE_PARAMS------------\n");
		for (i=0; i < MAX_NUM_OF_MWA; i++)
		{
			for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
			{
				fprintf(fp,"Layer %u NeuronGroup:%u\n", i, j);		
				
				for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE; k++)
				{
					fprintf(fp, "%llu\t%.16E\n", in_refractory_currents[m].noise_params[i][j][k].noise_addition_interval, in_refractory_currents[m].noise_params[i][j][k].noise_variance);
				}
			}
		}	
		fprintf(fp,"------------TEMPLATES------------\n");
		for (i=0; i < MAX_NUM_OF_MWA; i++)
		{
			for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
			{
				for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE; k++)
				{
					for (n = 0; n <  in_refractory_currents[m].num_of_template_samples; n++)		
						fprintf(fp, "%.16E\n", in_refractory_currents[m].template_samples[n].current_sample[i][j][k]);
				}
			}
		}
	}	
	fprintf(fp, "------------------End of InRefractoryCurrents---------------------\n");
	fprintf(fp, "------------------InTrialCurrents---------------------\n");
	for (p = 0; p < trial_types_data->num_of_trial_types; p++)
	{
		for (m = 0; m < current_templates->num_of_in_trial_currents; m++)
		{
			fprintf(fp, "------------------TrialType %d InTrialCurrent %d---------------------\n", p, m);
			fprintf(fp,"LENGTH\t%llu\n", in_trial_currents[p][m].template_length);
			fprintf(fp,"NUM_OF_SAMPLES\t%u\n", in_trial_currents[p][m].num_of_template_samples);
			fprintf(fp,"------------NOISE_PARAMS------------\n");
			for (i=0; i < MAX_NUM_OF_MWA; i++)
			{
					
				for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
				{
					fprintf(fp,"Layer %u NeuronGroup:%u\n", i, j);		
					
					for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE; k++)
					{
						fprintf(fp, "%llu\t%.16E\n", in_trial_currents[p][m].noise_params[i][j][k].noise_addition_interval, in_trial_currents[p][m].noise_params[i][j][k].noise_variance);
					}
				}
			}	
			fprintf(fp,"------------TEMPLATES------------\n");
			for (i=0; i < MAX_NUM_OF_MWA; i++)
			{
				for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
				{
					for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE; k++)
					{
						for (n = 0; n <  in_trial_currents[p][m].num_of_template_samples; n++)		
							fprintf(fp, "%.16E\n", in_trial_currents[p][m].template_samples[n].current_sample[i][j][k]);
					}
				}
			}	
		}
	}
	fprintf(fp, "------------------End of InTrialCurrents---------------------\n");
	fprintf(fp, "------------------NeuronParameters---------------------\n");
	for (i=0; i<MAX_NUM_OF_MWA; i++)
	{
		for (m=0; m<MAX_NUM_OF_CHAN_PER_MWA; m++)
		{
			for (n=0; n<MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE; n++)
			{
				ptr_neuron = &(network->layers[i]->neuron_groups[m]->neurons[n]);	
				a = ptr_neuron->iz_params->a;
				b = ptr_neuron->iz_params->b;
				c = ptr_neuron->iz_params->c + ptr_neuron->iz_params->v_resting;
				d = ptr_neuron->iz_params->d;
				k_np = ptr_neuron->iz_params->k;
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
				fprintf(fp, "%.16E\n%.16E\n%.16E\n%.16E\n%.16E\n%.16E\n%.16E\n%.16E\n%.16E\n%d\n%.16E\n%.16E\n%.16E\n%.16E\n", a, b, c, d, k_np, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory);
			}
		}			
	}
	fprintf(fp, "------------------End of NeuronParameters---------------------\n");
	fprintf(fp, "----------End of SpikeGenConfig Data----------\n");	
	fclose(fp);
	return 1;
}
int read_spike_gen_config_data_v0(int num, ...)
{	
	FILE *fp = NULL;		
	char message[600];	
	char line[1000];
	unsigned int line_cntr = 0;
	char word[100];
	char *end_ptr;	
	unsigned int max_num_of_daq_card, max_num_of_channel_per_daq_card, max_num_of_mwa, max_num_of_channel_per_mwa, max_num_of_units_per_chan, max_num_of_units_per_chan_to_handle, num_of_trial_start_available_currents, num_of_in_refractory_currents, num_of_in_trial_currents = 0;
	unsigned int i, j ,k ,m ,n ,p;
	unsigned int parker_sochacki_max_order;
	double parker_sochacki_error_tolerance;
	SpikeGenData *spike_gen_data = NULL;	
	GtkWidget *text_view = NULL;
	GtkTextBuffer *buffer = NULL;
	GtkTextIter start, end;
	CurrentTemplate *current_templates;
	CurrentPatternTemplate	*trial_start_available_currents, *in_refractory_currents, **in_trial_currents;
	TrialTypesData *trial_types_data;
	Network *network;
	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;
	Neuron		*ptr_neuron;
	unsigned int				num_of_template_samples;
	TimeStamp				template_length;
	bool has_unallocated_current_template;
	char *path = NULL;

	double a, b, c, d, k_np, C, v_resting, v_threshold, v_peak, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory;
	bool inhibitory;

  	va_list arguments;
	va_start ( arguments, num );   
    	path = va_arg ( arguments, char*); 
	spike_gen_data = va_arg ( arguments, SpikeGenData*); 
	text_view = va_arg ( arguments, GtkWidget*); 
	va_end ( arguments );

	current_templates = spike_gen_data->current_templates;
	network = spike_gen_data->network;
	trial_types_data = spike_gen_data->trial_types_data;

	if ((fp = fopen(path, "r")) == NULL)  { sprintf(message, "Couldn' t read %s.", path); print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", message); return 0; }
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  ----------SpikeGenConfig Data----------
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  VERSION	XX
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  MAX_NUM_OF_DAQ_CARD	XX
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_daq_card = (int)atof(word);	
	if (MAX_NUM_OF_DAQ_CARD != max_num_of_daq_card )
	{
		printf("ERROR: ConfigDaq file was saved when MAX_NUM_OF_DAQ_CARD = %d\n",max_num_of_daq_card);
		printf("ERROR: Now it is MAX_NUM_OF_DAQ_CARD = %d\n", MAX_NUM_OF_DAQ_CARD);	
		fclose(fp);
		return 0;
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}  //MAX_NUM_OF_CHANNEL_PER_DAQ_CARD
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_channel_per_daq_card = (int)atof(word);
	if (MAX_NUM_OF_CHANNEL_PER_DAQ_CARD != max_num_of_channel_per_daq_card)
	{
		printf("ERROR: ConfigDaq file was saved when MAX_NUM_OF_DAQ_CARD = %d\n", max_num_of_channel_per_daq_card);
		printf("ERROR: Now it is MAX_NUM_OF_DAQ_CARD = %d\n", MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);	
		fclose(fp);
		return 0; 
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}    //MAX_NUM_OF_MWA
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_mwa = (int)atof(word);
	if (MAX_NUM_OF_MWA != max_num_of_mwa)
	{
		printf("ERROR: ConfigDaq file was saved when MAX_NUM_OF_MWA = %d\n", max_num_of_mwa);
		printf("ERROR: Now it is MAX_NUM_OF_MWA = %d\n", MAX_NUM_OF_MWA);	
		fclose(fp);
		return 0;
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}    //MAX_NUM_OF_CHAN_PER_MWA
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_channel_per_mwa = (int)atof(word);	
	if (MAX_NUM_OF_CHAN_PER_MWA != max_num_of_channel_per_mwa)
	{
		printf("ERROR: ConfigDaq file was saved when MAX_NUM_OF_CHAN_PER_MWA = %d\n", max_num_of_channel_per_mwa);
		printf("ERROR: Now it is MAX_NUM_OF_CHAN_PER_MWA = %d\n", MAX_NUM_OF_CHAN_PER_MWA);	
		fclose(fp);
		return 0;
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}    //MAX_NUM_OF_UNIT_PER_CHAN
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_units_per_chan = (int)atof(word);	
	if (MAX_NUM_OF_UNIT_PER_CHAN != max_num_of_units_per_chan)
	{
		printf("ERROR: ConfigDaq file was saved when MAX_NUM_OF_UNIT_PER_CHAN = %d\n", max_num_of_units_per_chan);
		printf("ERROR: Now it is MAX_NUM_OF_UNIT_PER_CHAN = %d\n", MAX_NUM_OF_UNIT_PER_CHAN);	
		fclose(fp);
		return 0;
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}    //MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_units_per_chan_to_handle = (int)atof(word);	
	if (MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE != max_num_of_units_per_chan_to_handle)
	{
		printf("ERROR: ConfigDaq file was saved when MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE = %d\n", max_num_of_units_per_chan_to_handle);
		printf("ERROR: Now it is MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE = %d\n", MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE);	
		fclose(fp);
		return 0;
	}
	spike_gen_data->network = allocate_network(spike_gen_data->network);	 // deallocates previously allocated network and brings a new one.
	for (i = 0; i < max_num_of_mwa; i++)
	{
		for (j=0; j<max_num_of_channel_per_mwa; j++)
		{
			if (!add_iz_neurons_to_layer(spike_gen_data->network, max_num_of_units_per_chan_to_handle, i, 0, 0, 0, 0, 0, 100.0, 0, 0, 0, 0, 0, 0, 1.0, 1.0, 0))	// create neurons and neuron groups with no parameter.
				{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "! add_iz_neurons_to_layer."); }
		}		
	}
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}    //NUM_OF_TRIAL_START_AVAILABLE_CURRENTS
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	num_of_trial_start_available_currents = (int)atof(word);	
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}    //NUM_OF_IN_REFRACTORY_CURRENTS
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	num_of_in_refractory_currents = (int)atof(word);	
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}    //NUM_OF_TRIAL_TYPES
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	if (spike_gen_data->trial_types_data->num_of_trial_types != strtoull(word, &end_ptr, 10)) {  printf("ERROR: Inconvenient num of trial types %u.\n", line_cntr);  fclose(fp); return 0; } else {line_cntr++;}    //NUM_OF_TRIAL_TYPES
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
	current_templates = spike_gen_data->current_templates;
	trial_start_available_currents = current_templates->trial_start_available_currents;
	in_refractory_currents = current_templates->in_refractory_currents;
	in_trial_currents = current_templates->in_trial_currents;

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
 
	if (strcmp(line, "------------------Notes---------------------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------------Notes---------------------) != 0.");	
	}  		

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));	
       	gtk_text_buffer_get_start_iter ( buffer, &start);
        gtk_text_buffer_get_end_iter ( buffer, &end);
	gtk_text_buffer_delete (buffer, &start, &end);
       	gtk_text_buffer_get_start_iter ( buffer, &start);
        gtk_text_buffer_get_end_iter ( buffer, &end);
	while (fgets(line, sizeof line, fp ) != NULL) 
	{ 
		if (strcmp(line, "------------------Bottom of Notes---------------------\n") == 0)
			break;
       		gtk_text_buffer_get_end_iter ( buffer, &end);   				
		gtk_text_buffer_insert(buffer, &end, line, -1);
		i++;
		if (i == 200)
		{
			print_message(WARNING_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_notes", "/notes file has more than 200 lines(upper limit)");
       			break; 				 				
		}
	}
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }  
	if (strcmp(line, "------------------End of Notes---------------------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------------End of Notes---------------------) != 0.");	
	}  	
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }  //
	if (strcmp(line, "------------------TrialStartAvailableCurrents---------------------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------------TrialStartAvailableCurrents---------------------) != 0.");	
	}  	
	for (m = 0; m < current_templates->num_of_trial_start_available_currents; m++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }  //------------------TrialStartAvailableCurrent %d---------------------
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); } 
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "!get_word_in_line."); }
		template_length = strtoull(word, &end_ptr, 10);
		printf ("%llu\n", template_length);
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); } 
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "!get_word_in_line."); }
		num_of_template_samples = strtoull(word, &end_ptr, 10);
		printf ("%u\n", num_of_template_samples);
		if (!submit_current_length_trial_start_available_status(spike_gen_data->network, current_templates, m, template_length, &has_unallocated_current_template))
			{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "!submit_current_length_trial_start_available_status."); }
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }  //------------NOISE_PARAMS------------
		if (strcmp(line, "------------NOISE_PARAMS------------\n") != 0)
		{
			fclose(fp);
			return print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------TEMPLATES------------) != 0.");	
		}  	
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
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
		if (strcmp(line, "------------TEMPLATES------------\n") != 0)
		{
			fclose(fp);
			return print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------TEMPLATES------------) != 0.");	
		}  	
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
	}
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }  //
	if (strcmp(line, "------------------End of TrialStartAvailableCurrents---------------------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------------End of TrialStartAvailableCurrents---------------------) != 0.");	
	}  	
	if (has_unallocated_current_template)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "has_unallocated_current_template."); 

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }  //
	if (strcmp(line, "------------------InRefractoryCurrents---------------------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------------InRefractoryCurrents---------------------) != 0.");	
	}  	
	for (m = 0; m < current_templates->num_of_in_refractory_currents; m++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }  //------------------InRefractoryCurrent %d---------------------
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
	}	
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }  //
	if (strcmp(line, "------------------End of InRefractoryCurrents---------------------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------------End of InRefractoryCurrents---------------------) != 0.");	
	}  	
	if (has_unallocated_current_template)
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "has_unallocated_current_template."); 




	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }  //
	if (strcmp(line, "------------------InTrialCurrents---------------------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------------InTrialCurrents---------------------) != 0.");	
	}  	
	for (p = 0; p < trial_types_data->num_of_trial_types; p++)
	{
		for (m = 0; m < current_templates->num_of_in_trial_currents; m++)
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }  // "------------------TrialType %d InTrialCurrent %d---------------------\n"
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
		}
	}

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
	if (strcmp(line, "------------------End of InTrialCurrents---------------------\n") != 0) 
	{ 
		fclose(fp); 
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "End of InTrialCurrentPattern file mismatch."); 
	}



	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
	if (strcmp(line, "------------------NeuronParameters---------------------\n") != 0) 
	{ 
		fclose(fp); 
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "strcmp(line, ------------------NeuronParameters---------------------) != 0.."); 
	}

	for (i=0; i<network->layer_count; i++)
	{
		ptr_layer = network->layers[i];			
		for (m=0; m<ptr_layer->neuron_group_count; m++)
		{
			ptr_neuron_group = ptr_layer->neuron_groups[m];
			for (n=0; n<ptr_neuron_group->neuron_count; n++)
			{
				ptr_neuron = &(ptr_neuron_group->neurons[n]);	
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
				a = atof(line);	
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
				b = atof(line);
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }  
				c = atof(line);	
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
				d = atof(line);			
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
				k_np = atof(line);				
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
				C = atof(line);					
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
				v_resting = atof(line);		
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
				v_threshold = atof(line);						
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
				v_peak = atof(line);						
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
				inhibitory = (bool)atof(line);		
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
				E_excitatory = atof(line);		
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
				E_inhibitory = atof(line);						
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
				tau_excitatory = atof(line);						
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
				tau_inhibitory = atof(line);																															
				if (!submit_new_iz_neuron_params(network, ptr_neuron, a, b, c, d, k_np, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory))
					{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_neuron_params", "!submit_new_iz_neuron_param()."); }
			}
		}			
	}
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
	if (strcmp(line, "------------------End of NeuronParameters---------------------\n") != 0) 
	{ 
		fclose(fp); 
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "strcmp(line, ------------------End of NeuronParameters---------------------) != 0.."); 
	}




	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); } 
	if (strcmp(line, "----------End of SpikeGenConfig Data----------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ----------End of SpikeGenConfig Data----------) != 0.");	
	}  
	print_message(INFO_MSG ,"ConfigDaq", "FileHandler", "read_config_daq_data_v0", "Succesuflly read ConfigDaq data file.");	
	return 1;
}
