#include <gtk/gtk.h>

void print_on_entry(GtkWidget* input){
	int len = gtk_entry_get_text_length( GTK_ENTRY( input) );
	gtk_editable_delete_text(GTK_EDITABLE( input), 0, len);
	gtk_editable_insert_text(GTK_EDITABLE( input ), "Testa", 5, &len);
}

GtkWidget* create_window(GtkApplication* app){
	GtkWidget* window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Hello World window");
	gtk_window_set_default_size( GTK_WINDOW(window), 400, 200);
	return window;
}

static void activate(GtkApplication* app, gpointer user_data){
	GtkWidget* window = create_window(app);
	GtkWidget* box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	GtkWidget* grid = gtk_grid_new();
	gtk_container_add ( GTK_CONTAINER(window), box);
	gtk_container_add(GTK_CONTAINER(box), grid);

	GtkWidget* label = gtk_label_new("Hello");
	gtk_grid_attach( GTK_GRID(grid), label, 1, 0, 1 ,1);

	GtkWidget* input = 	gtk_entry_new();
	gtk_grid_attach( GTK_GRID(grid), input, 0, 0, 1 ,1);

	GtkWidget* button = gtk_button_new_with_label("Convert");
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(print_on_entry), input);
	gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 2);

	gtk_widget_show_all(window);
}

int main(int argc, char **argv){
	GtkApplication * app = gtk_application_new("org.gtk_example", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;
} 

