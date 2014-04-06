#include "FileHandler_v0.h"




int write_spike_gen_config_data_v0(int num, ...)
{
	FILE *fp;	
	char  *path_folder;
	char  path[600];
	SpikeGenData *spike_gen_data = NULL;
	TrialHandParadigmRobotReach	*paradigm = NULL;
	GtkWidget *text_view = NULL;
	GtkTextBuffer *buffer = NULL;
	Network *network = NULL;
	ThreePhasePoissonFiring		*firing_rate_data = NULL;



	char *text_buffer;
	GtkTextIter start, end;
	char message[600];
	int i, j, k, p, char_count;
  	va_list arguments;
	va_start ( arguments, num );   
    	path_folder = va_arg ( arguments, char*); 
      	spike_gen_data = va_arg ( arguments, SpikeGenData*); 	
	text_view = va_arg ( arguments, GtkWidget*); 	
	va_end ( arguments );
	paradigm = spike_gen_data->trial_hand_paradigm;
	network = spike_gen_data->network;	

	firing_rate_data = spike_gen_data->firing_rate_data;

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
	fprintf(fp,"NUM_OF_TRIAL_TYPES\t%u\n", paradigm->num_of_robot_target_positions);
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
	fprintf(fp, "------------------TrialStartAvailableFiringRates---------------------\n");

	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			fprintf(fp,"Layer %u NeuronGroup:%u\n", i, j);		
			for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE; k++)
			{
				fprintf(fp, "%.16E\n", firing_rate_data->trial_start_available_firing.rates[i][j][k]);
			}
		}
	}
	fprintf(fp, "------------------End of TrialStartAvailableFiringRates---------------------\n");
	fprintf(fp, "------------------InRefractoryFiringRates---------------------\n");
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			fprintf(fp,"Layer %u NeuronGroup:%u\n", i, j);		
			for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE; k++)
			{
				fprintf(fp, "%.16E\n", firing_rate_data->in_refractory_firing.rates[i][j][k]);
			}
		}
	}
	fprintf(fp, "------------------End of InRefractoryFiringRates---------------------\n");
	fprintf(fp, "------------------InTrialFiringRates---------------------\n");
	for (p = 0; p < paradigm->num_of_robot_target_positions; p++)
	{
		for (i=0; i < MAX_NUM_OF_MWA; i++)
		{
			for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
			{
				fprintf(fp,"Layer %u NeuronGroup:%u\n", i, j);		
				for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE; k++)
				{
					fprintf(fp, "%.16E\n", firing_rate_data->in_trial_firing[p].rates[i][j][k]);
				}
			}
		}
	}
	fprintf(fp, "------------------End of InTrialFiringRates---------------------\n");

	fprintf(fp, "----------End of SpikeGenConfig Data----------\n");	
	fclose(fp);
	print_message(INFO_MSG ,"BMISpikeGenerator", "FileHandler", "read_config_daq_data_v0", "Succesuflly wrote BMISpikeGenerator data file.");	
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
	unsigned int max_num_of_daq_card, max_num_of_channel_per_daq_card, max_num_of_mwa, max_num_of_channel_per_mwa, max_num_of_units_per_chan, max_num_of_units_per_chan_to_handle ;
	unsigned int i, j ,k ,p;

	SpikeGenData *spike_gen_data = NULL;	
	GtkWidget *text_view = NULL;
	GtkTextBuffer *buffer = NULL;
	GtkTextIter start, end;

	Layer		*ptr_layer;
	NeuronGroup	*ptr_neuron_group;



	char *path = NULL;



  	va_list arguments;
	va_start ( arguments, num );   
    	path = va_arg ( arguments, char*); 
	spike_gen_data = va_arg ( arguments, SpikeGenData*); 
	text_view = va_arg ( arguments, GtkWidget*); 
	va_end ( arguments );

	if ((fp = fopen(path, "r")) == NULL)  { sprintf(message, "Couldn' t read %s.", path); print_message(ERROR_MSG ,"BMISpikeGenerator", "FileHandler_v0", "read_config_daq_data_v0", message); return 0; }
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"BMISpikeGenerator", "FileHandler_v0", "read_config_daq_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  ----------SpikeGenConfig Data----------
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"BMISpikeGenerator", "FileHandler_v0", "read_config_daq_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  VERSION	XX
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"BMISpikeGenerator", "FileHandler_v0", "read_config_daq_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  MAX_NUM_OF_DAQ_CARD	XX
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_daq_card = (int)atof(word);	
	if (MAX_NUM_OF_DAQ_CARD != max_num_of_daq_card )
	{
		printf("ERROR: BMISpikeGenerator file was saved when MAX_NUM_OF_DAQ_CARD = %d\n",max_num_of_daq_card);
		printf("ERROR: Now it is MAX_NUM_OF_DAQ_CARD = %d\n", MAX_NUM_OF_DAQ_CARD);	
		fclose(fp);
		return 0;
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}  //MAX_NUM_OF_CHANNEL_PER_DAQ_CARD
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_channel_per_daq_card = (int)atof(word);
	if (MAX_NUM_OF_CHANNEL_PER_DAQ_CARD != max_num_of_channel_per_daq_card)
	{
		printf("ERROR: BMISpikeGenerator file was saved when MAX_NUM_OF_DAQ_CARD = %d\n", max_num_of_channel_per_daq_card);
		printf("ERROR: Now it is MAX_NUM_OF_DAQ_CARD = %d\n", MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);	
		fclose(fp);
		return 0; 
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}    //MAX_NUM_OF_MWA
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_mwa = (int)atof(word);
	if (MAX_NUM_OF_MWA != max_num_of_mwa)
	{
		printf("ERROR: BMISpikeGenerator file was saved when MAX_NUM_OF_MWA = %d\n", max_num_of_mwa);
		printf("ERROR: Now it is MAX_NUM_OF_MWA = %d\n", MAX_NUM_OF_MWA);	
		fclose(fp);
		return 0;
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}    //MAX_NUM_OF_CHAN_PER_MWA
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_channel_per_mwa = (int)atof(word);	
	if (MAX_NUM_OF_CHAN_PER_MWA != max_num_of_channel_per_mwa)
	{
		printf("ERROR: BMISpikeGenerator file was saved when MAX_NUM_OF_CHAN_PER_MWA = %d\n", max_num_of_channel_per_mwa);
		printf("ERROR: Now it is MAX_NUM_OF_CHAN_PER_MWA = %d\n", MAX_NUM_OF_CHAN_PER_MWA);	
		fclose(fp);
		return 0;
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}    //MAX_NUM_OF_UNIT_PER_CHAN
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_units_per_chan = (int)atof(word);	
	if (MAX_NUM_OF_UNIT_PER_CHAN != max_num_of_units_per_chan)
	{
		printf("ERROR: BMISpikeGenerator file was saved when MAX_NUM_OF_UNIT_PER_CHAN = %d\n", max_num_of_units_per_chan);
		printf("ERROR: Now it is MAX_NUM_OF_UNIT_PER_CHAN = %d\n", MAX_NUM_OF_UNIT_PER_CHAN);	
		fclose(fp);
		return 0;
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}    //MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_units_per_chan_to_handle = (int)atof(word);	
	if (MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE != max_num_of_units_per_chan_to_handle)
	{
		printf("ERROR: BMISpikeGenerator file was saved when MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE = %d\n", max_num_of_units_per_chan_to_handle);
		printf("ERROR: Now it is MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE = %d\n", MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE);	
		fclose(fp);
		return 0;
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}    //NUM_OF_TRIAL_TYPES
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	if (spike_gen_data->trial_hand_paradigm->num_of_robot_target_positions != strtoull(word, &end_ptr, 10)) {  printf("ERROR: Inconvenient num of trial types %u.\n", line_cntr);  fclose(fp); return 0; } else {line_cntr++;}    //NUM_OF_TRIAL_TYPES



	for (i = 0; i < max_num_of_mwa; i++)
	{
		for (j=0; j<max_num_of_channel_per_mwa; j++)
		{
			if (!add_poisson_neurons_to_layer(spike_gen_data->network, max_num_of_units_per_chan_to_handle, i, FALSE,  0))
				{ fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "! add_poisson_neurons_to_layer."); }

			for (k = 0; k < max_num_of_units_per_chan_to_handle; k++)
			{
				(*spike_gen_data->sorted_spike_time_stamp)[i][j].included_units[k] = TRUE;	
			}	
		}		
	}
	print_message(INFO_MSG ,"BMISpikeGenerator", "FileHandler", "read_config_daq_data_v0", "Succesuflly read network structure.");	
 
	spike_gen_data->firing_rate_data = allocate_three_phase_poisson_firing_data(spike_gen_data->network, spike_gen_data->trial_hand_paradigm, spike_gen_data->firing_rate_data);

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}    //NUM_OF_TRIAL_TYPES
	if (strcmp(line, "------------------Notes---------------------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"BMISpikeGenerator", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------------Notes---------------------) != 0.");	
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
		return print_message(ERROR_MSG ,"BMISpikeGenerator", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------------End of Notes---------------------) != 0.");	
	}  	
	
	print_message(INFO_MSG ,"BMISpikeGenerator", "FileHandler", "read_config_daq_data_v0", "Succesuflly read Notes.");	

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }  //
	if (strcmp(line, "------------------TrialStartAvailableFiringRates---------------------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"BMISpikeGenerator", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------------TrialStartAvailableFiringRates---------------------) != 0.");	
	}  	

	for (i=0; i < spike_gen_data->network->layer_count; i++)
	{
		ptr_layer = spike_gen_data->network->layers[i];
		for (j=0; j<ptr_layer->neuron_group_count; j++)
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
			ptr_neuron_group = ptr_layer->neuron_groups[j];
			for (k=0; k<ptr_neuron_group->neuron_count; k++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
				if(!get_word_in_line('\t', 0, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "!get_word_in_line."); }	
				spike_gen_data->firing_rate_data->trial_start_available_firing.rates[i][j][k] = atof(word);
			}
		}
	}

	print_message(INFO_MSG ,"BMISpikeGenerator", "FileHandler", "read_config_daq_data_v0", "Succesuflly read trial_start_available_firing.rates.");	

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }  //
	if (strcmp(line, "------------------End of TrialStartAvailableFiringRates---------------------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"BMISpikeGenerator", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------------End of TrialStartAvailableFiringRates---------------------) != 0.");	
	}  	


	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }  //
	if (strcmp(line, "------------------InRefractoryFiringRates---------------------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"BMISpikeGenerator", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------------InRefractoryFiringRates---------------------) != 0.");	
	}  	
	for (i=0; i < spike_gen_data->network->layer_count; i++)
	{
		ptr_layer = spike_gen_data->network->layers[i];	
		for (j=0; j<ptr_layer->neuron_group_count; j++)
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
			ptr_neuron_group = ptr_layer->neuron_groups[j];
			for (k=0; k<ptr_neuron_group->neuron_count; k++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
				if(!get_word_in_line('\t', 0, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "!get_word_in_line."); }	
				spike_gen_data->firing_rate_data->in_refractory_firing.rates[i][j][k] = atof(word);
			}
		}
	}

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }  //
	if (strcmp(line, "------------------End of InRefractoryFiringRates---------------------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"BMISpikeGenerator", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------------End of InRefractoryFiringRates---------------------) != 0.");	
	}  	

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); }  //
	if (strcmp(line, "------------------InTrialFiringRates---------------------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"BMISpikeGenerator", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ------------------InTrialFiringRates---------------------) != 0.");	
	}  	
	for (p = 0; p < spike_gen_data->trial_hand_paradigm->num_of_robot_target_positions; p++)
	{
		for (i=0; i < spike_gen_data->network->layer_count; i++)
		{
			ptr_layer = spike_gen_data->network->layers[i];	
			for (j=0; j<ptr_layer->neuron_group_count; j++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
				ptr_neuron_group = ptr_layer->neuron_groups[j];
				for (k=0; k<ptr_neuron_group->neuron_count; k++)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "fgets() == NULL."); }
					if(!get_word_in_line('\t', 0, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_trial_start_available_current_pattern_templates", "!get_word_in_line."); }	
					spike_gen_data->firing_rate_data->in_trial_firing[p].rates[i][j][k] = atof(word);
				}
			}
		}
	}

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "fgets() == NULL."); }
	if (strcmp(line, "------------------End of InTrialFiringRates---------------------\n") != 0) 
	{ 
		fclose(fp); 
		return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_in_trial_current_pattern_templates", "End of InTrialFiringRates file mismatch."); 
	}

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"IzNeuronSimulators", "SpikeGenDataSaveLoad_v0", "load_main_meta_file", "fgets() == NULL."); } 
	if (strcmp(line, "----------End of SpikeGenConfig Data----------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"BMISpikeGenerator", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ----------End of SpikeGenConfig Data----------) != 0.");	
	}  
	print_message(INFO_MSG ,"BMISpikeGenerator", "FileHandler", "read_config_daq_data_v0", "Succesuflly read BMISpikeGenerator data file.");	
	return 1;
}
