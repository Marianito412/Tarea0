//---------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                     LIBRARIES
//---------------------------------------------------------------------------------------------------------------------------------------------------------
#include <gtk/gtk.h>
#include <stdio.h>

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                     VARIABLES
//---------------------------------------------------------------------------------------------------------------------------------------------------------
GtkWidget *window;
GtkWidget *n_entry;
GtkWidget *k_entry;
GtkWidget *generate_button;
GtkColorButton *color1_button;
GtkColorButton *color2_button;
GtkColorButton *color3_button;
GtkColorButton *color4_button;
GdkRGBA color1, color2, color3, color4;
const gchar *n_text;
const gchar *k_text;

//---------------------------------------------------------------------------------------------------------------------------------------------------------
/*  Displays an error message in a dialog window. 
    
    Parameters:
    param: message The error message to be displayed.
*/
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void show_error_dialog(const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
/*  Function that is activated when a color button is selected.
    
    Parameters:
    widget: GtkColorButton that triggered the event.
    user_data: A string indicating which button was clicked
*/
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void on_color_button_set(GtkColorButton *widget, gpointer user_data) {
    GdkRGBA color;
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(widget), &color);

    const char *button_name = (const char *)user_data;

    if (g_strcmp0(button_name, "Button 1") == 0) {
        color1 = color;
    } else if (g_strcmp0(button_name, "Button 2") == 0) {
        color2 = color;
    } else if (g_strcmp0(button_name, "Button 3") == 0) {
        color3 = color;
    } else if (g_strcmp0(button_name, "Button 4") == 0) {
        color4 = color;
    }
    
    g_print("%s seleccionó el color: RGBA(%f, %f, %f, %f)\n",
            button_name, color.red, color.green, color.blue, color.alpha);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------
/*  Function that is activated when the "Generate" button is clicked.

    This function retrieves the numerical values from the input fields and 
    validates them. If the values are less than 4, an error message is displayed
    
    Parameters:
    widget: the button that triggered the event.
*/
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void Generate_button_clicked(GtkWidget *widget){
    k_text = gtk_entry_get_text(GTK_ENTRY(n_entry));
    n_text = gtk_entry_get_text(GTK_ENTRY(k_entry));

    int n_value = atoi(n_text);
    int k_value = atoi(k_text);

    if (n_value < 4 || k_value < 4) {
        show_error_dialog("The values for data and rays must be integers greater than or equal to 4.");
        return; 
    }

    printf("N: %s\n", n_text);
    printf("K: %s\n", k_text);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
// Main function of the program
//---------------------------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);  

    GtkBuilder *builder = gtk_builder_new_from_file ("interface.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "Window"));
    n_entry = GTK_WIDGET(gtk_builder_get_object(builder, "N_Entry"));
    k_entry = GTK_WIDGET(gtk_builder_get_object(builder, "K_Entry"));
    generate_button = GTK_WIDGET(gtk_builder_get_object(builder, "Generate_button"));

    color1_button = GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "color1_button"));
    color2_button = GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "color2_button"));
    color3_button = GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "color3_button"));
    color4_button = GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "color4_button"));

    g_signal_connect(color1_button, "color-set", G_CALLBACK(on_color_button_set), "Button 1");
    g_signal_connect(color2_button, "color-set", G_CALLBACK(on_color_button_set), "Button 2");
    g_signal_connect(color3_button, "color-set", G_CALLBACK(on_color_button_set), "Button 3");
    g_signal_connect(color4_button, "color-set", G_CALLBACK(on_color_button_set), "Button 4");

    g_signal_connect(generate_button, "clicked", G_CALLBACK(Generate_button_clicked), NULL);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_widget_show_all(window);
    
    gtk_main();
    
    return 0;
}   