#include <gtk/gtk.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
	GtkWidget* w1;
	GtkWidget* w2;
	GtkWidget* w3;
} widgets_group;

typedef struct{
	bool does_contain_small_letters;
	bool does_contain_capital_letters;
	bool does_contain_digits;
	bool does_contain_other_character;
	bool has_8_characters_least;
	short sum;
} password_content;

bool is_value_in_range(int val, int min, int max){
	return val >= min && val <= max;
}

void get_password_rating(const gchar* password, password_content* pass_cont){
	pass_cont->does_contain_small_letters = false;
	pass_cont->does_contain_capital_letters = false;
	pass_cont->does_contain_digits = false;
	pass_cont->does_contain_other_character = false;
	pass_cont->has_8_characters_least = strlen(password)>=8;
	for(int i=0; i<strlen(password); i++){
		char letter = password[i];
		if (is_value_in_range(letter, 'a', 'z'))
			pass_cont->does_contain_small_letters = true; 
		else if (is_value_in_range(letter, 'A', 'Z'))
			pass_cont->does_contain_capital_letters =true; 
		else if (is_value_in_range(letter, '0', '9'))
			pass_cont->does_contain_digits = true; 
		else
			pass_cont->does_contain_other_character = true;
	}

	pass_cont->sum =
		pass_cont->does_contain_small_letters+
		pass_cont->does_contain_capital_letters+
		pass_cont->does_contain_digits+
		pass_cont->does_contain_other_character+
		pass_cont->has_8_characters_least;
}

const char* create_warning_message(password_content* pass_cont){
	char* buff = calloc(300, 1);
	if (!pass_cont->does_contain_other_character)
		strcat(buff, "Your password does not contain other characters\n");
	if (!pass_cont->has_8_characters_least)
		strcat(buff, "Your password is shorter than 8 characters\n");
	if (!pass_cont->does_contain_small_letters)
		strcat(buff, "Your password does not contain small characters\n");
	if (!pass_cont->does_contain_capital_letters)
		strcat(buff, "Your password does not contain capital letters\n");
	if (!pass_cont->does_contain_digits)
		strcat(buff, "Your password does not contain digits\n");

	return buff;
}

void set_warnings(GtkWidget* label, password_content* pass_cont){
	const gchar* warnings=create_warning_message(pass_cont);
	gtk_label_set_text(GTK_LABEL(label), warnings);
}

void react_to_user_input(widgets_group* data){
	GtkWidget* input = data->w1;
	GtkWidget* bar = data->w2;
	GtkWidget* label = data->w3;
	const gchar* text = gtk_entry_get_text(GTK_ENTRY(input));
	password_content pass_cont;
	get_password_rating(text, &pass_cont);
	set_warnings(label, &pass_cont);
	gtk_level_bar_set_value(GTK_LEVEL_BAR(bar), pass_cont.sum/5.);
}

GtkWidget* create_window(GtkApplication* app){
	GtkWidget* window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Password Checker");
	gtk_window_set_default_size( GTK_WINDOW(window), 400, 200);
	return window;
}

widgets_group* create_widgets_group(){
	widgets_group* w = malloc(sizeof(widgets_group));
	w->w1 = gtk_entry_new();
	g_signal_connect_swapped(w->w1, "insert-text", G_CALLBACK(react_to_user_input), w);
	g_signal_connect_swapped(w->w1, "delete-text", G_CALLBACK(react_to_user_input), w);
	gtk_entry_set_visibility(GTK_ENTRY( w->w1 ), FALSE);

	w->w2 = gtk_level_bar_new();
	w->w3 = gtk_label_new("");
	return w;
}

void setup_widgets_on_grid(GtkWidget* grid){
	widgets_group* w = create_widgets_group();
	gtk_grid_attach( GTK_GRID(grid), w->w1, 0, 0, 1 ,1);
	gtk_grid_attach(GTK_GRID(grid), w->w2, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), w->w3, 1, 0, 1, 1);
}

static void activate(GtkApplication* app, gpointer user_data){
	GtkWidget* window = create_window(app);
	GtkWidget* grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);

	setup_widgets_on_grid(grid);
	gtk_widget_show_all(window);
}

int main(int argc, char **argv){
	GtkApplication * app = gtk_application_new("org.gtk_example", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;
} 

