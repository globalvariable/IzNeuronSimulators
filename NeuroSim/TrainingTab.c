#include "TrainingTab.h"



bool create_training_tab(GtkWidget * tabs)
{

	GtkWidget *frame, *frame_label, *table, *hbox;

        frame = gtk_frame_new (NULL);
        frame_label = gtk_label_new ("     TRAINING     ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (tabs), frame, frame_label);  


 	hbox = gtk_hbox_new(TRUE, 0);
        gtk_container_add (GTK_CONTAINER (frame), hbox);

	table = gtk_table_new(6,8,TRUE);
	gtk_box_pack_start(GTK_BOX(hbox),table, TRUE,TRUE,0);
	
	return TRUE;

}
