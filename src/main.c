#include <gtk/gtk.h>

typedef struct{
	GtkWidget* w1;
	GtkWidget* w2;
} widgets_payload;

void print_on_entry(widgets_payload* data){
	GtkWidget* input = data->w1;
	GtkWidget* bar = data->w2;
	const gchar* text = gtk_entry_get_text(GTK_ENTRY(input));
	printf("%s\n", text);
	gtk_level_bar_set_value(GTK_LEVEL_BAR(bar), 4/8.);
}

GtkWidget* create_window(GtkApplication* app){
	GtkWidget* window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Password Checker");
	gtk_window_set_default_size( GTK_WINDOW(window), 400, 200);
	return window;
}

void connect_entry_with_bar(GtkWidget* entry, GtkWidget* bar){
	widgets_payload* w = malloc(sizeof(widgets_payload));
	w->w1 = entry;
	w->w2 = bar;
	g_signal_connect_swapped(entry, "insert-text", G_CALLBACK(print_on_entry), w);
	g_signal_connect_swapped(entry, "delete-text", G_CALLBACK(print_on_entry), w);

}

static void activate(GtkApplication* app, gpointer user_data){
	GtkWidget* window = create_window(app);
	GtkWidget* grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);

	GtkWidget* input = 	gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY( input ), FALSE);
	gtk_grid_attach( GTK_GRID(grid), input, 0, 0, 1 ,1);

	GtkWidget* bar = gtk_level_bar_new();
	gtk_grid_attach(GTK_GRID(grid), bar, 0, 1, 1, 1);
	connect_entry_with_bar(input, bar);

	gtk_widget_show_all(window);
}

int main(int argc, char **argv){
	GtkApplication * app = gtk_application_new("org.gtk_example", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;
} 

