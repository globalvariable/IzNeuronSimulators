#include "Gui.h"

static RtTasksData *rt_tasks_data = NULL;
static NeuralNet2GuiMsg *static_msgs_neural_net_2_gui = NULL;
static GtkWidget *btn_select_directory_to_save = NULL;
static SpikeData **in_silico_spike_data_for_recording = NULL;
static unsigned int num_of_dedicated_cpu_threads = 0;

static GtkWidget *hybrid_net_rl_bmi_gui_tabs;

GtkWidget *get_gui_tabs(void)
{
	return hybrid_net_rl_bmi_gui_tabs;
}



static gboolean timeout_callback(gpointer user_data) ;

void create_gui(void)
{
	GtkWidget *window, *vbox;

	static_msgs_neural_net_2_gui = get_hybrid_net_rl_bmi_data()->msgs_neural_net_2_gui;
	rt_tasks_data = get_hybrid_net_rl_bmi_data()->rt_tasks_data;
	in_silico_spike_data_for_recording = get_hybrid_net_rl_bmi_data()->in_silico_spike_data_for_recording;
	num_of_dedicated_cpu_threads = get_hybrid_net_rl_bmi_data()->num_of_dedicated_cpu_threads;

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

  	btn_select_directory_to_save = gtk_file_chooser_button_new ("Select Directory", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);

 	if (! create_network_view_gui(btn_select_directory_to_save))
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "create_gui", "create_gui().");

	gtk_widget_show_all(window);

  	g_timeout_add(50, timeout_callback, NULL);		
		   				
	return;
}

static gboolean timeout_callback(gpointer user_data) 
{
	char *path_temp, *path;
	static bool recording = FALSE;
	unsigned int i, recording_number;
	NeuralNet2GuiMsgItem msg_item;
	SpikeData *spike_data;

	while (get_next_neural_net_2_gui_msg_buffer_item(static_msgs_neural_net_2_gui, &msg_item))
	{
		switch (msg_item.msg_type)
		{
			case NEURAL_NET_2_GUI_MSG_START_RECORDING:
				path_temp = NULL; path = NULL;
				path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
				path = &path_temp[7];   // since     uri returns file:///home/....	
				recording_number = msg_item.additional_data;
				if (!(*create_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(5, path, &rt_tasks_data->current_system_time, recording_number, in_silico_spike_data_for_recording, num_of_dedicated_cpu_threads))	
				{
					print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "timeout_callback", " *create_data_directory().");		
					exit(1);
				}
				recording = TRUE;	
				break;
			case NEURAL_NET_2_GUI_MSG_STOP_RECORDING:
				if (! (*fclose_all_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(3, &rt_tasks_data->current_system_time, in_silico_spike_data_for_recording, num_of_dedicated_cpu_threads))	
				{
					print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "timeout_callback", " *fclose_all_data_file().");		
					exit(1);
				}
				recording = FALSE;		
				break;
			case NEURAL_NET_2_GUI_MSG_CANCEL_RECORDING:
				path_temp = NULL; path = NULL;
				path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
				path = &path_temp[7];   // since     uri returns file:///home/....		

				recording_number = msg_item.additional_data;
				if (! (*fclose_all_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(3, &rt_tasks_data->current_system_time, in_silico_spike_data_for_recording, num_of_dedicated_cpu_threads))	
				{
					print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "timeout_callback", "! *fclose_all_data_files().");
					exit(1);
				}
				if (! (*delete_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, path, recording_number))
				{
					print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "timeout_callback", " *delete_all_data_files().");
					exit(1);
				}
				recording = FALSE;
				break;
			default:
				return print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "timeout_callback", "switch (msg_item.msg_type) - default");
		}
	}
	if (recording)
	{
		if (!(*write_to_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, in_silico_spike_data_for_recording, num_of_dedicated_cpu_threads))	
		{
			print_message(ERROR_MSG ,"HybridNetRLBMI", "Gui", "timeout_callback", " *write_to_data_files().");		
			exit(1);
		}	
	}
	else
	{
		for (i = 0; i < num_of_dedicated_cpu_threads; i++)
		{
			spike_data = in_silico_spike_data_for_recording[i];
			spike_data->buff_idx_read = spike_data->buff_idx_write;
		}
	}
	return TRUE;
}
