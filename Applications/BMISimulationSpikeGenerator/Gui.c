#include "Gui.h"

static GtkWidget *bmi_simulation_spike_generator_gui_tabs;

GtkWidget *get_gui_tabs(void)
{
	return bmi_simulation_spike_generator_gui_tabs;
}


void create_gui(void)
{
	GtkWidget *window, *vbox;

 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1020);
  	gtk_window_set_deletable(GTK_WINDOW(window), FALSE);
//  	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  	gtk_window_set_title(GTK_WINDOW(window), "BMISimulationSpikeGenerator");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  	vbox = gtk_vbox_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	bmi_simulation_spike_generator_gui_tabs = gtk_notebook_new ();
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (bmi_simulation_spike_generator_gui_tabs), GTK_POS_TOP);
        gtk_box_pack_start(GTK_BOX(vbox),bmi_simulation_spike_generator_gui_tabs, TRUE, TRUE, 0);

 	if (! create_current_pattern_view_gui())
		return;     

	gtk_widget_show_all(window);

//	initialize_spike_pattern_generator_data_read_write_handlers();
	
  	g_signal_connect (GTK_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
  		   				
	return;
}


