#include "concurrency.h"
#include "display.h"
/****************************************************/
// DO NOT MODIFY THIS FILE!
// IMPORTANT: THIS FILE WILL BE REPLACED WHEN GRADING
// 
// DISCLAIMER: This file doesn't handle errors 
// pertaining to ncurses properly.
/****************************************************/
static WINDOW *histwin;
static WINDOW *statwin;
static WINDOW *msgwin;

static int maxhistlen;
static clock_t start_time;
static clock_t lap;

void setup_ncurses(int mode) {
    initscr();
    cbreak();
    noecho();
    curs_set(false);
    int mx, my;
    getmaxyx(stdscr, my, mx);

    maxhistlen = mx - 20;
    msgwin =  newwin(MSGWINH, mx, my-MSGWINH, 0);
    statwin = newwin(STATWINH, mx, my-MSGWINH-STATWINH, 0);
    histwin = newwin(my-MSGWINH-STATWINH, mx, 0, 0);
    
    box(msgwin, 0, 0);
    box(statwin, 0, 0);
    box(histwin, 0, 0);
    
    char *desc;
    switch(mode) {
        case SEQUENTIAL: desc = "Sequential"; break;
        case CONCURRENT_CG: desc = "Concurrent Coarse Grain"; break; 
        case CONCURRENT_FG: desc = "Concurrent Fine Grain"; break; 
        case READERWRITER: desc = "Reader-Writer (Coarse Grain Locks)"; break; 
    }
    
    mvwprintw(msgwin, 2, 2, "Execution Mode: %s", desc);
    mvwprintw(msgwin, 3, 2, "Processing...");
    lap = clock() - start_time;
    mvwprintw(msgwin, 4, 2, "Elapsed time (s): %0.2f", ((double)lap)/CLOCKS_PER_SEC);
    
    
    refresh();
    wrefresh(msgwin);
    wrefresh(statwin);
    wrefresh(histwin);
}

void teardown_ncurses() {
    mvwprintw(msgwin, 3, 2, "Execution Complete. Press any key to exit.");
    wrefresh(msgwin);
    getch();
    delwin(msgwin);
    delwin(statwin);
    delwin(histwin);
    endwin();
}

void draw_stats(int n, float mean, float median, int mode) {
    werase(statwin);
    box(statwin, 0, 0);

    mvwprintw(statwin, 1, 2, "Stats:");
    mvwprintw(statwin, 3, 2, "Mean:   %0.2f", mean);
    mvwprintw(statwin, 4, 2, "Median: %0.2f", median);
    mvwprintw(statwin, 5, 2, "Mode:   %d", mode);
    mvwprintw(statwin, 6, 2, "# Elements: %d", n);

    wrefresh(statwin);
}

void draw_histogram(int n, int *snapshot) {
    int i;
    float percent[HISTSIZE];

    werase(histwin);
    box(histwin, 0, 0);

    for (i = 0; i < HISTSIZE; i++) {
        percent[i] = ((float)snapshot[i])/(float)n;
    }

    mvwprintw(histwin, 1, 2, "Histogram:");
    for (i = 0; i < HISTSIZE; i++) {
        int row = i + HISTROWOFFSET;
        mvwprintw(histwin, row, 1, " %d: ", i);
        int j;
        for (j = 0; j < (int)(percent[i] * maxhistlen); j++) {
            mvwprintw(histwin, row, j + HISTCOLOFFSET, "=");
        }
        mvwprintw(histwin, row, j + HISTCOLOFFSET, "| %.2f%%", percent[i]*100); 
    }
    wrefresh(histwin);
}

void draw_final_data() {
    float mean, median;
    int n, mode;
    n = statdata(&mean, &median, &mode);
    draw_histogram(n, histogram);
    draw_stats(n, mean, median, mode);
}

void* display_task(void* data) {
    start_time = clock();
    // setup the display
    setup_ncurses(*(int*)data);
    free(data);
    int i;
    struct epoll_event events[3]; // stat, hist and cmd
    int epollfd;
    // this thread will collect data from the read ends of the pipes
    int fds[3] = {statpipe[0], histpipe[0], cmdpipe[0]};
    if ((epollfd = epoll_create1(0)) < 0) {
        perror("epoll_create1");
        exit(1);
    }
    // the descriptors being added are read-only thus EPOLLIN    
    for (i = 0; i < 3; i++) {
        events[i].events = EPOLLIN;
        events[i].data.fd = fds[i];
        if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fds[i], &events[i]) < 0) {
            perror("epoll_ctl");
            exit(1);
        }
    }
    // start polling
    while(1) {
    int nfds = epoll_wait(epollfd, events, 3, READSLEEP);
    if (nfds < 0) {
        perror("epoll_wait");
        exit(1);
    }
    // update time
    lap = clock() - start_time;
    mvwprintw(msgwin, 4, 2, "Elapsed time (s): %0.2f", ((double)lap)/CLOCKS_PER_SEC);
    wrefresh(msgwin);

    for (i = 0; i < nfds; i++) {
        int fd = events[i].data.fd;
        if (fd == statpipe[0]) {
            statsnap s;
            if (Read(fd, &s, sizeof(statsnap)) > 0) {
                draw_stats(s.n, s.mean, s.median, s.mode);
            }
        }
        if (fd == histpipe[0]) {
            histsnap s;
            if (Read(fd, &s, sizeof(histsnap)) > 0) {
                draw_histogram(s.n, s.hist);
            }
        }
        if (fd == cmdpipe[0]) {
            draw_final_data();
            teardown_ncurses();
            return NULL;
        }
    }
    }
    return NULL;
}
