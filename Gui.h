#ifndef GUI_H
#define GUI_H

#include "RTHandler.h"
#include "Notebooks.h"

GtkWidget *entry_num_of_layers;
GtkWidget *entry_num_of_neuron_for_group;
GtkWidget *entry_v;
GtkWidget *entry_a;
GtkWidget *entry_b;
GtkWidget *entry_c;
GtkWidget *entry_d;
GtkWidget *entry_k;
GtkWidget *entry_C;
GtkWidget *entry_E_excitatory;
GtkWidget *entry_excitatory_tau;  
GtkWidget *entry_E_inhibitory;
GtkWidget *entry_inhibitory_tau;
GtkWidget *entry_v_threshold;
GtkWidget *entry_v_peak;	
GtkWidget *entry_v_resting;
GtkWidget *entry_I_inject;
GtkWidget *entry_randomize_params;
GtkWidget *entry_inhibitory;
GtkWidget *entry_buffer_size_for_dynamics_log;
GtkWidget *entry_add_nrn_grp_to_layer;


GtkWidget *btn_create_network;
GtkWidget *btn_log_neuron_dynamics_on_off;
GtkWidget *btn_submit_neuron_dynamics_logging_buffer_size ;
GtkWidget *combo_neuron_type;
GtkWidget *btn_add_nrn_grp_to_layer;


GtkWidget *entry_weight_excitatory_min;
GtkWidget *entry_weight_excitatory_max;
GtkWidget *entry_weight_inhibitory_min;
GtkWidget *entry_weight_inhibitory_max;

GtkWidget *entry_excitatory_delay_min;
GtkWidget *entry_excitatory_delay_max;
GtkWidget *entry_inhibitory_delay_min;
GtkWidget *entry_inhibitory_delay_max;

GtkWidget *entry_layer_num_to_connect;
GtkWidget *entry_layer_num_to_connect_to;
GtkWidget *entry_connection_probability;

GtkWidget *btn_connect_layer_to_layer;

GtkWidget *entry_parker_sochacki_max_order;
GtkWidget *entry_parker_sochacki_error_tolerance; 
GtkWidget *entry_newton_raphson_max_iter;
GtkWidget *entry_newton_raphson_error_tolerance;
GtkWidget *entry_simulation_given_step_size_ns;
GtkWidget *btn_submit_parker_sochacki_params;
GtkWidget *btn_clear_parker_sochacki_polynomials;

GtkWidget *btn_start_stop_simulation;
GtkWidget *btn_resume_nonrt_simulation;
GtkWidget *entry_simulation_duration;
GtkWidget *btn_start_stop_rt_simulation;
GtkWidget *btn_self_clock;

GtkWidget *entry_interrogate_layer_num;
GtkWidget *entry_interrogate_neuron_group_num;
GtkWidget *entry_interrogate_neuron_num;


GtkWidget *btn_interrogate_neuron;
GtkWidget *btn_interrogate_neuron_group;
GtkWidget *btn_interrogate_layer;
GtkWidget *btn_reset_neurons_in_group;
GtkWidget *btn_reset_neuron;
GtkWidget *btn_destroy_all_connections_btwn_layers;
GtkWidget *btn_reset_all_synapses_with_new_params;
GtkWidget *btn_reset_all_neurons;

GtkWidget *btn_submit_number_of_frames_for_neuron_dynamics;
GtkWidget *entry_number_of_frames_for_neuron_dynamics;

// Functions
void create_gui(void);
void create_network_button_func(void);
void combo_neuron_type_func(void);
void add_nrn_grp_to_layer_button_func(void);

void connect_layer_to_layer_button_func(void);

void submit_parker_sochacki_params_button_func(void);
void clear_parker_sochacki_polynomials_button_func(void);

void start_stop_simulation_button_func(void);
void resume_nonrt_simulation_button_func(void);
void start_stop_rt_simulation_button_func(void);
void self_clock_button_func(void);

void interrogate_neuron_button_func(void);
void interrogate_neuron_group_button_func(void);
void interrogate_layer_button_func(void);

void log_neuron_dynamics_on_off_button_func(void);
void submit_logging_buffer_size_button_func(void);

void destroy_all_connections_btwn_layers_button_func(void);
void reset_neurons_in_group_button_func(void);
void reset_neuron_button_func(void);
void reset_all_synapses_with_new_params_button_func(void);
void reset_all_neurons_button_func(void);

void submit_number_of_frames_for_neuron_dynamics_button_func(void);

#endif
