#include "gpv.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo.h>
#include <math.h>

char current_savefile[256] = "saves/dummy_save.txt";
int punkte_woche[MAX_PUNKTE];
double punkte_gewicht[MAX_PUNKTE];
int punkte_count = 0;

int woche = -1;
double gewicht = -1;
GtkWidget* g_drawing_area = NULL;
GtkWidget* g_entry1 = NULL;
GtkWidget* g_entry2 = NULL;
GtkWidget* g_summary = NULL;
GtkWidget* g_progressPerWeek = NULL;
GtkWidget* g_button_save = NULL;

const char* PLACEHOLDER_FILE = "saves/Auswahl";

GtkWidget* window = NULL;
GtkWidget* window2 = NULL;
GtkWidget* window3 = NULL;

// Zoom Variablen
static double zoomFactor = 1.0;
static double offsetX = 0.0;
static double offsetY = 0.0;

// Zu Fenster 2 wechseln (von Fenster 1)
void switchToWindow2From1(GtkWidget* widget, gpointer data){
    if(window) gtk_widget_hide(window);
    if(window2) gtk_widget_show_all(window2);
}

// Zu Fenster 1 wechseln (von Fenster 2)
void switchToWindow1From2(GtkWidget* widget, gpointer data){
    if(window2) gtk_widget_hide(window2);
    if(window) gtk_widget_show_all(window);
}

// Zu Fenster 3 wechseln (von Fenster 1)
void switchToWindow3From1(GtkWidget* widget, gpointer data){
    if(window) gtk_widget_hide(window);
    if(window3) gtk_widget_show_all(window3);
}

// Zu Fenster 1 wechseln (von Fenster 3)
void switchToWindow1From3(GtkWidget* widget, gpointer data){
    if(window3) gtk_widget_hide(window3);
    if(window) gtk_widget_show_all(window);
}

// Bilder skalieren
GtkWidget* load_scaled_image(const char* filename, int width, int height){
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_scale(filename, width, height, TRUE, NULL);
    if (!pixbuf){
        g_printerr("Konnte %s nicht laden\n", filename);
        return gtk_label_new("Fehler: Bild fehlt");
    }
    GtkWidget* image = gtk_image_new_from_pixbuf(pixbuf);
    g_object_unref(pixbuf);
    return image;
}


// Werte in Datei speichern
void save_data_to_file(){
    FILE* fp = fopen(current_savefile, "w");
    if(!fp) return;
    fprintf(fp, "%d\n", punkte_count);
    for(int i = 0; i < punkte_count; i++){
        fprintf(fp, "%d %.2f\n", punkte_woche[i], punkte_gewicht[i]);
    }
    fclose(fp);
}


// Werte aus Datei laden
void load_data_from_file(){
    FILE* fp = fopen(current_savefile, "r");
    if(!fp) return;
    if(fscanf(fp, "%d", &punkte_count) != 1){
        fclose(fp);
        punkte_count = 0;
        return;
    }
    for(int i = 0; i < punkte_count; i++){
        fscanf(fp, "%d %lf", &punkte_woche[i], &punkte_gewicht[i]);
    }
    fclose(fp);
}

// Gesamte Kg +/- berechnen
void calculateProgress(char* buffer, size_t size){
    if(punkte_count <2){
        snprintf(buffer, size, "");
        return;
    }

    double KgDifference = punkte_gewicht[punkte_count-1] - punkte_gewicht[0];
    double KgPercentChange = (punkte_gewicht[punkte_count-1] / punkte_gewicht[0]) * 100 - 100;

    if(KgDifference > 0){
        snprintf(buffer, size, "+%.2f Kg in %d Wochen (+%.2f%%)", fabs(KgDifference), punkte_woche[punkte_count-1], fabs(KgPercentChange));
    } else if (KgDifference < 0){
        snprintf(buffer, size, "-%.2f Kg in %d Wochen (-%.2f%%)", fabs(KgDifference), punkte_woche[punkte_count-1], fabs(KgPercentChange));
    } else {
        snprintf(buffer, size, "Keine Änderung in %d Wochen", punkte_woche[punkte_count-1]);
    }
}

