#ifndef BMI_SPIKE_GENERATOR_H
#define BMI_SPIKE_GENERATOR_H



typedef struct __SpikeGenData SpikeGenData;

#include "Gui.h"
#include "BMISpikeGeneratorRtTask.h"
#include "../../../BlueSpike/SpikeTimeStamp.h"
#include "../../../BlueSpike/System/ShmSemNum/ShmSemNum.h"
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../Library/InjectionCurrentData/InjectionCurrentData.h"
#include "../../Library/NeuronDynamicsData/NeuronDynamicsData.h"
#include "../../Library/SpikeData/SpikeData.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2SpikeGen.h"
#include "../../../ExperimentHandlers/Library/Messages/SpikeGen2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Status/TrialStatus.h"

#define LEFT_ROBOT_TARGET		0
#define RIGHT_ROBOT_TARGET	1


struct __SpikeGenData		// get gloabal trial stats by searching though TrialTypeData
{
	SpikeTimeStamp 				*sorted_spike_time_stamp;    /// spike time stamps from biological neurons 
	RtTasksData					*rt_tasks_data;
	Network						*network;
	CurrentTemplate				*current_templates;
	CurrentPatternBufferLimited		*limited_current_pattern_buffer;
	NeuronDynamicsBufferLimited	*limited_neuron_dynamics_buffer;
	SpikeData					**spike_data;
	TrialHand2SpikeGenMsg		*msgs_trial_hand_2_spike_gen;
	SpikeGen2TrialHandMsg		*msgs_spike_gen_2_trial_hand;
	TrialStatusEvents				*trial_status_events;
	TrialHandParadigmRobotReach	*trial_hand_paradigm;
};


SpikeGenData* get_bmi_simulation_spike_generator_data(void);



#endif


