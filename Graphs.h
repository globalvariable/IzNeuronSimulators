#ifndef GRAPHS_H
#define GRAPHS_H

#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>

#include "Logs.h"


GtkWidget *box_input_neurons;
GtkWidget *databox_input_neurons;

GtkWidget *box_hidden_neurons;
GtkWidget *databox_hidden_neurons;

GtkWidget *box_output_neurons;
GtkWidget *databox_output_neurons;

GtkWidget ***neuron_dynamics_box;
GtkWidget ***neuron_dynamics_databox;
GtkDataboxGraph ***neuron_dynamics_graph;

float ***neuron_dynamics_graph_y_axis;
float *neuron_dynamics_graph_x_axis;

int num_of_frames_for_neuron_dynamics;

GdkColor color_bg_network;
GdkColor color_bg_neuron_dynamics;
GdkColor color_neuron_dynamics_line;
GdkColor color_neuron_dynamics_grid;

#endif
