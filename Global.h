#include <stdlib.h>
#include <stdbool.h>
#include <rtai_serial.h>
#include <rtai_lxrt.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>

#define SHMSIZ  0
#define NUM_OF_CHAN 16 
#define NUM_OF_SAMP_IN_BUFF 40000  
#define BUFFNAME  "SHBUFF"
#define SPIKE_MIN_END_SAMP_NUM   15

#define NUM_OF_SAMP_PER_SPIKE 30
#define NUM_OF_TEMP_PER_CHAN 3

#define MS100    100000000 //100 msec
#define MS20     20000000 //20 msec
#define MS10     10000000 //10 msec

#define MY_CPU_ID 3 

#define TIMESTEP 20
#define NOSE_POKE_INTRVL 500 //msec
#define ENABLE_LEVER_INTRVL 100 //msec

#define RX_BUFLEN 3
#define TX_BUFLEN 2

#define LEFTTARGET 0
#define RIGHTTARGET 1
#define TARGET_SIDE_HISTORY 4

typedef struct{
	union
	{
		struct{
			unsigned OpenValve:1;
			unsigned ResetTrial:1;
			unsigned RightLED:1;
			unsigned LeftLED:1;
			unsigned Buzzer:1;
			unsigned EnableLever:1;
			unsigned None6:1;						
			unsigned None7:1;
		};
		unsigned AllCommand;
	} Command;
	union
	{
		struct{
			unsigned RightLever:1;
			unsigned LeftLever:1;
			unsigned NosePoke:1;
			unsigned None1:1;
			unsigned Resetting:1;
			unsigned ReachedRightTarget:1;
			unsigned ReachedLeftTarget:1;						
			unsigned None7:1;
		};
		unsigned AllStatus;
	} Status;
	short int ShortInt_Status0;
	short int ShortInt_Status1;	 	
} ExpEnv; 

typedef struct
{ 
	unsigned char Phase;
	union 
	{
		struct
		{
			unsigned char low;
			unsigned char high;
		};
		short int Location;
	} Location;	
	union 
	{
		struct
		{
			unsigned Speed:4;
			unsigned Direction:1;
			unsigned Check:3;
		};
		unsigned AllCommand;
	} Command; 	
} StpMtr;


ExpEnv Environment;
StpMtr StepMotor;

union 			
{
	struct 
	{
		unsigned TrialAvailable:1;
		unsigned InTrial:1;
		unsigned TrialResetRequest:1;
		unsigned RewardRequest:1;
		unsigned BetweenTrial:1;
		unsigned LeverPress:1;
		unsigned RightTargetSelected:1;						
		unsigned LeftTargetSelected:1;
		unsigned RandomTargetSelected:1;
		unsigned RandomTrajectorySelected:1;
		unsigned BuzzerRequest:1;
		unsigned InvalidNosePoke:1;	
		unsigned GuideMoveAvailable:1;	
		unsigned FirstNosePoke:1;
		unsigned SecondNosePoke:1;
		unsigned BetweenNosePokes:1;
		unsigned RandomSpeedSelected:1;
		unsigned EnableLeverRequest:1;
		unsigned Speed_0:4;
	} ;
	unsigned AllFlags;     //unsigned int
} CageManagerFlags;

union 			
{
	struct 
	{
		unsigned Motor_0_Direction:1;
		unsigned Motor_0_Speed:4;
		unsigned None0t:1;
		unsigned None1:1;
		unsigned None2:1;
		unsigned None3:1;
		unsigned None4:1;						
		unsigned None5:1;
		unsigned None6:1;
		unsigned None7:1;
		unsigned None8:1;
		unsigned None9:1;	
		unsigned None10:1;	
		unsigned None11:1;
		unsigned None12:1;
		unsigned None13:1;
		unsigned None14:1;
		unsigned None15:1;
		unsigned None16:4;
	} ;
	unsigned AllFlags;			//unsigned int
} NeuroControllerFlags;

