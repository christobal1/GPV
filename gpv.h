#ifndef GPV_H
#define GPV_H

#include <gtk/gtk.h>

#define MAX_PUNKTE 1000

extern char current_savefile[256];
extern int punkte_woche[MAX_PUNKTE];
extern double punkte_gewicht[MAX_PUNKTE];
extern int punkte_count;
extern const char* PLACEHOLDER_FILE;
extern int woche;
extern double gewicht;

// Globale Fenster
extern GtkWidget* window;
extern GtkWidget* window2;
extern GtkWidget* window3;

// Globale Widgets
extern GtkWidget* g_drawing_area;
extern GtkWidget* g_entry1;
extern GtkWidget* g_entry2;
extern GtkWidget* g_summary;
extern GtkWidget* g_labelCurrentWeek;
extern GtkWidget* g_progressPerWeek;
extern GtkWidget* g_button_save;

// Funktionen
void switchToWindow2From1(GtkWidget* widget, gpointer data);
void switchToWindow1From2(GtkWidget* widget, gpointer data);
void switchToWindow3From1(GtkWidget* widget, gpointer data);
void switchToWindow1From3(GtkWidget* widget, gpointer data);
GtkWidget* load_scaled_image(const char* filename, int width, int height);
void save_data_to_file();
void load_data_from_file();
void updateCurrentWeekLabel();
void calculateProgress(char* buffer, size_t size);
void update_summary();
void calculateWeeklyProgress(char* buffer, size_t size);
void update_weekly_summary();
void set_placeholder_mode(int on);

// GTK Callbacks
void on_combo_changed(GtkComboBoxText* combo, gpointer user_data);
void on_button_clicked(GtkWidget* widget, gpointer data);
void on_button2_clicked(GtkWidget* wdiget, gpointer data);
void on_button3_clicked(GtkWidget* widget, gpointer data);
gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data);
gboolean on_scroll(GtkWidget* widget, GdkEventScroll* event, gpointer user_data);

#endif