// Aktualisierung der Kg +/-
void update_summary() {
    if (!g_summary) return;
    char buffer[100];
    calculateProgress(buffer, sizeof(buffer));
    gtk_label_set_text(GTK_LABEL(g_summary), buffer);
}


// Wöchentliche Kg +/- berechnen
void calculateWeeklyProgress(char* buffer, size_t size){
    if(punkte_count < 2){
        snprintf(buffer, size, "");
        return;
    }

    double totalKgDifference = (punkte_gewicht[punkte_count-1] - punkte_gewicht[0]);
    int weeksDifference = punkte_woche[punkte_count-1] - punkte_woche[0];

    if(weeksDifference <= 0){
        snprintf(buffer, size, "");
        return;
    }

    double weeklyKgDifference = totalKgDifference / weeksDifference;

    if(weeklyKgDifference > 0.0){
        snprintf(buffer, size, "Durchschnittlich +%.2f Kg pro Woche", fabs(weeklyKgDifference));
    } else if(weeklyKgDifference < 0.0){
        snprintf(buffer, size, "Durschnittlich -%.2f Kg pro Woche", fabs(weeklyKgDifference));
    } else {
        snprintf(buffer, size, "");
    }
}

void update_weekly_summary(){
    if(!g_progressPerWeek) return;
    char buffer[100];
    calculateWeeklyProgress(buffer, sizeof(buffer));
    gtk_label_set_text(GTK_LABEL(g_progressPerWeek), buffer);
}


// Platzhalter Datei
void set_placeholder_mode(int on){
    gtk_widget_set_sensitive(g_entry1, !on);
    gtk_widget_set_sensitive(g_entry2, !on);
    gtk_widget_set_sensitive(g_button_save, !on);

    if(on){
        punkte_count = 0;
        gtk_label_set_text(GTK_LABEL(g_summary), "");
    }
    if(g_drawing_area){
        gtk_widget_queue_draw(g_drawing_area);
    }
}

// Callback-Funktion für Dropdown-Menü
void on_combo_changed(GtkComboBoxText* combo, gpointer user_data){
    const char* filename = gtk_combo_box_text_get_active_text(combo);

    if(!filename){
        return;
    }

    snprintf(current_savefile, sizeof(current_savefile), "saves/%s", filename);
    current_savefile[sizeof(current_savefile)-1] = '\0';

    if(strcmp(current_savefile, PLACEHOLDER_FILE) == 0){
        set_placeholder_mode(1); //Eingaben blockiert
    } else {
        load_data_from_file();
        set_placeholder_mode(0); //Eingaben frei
        update_summary();
        update_weekly_summary();
    }
}

// Callback-Funktion, wenn der Speichern-Button gedrückt wird
void on_button_clicked(GtkWidget* widget, gpointer data) {
    const char* woche_text = gtk_entry_get_text(GTK_ENTRY(g_entry1));
    const char* gewicht_text = gtk_entry_get_text(GTK_ENTRY(g_entry2));

    if(strlen(woche_text) == 0 || strlen(gewicht_text) == 0){
        return;
    }

    int neue_woche = atoi(woche_text);
    double neues_gewicht = atof(gewicht_text);

    if(neue_woche < 0 || neues_gewicht < 0){
        return;
    }

    // Prüfen ob Woche existiert
    int index = -1;
    for(int i=0; i<punkte_count; i++){
        if(punkte_woche[i] == neue_woche){
            index = i;
            break;
        }
    }

    if(index >= 0){
        punkte_gewicht[index] = neues_gewicht;
        g_print("Aktualisiert: Woche %d, Gewicht %f\n", neue_woche, neues_gewicht);
    } else {
        if (punkte_count < MAX_PUNKTE){
            punkte_woche[punkte_count] = neue_woche;
            punkte_gewicht[punkte_count] = neues_gewicht;
            punkte_count++;
            g_print("Gespeichert: Woche %d, Gewicht %f\n", woche, gewicht);
        }
    }
    

    save_data_to_file();

    if(g_drawing_area){
        gtk_widget_queue_draw(g_drawing_area);
    }
    gtk_entry_set_text(GTK_ENTRY(g_entry1), "");
    gtk_entry_set_text(GTK_ENTRY(g_entry2), "");

    update_summary();
    update_weekly_summary();
}

