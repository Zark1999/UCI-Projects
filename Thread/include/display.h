#ifndef DISPLAY_H
#define DISPLAY_H

#define HISTWIDTH 500
#define HISTSTART 0
#define HISTROWOFFSET 3
#define HISTCOLOFFSET 5
#define STATSTART 13

#define MSGWINH 7
#define STATWINH 15

#include <time.h>

void setup_ncurses(int mode);
void teardown_ncurses();
void draw_histogram(int n, int *snapshot);
void draw_stats(int n, float mean, float median, int mode);
void draw_final_data();
void* display_task(void* data);

#endif
