#define LOGS_H

#ifndef NEURON_H
#include "Neuron.h"
#endif

#define MAX_BUFF_SIZE_FOR_NEURON_DYNAMICS_LOGGING 10000

int neuron_dynamics_log_buffer_size;
bool neuron_dynamics_logging_on;

void log_neuron_dynamics(neuron *nrn);

void set_buffer_size_for_neuron_dynamics_logging(int buffer_size);
int get_buffer_size_for_neuron_dynamics_logging(void);
void turn_neuron_dynamics_logging_on(void);
void turn_neuron_dynamics_logging_off(void);

void clear_neuron_dynamics_log(neuron *nrn);


