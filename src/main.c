#include <gtk/gtk.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
	GtkWidget* w1;
	GtkWidget* w2;
	GtkWidget* w3;
} widgets_payload;

typedef struct{
	short does_contain_small_letters;
	short does_contain_capital_letters;
	short does_contain_digits;
	short does_contain_other_character;
	short has_8_characters_least;
	short sum;
} password_content;

static bool is_value_in_range(int val, int min, int max){
	return val >= min && val <= max;
}

static void rate_password(const gchar* password, password_content* pass_cont){
	pass_cont->does_contain_small_letters = 0;
	pass_cont->does_contain_capital_letters = 0;
	pass_cont->does_contain_digits = 0;
	pass_cont->does_contain_other_character = 0;
	pass_cont->has_8_characters_least = (int)(strlen(password)>=8);
	for(int i=0; i<strlen(password); i++){
		char letter = password[i];
		pass_cont->does_contain_small_letters |= (int)(is_value_in_range(letter, 'a', 'z'));
		pass_cont->does_contain_capital_letters |= (int)(is_value_in_range(letter, 'A', 'Z'));
		pass_cont->does_contain_digits |= (int)(is_value_in_range(letter, '0', '9'));
		// if none of the values above, then we have a special character
		if( ! (is_value_in_range(letter, 'a', 'z') || is_value_in_range(letter, 'A', 'Z') || is_value_in_range(letter, '0', '9')))
			pass_cont->does_contain_other_character = 1;
	}

	pass_cont->sum =
		pass_cont->does_contain_small_letters+
		pass_cont->does_contain_capital_letters+
		pass_cont->does_contain_digits+
		pass_cont->does_contain_other_character+
		pass_cont->has_8_characters_least;
}

const gchar* create_warning_message(password_content* pass_cont){
	const char* has_other_chars = (pass_cont->does_contain_other_character == 0)? "Your password does not contain other characters\n" : "";
	const char* has_8_chars = (pass_cont->has_8_characters_least == 0)? "Your password is shorter than 8 characters\n" : "";
	const char* has_small_letters = (pass_cont->does_contain_small_letters == 0)? "Your password does not contain small characters\n" : "";
	const char* has_capital_letters = (pass_cont->does_contain_capital_letters == 0)? "Your password does not contain capital letters\n" : "";
	const char* has_digits = (pass_cont->does_contain_digits == 0)? "Your password does not contain digits\n" : "";
	char* buff = calloc(300, 1);
	strcat(buff, has_other_chars);
	strcat(buff, has_8_chars);
	strcat(buff, has_small_letters);
	strcat(buff, has_digits);
	strcat(buff, has_capital_letters);
	const char* result = buff;
	return result;
}

void set_warnings(GtkWidget* label, password_content* pass_cont){
	const gchar* warnings=create_warning_message(pass_cont);
	gtk_label_set_text(GTK_LABEL(label), warnings);
}

void react_to_user_input(widgets_payload* data){
	GtkWidget* input = data->w1;
	GtkWidget* bar = data->w2;
	GtkWidget* label = data->w3;
	const gchar* text = gtk_entry_get_text(GTK_ENTRY(input));
	password_content* pass_cont = malloc(sizeof(password_content));
	rate_password(text, pass_cont);
	set_warnings(label, pass_cont);
	gtk_level_bar_set_value(GTK_LEVEL_BAR(bar), pass_cont->sum/5.);
	free(pass_cont);
}

GtkWidget* create_window(GtkApplication* app){
	GtkWidget* window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Password Checker");
	gtk_window_set_default_size( GTK_WINDOW(window), 400, 200);
	return window;
}

void setup_widgets_on_grid(GtkWidget* grid){
	widgets_payload* w = malloc(sizeof(widgets_payload));
	w->w1 = gtk_entry_new();
	w->w2 = gtk_level_bar_new();
	w->w3 = gtk_label_new("");
	gtk_entry_set_visibility(GTK_ENTRY( w->w1 ), FALSE);
	gtk_grid_attach( GTK_GRID(grid), w->w1, 0, 0, 1 ,1);
	gtk_grid_attach(GTK_GRID(grid), w->w2, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), w->w3, 1, 0, 1, 1);
	g_signal_connect_swapped(w->w1, "insert-text", G_CALLBACK(react_to_user_input), w);
	g_signal_connect_swapped(w->w1, "delete-text", G_CALLBACK(react_to_user_input), w);
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

