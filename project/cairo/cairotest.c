#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include "../maze/mazestruct.h"

/**************************macros***************/
#define WINDOW_WIDTH 830
#define WINDOW_HEIGHT 830
#define GRID_SIZE 100

static void do_drawing(cairo_t *cr);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{      
  do_drawing(cr);

  return FALSE;
}

static void do_drawing(cairo_t *cr)
{ 
  int spot_height = 800/GRID_SIZE;
  int indent = (WINDOW_HEIGHT - (GRID_SIZE * spot_height))/2;
  for (int i = 0; i < GRID_SIZE; i++){
    for (int j = 0; j < GRID_SIZE; j++){

      //set the color and draw the grids
      cairo_set_source_rgb(cr, 0.5, 0.5, 1);
      cairo_rectangle(cr, i*spot_height + 1 + indent, j*spot_height + 1 + indent, spot_height - 1, spot_height -1);
      cairo_fill(cr);

      //set the color to black and draw west walls
      cairo_set_source_rgb(cr, 0, 0, 0);
      cairo_rectangle(cr, i*spot_height + indent, j*spot_height + indent, 1, spot_height);
      cairo_fill(cr);

      //set the color to black and draw north walls
      cairo_set_source_rgb(cr, 0, 0, 0);
      cairo_rectangle(cr, i*spot_height + indent, j*spot_height + indent, spot_height, 1);
      cairo_fill(cr);

      //draw a south wall
      cairo_set_source_rgb(cr, 0, 0, 0);
      cairo_rectangle(cr, i*spot_height + indent, (j+1)*spot_height + indent, spot_height, 1);
      cairo_fill(cr);

      //set the color to black and draw east walls
      cairo_set_source_rgb(cr, 0, 0, 0);
      cairo_rectangle(cr, (i+1)*spot_height + indent, j*spot_height + indent, 1, spot_height);
      cairo_fill(cr);


    }
  }

}

gboolean time_handler(GtkWidget *widget) {
    
  if (widget->window == NULL) return FALSE;

  gtk_widget_queue_draw(widget);
  
  return TRUE;
}


int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *darea;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);

  g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL); 
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_HEIGHT, WINDOW_WIDTH);
  g_timeout_add(1000, (GSourceFunc) time_handler, (gpointer) window); 
  gtk_window_set_title(GTK_WINDOW(window), "GTK window");

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}

//gcc cairotest.c -o cairotest `pkg-config --cflags --libs gtk+-3.0`