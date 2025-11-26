#ifndef LANGUAGES_H
#define LANGUAGES_H

extern const char* week;
extern const char* weigth;
extern const char* save;
extern const char* resetZoom;
extern const char* resetFile;
extern const char* instruction;
extern const char* backToGraph;
extern const char* chooseWorkout;
extern const char* logWorkout;
extern const char* pushSave;
extern const char* canResetZoom;
extern const char* canResetFile;
static int restart_requested;
static char **saved_argv;


char* returnLanguage(void);
char* checkLanguage(void);
void loadLanguageStrings(void);
void toggleLanguage(int argc, char* argv[]);
void request_restart(char **argv);
void restart_if_requested(void);

#endif