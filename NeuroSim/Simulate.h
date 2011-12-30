#ifndef SIMULATE_H
#define SIMULATE_H


#include "Network/Network.h"
#include "ParkerSochacki/ParkerSochacki.h"
#include "Logs.h"



bool non_rt_simulation_ongoing;
bool rt_simulation_ongoing;

double nonrt_simulation_duration_ms;

unsigned int simulation_given_step_size_ns;

void simulate_parker_sochacki(void);


#endif
