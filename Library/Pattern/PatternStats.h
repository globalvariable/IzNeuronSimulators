#ifndef PATTERN_STATS_H
#define PATTERN_STATS_H


typedef struct __PatternStats PatternStats;
typedef struct __PatternData PatternData;

#include "../../../BlueSpike/Library/Misc/Misc.h"
#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../../BlueSpike/TimeStamp.h"
#include "PatternType.h"

struct __PatternStats
{
	PatternData	*patterns_data;
	PatternData	*current_pattern_data;	
	unsigned int 	num_of_patterns;
	TimeStamp	max_pattern_length;   
	TimeStamp	min_pattern_length;   	
	TimeStamp	max_pattern_interval; 
	TimeStamp	min_pattern_interval; 	  		
};

struct __PatternData
{
	TimeStamp	pattern_start_time;   
	TimeStamp	pattern_length;
	PatternType	type;	// left, right, tracjectroy right etc. 
	PatternData	*next;
};



#endif
