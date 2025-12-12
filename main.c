#include "gpv.h"
#include "languages.h"
#include <gtk/gtk.h>
#include <locale.h>

//GtkWidget* window = NULL;
//GtkWidget* window2 = NULL;

int main(int argc, char *argv[]) {
    setlocale(LC_NUMERIC, "C");  // Dezimaltrennzeichen = Punkt
    const char* programmName = "GPV (Gym Progress Visualizer) 1.2.1";

    
    GtkWidget* drawing_area;
    GtkWidget* hbox;
    GtkWidget* vbox;
    GtkWidget* entry1;
    GtkWidget* entry2;
    GtkWidget* label1;
    GtkWidget* label2;
    GtkWidget* button1;
    GtkWidget* button2;
    GtkWidget* button3;
    GtkWidget* buttonToWindow2;
    GtkWidget* buttonToWindow3;
    GtkWidget* buttonLanguage;

    GtkWidget* buttonToWindow1From2;
    GtkWidget* image1;
    GtkWidget* image2;
    GtkWidget* image3;
    GtkWidget* image4;
    GtkWidget* image5;

    GtkWidget* buttonToWindow1From3;

    // GTK initialisieren
    gtk_init(&argc, &argv);
    printf("GTK erfolgreich intialisiert!\n");
    GdkDisplay* display = gdk_display_get_default();
    GdkMonitor* monitor = gdk_display_get_primary_monitor(display);
    GdkRectangle geometry;
    gdk_monitor_get_geometry(monitor, &geometry);

    int windowX = 0.8 * geometry.width;
    int windowY = 0.8 * geometry.height;
    printf("Starte Window mit: %d x %d\n", windowX, windowY);

    // Fenster 1 erstellen
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), programmName);
    gtk_window_set_default_size(GTK_WINDOW(window), windowX, windowY);

    // Fenster 2 erstellen
    window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window2), programmName);
    gtk_window_set_default_size(GTK_WINDOW(window2), windowX, windowY);

    // Fenster 3 erstellen
    window3 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window3), programmName);
    gtk_window_set_default_size(GTK_WINDOW(window3), windowX, windowY);
    
    // Sprache laden
    loadLanguageStrings();












    // Fenster 1 Inhalt:

    // Hauptcontainer
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    // Zeichenoberfläche 
    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 700, 500);

    // Scroll-Events aktivieren
    gtk_widget_add_events(drawing_area, GDK_SCROLL_MASK);

    // Vertikale Box für Eingabefelder
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // Eingabefelder und Labels
    label1 = gtk_label_new(week);
    entry1 = gtk_entry_new();
    label2 = gtk_label_new(weigth);
    entry2 = gtk_entry_new();
    button1 = gtk_button_new_with_label(save);
    button2 = gtk_button_new_with_label(resetZoom);
    button3 = gtk_button_new_with_label(resetFile);
    buttonToWindow2 = gtk_button_new_with_label(instruction);
    buttonToWindow3 = gtk_button_new_with_label("x");
    buttonLanguage = gtk_button_new_with_label(checkLanguage());

    // Globale Zeiger setzen
    g_drawing_area = drawing_area;
    g_entry1 = entry1;
    g_entry2 = entry2;
    g_button_save = button1;
    g_summary = gtk_label_new("");
    g_progressPerWeek = gtk_label_new("");

    GtkWidget* combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "Auswahl");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "abs/ab_pulldown.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "abs/leg_raises.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "arms/barbell_curl.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "arms/biceps_curl.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "arms/laying_biceps_curl.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "arms/brachioradialis.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "arms/cat_curl.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "arms/concentration_curl.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "arms/skullcrushers.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "arms/triceps_pushdown.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "back/back_extension.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "back/kelso_shrug.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "back/lat_pulldown.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "back/lat_pullover.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "back/lat_row.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "back/rdl.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "back/tbar_row.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "chest/benchpress.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "chest/butterfly.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "chest/dumbbell_press.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "chest/incline_dumbbell_press.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "chest/incline_smith_press.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "chest/upper_fly.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "legs/calves_sitting.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "legs/calves_standing.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "legs/hack_squat.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "legs/hamstring_curl.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "legs/hip_thrust.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "legs/leg_extension.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "legs/legpress.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "legs/adductor.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "legs/abductor.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "shoulders/butterfly_reverse.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "shoulders/lateral_raises.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "shoulders/shoulder_press.txt");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "shoulders/shoulder_row.txt");

    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);

    // Dateiname direkt setzen!
    const char* filename = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
    if(filename) {
        snprintf(current_savefile, sizeof(current_savefile), "saves/%s", filename);
    }


    gtk_box_pack_start(GTK_BOX(vbox), buttonToWindow2, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), buttonToWindow3, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), combo, FALSE, FALSE, 28);
    gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), entry1, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), entry2, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button1, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), g_summary, FALSE, FALSE, 50);
    gtk_box_pack_start(GTK_BOX(vbox), g_progressPerWeek, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), button2, FALSE, FALSE, 20);
    gtk_box_pack_start(GTK_BOX(vbox), button3, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), buttonLanguage, FALSE , FALSE, 0);


    // Summary für Start ausfüllen
    char buffer[100];
    calculateProgress(buffer, sizeof(buffer));
    gtk_label_set_text(GTK_LABEL(g_summary), buffer);

    // Wöchentliche Summary für Start ausfüllen
    char buffer2[100];
    calculateWeeklyProgress(buffer2, sizeof(buffer2));
    gtk_label_set_text(GTK_LABEL(g_progressPerWeek), buffer2);


    // Werte aus Datei laden
    if(strcmp(current_savefile, PLACEHOLDER_FILE) == 0){
        set_placeholder_mode(1);
    } else {
        load_data_from_file();
        set_placeholder_mode(0);
        update_summary();
        update_weekly_summary();
    }

    // Zeichenfläche und Eingabefelder in Hauptcontainer einfügen
    gtk_box_pack_start(GTK_BOX(hbox), drawing_area, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 10);

    // Hauptcontainer in Fenster einfügen
    gtk_container_add(GTK_CONTAINER(window), hbox);












    //Fenster 2 Inhalt:

    buttonToWindow1From2 = gtk_button_new_with_label(backToGraph);

    // Scrolled Window für Fenster 2
    GtkWidget* scrolled2 = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_set_border_width(GTK_CONTAINER(scrolled2), 10);

    // Scroll Policy: vertikal automatisch, horizontal nie
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled2), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    GtkWidget* vbox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_add(GTK_CONTAINER(scrolled2), vbox2);

    // VBox mit Bildern
    image1 = load_scaled_image("info_pictures/instruction1.png", windowX * 0.7, -1);
    image2 = load_scaled_image("info_pictures/instruction2.png", windowX * 0.7, -1);
    image3 = load_scaled_image("info_pictures/instruction3.png", windowX * 0.7, -1);
    image4 = load_scaled_image("info_pictures/instruction4.png", windowX * 0.7, -1);
    image5 = load_scaled_image("info_pictures/instruction5.png", windowX * 0.7, -1);
    GtkWidget* label_win2 = gtk_label_new("Hallo");

    gtk_widget_set_halign(image1, GTK_ALIGN_START);
    gtk_widget_set_halign(image2, GTK_ALIGN_START);
    gtk_widget_set_halign(image3, GTK_ALIGN_START);
    gtk_widget_set_halign(image4, GTK_ALIGN_START);
    gtk_widget_set_halign(image5, GTK_ALIGN_START);

    GtkWidget* infoText1 = gtk_label_new(chooseWorkout);
    GtkWidget* infoText2 = gtk_label_new(logWorkout);
    GtkWidget* infoText3 = gtk_label_new(pushSave);
    GtkWidget* infoText4 = gtk_label_new(canResetZoom);
    GtkWidget* infoText5 = gtk_label_new(canResetFile);

    GtkWidget* hboxInfo1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(hboxInfo1), image1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hboxInfo1), infoText1, TRUE, TRUE, 0);

    GtkWidget* hboxInfo2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(hboxInfo2), image2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hboxInfo2), infoText2, TRUE, TRUE, 0);

    GtkWidget* hboxInfo3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(hboxInfo3), image3, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hboxInfo3), infoText3, TRUE, TRUE, 0);

    GtkWidget* hboxInfo4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(hboxInfo4), image4, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hboxInfo4), infoText4, TRUE, TRUE, 0);

    GtkWidget* hboxInfo5 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(hboxInfo5), image5, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hboxInfo5), infoText5, TRUE, TRUE, 0);

    // Inhalte einfügen
    gtk_box_pack_start(GTK_BOX(vbox2), label_win2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox2), hboxInfo1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox2), hboxInfo2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox2), hboxInfo3, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox2), hboxInfo3, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox2), hboxInfo4, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox2), hboxInfo5, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox2), buttonToWindow1From2, FALSE, FALSE, 0);


    gtk_container_add(GTK_CONTAINER(window2), scrolled2);







    // Fenster 3 Inhalt

    buttonToWindow1From3 = gtk_button_new_with_label(backToGraph);

    GtkWidget* hboxWin3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(hboxWin3), buttonToWindow1From3, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window3), hboxWin3);





    




    // Dropdown-Callback verbinden
    g_signal_connect(combo, "changed", G_CALLBACK(on_combo_changed), NULL);

    // Zeichen-Callback verbinden
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw), NULL);

    // Scroll-Callback verbinden
    g_signal_connect(G_OBJECT(drawing_area), "scroll-event", G_CALLBACK(on_scroll), NULL);
    
    // Button-Callback verbinden
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button_clicked), NULL);

    g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), NULL);

    g_signal_connect(button3, "clicked", G_CALLBACK(on_button3_clicked), NULL);

    g_signal_connect(buttonToWindow2, "clicked", G_CALLBACK(switchToWindow2From1), NULL);

    g_signal_connect(buttonToWindow3, "clicked", G_CALLBACK(switchToWindow3From1), NULL);

    g_signal_connect(buttonToWindow1From2, "clicked", G_CALLBACK(switchToWindow1From2), NULL);

    g_signal_connect(buttonToWindow1From3, "clicked", G_CALLBACK(switchToWindow1From3), NULL);


    g_signal_connect(buttonLanguage, "clicked", G_CALLBACK(toggleLanguage), NULL);

    // Signal verbinden: Fenster schließen
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(window2, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);

    // Alles anzeigen
    gtk_widget_show_all(window);

    // Hauptschleife starten
    gtk_main();
    restart_if_requested();

    return 0;
}