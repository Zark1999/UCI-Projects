#include "concurrency.h"

// this is an example of the sequential code
void sequential(char *dirname) {
    // try to change to directory specified in args
    Chdir(dirname);
    // open directory stream
    DIR *dp = Opendir("./");

    struct dirent *ep;
    // start by assuming there will be only 8 files
    // reallocate if this guess is too small
    int numfds = 8;
    // allocate space for file information
    int *fds = Malloc(numfds*sizeof(int));
    int count = 0;
    while ((ep = readdir(dp)) != NULL) {
        // make sure file exists
        struct stat sb;
        Stat(ep->d_name, &sb);
        // make sure that its a "regular" file (i.e not a directory, link etc.)
        if ((sb.st_mode & S_IFMT) != S_IFREG) {
            continue;
        }
        // open file 
        fds[count] = Open(ep->d_name, O_RDONLY);
        count++;
        // if the number of files is more than what we guessed
        // ask for more space
        if (count == numfds) {
            numfds = numfds * 2;
            fds = Realloc(fds, numfds*sizeof(int));
        }
    }
    // cleanup
    Closedir(dp);

    int i;
    // go through all the files
    for (i = 0; i < count; i++) {
        char c;
        int rv;
        // read each file one byte at a time and build histogram
        while((rv = Read(fds[i], &c, 1)) != 0) {
            c -= '0';
            // if the byte is out of range, skip it
            if (c < 0 || c >= HISTSIZE) {
                fprintf(stderr, "skipping %c\n", c);
                continue;
            }
            histogram[(int)c]++;           
        }
        // cleanup
        Close(fds[i]);
    }
    // cleanup
    free(fds);
    // work is done, return
}
