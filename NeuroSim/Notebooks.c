#include "Notebooks.h"
#include <string.h>

bool allocate_frames_and_graphs_for_neuron_dynamics(int num_of_frm)
{
	int i, j;
        GtkWidget *vbox, *lbl;

	num_of_frames_for_neuron_dynamics = num_of_frm;

	neuron_dynamics_frame = g_new0(GtkWidget*, num_of_frames_for_neuron_dynamics);
	neuron_dynamics_frame_label = g_new0(GtkWidget*, num_of_frames_for_neuron_dynamics);
	neuron_dynamics_table = g_new0(GtkWidget*, num_of_frames_for_neuron_dynamics);
	neuron_dynamics_box= g_new0(GtkWidget**, num_of_frames_for_neuron_dynamics);
	neuron_dynamics_databox= g_new0(GtkWidget**, num_of_frames_for_neuron_dynamics);
	neuron_dynamics_graph= g_new0(GtkDataboxGraph **, num_of_frames_for_neuron_dynamics);
	neuron_dynamics_graph_y_axis= g_new0(float**, num_of_frames_for_neuron_dynamics);
	neuron_dynamics_graph_x_axis= g_new0(float, neuron_dynamics_log_buffer_size);
	for (i=0; i<neuron_dynamics_log_buffer_size; i++)
	{
		neuron_dynamics_graph_x_axis[i] = i;
	}
	entry_neuron_dynamics_neuron_num= g_new0(GtkWidget*, num_of_frames_for_neuron_dynamics);
	entry_neuron_dynamics_neuron_group = g_new0(GtkWidget*, num_of_frames_for_neuron_dynamics);
	entry_neuron_dynamics_neuron_layer = g_new0(GtkWidget*, num_of_frames_for_neuron_dynamics);
	entry_neuron_dynamics_graph_num = g_new0(GtkWidget*, num_of_frames_for_neuron_dynamics);
	combo_neuron_dynamics_value	= g_new0(GtkWidget*, num_of_frames_for_neuron_dynamics);
	btn_neuron_dynamics_assign= g_new0(GtkWidget*, num_of_frames_for_neuron_dynamics);
	btn_neuron_dynamics_refresh= g_new0(GtkWidget*, num_of_frames_for_neuron_dynamics);

	graph_to_neuron_map_table = g_new0(graph_to_neuron_map*, num_of_frames_for_neuron_dynamics);

	for (i=0; i<num_of_frames_for_neuron_dynamics; i++)
	{
		char str[20], str_num[20];
		sprintf(str_num, "%d" , i);
		strcpy (str,"Neuron Dyn ");
		strcat (str, str_num);
		neuron_dynamics_frame[i] = gtk_frame_new (str);
		neuron_dynamics_frame_label[i] = gtk_label_new (str); 
		neuron_dynamics_table[i]= gtk_table_new(2,21,TRUE);
		neuron_dynamics_box[i] = g_new0(GtkWidget*, NUM_OF_GRAPH_PER_FRAME);
		neuron_dynamics_databox[i] =g_new0(GtkWidget*, NUM_OF_GRAPH_PER_FRAME);
		neuron_dynamics_graph[i] =g_new0(GtkDataboxGraph *, NUM_OF_GRAPH_PER_FRAME);
		neuron_dynamics_graph_y_axis[i] = g_new0(float*, NUM_OF_GRAPH_PER_FRAME);

		graph_to_neuron_map_table[i] = g_new0(graph_to_neuron_map, NUM_OF_GRAPH_PER_FRAME);

		for (j=0; j<NUM_OF_GRAPH_PER_FRAME; j++)
		{
			gtk_databox_create_box_with_scrollbars_and_rulers (&neuron_dynamics_box[i][j], &neuron_dynamics_databox[i][j],TRUE, TRUE, TRUE, TRUE);
			neuron_dynamics_graph_y_axis[i][j] = g_new0(float, neuron_dynamics_log_buffer_size);
			neuron_dynamics_graph[i][j]= GTK_DATABOX_GRAPH(gtk_databox_lines_new (neuron_dynamics_log_buffer_size, neuron_dynamics_graph_x_axis, neuron_dynamics_graph_y_axis[i][j], &color_neuron_dynamics_line, 0));
			gtk_databox_graph_add (GTK_DATABOX (neuron_dynamics_box[i][j]), neuron_dynamics_graph[i][j]);
			gtk_databox_graph_add (GTK_DATABOX (neuron_dynamics_box[i][j]), gtk_databox_grid_new (21, 21, &color_neuron_dynamics_grid, 0));
	  		gtk_widget_modify_bg (neuron_dynamics_box[i][j], GTK_STATE_NORMAL, &color_bg_neuron_dynamics);
			gtk_widget_show (neuron_dynamics_box[i][j]);
			gtk_widget_show (neuron_dynamics_databox[i][j]);
		}

		gtk_container_add (GTK_CONTAINER (neuron_dynamics_frame[i] ), neuron_dynamics_table[i]);
		vbox = gtk_vbox_new(FALSE, 0);
		lbl = gtk_label_new("Neuron # :");
        	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE,0);
		entry_neuron_dynamics_neuron_num[i]= gtk_entry_new();
        	gtk_box_pack_start(GTK_BOX(vbox),entry_neuron_dynamics_neuron_num[i], FALSE,FALSE,0);
		gtk_widget_set_size_request(entry_neuron_dynamics_neuron_num[i], 10, 25) ;
		lbl = gtk_label_new("Group # :");
        	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE,0);
		entry_neuron_dynamics_neuron_group[i]= gtk_entry_new();
        	gtk_box_pack_start(GTK_BOX(vbox),entry_neuron_dynamics_neuron_group[i], FALSE,FALSE,0);
		gtk_widget_set_size_request(entry_neuron_dynamics_neuron_group[i], 10, 25) ;
		lbl = gtk_label_new("Layer # :");
        	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE,0);
		entry_neuron_dynamics_neuron_layer[i]= gtk_entry_new();
        	gtk_box_pack_start(GTK_BOX(vbox),entry_neuron_dynamics_neuron_layer[i], FALSE,FALSE,0);
		gtk_widget_set_size_request(entry_neuron_dynamics_neuron_layer[i], 10, 25) ;
		lbl = gtk_label_new("Graph # :");
        	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE,0);
		entry_neuron_dynamics_graph_num[i]= gtk_entry_new();
        	gtk_box_pack_start(GTK_BOX(vbox),entry_neuron_dynamics_graph_num[i], FALSE,FALSE,0);
		gtk_widget_set_size_request(entry_neuron_dynamics_graph_num[i], 10, 25) ;
		combo_neuron_dynamics_value[i]= gtk_combo_box_new_text();
        	gtk_box_pack_start(GTK_BOX(vbox),combo_neuron_dynamics_value[i], FALSE,FALSE,0);
		gtk_widget_set_size_request(combo_neuron_dynamics_value[i], 10, 25) ;
		gtk_combo_box_append_text(GTK_COMBO_BOX(combo_neuron_dynamics_value[i]), "v");
		gtk_combo_box_append_text(GTK_COMBO_BOX(combo_neuron_dynamics_value[i]), "u");
		gtk_combo_box_append_text(GTK_COMBO_BOX(combo_neuron_dynamics_value[i]), "exci_cond");	
		gtk_combo_box_append_text(GTK_COMBO_BOX(combo_neuron_dynamics_value[i]), "inhi_cond");	
		btn_neuron_dynamics_assign[i]= gtk_button_new_with_label("Assign");
		gtk_box_pack_start(GTK_BOX(vbox),btn_neuron_dynamics_assign[i], FALSE,FALSE,0);
		btn_neuron_dynamics_refresh[i] = gtk_button_new_with_label("Refresh");
		gtk_box_pack_start(GTK_BOX(vbox),btn_neuron_dynamics_refresh[i], FALSE,FALSE,0);

   		gtk_table_attach_defaults(GTK_TABLE(neuron_dynamics_table[i]), vbox, 20,21,0,2);      // Put adjustments the rightmost

		vbox = gtk_vbox_new(TRUE, 0);
   		gtk_table_attach_defaults(GTK_TABLE(neuron_dynamics_table[i]), vbox, 0,1,0,1);      // Neuron number indicator
		vbox = gtk_vbox_new(TRUE, 0);
   		gtk_table_attach_defaults(GTK_TABLE(neuron_dynamics_table[i]), vbox, 0,1,1,2);  // Neuron number indicator
		vbox = gtk_vbox_new(TRUE, 0);
   		gtk_table_attach_defaults(GTK_TABLE(neuron_dynamics_table[i]), vbox, 10,11,0,1);  // Neuron number indicator
		vbox = gtk_vbox_new(TRUE, 0);
   		gtk_table_attach_defaults(GTK_TABLE(neuron_dynamics_table[i]), vbox, 10,11,1,2);  // Neuron number indicator

  		gtk_table_attach_defaults(GTK_TABLE(neuron_dynamics_table[i]), neuron_dynamics_databox[i][0],1,10,0,1);
 		gtk_table_attach_defaults(GTK_TABLE(neuron_dynamics_table[i]), neuron_dynamics_databox[i][1],1,10,1,2);
 		gtk_table_attach_defaults(GTK_TABLE(neuron_dynamics_table[i]), neuron_dynamics_databox[i][2],11,20,0,1);
 		gtk_table_attach_defaults(GTK_TABLE(neuron_dynamics_table[i]), neuron_dynamics_databox[i][3],11,20,1,2);


		gtk_notebook_append_page (GTK_NOTEBOOK (tabs), neuron_dynamics_frame[i], neuron_dynamics_frame_label[i] );  
		gtk_widget_show_all(neuron_dynamics_frame[i]);

		g_signal_connect_swapped(G_OBJECT(btn_neuron_dynamics_assign[i]), "clicked", G_CALLBACK(assign_neuron_dynamics_button_func), NULL);
		g_signal_connect_swapped(G_OBJECT(btn_neuron_dynamics_refresh[i]), "clicked", G_CALLBACK(refresh_neuron_dynamics_button_func), NULL);
	}
	refresh_all_neuron_dynamics_graph();

	return TRUE;
}

