#define FRAMES_H

#include "Graphs.h"
#ifndef NETWORK_H
#include "Network.h"
#endif

#define MAX_NUM_OF_FRAME_FOR_NEURON_DYNAMICS 10
#define NUM_OF_GRAPH_PER_FRAME 4

GtkWidget *tabs;

GtkWidget *frame_entries;
GtkWidget *label_frame_entries;

GtkWidget *frame_network_visualization;
GtkWidget *label_network_visualization;

GtkWidget **neuron_dynamics_frame;
GtkWidget **neuron_dynamics_frame_label;
GtkWidget **neuron_dynamics_table;

GtkWidget **entry_neuron_dynamics_neuron_num;
GtkWidget **entry_neuron_dynamics_neuron_group;
GtkWidget **entry_neuron_dynamics_neuron_layer;
GtkWidget **entry_neuron_dynamics_graph_num;
GtkWidget **combo_neuron_dynamics_value;
GtkWidget **btn_neuron_dynamics_assign;
GtkWidget **btn_neuron_dynamics_refresh;

typedef struct grph_nrn_map
{
	int neuron_layer;
	int neuron_group;
	int neuron_num;
	int dynamic_value;
} graph_to_neuron_map;

graph_to_neuron_map **graph_to_neuron_map_table;

int num_of_frames_for_neuron_dynamics;

bool allocate_frames_and_graphs_for_neuron_dynamics(int num_of_frm);
void refresh_all_neuron_dynamics_graph(void);

void assign_neuron_dynamics_button_func(void);
void refresh_neuron_dynamics_button_func(void);

