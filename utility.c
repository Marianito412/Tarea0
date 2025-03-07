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
GdkRGBA lerpColor(GdkRGBA A, GdkRGBA B, GdkRGBA C, GdkRGBA D, float alpha) {
    GdkRGBA newColor;
    if (alpha < 0.33) {
        newColor.red = A.red + (B.red - A.red) * (alpha / 0.33);
        newColor.green = A.green + (B.green - A.green) * (alpha / 0.33);
        newColor.blue = A.blue + (B.blue - A.blue) * (alpha / 0.33);
        newColor.alpha = A.alpha + (B.alpha - A.alpha) * (alpha / 0.33);
    } else if (alpha < 0.66) {
        newColor.red = B.red + (C.red - B.red) * ((alpha - 0.33) / 0.33);
        newColor.green = B.green + (C.green - B.green) * ((alpha - 0.33) / 0.33);
        newColor.blue = B.blue + (C.blue - B.blue) * ((alpha - 0.33) / 0.33);
        newColor.alpha = B.alpha + (C.alpha - B.alpha) * ((alpha - 0.33) / 0.33);
    } else {
    newColor.red = C.red + (D.red - C.red) * ((alpha - 0.66) / 0.34);
        newColor.green = C.green + (D.green - C.green) * ((alpha - 0.66) / 0.34);
        newColor.blue = C.blue + (D.blue - C.blue) * ((alpha - 0.66) / 0.34);
        newColor.alpha = C.alpha + (D.alpha - C.alpha) * ((alpha - 0.66) / 0.34);
    }
    return newColor;
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


    int* D = generateArray(k); // Inicializar vector


    shuffleArray(D, k); // Barajar vector





    // Dibujar rayos


    for (unsigned int i = 0; i < N; i++) {  // Iterar sobre N, no sobre k


        int index = i % k;  // Usar el operador módulo para repetir los valores de D si N > k


        double angle = calculateAngle(i, N);   


        double length = calculateLength(D[index], k, radius);  


        float ratio = (float)index / (float)(k - 1); // ratio va de 0 a 1





        // Interpolar colores usando lerpColor


        GdkRGBA finalColor = lerpColor(colorC0, colorC1, colorC2, colorC3, ratio);





        drawRay(cr, centerX, centerY, angle, length, finalColor);


    }





    free(D); // Liberar memoria del array generado


    return FALSE;


}





// Función para redibujar


static void onShuffleAndDraw(GtkWidget *drawingArea) {


    gtk_widget_queue_draw(drawingArea);    


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
    GtkWidget *drawingArea = GTK_WIDGET(g_object_get_data(G_OBJECT(spinButton), "drawing-area"));
    gtk_widget_queue_draw(drawingArea);
}