void assign_neuron_dynamics_button_func(void)
{
	int frame, graph, layer, neuron_num, group, dyn_val; 
	frame = gtk_notebook_get_current_page (GTK_NOTEBOOK (tabs)) -2;  // entries and network visualization tabs
	graph = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_dynamics_graph_num[frame])));
	layer = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_dynamics_neuron_layer[frame])));
	group = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_dynamics_neuron_group[frame])));
	neuron_num = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_neuron_dynamics_neuron_num[frame])));
	dyn_val = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_neuron_dynamics_value[frame]));
	if ((dyn_val < 0) || (dyn_val>CONDUCTANCE_INHIBITORY_LOG))
	{
		printf ("Select combobox index from Drop-Down list is %d\n", dyn_val);
		printf ("membrane potenatial (v) is selected automatically\n");
		dyn_val = 0;
	}

	if (is_neuron_allocated(layer, group, neuron_num) == NULL)
		return;
	graph_to_neuron_map_table[frame][graph].neuron_layer = layer;
	graph_to_neuron_map_table[frame][graph].neuron_group = group;
	graph_to_neuron_map_table[frame][graph].neuron_num = neuron_num;
	graph_to_neuron_map_table[frame][graph].dynamic_value = dyn_val;
	refresh_all_neuron_dynamics_graph();

}