// Callback-Funktion, wenn der "Zoom zurücksetzen" Button gedrückt wird
void on_button2_clicked(GtkWidget* wdiget, gpointer data){
    zoomFactor = 1.0;
    offsetX = 0.0;
    offsetY = 0.0;
    if(g_drawing_area){
        gtk_widget_queue_draw(g_drawing_area);
    }
}

// Callback-Funktion, wenn der "Datei zurücksetzen" Button gedrückt wird
void on_button3_clicked(GtkWidget* widget, gpointer data){
    FILE* fp = fopen(current_savefile, "w");
    if (fp == NULL){
        perror("Datei konnte nicht geöffnet werden.");
        return;
    }
    fclose(fp);

    //Speicher in RAM zurücksetzen
    punkte_count = 0;
    memset(punkte_woche, 0, sizeof(punkte_woche));
    memset(punkte_gewicht, 0, sizeof(punkte_gewicht));

    update_summary();
    update_weekly_summary();

    if(g_drawing_area){
        gtk_widget_queue_draw(g_drawing_area);
    }

    printf("Datei erfolgreich zurückgesetzt!");
}


// Callback-Funktion für das Zeichnen
gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data){

    // Zoom
    cairo_save(cr);
    cairo_translate(cr, offsetX, offsetY);
    cairo_scale(cr, zoomFactor, zoomFactor);

    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    int width = allocation.width;
    int height = allocation.height;

    // Achsen-Rand
    int x0 = 40;
    int y0 = height - 40;
    int x_max = width - 20;
    int y_min = 20;

    // Hintergrundfarbe
    cairo_set_source_rgb(cr, 18.0/255.0, 18.0/255.0, 18.0/255.0);
    cairo_paint(cr);

    // Achsen zeichnen
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_set_line_width(cr, 2);

    // X-Achse
    cairo_move_to(cr, x0, y0);
    cairo_line_to(cr, x_max, y0);

    // Y-Achse
    cairo_move_to(cr, x0, y0);
    cairo_line_to(cr, x0, y_min);

    // Achsen-Beschriftung
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12);

    // Y-Achse
    for(int v = 0; v <= 300; v += 10){
        int y = y0 - (v * (y0 - y_min) / 300);
        char label[16];
        snprintf(label, sizeof(label), "%d", v);
        cairo_move_to(cr, x0 - 30, y + 5);
        cairo_show_text(cr, label);

        // Achsen-Striche
        cairo_move_to(cr, x0 - 5, y);
        cairo_line_to(cr, x0 + 5, y);
    }

    // X-Achse
    for(int v = 0; v <= 50; v += 1){
        int x = x0 + (v * (x_max - x0) / 50);
        char label[16];
        snprintf(label, sizeof(label), "%d", v);
        cairo_move_to(cr, x - 10, y0 + 20);
        cairo_show_text(cr, label);

        // Achsen-Striche
        cairo_move_to(cr, x, y0 - 5);
        cairo_line_to(cr, x, y0 + 5);
    }

    cairo_stroke(cr);



    // Punkte zeichnen und mit Linien verbinden
    if(punkte_count > 0){
        int x = x0 + (punkte_woche[0] * (x_max - x0) / 50);
        int y = y0 - (punkte_gewicht[0] * (y0 - y_min) / 300);
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_arc(cr, x, y, 4, 0, 2 * G_PI);
        cairo_fill(cr);
        
        if(punkte_count > 1){
            cairo_set_line_width(cr, 2);
            for(int i = 1; i < punkte_count; i++){
                if(punkte_gewicht[i] > punkte_gewicht[i-1]){
                    cairo_set_source_rgb(cr, 0.2, 0.8, 0.2); //grün
                } else if(punkte_gewicht[i] < punkte_gewicht[i-1]){
                    cairo_set_source_rgb(cr, 0.8, 0.2, 0.2); //rot
                } else {
                    cairo_set_source_rgb(cr, 0.9, 0.9, 0.0); //orange
                }

                int x1 = x0 + (punkte_woche[i-1] * (x_max - x0) / 50);
                int y1 = y0 - (punkte_gewicht[i-1] * (y0 - y_min) / 300);
                int x2 = x0 + (punkte_woche[i] * (x_max - x0) / 50);
                int y2 = y0 - (punkte_gewicht[i] * (y0 - y_min) / 300);
                cairo_move_to(cr, x1, y1);
                cairo_line_to(cr, x2, y2);
                cairo_stroke(cr);
                cairo_arc(cr, x2, y2, 4, 0, 2 * G_PI);
                cairo_fill(cr);
            }
        }
    }



    // Erwartete künftige Punkte zeichnen
    if(punkte_count > 1){
        
        int last_index = punkte_count - 1;

        int x_last = x0 + (punkte_woche[last_index] * (x_max - x0) / 50);
        int y_last = y0 - (punkte_gewicht[last_index] * (y0 - y_min) / 300);

        //Steigung berechnen
        double weight_difference = punkte_gewicht[last_index] - punkte_gewicht[0];
        double m;

        if(weight_difference < 0.0 || weight_difference == 0.0){
            m = 1.08; //Faktor 1.08 für realistisches Wachstum
        } else if (weight_difference > 0.0){
            m = (double)(punkte_gewicht[last_index] - punkte_gewicht[0]) / (punkte_woche[last_index] - punkte_woche[0]) * 0.8; // Faktor 0.8 für realistisches Wachstum
        }
        
        cairo_set_source_rgb(cr, 0.9, 0.5, 0.0);
        cairo_set_line_width(cr, 2);

        int future_weeks = 5;
        int prev_x = x_last;
        int prev_y = y_last;

        for(int i = 1; i <= future_weeks; i++){
            int w_future = punkte_woche[last_index] + i;
            double gewicht_future = punkte_gewicht[last_index] + m * i;

            int x_future = x0 + (w_future * (x_max - x0) / 50);
            int y_future = y0 - (gewicht_future * (y0 - y_min) / 300);

            cairo_move_to(cr, prev_x, prev_y);
            cairo_line_to(cr, x_future, y_future);
            cairo_stroke(cr);

            cairo_arc(cr, x_future, y_future, 3, 0, 2 * G_PI);
            cairo_fill(cr);

            prev_x = x_future;
            prev_y = y_future;
        }
    }




    cairo_restore(cr);
    return FALSE;
}

// Callback-Funktion für Scrollen
gboolean on_scroll(GtkWidget* widget, GdkEventScroll* event, gpointer user_data){

    double mouseX = event->x;
    double mouseY = event->y;
    double oldZoom = zoomFactor;

    if(event->direction == GDK_SCROLL_UP){
        zoomFactor *= 1.1;
    } else if(event->direction == GDK_SCROLL_DOWN){
        zoomFactor /= 1.1;
    }

    offsetX = mouseX - (mouseX - offsetX) * (zoomFactor / oldZoom);
    offsetY = mouseY - (mouseY - offsetY) * (zoomFactor / oldZoom);

    if(g_drawing_area){
        gtk_widget_queue_draw(g_drawing_area);
    }
    return TRUE;
}