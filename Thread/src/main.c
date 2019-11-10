#include "concurrency.h"
#include "display.h"

// instantiate the extern histogram 
int histogram[HISTSIZE];
// communication channels to display thread 
int statpipe[2];
int histpipe[2];
int cmdpipe[2];

void print_usage(char *executable) {
    fprintf(stderr, "Incorrect Usage.\n\n%s S|c|C|r|R DIRECTORY\n\n", executable);
    exit(1);
}

int main(int argc, char *argv[]) {
    int opt; 
    int mode;     
    if (argc != 3) {
        print_usage(argv[0]);
    }
    bool too_many_opts = false;
    while((opt = getopt(argc, argv, ":ScCr")) != -1) {  
        if (too_many_opts) {
            print_usage(argv[0]);
        }
        switch(opt) {  
            case 'S': mode = SEQUENTIAL; too_many_opts = true; break;
            case 'c': mode = CONCURRENT_CG; too_many_opts = true; break;
            case 'C': mode = CONCURRENT_FG; too_many_opts = true; break;
            case 'r': mode = READERWRITER; too_many_opts = true; break;
            case '?': fprintf(stderr, "Unrecognized option\n"); print_usage(argv[0]);
        }  
    }
      
    // optind is for the extra arguments 
    // which are not parsed 
    if (optind >= argc) {
        fprintf(stderr, "No directory specified\n"); print_usage(argv[0]);
    }

    // setup communication pipes
    Pipe(statpipe); // this pipe is used to communicate between the stats reader thread and the display thread. (only used in part 2)
    Pipe(histpipe); // this pipe is used to communicate between the histogram reader thread and the display thread. (only used in part 2)
    Pipe(cmdpipe);  // this pipe is used to send a command to the display thread to terminate 

    pthread_t display_thread; 
    // start display thread
    int *modeptr = Malloc(sizeof(int));
    *modeptr = mode; 
    Pthread_create(&display_thread, NULL, display_task, modeptr);

    // these functions calculate the histogram
    // wait for worker threads to finish (join) and return
    char *dirname = argv[optind];
    switch(mode) {
        case SEQUENTIAL:    sequential(dirname); break;
        case CONCURRENT_CG: concurrent_cg(dirname); break; 
        case CONCURRENT_FG: concurrent_fg(dirname); break;
        case READERWRITER:  readerwriter(dirname); break;
    }
    
    // send a byte of data on the command pipe to terminate display thread
    char terminate = '\0';
    Write(cmdpipe[1], &terminate, 1);
    
    Pthread_join(display_thread, NULL);
    return 0;
}
