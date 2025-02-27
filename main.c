#include <gtk/gtk.h>
#include <cairo.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>  // Para usar el tipo de dato 'bool'

bool Changed = false;

// Variables globales para N y k
unsigned int N = 4; // Número de rayos
unsigned int k = 4; // Número de datos

// Variables globales para los colores
GdkRGBA colorC0 = {1.0, 1.0, 1.0, 1.0}; 
GdkRGBA colorC1 = {1.0, 1.0, 1.0, 1.0}; 
GdkRGBA colorC2 = {1.0, 1.0, 1.0, 1.0}; 
GdkRGBA colorC3 = {1.0, 1.0, 1.0, 1.0}; 

// Prototipos de funciones
int* generateArray(unsigned int k);
void shuffleArray(int* array, unsigned int k);
bool validateInput(unsigned int N, unsigned int k);

// Función para generar el vector D con valores de 1 a k
int* generateArray(unsigned int k) {
    int* array = (int*)calloc(k, sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el array.\n");
        exit(EXIT_FAILURE);
    }
    for (unsigned int i = 0; i < k; i++) {
        array[i] = i + 1;  // Inicializar el array con valores de 1 a k
    }
    return array;
}

// Función para barajar el vector D
void shuffleArray(int* array, unsigned int k) {
    for (unsigned int i = 0; i < k; i++) {
        int swapper = rand() % k;  // Índice aleatorio entre 0 y k-1
        int t = array[swapper];
        array[swapper] = array[i];
        array[i] = t;
    }
}

// Función para validar los datos de entrada
bool validateInput(unsigned int N, unsigned int k) {
    // Verificar que N y k sean mayores o iguales a 4
    if (N < 4 || k < 4) {
        fprintf(stderr, "Error: N y k deben ser mayores o iguales a 4.\n");
        return false;
    }

    // Si todo está bien, retornar true
    return true;
}

// Función para calcular el ángulo de un rayo
double calculateAngle(int i, int totalRays) {
    if (i == 0) {
        return 0;  // Garantiza que el primer rayo esté en 0 radianes
    }
    return (i / (double)totalRays) * 2 * G_PI;
}


// Función para calcular la longitud de un rayo
double calculateLength(int Di, int maxVal, double maxLength) {
    return (Di / (double)maxVal) * maxLength;   // Longitud proporcional
}

// Función para interpolar entre cuatro colores según un ratio
void lerpColor(GdkRGBA A, GdkRGBA B, GdkRGBA C, GdkRGBA D, float alpha, GdkRGBA* output) {
    if (alpha < 0.33) {
        output->red = A.red + (B.red - A.red) * (alpha / 0.33);
        output->green = A.green + (B.green - A.green) * (alpha / 0.33);
        output->blue = A.blue + (B.blue - A.blue) * (alpha / 0.33);
        output->alpha = A.alpha + (B.alpha - A.alpha) * (alpha / 0.33);
    } else if (alpha < 0.66) {
        output->red = B.red + (C.red - B.red) * ((alpha - 0.33) / 0.33);
        output->green = B.green + (C.green - B.green) * ((alpha - 0.33) / 0.33);
        output->blue = B.blue + (C.blue - B.blue) * ((alpha - 0.33) / 0.33);
        output->alpha = B.alpha + (C.alpha - B.alpha) * ((alpha - 0.33) / 0.33);
    } else {
        output->red = C.red + (D.red - C.red) * ((alpha - 0.66) / 0.34);
        output->green = C.green + (D.green - C.green) * ((alpha - 0.66) / 0.34);
        output->blue = C.blue + (D.blue - C.blue) * ((alpha - 0.66) / 0.34);
        output->alpha = C.alpha + (D.alpha - C.alpha) * ((alpha - 0.66) / 0.34);
    }
}

// Función para dibujar un rayo
void drawRay(cairo_t *cr, double x0, double y0, double angle, double length, GdkRGBA color) {
    double x1 = x0 + length * cos(angle);   
    double y1 = y0 - length * sin(angle);   

    cairo_set_source_rgba(cr, color.red, color.green, color.blue, color.alpha);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr, x0, y0);
    cairo_line_to(cr, x1, y1);
    cairo_stroke(cr);
}

