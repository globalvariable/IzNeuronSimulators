#ifndef INJECTION_CURRENT_DATA_H
#define INJECTION_CURRENT_DATA_H

typedef struct __InitializationCurrentParams InitializationCurrentParams;
typedef struct __CurrentNoiseParams CurrentNoiseParams;
typedef double InjectionCurrent;
typedef struct __NeuronCurrentTemplate NeuronCurrentTemplate;
typedef struct __CurrentPatternTemplate CurrentPatternTemplate;
typedef struct __CurrentTemplate CurrentTemplate;
typedef struct __NeuronCurrentSample NeuronCurrentSample;
typedef struct __CurrentPatternBuffer CurrentPatternBuffer;
typedef struct __CurrentStartTimes CurrentStartTimes;
typedef struct __SelectedNeuronCurrents SelectedNeuronCurrents;
typedef struct __SelectedNeuronCurrList SelectedNeuronCurrList;
typedef struct __CurrentPatternBufferLimited CurrentPatternBufferLimited;
typedef struct __ConstantCurrent ConstantCurrent;

#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../BlueSpike/Library/Misc/Randn.h"
#include "../Network/Network.h"
#include "../../../ExperimentHandlers/Applications/BMIExpController/ExpParadigm/TrialHandParadigm.h"

struct __CurrentNoiseParams
{
	TimeStamp	prev_noise_addition_time;	
	TimeStamp	noise_addition_interval;	
	double		noise_variance;	
};
struct __NeuronCurrentSample
{
	InjectionCurrent		***current_sample;
};
struct __CurrentPatternTemplate
{
	CurrentNoiseParams		***noise_params;
	NeuronCurrentSample		*template_samples;
	unsigned int				num_of_template_samples;
	TimeStamp				template_start_time;
	TimeStamp				template_length;
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

struct __SelectedNeuronCurrents
{
	double	*neuron_current;	// num_of_selected_neurons
};

struct __SelectedNeuronCurrList
{	
	unsigned int	layer;
	unsigned int	neuron_group;
	unsigned int	neuron_num;
};

struct __CurrentPatternBuffer
{
	pthread_mutex_t 			mutex;
	NeuronCurrentSample		*current_buffer;
	unsigned int				buff_write_idx;
	unsigned int				buffer_size;
	TimeStamp				last_sample_time;
};

struct __CurrentPatternBufferLimited		// For faster handling of current buffer.
{
	pthread_mutex_t 			mutex;
	SelectedNeuronCurrents	*buffer;
	unsigned int 				buff_write_idx;
	unsigned int 				buffer_size;
	TimeStamp				last_sample_time;
	SelectedNeuronCurrList		*selected_currs;
	unsigned int 				num_of_selected_neurons;
};

struct __ConstantCurrent
{
	InjectionCurrent		***current;
};

CurrentTemplate* allocate_current_templates(Network *network, TrialHandParadigmRobotReach *paradigm, CurrentTemplate* current_data, unsigned int num_of_trial_start_available_currents, unsigned int num_of_in_refractory_currents, unsigned int num_of_in_trial_currents);
CurrentTemplate* deallocate_current_templates(Network *network, TrialHandParadigmRobotReach *paradigm, CurrentTemplate* current_data);

ConstantCurrent* allocate_constant_current(Network * network, ConstantCurrent* constant_current);
ConstantCurrent* deallocate_constant_current(Network * network, ConstantCurrent* constant_current);

bool submit_current_length_trial_start_available_status(Network *network, CurrentTemplate* current_data, unsigned int trial_start_available_current_num, TimeStamp current_length, bool *has_unallocated_current_template);
bool submit_current_length_in_refractory_status(Network *network, CurrentTemplate* current_data, unsigned int in_refractory_current_num, TimeStamp current_length, bool *has_unallocated_current_template);

CurrentPatternBuffer* allocate_current_pattern_buffer(Network *network, CurrentPatternBuffer *buffer, unsigned int buffer_size);
CurrentPatternBuffer* deallocate_current_pattern_buffer(Network *network, CurrentPatternBuffer *buffer);

bool determine_in_trial_current_number_randomly(CurrentTemplate* current_template, unsigned int *current_num);	
bool determine_trial_start_available_current_number_randomly(CurrentTemplate* current_template, unsigned int *current_num);
bool determine_in_refractory_current_number_randomly(CurrentTemplate* current_template, unsigned int *current_num);

bool get_in_trial_current_pattern_template(CurrentTemplate* current_template, CurrentPatternTemplate** pattern_template, unsigned int trial_type_idx, unsigned int current_num);
bool get_trial_start_available_current_pattern_template(CurrentTemplate* current_template, CurrentPatternTemplate** pattern_template, unsigned int current_num);	
bool get_in_refractory_current_pattern_template(CurrentTemplate* current_template, CurrentPatternTemplate** pattern_template, unsigned int current_num);

void reset_prev_noise_addition_times_for_current_template(Network *network, CurrentPatternTemplate* pattern_template);	
bool load_current_template_sample_to_neurons_with_noise(Network *network, CurrentPatternTemplate* pattern_template, unsigned int current_template_read_idx, TimeStamp now);
bool push_neuron_currents_to_current_pattern_buffer(Network *network, CurrentPatternBuffer* current_pattern_buffer, TimeStamp sampling_time);
bool get_current_pattern_buffer_last_sample_time_and_write_idx(CurrentPatternBuffer *buffer, TimeStamp *last_sample_time, unsigned int *write_idx);

CurrentPatternBufferLimited* allocate_current_pattern_buffer_limited(Network *network, CurrentPatternBufferLimited* buffer, unsigned int buffer_size, unsigned int num_of_selected_neurons);
CurrentPatternBufferLimited* deallocate_current_pattern_buffer_limited(Network *network, CurrentPatternBufferLimited* buffer);
bool submit_selected_neuron_to_current_pattern_buffer_limited(Network *network, CurrentPatternBufferLimited* buffer, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num,  unsigned int list_idx);
bool push_neuron_currents_to_neuron_current_buffer_limited(Network *network, CurrentPatternBufferLimited* buffer, TimeStamp sampling_time);
bool get_current_pattern_buffer_limited_last_sample_time_and_write_idx(CurrentPatternBufferLimited *buffer, TimeStamp *last_sample_time, unsigned int *write_idx);
#endif
