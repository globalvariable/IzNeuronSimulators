#ifndef MOTOR_OUTPUT_TIME_STAMP_H
#define MOTOR_OUTPUT_TIME_STAMP_H


#define MAX_NUM_OF_MOTOR_OUTPUTS 			1
#define MOTOR_OUTPUT_TIME_STAMP_BUFF_SIZE	100

typedef unsigned int MotorOutputAdditionalData;
typedef struct __MotorOutputTimeStampItem MotorOutputTimeStampItem;
typedef struct __MotorOutputTimeStamp MotorOutputTimeStamp;

#include "MotorOutputTypes.h"
#include "../../../BlueSpike/TimeStamp.h"

struct __MotorOutputTimeStampItem // no MUTEX, only one write process should read here. 
{
	TimeStamp 						output_time;		
	MotorOutputType					output_type;
	MotorOutputAdditionalData 			additional_data;
};

struct __MotorOutputTimeStamp	
{
	MotorOutputTimeStampItem		buff[MOTOR_OUTPUT_TIME_STAMP_BUFF_SIZE];
	unsigned int					buff_write_idx;	// only one output sender can write into this buffer and edit this write index
	unsigned int					buff_read_idx;	// only one output handler can edit this read index
};

void write_to_motor_output_time_stamp_buffer(MotorOutputTimeStamp *time_stamps, unsigned int motor_output_num, TimeStamp output_time, MotorOutputType output_type, MotorOutputAdditionalData additional_data);
bool get_next_motor_output_buffer_item(MotorOutputTimeStamp *time_stamps, unsigned int motor_output_num, MotorOutputTimeStampItem **output_item);  // only output handler can use this function

#endif
