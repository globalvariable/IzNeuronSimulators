#ifndef HYBRID_NET_RL_BMI_RT_TASK_H
#define HYBRID_NET_RL_BMI_RT_TASK_H


#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "HybridNetRLBMI.h"
#include "../../../BlueSpike/Library/RtTaskStats/RtTaskStats.h"


void hybrid_net_rl_bmi_create_rt_threads(void);
void hybrid_net_rl_bmi_kill_rt_tasks(void);












#endif
