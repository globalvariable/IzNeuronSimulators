#ifndef INJECTION_CURRENT_DATA_H
#define INJECTION_CURRENT_DATA_H

typedef struct __InitializationCurrentParams InitializationCurrentParams;
typedef struct __CurrentNoiseParams CurrentNoiseParams;
typedef double InjectionCurrent;
typedef struct __NeuronCurrentTemplate NeuronCurrentTemplate;
typedef struct __CurrentPatternTemplate CurrentPatternTemplate;
typedef struct __CurrentTemplate CurrentTemplate;
typedef struct __CurrentPatternBuffer CurrentPatternBuffer;
typedef struct __InjectionCurrentData InjectionCurrentData;

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
	InjectionCurrent		***neuron_current_pattern;
	unsigned int			buff_write_idx;
	unsigned int			buff_size;
};

struct __InjectionCurrentData
{
	CurrentTemplate		*current_templates;
	CurrentPatternBuffer	*current_pattern_buffer;
};

InjectionCurrentData* allocate_injection_current_data(Network *network, TrialsData *trials_data, InjectionCurrentData* injection_current_data, unsigned int num_of_trial_start_available_currents, unsigned int num_of_in_refractory_currents, unsigned int num_of_in_trial_currents);
InjectionCurrentData* deallocate_injection_current_data(Network *network, TrialsData *trials_data, InjectionCurrentData* injection_current_data);
bool submit_current_length_trial_start_available_status(Network *network, InjectionCurrentData* current_data, unsigned int trial_start_available_current_num, TimeStamp current_length);
bool submit_current_length_in_refractory_status(Network *network, InjectionCurrentData* current_data, unsigned int in_refractory_current_num, TimeStamp current_length);
bool submit_initialization_current(InjectionCurrentData* current_data, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, TimeStamp duration, double max_current);
bool submit_noise_params(InjectionCurrentData* current_data, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, TimeStamp noise_addition_interval, double noise_variance);
bool submit_current_template_sample(InjectionCurrentData* current_data, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, TimeStamp sampling_time, double current);
bool interrogate_neuron_current_pattern_params(InjectionCurrentData* current_data, unsigned int trial_num, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num);


#endif
