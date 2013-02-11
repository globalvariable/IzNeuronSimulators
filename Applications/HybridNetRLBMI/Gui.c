#include "Gui.h"

static GtkWidget *hybrid_net_rl_bmi_gui_tabs;

GtkWidget *get_gui_tabs(void)
{
	return hybrid_net_rl_bmi_gui_tabs;
}


void create_gui(void)
{
	GtkWidget *window, *vbox;

 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1020);
  	gtk_window_set_deletable(GTK_WINDOW(window), FALSE);
  	gtk_window_set_title(GTK_WINDOW(window), "HybridNetRLBMI");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  	vbox = gtk_vbox_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	hybrid_net_rl_bmi_gui_tabs = gtk_notebook_new ();
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (hybrid_net_rl_bmi_gui_tabs), GTK_POS_TOP);
        gtk_box_pack_start(GTK_BOX(vbox),hybrid_net_rl_bmi_gui_tabs, TRUE, TRUE, 0);



 	if (! create_network_view_gui())
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "create_gui", "create_gui().");

	gtk_widget_show_all(window);

	return;
}


