#ifndef CURRENT_PATTERNS_H
#define CURRENT_PATTERNS_H

typedef struct __InitializationCurrentParams InitializationCurrentParams;
typedef struct __CurrentNoiseParams CurrentNoiseParams;
typedef struct __NeuronCurrent NeuronCurrent;
typedef struct __NeuronCurrentTemplateData NeuronCurrentTemplateData;
typedef struct __CurrentPatterns CurrentPatterns;
typedef struct __CurrentPatternTemplate CurrentPatternTemplate;
typedef struct __CurrentPattern CurrentPattern;
typedef struct __CurrentPatternBuffer CurrentPatternBuffer;


#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../Network/Network.h"
#include "../../../TrialControllers/Library/TrialStats/TrialStats.h"


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
struct __NeuronCurrent
{
	double 	*pattern;
};

struct __NeuronCurrentTemplateData
{
	InitializationCurrentParams	init_params;
	CurrentNoiseParams 		noise_params;
	NeuronCurrent			pattern_template;	
};

struct __CurrentPatterns
{
	TrialStats				*trial_stats;	// get num of patterns from num of trials
	Network 				*network;    // whose pattern is this?
	CurrentPatternTemplate	*current_pattern_templates;         //trial_stats->num_of_trial_types
	CurrentPattern			*head;
	CurrentPattern			*tail;		// current pattern
	CurrentPatternBuffer		*buffer; 
};

struct __CurrentPatternTemplate
{
	TimeStamp				sampling_interval;
	unsigned int				num_of_samples;
	TimeStamp				*sampling_times;
	NeuronCurrentTemplateData	***neuron_current_templates;
};

struct __CurrentPattern
{
	NeuronCurrent			***neuron_current_pattern;
	TimeStamp			*sampling_times;
	unsigned int			num_of_samples;			// determine this according to write_idx in SingleCurrentPattern
	double				initialization_current;
	CurrentPattern			*prev;
	CurrentPattern			*next;
};

struct __CurrentPatternBuffer
{
	NeuronCurrent			***neuron_current_pattern;
	TimeStamp			*sampling_times;			// num_of_samples in CurrentPatternTemplate
	unsigned int			write_idx;
};


CurrentPatterns* allocate_current_patterns(Network *network, TrialStats *trial_stats, CurrentPatterns* current_patterns);
CurrentPatterns* deallocate_current_patterns(Network *network, TrialStats *trial_stats, CurrentPatterns* current_patterns);
bool increment_current_template_types_in_current_pattern_templates(CurrentPatterns* current_patterns, TimeStamp sampling_interval);
bool submit_initialization_current(CurrentPatterns* current_patterns, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, TimeStamp duration, double max_current);
bool submit_noise_params(CurrentPatterns* current_patterns, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, TimeStamp	noise_addition_interval, double noise_variance);
bool submit_current_template_sample(CurrentPatterns* current_patterns, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num, TimeStamp sampling_time, double current);
bool interrogate_neuron_current_pattern_params(CurrentPatterns* current_patterns, unsigned int trial_num, unsigned int pattern_type_num, unsigned int layer, unsigned int neuron_group, unsigned int neuron_num);
bool get_current_pattern(CurrentPatterns* current_patterns, unsigned int trial_num, CurrentPattern **current_pattern);
#endif
