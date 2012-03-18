#ifndef INJECTION_CURRENT_DATA_H
#define INJECTION_CURRENT_DATA_H

typedef struct __InitializationCurrentParams InitializationCurrentParams;
typedef struct __CurrentNoiseParams CurrentNoiseParams;
typedef double InjectionCurrent;
typedef struct __NeuronCurrentTemplate NeuronCurrentTemplate;
typedef struct __CurrentPatternTemplate CurrentPatternTemplate;
typedef struct __CurrentTemplate CurrentTemplate;
typedef struct __CurrentPatternBuffer CurrentPatternBuffer;

#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"
#include "../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialsData.h"


struct __InitializationCurrentParams
{
	double		max_current;
	double		min_current;
	TimeStamp	duration;
	double		v_after_init;
	double		u_after_init;
};
struct __CurrentNoiseParams
{
	TimeStamp	noise_addition_interval;	
	double		noise_variance;	
};
struct __NeuronCurrentTemplate
{
	InitializationCurrentParams	init_params;
	CurrentNoiseParams 		noise_params;
	InjectionCurrent			*current;	  // num_of_current_samples
};

struct __CurrentPatternTemplate
{
	NeuronCurrentTemplate	***templates;
	unsigned int				num_of_current_samples;
};

struct __CurrentTemplate
{
	unsigned int num_of_trial_start_available_currents;
	unsigned int num_of_in_refractory_currents;
	unsigned int num_of_in_trial_currents;   // left / right target currents   // identical number of trial currents for trial types. (use OKEK of number of currents for 
									   // each trial having different numbers of trial currents (for random trial current selection))
	CurrentPatternTemplate	*trial_start_available_currents;
	CurrentPatternTemplate	*in_refractory_currents;
	CurrentPatternTemplate	**in_trial_currents;    // [trials_data->trial_types_data->num_of_types][num_of_in_trial_currents]
};

struct __CurrentPatternBuffer
{
	InjectionCurrent		****neuron_current_buffer;
	unsigned int			buff_write_idx;
	unsigned int			buffer_size;
};

CurrentTemplate* allocate_current_templates(Network *network, TrialsData *trials_data, CurrentTemplate* current_data, unsigned int num_of_trial_start_available_currents, unsigned int num_of_in_refractory_currents, unsigned int num_of_in_trial_currents);
CurrentTemplate* deallocate_current_templates(Network *network, TrialsData *trials_data, CurrentTemplate* current_data);
bool submit_current_length_trial_start_available_status(Network *network, CurrentTemplate* current_data, unsigned int trial_start_available_current_num, TimeStamp current_length);
bool submit_current_length_in_refractory_status(Network *network, CurrentTemplate* current_data, unsigned int in_refractory_current_num, TimeStamp current_length);

CurrentPatternBuffer* allocate_current_pattern_buffer(Network *network, CurrentPatternBuffer *buffer, unsigned int buffer_size);
CurrentPatternBuffer* deallocate_current_pattern_buffer(Network *network, CurrentPatternBuffer *buffer);


#endif
