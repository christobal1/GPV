#include "languages.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>

const char* week;
const char* weigth;
const char* save;
const char* resetZoom;
const char* resetFile;
const char* instruction;
const char* backToGraph;
const char* chooseWorkout;
const char* logWorkout;
const char* pushSave;
const char* canResetZoom;
const char* canResetFile;
static int restart_requested = 0;
static char **saved_argv;


char* returnLanguage(){
    static char buffer[50];
    FILE* file = fopen("details/languageSelected.txt", "r");
    if(!file){
        perror("Fehler bei Öffnen der Datei");
        return NULL;
    }
    
    if(fgets(buffer, sizeof(buffer), file) == NULL){
        fclose(file);
        return NULL;
    }

    fclose(file);

    buffer[strcspn(buffer, "\n")] = '\0'; // \n entfernen

    return buffer;
}



char* checkLanguage(void){
    char* buffer = returnLanguage();

    printf("Sprache geladen: %s\n", buffer);
    
    if(strcmp(buffer, "Deutsch") == 0){
        return "Deutsch";
    } else if (strcmp(buffer, "English") == 0){
        return "English";
    } else {
        return NULL;
    }
}


void loadLanguageStrings(void){
    char* language = checkLanguage();
    if(!language){
        printf("Keine Sprache gefunden, Standard auf Englisch.\n");
        language = "English";
    }

    const char* filename = (strcmp(language, "Deutsch") == 0) ? "details/Deutsch.txt" : "details/English.txt";

    FILE* file = fopen(filename, "r");
    if(!file){
        perror("Fehler bei Öffnen von Datei mit Wörtern");
        return;
    }

    static char line[256];

    if(fgets(line, sizeof(line), file)) week = strdup(line);
    if(fgets(line, sizeof(line), file)) weigth = strdup(line);
    if(fgets(line, sizeof(line), file)) save = strdup(line);
    if(fgets(line, sizeof(line), file)) resetZoom = strdup(line);
    if(fgets(line, sizeof(line), file)) resetFile = strdup(line);
    if(fgets(line, sizeof(line), file)) instruction = strdup(line);
    if(fgets(line, sizeof(line), file)) backToGraph = strdup(line);
    if(fgets(line, sizeof(line), file)) chooseWorkout = strdup(line);
    if(fgets(line, sizeof(line), file)) logWorkout = strdup(line);
    if(fgets(line, sizeof(line), file)) pushSave = strdup(line);
    if(fgets(line, sizeof(line), file)) canResetZoom = strdup(line);
    if(fgets(line, sizeof(line), file)) canResetFile = strdup(line);


    fclose(file);
}

void toggleLanguage(int argc, char* argv[]){
    char* language = checkLanguage();
    FILE* file = fopen("details/languageSelected.txt", "w");
    if(!file){
        perror("Fehler bei Öffnen der Datei für ausgewählte Sprache");
    }
    if(strcmp(language, "Deutsch") == 0){
        fputs("English", file);
        printf("Sprache auf Englisch gewechselt.\n");
    } else if(strcmp(language, "English") == 0){
        fputs("Deutsch", file);
        printf("Sprache auf Deutsch gewechselt.\n");
    }
    fclose(file);
    gtk_main_quit();
    
}

void request_restart(char **argv) {
    saved_argv = argv;
    restart_requested = 1;
    gtk_main_quit();
}

void restart_if_requested(void) {
    if (restart_requested) {
        execvp(saved_argv[0], saved_argv);
        perror("Neustart fehlgeschlagen");
        exit(EXIT_FAILURE);
    }
}

/**
// App Neustart
    printf("Programm wird neu gestartet...\n");
    fflush(stdout);
    
    execlp(argv[0], argv[0], NULL);
    gtk_main_quit();
    perror("Neustart fehlgeschlagen");
    exit(EXIT_FAILURE);
 */