// Función de dibujo principal
static gboolean drawRays(GtkWidget *widget, cairo_t *cr) {

    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    // Limpiar el fondo
    cairo_set_source_rgb(cr, 0, 0, 0);  // Fondo negro (puedes cambiarlo)
    cairo_paint(cr);  // Llena toda la superficie con el color de fondo

    double centerX = width / 2.0;  // Centro del círculo
    double centerY = height / 2.0; // Centro del círculo
    double radius = (width < height) ? width / 2.25 : height / 2.25; // Radio del círculo

    // Dibujar el círculo
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_set_line_width(cr, 2.0);  // Grosor de la línea
    cairo_arc(cr, centerX, centerY, radius, 0, 2 * G_PI); 
    cairo_stroke(cr);   

    // Vector D
    if (Changed){
        int* D = generateArray(k); // Inicializar vector
        shuffleArray(D, k); // Barajar vector
    }

    // Dibujar rayos
    for (unsigned int i = 0; i < N; i++) {  // Iterar sobre N, no sobre k
        int index = i % k;  // Usar el operador módulo para repetir los valores de D si N > k
        double angle = calculateAngle(i, N);   
        double length = calculateLength(D[index], k, radius);  
        float ratio = (float)index / (float)(k - 1); // ratio va de 0 a 1

        // Interpolar colores usando lerpColor
        GdkRGBA finalColor; 
        lerpColor(colorC0, colorC1, colorC2, colorC3, ratio, &finalColor);

        drawRay(cr, centerX, centerY, angle, length, finalColor);
    }

    free(D); // Liberar memoria del array generado
    Changed = false;
    return FALSE;
}

// Función para redibujar
static void onShuffleAndDraw(GtkButton* GenerateButton, gpointer userData) {
    Changed = true;
    GtkWidget *drawingArea = GTK_WIDGET(g_object_get_data(G_OBJECT(GenerateButton), "drawing-area"));
    gtk_widget_queue_draw(drawingArea);    
}

void destroy(GtkWidget* widget, gpointer data)
{
    gtk_main_quit();
}

// Función para actualizar N y k cuando el usuario cambia los valores
static void onSpinButtonChanged(GtkSpinButton *spinButton, gpointer userData) {
    unsigned int *value = (unsigned int *)userData;
    unsigned int newValue = gtk_spin_button_get_value_as_int(spinButton);

    // Validar el nuevo valor
    if (!validateInput((value == &N) ? newValue : N, (value == &k) ? newValue : k)) {
        gtk_spin_button_set_value(spinButton, *value);  // Revertir al valor anterior si no es válido
        return;
    }

    // Actualizar el valor
    *value = newValue;

    // Redibujar los rayos
    //GtkWidget *drawingArea = GTK_WIDGET(g_object_get_data(G_OBJECT(spinButton), "drawing-area"));
    //gtk_widget_queue_draw(drawingArea);
}

/*
void on_color_button_set(GtkColorButton* widget, gpointer user_data) {
    GdkRGBA color;
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(widget), &color);

    const char button_name = (const char *)user_data;

    if (g_strcmp0(button_name, "Button 1") == 0) {
        colorC0 = color;
    } else if (g_strcmp0(button_name, "Button 2") == 0) {
        colorC1 = color;
    } else if (g_strcmp0(button_name, "Button 3") == 0) {
        colorC2 = color;
    } else if (g_strcmp0(button_name, "Button 4") == 0) {
        colorC3 = color;
    }

    g_print("%s seleccionó el color: RGBA(%f, %f, %f, %f)\n",
            button_name, color.red, color.green, color.blue, color.alpha);
}
*/

