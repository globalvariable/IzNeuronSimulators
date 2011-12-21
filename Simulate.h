#define SIMULATE_H

#ifndef NETWORK_H
#include "Network.h"
#endif
#include "ParkerSochacki.h"

#ifndef LOGS_H
#include "Logs.h"
#endif


bool non_rt_simulation_ongoing;
bool rt_simulation_ongoing;

double nonrt_simulation_duration_ms;

unsigned int simulation_given_step_size_ns;

void simulate_parker_sochacki(void);

