#include "MotorOutputTimeStamp.h"



void write_to_motor_output_time_stamp_buffer(MotorOutputTimeStamp *time_stamps, unsigned int motor_output_num, TimeStamp output_time, MotorOutputType output_type, MotorOutputAdditionalData additional_data)
{
	unsigned int *idx;
	MotorOutputTimeStamp *motor_output_from_neural_net = &(time_stamps[motor_output_num]);
	MotorOutputTimeStampItem *item; 
	idx = &(motor_output_from_neural_net->buff_write_idx);
	item = &(motor_output_from_neural_net->buff[*idx]);
	item->output_time = output_time;
	item->output_type = output_type;
	item->additional_data = additional_data;
	if (((*idx) + 1) ==  MOTOR_OUTPUT_TIME_STAMP_BUFF_SIZE)
		(*idx) = 0;
	else
		(*idx)++;
	return ;
}
bool get_next_motor_output_buffer_item(MotorOutputTimeStamp *time_stamps, unsigned int motor_output_num, MotorOutputTimeStampItem **output_item)  // only output handler can use this function
{
	MotorOutputTimeStamp *motor_output_from_neural_net; 
	motor_output_from_neural_net = &(time_stamps[motor_output_num]);
	if (motor_output_from_neural_net->buff_read_idx ==motor_output_from_neural_net->buff_write_idx)
		return FALSE;
	*output_item = &(motor_output_from_neural_net->buff[motor_output_from_neural_net->buff_read_idx]);
	if ((motor_output_from_neural_net->buff_read_idx + 1) == MOTOR_OUTPUT_TIME_STAMP_BUFF_SIZE)
		motor_output_from_neural_net->buff_read_idx = 0;
	else
		motor_output_from_neural_net->buff_read_idx++;
	return TRUE;
}