void refresh_neuron_dynamics_button_func(void)
{
	refresh_all_neuron_dynamics_graph();
}

void refresh_all_neuron_dynamics_graph(void)
{
	int i, j, k;
	int neuron_num, group, layer, dyn_val;
	neuron *nrn;
	for (i=0; i<num_of_frames_for_neuron_dynamics; i++)
	{
		for (j=0; j<NUM_OF_GRAPH_PER_FRAME; j++)
		{
			layer = graph_to_neuron_map_table[i][j].neuron_layer;
			group = graph_to_neuron_map_table[i][j].neuron_group;
			neuron_num = graph_to_neuron_map_table[i][j].neuron_num;
			dyn_val = graph_to_neuron_map_table[i][j].dynamic_value;
			nrn = is_neuron_allocated(layer, group, neuron_num);
			for (k=0; k<neuron_dynamics_log_buffer_size; k++)
			{
				neuron_dynamics_graph_y_axis[i][j][k] = (float)nrn->dynamics_log[dyn_val][k];
			}
			if (dyn_val == V_LOG)	
				gtk_databox_set_total_limits (GTK_DATABOX (neuron_dynamics_box[i][j]), 0, neuron_dynamics_log_buffer_size-1, 150, -100);
			if (dyn_val == U_LOG)
				gtk_databox_set_total_limits (GTK_DATABOX (neuron_dynamics_box[i][j]), 0, neuron_dynamics_log_buffer_size-1, 500, -500);
			if (dyn_val == CONDUCTANCE_EXCITATORY_LOG)
				gtk_databox_set_total_limits (GTK_DATABOX (neuron_dynamics_box[i][j]), 0, neuron_dynamics_log_buffer_size-1, 100, -100);
			if (dyn_val == CONDUCTANCE_INHIBITORY_LOG)
				gtk_databox_set_total_limits (GTK_DATABOX (neuron_dynamics_box[i][j]), 0, neuron_dynamics_log_buffer_size-1, 100, -100);
		}		
	}

	return;
}