typedef struct sample_data
{
	float data[NUM_OF_CHAN];
} scan_data;

typedef struct spk_data
{
	int data[NUM_OF_CHAN];
} spike_data;

typedef struct templ_data
{
	double template[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN][NUM_OF_SAMP_PER_SPIKE];   //mean
	double S[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN][NUM_OF_SAMP_PER_SPIKE][NUM_OF_SAMP_PER_SPIKE]; //covariance
	double  inv_S[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN][NUM_OF_SAMP_PER_SPIKE][NUM_OF_SAMP_PER_SPIKE];
	double sqrt_det_S[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN];
	double log_det_S[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN];	
	double diff_thres[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN];
	bool sorting_on[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN];
	bool include_unit[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN];
} template_matching_data;

typedef struct{
	union
	{
		struct{
			unsigned Bit0:1;
			unsigned Bit1:1;
			unsigned Bit2:1;
			unsigned Bit3:1;
			unsigned Bit4:1;
			unsigned Bit5:1;
			unsigned Bit6:1;						
			unsigned Bit7:1;
			unsigned Bit8:1;
			unsigned Bit9:1;
			unsigned Bit10:1;
			unsigned Bit11:1;
			unsigned Bit12:1;
			unsigned Bit13:1;
			unsigned Bit14:1;						
			unsigned Bit15:1;
			unsigned Bit16:1;
			unsigned Bit17:1;
			unsigned Bit18:1;
			unsigned Bit19:1;
			unsigned Bit20:1;						
			unsigned Bit21:1;
			unsigned Bit22:1;						
			unsigned Bit23:1;
		};
		unsigned AllFlags;
	} StatusFlags;
} StaFlag; 

typedef struct{
	union
	{
		struct{
			unsigned Bit0:1;
			unsigned Bit1:1;
			unsigned Bit2:1;
			unsigned Bit3:1;
			unsigned Bit4:1;
			unsigned Bit5:1;
			unsigned Bit6:1;						
			unsigned Bit7:1;
			unsigned Bit8:1;
			unsigned Bit9:1;
			unsigned Bit10:1;
			unsigned Bit11:1;
			unsigned Bit12:1;
			unsigned Bit13:1;
			unsigned Bit14:1;						
			unsigned Bit15:1;
			unsigned Bit16:1;
			unsigned Bit17:1;
			unsigned Bit18:1;
			unsigned Bit19:1;
			unsigned Bit20:1;						
			unsigned Bit21:1;
			unsigned Bit22:1;						
			unsigned Bit23:1;
		};
		unsigned AllFlags;
	} CommandFlags;
} CmndFlag; 

typedef struct buff_data 
{
	int scan_number_write;
	int scan_number_read;
	scan_data scan[NUM_OF_SAMP_IN_BUFF];
	scan_data filtered_scan[NUM_OF_SAMP_IN_BUFF];
	spike_data spike_peak[NUM_OF_SAMP_IN_BUFF];
	spike_data spike_end[NUM_OF_SAMP_IN_BUFF];	
	float Threshold[NUM_OF_CHAN];
	bool in_spike[NUM_OF_CHAN];	
	bool filter_on;
	ExpEnv Environment[NUM_OF_SAMP_IN_BUFF];
	ExpEnv Curr_Environment;
	template_matching_data spike_template; 	
	bool sorting_on;
	spike_data sorted_spike_data[NUM_OF_SAMP_IN_BUFF];
	bool highpass_400Hz_on;
	StaFlag	RTStatusFlags[NUM_OF_SAMP_IN_BUFF];
	StaFlag	Curr_RTStatusFlags;
	int jitter[1000];
	int jitter_idx;	
	bool lowpass_4th_on;
	CmndFlag	RTCommandFlags[NUM_OF_SAMP_IN_BUFF];
	CmndFlag	Curr_RTCommandFlags;	
} buff_data_struct;

buff_data_struct *buff;

