#ifndef CURRENT_PATTERNS_H
#define CURRENT_PATTERNS_H


typedef struct __CurrentPatterns CurrentPatterns;
typedef struct __CurrentPatternTemplate CurrentPatternTemplate;
typedef struct __InitializationCurrent InitializationCurrent;
typedef struct __InitializationCurrentNeuron InitializationCurrentNeuron;
typedef struct __CurrentTemplate CurrentTemplate;
typedef struct __NeuronCurrentTemplate NeuronCurrentTemplate;
typedef struct __CurrentNoiseParams CurrentNoiseParams;
typedef struct __NeuronCurrentNoiseParams NeuronCurrentNoiseParams;
typedef struct __CurrentPattern CurrentPattern;
typedef struct __NeuronCurrentPattern NeuronCurrentPattern;


#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"
#include "../../../TrialControllers/Library/TrialStats/TrialStats.h"


struct __CurrentPatterns
{
	TrialStats				*trial_stats;	// get num of patterns from num of trials
	Network 				*network;    // whose pattern is this?
	CurrentPatternTemplate	*current_pattern_templates;         //trial_stats->num_of_trial_types
	CurrentPattern			*head;
	CurrentPattern			*tail;		// current pattern
};

struct __InitializationCurrentNeuron
{
	double		max_current;
	TimeStamp	duration;
};

struct __InitializationCurrent
{
	InitializationCurrentNeuron		***init_current_neuron;
};

struct __CurrentTemplate
{
	TimeStamp				sampling_interval;
	unsigned int				num_of_samples;	
	NeuronCurrentTemplate		***neuron_current_templates;
};

struct __NeuronCurrentTemplate
{
	double *current;
};

struct __CurrentNoiseParams
{
	NeuronCurrentNoiseParams ***neuron_noise_params;
};

struct __NeuronCurrentNoiseParams
{
	TimeStamp	noise_addition_interval;	
	double		noise_variance;	
};

struct __CurrentPatternTemplate
{
	TrialType				template_type;
	InitializationCurrent		init_currents;
	CurrentTemplate		current_templates;
	CurrentNoiseParams		noise_params;
};

struct __CurrentPattern
{
	TrialType				pattern_type;
	NeuronCurrentPattern		***neuron_current_pattern;
	TimeStamp			*sampling_times;
	unsigned int			num_of_samples;			// determine this according to write_idx in SingleCurrentPattern
	CurrentPattern			*prev;
	CurrentPattern			*next;
};

struct __NeuronCurrentPattern
{
	double	initialization_current;
	double	*current;                      
};

struct __SingleCurrentPattern
{
	TrialType				pattern_type;
	NeuronCurrentPattern		***neuron_current_pattern;
	TimeStamp			*sampling_times;
	unsigned int			write_idx;
	unsigned int			allocated_num_of_samples;		
};


CurrentPatterns* allocate_current_patterns(Network *network, TrialStats *trial_stats, CurrentPatterns* current_patterns);
CurrentPatterns* deallocate_current_patterns(Network *network, TrialStats *trial_stats, CurrentPatterns* current_patterns);
bool add_trial_type_to_current_pattern_templates(CurrentPatterns* current_patterns, TrialType pattern_type, TimeStamp sampling_interval);
bool submit_initialization_current(CurrentPatterns* current_patterns, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, TimeStamp duration, double max_current);
bool submit_noise_params(CurrentPatterns* current_patterns, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, TimeStamp	noise_addition_interval, double noise_variance);
bool submit_current_template_sample(CurrentPatterns* current_patterns, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, TimeStamp sampling_time, double current);

#endif
