// Include gtk
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

int* generateArray(unsigned int k)
{
 int* array = (int*)calloc(k, sizeof(int));

  for (int i = 0; i<k; i++){
    array[i] = i;
  }
  return array;
}

void shuffleArray(int* array, unsigned int k){
  for (int i = 0; i<k; i++){
    int swapper = rand() % (k+1);
    int t = array[swapper];
    array[swapper] = array[i];
    array[i] = t;
  }
}

GdkRGBA lerpColor(GdkRGBA A, GdkRGBA B, GdkRGBA C, GdkRGBA D, float alpha){
  GdkRGBA NewColor;
  if (alpha<0.33){
    NewColor.red = A.red + (B.red-A.red)*alpha;
    NewColor.green = A.green + (B.green-A.green)*alpha;
    NewColor.blue = A.blue + (B.blue-A.blue)*alpha;
    NewColor.alpha = A.alpha + (B.alpha-A.alpha)*alpha;
  }
  else if (alpha<0.66){
    NewColor.red = B.red + (C.red-B.red)*alpha;
    NewColor.green = B.green + (C.green-B.green)*alpha;
    NewColor.blue = B.blue + (C.blue-B.blue)*alpha;
    NewColor.alpha = B.alpha + (C.alpha-B.alpha)*alpha;
  }
  else{
    NewColor.red = C.red + (D.red-C.red)*alpha;
    NewColor.green = C.green + (D.green-C.green)*alpha;
    NewColor.blue = C.blue + (D.blue-C.blue)*alpha;
    NewColor.alpha = C.alpha + (D.alpha-C.alpha)*alpha;
  }
  return NewColor;
}

static void on_activate (GtkApplication *app) {
  // Create a new window
  GtkWidget *window = gtk_application_window_new (app);
  // Create a new button
  GtkWidget *button = gtk_button_new_with_label ("Hello, World!");
  // When the button is clicked, close the window passed as an argument
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_window_close), window);
  gtk_window_set_child (GTK_WINDOW (window), button);
  gtk_window_present (GTK_WINDOW (window));
}

int main (int argc, char *argv[]) {
  srand(time(NULL));
  // Create a new application
  GtkApplication *app = gtk_application_new ("com.example.GtkApplication",
                                             G_APPLICATION_DEFAULT_FLAGS);

  
  g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);
  return g_application_run (G_APPLICATION (app), argc, argv);
}
 