int main(int argc, char *argv[]) {
    srand(time(NULL));  // Inicializar la semilla del generador de números aleatorios
    gtk_init(&argc, &argv);

    // Validar los valores iniciales de N y k
    if (!validateInput(N, k)) {
        return 1;  // Salir del programa si los valores no son válidos
    }

     GtkBuilder *builder = gtk_builder_new_from_file ("interface.glade");

    // Ventana principal
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "Window"));
    //gtk_window_set_title(GTK_WINDOW(window), "Rayos con 4 Colores");
    //gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Contenedor vertical
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Área de dibujo
    GtkWidget *drawingArea = GTK_WIDGET(gtk_builder_get_object(builder, "drawing_area"));
    gtk_box_pack_start(GTK_BOX(box), drawingArea, TRUE, TRUE, 0);
    g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(drawRays), NULL);

    // Campos de entrada para N y k
    GtkWidget *spinButtonN = GTK_WIDGET(gtk_builder_get_object(builder, "N_Entry"));
    GtkWidget *spinButtonK = GTK_WIDGET(gtk_builder_get_object(builder, "K_Entry"));

    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinButtonN), 4, 10000000000);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinButtonK), 4, 10000000000);

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinButtonN), N);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinButtonK), k);

    //gtk_box_pack_start(GTK_BOX(box), gtk_label_new("N (Número de rayos):"), FALSE, FALSE, 0);
    //gtk_box_pack_start(GTK_BOX(box), spinButtonN, FALSE, FALSE, 0);
    //gtk_box_pack_start(GTK_BOX(box), gtk_label_new("k (Número de datos):"), FALSE, FALSE, 0);
    //gtk_box_pack_start(GTK_BOX(box), spinButtonK, FALSE, FALSE, 0);

    // Conectar los campos de entrada a la lógica del programa
    g_signal_connect(spinButtonN, "value-changed", G_CALLBACK(onSpinButtonChanged), &N);
    g_signal_connect(spinButtonK, "value-changed", G_CALLBACK(onSpinButtonChanged), &k);

    // Asociar el área de dibujo a los campos de entrada
    g_object_set_data(G_OBJECT(spinButtonN), "drawing-area", drawingArea);
    g_object_set_data(G_OBJECT(spinButtonK), "drawing-area", drawingArea);

    // Botones de color
    GtkColorButton *colorButtonC0 = GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "color1_button"));
    GtkColorButton *colorButtonC1 = GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "color2_button"));
    GtkColorButton *colorButtonC2 = GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "color3_button"));
    GtkColorButton *colorButtonC3 = GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "color4_button"));

    //gtk_box_pack_start(GTK_BOX(box), colorButtonC0, FALSE, FALSE, 0);
    //gtk_box_pack_start(GTK_BOX(box), colorButtonC1, FALSE, FALSE, 0);
    //gtk_box_pack_start(GTK_BOX(box), colorButtonC2, FALSE, FALSE, 0);
    //gtk_box_pack_start(GTK_BOX(box), colorButtonC3, FALSE, FALSE, 0);

    // Configurar colores iniciales en los botones de color
    gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(colorButtonC0), &colorC0);
    gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(colorButtonC1), &colorC1);
    gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(colorButtonC2), &colorC2);
    gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(colorButtonC3), &colorC3);

    // Conectar botones de color
    //g_signal_connect(colorButtonC0, "color-set", G_CALLBACK(on_color_button_set), "Button 1");
    //g_signal_connect(colorButtonC1, "color-set", G_CALLBACK(on_color_button_set), "Button 2");
    //g_signal_connect(colorButtonC2, "color-set", G_CALLBACK(on_color_button_set), "Button 3");
    //g_signal_connect(colorButtonC3, "color-set", G_CALLBACK(on_color_button_set), "Button 4");

    g_signal_connect(colorButtonC0, "color-set", G_CALLBACK(gtk_color_chooser_get_rgba), &colorC0);
    g_signal_connect(colorButtonC1, "color-set", G_CALLBACK(gtk_color_chooser_get_rgba), &colorC1);
    g_signal_connect(colorButtonC2, "color-set", G_CALLBACK(gtk_color_chooser_get_rgba), &colorC2);
    g_signal_connect(colorButtonC3, "color-set", G_CALLBACK(gtk_color_chooser_get_rgba), &colorC3);

    // Botón de redibujar
    GtkWidget *button = GTK_WIDGET(gtk_builder_get_object(builder, "Generate_button"));
    
    g_object_set_data(G_OBJECT(button), "drawing-area", drawingArea);
    g_signal_connect(button, "clicked", G_CALLBACK(onShuffleAndDraw), NULL);

    //Boton de cerrar
    GtkWidget *closeButton = GTK_WIDGET(gtk_builder_get_object(builder, "Exit_button"));
    g_signal_connect(closeButton, "clicked", G_CALLBACK(destroy), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;   
}