#include "concurrency.h" 

int Pipe(int pipefd[2]) {
    int rv; 
    if ((rv = pipe(pipefd)) < 0) {
        perror("pipe");
        exit(1);
    }
    return rv;
}

int Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg) {
    int rv;
     
    if ((rv = pthread_create(thread, attr, start_routine, arg)) < 0) {
        perror("pthread_create");
        exit(1);
    }
    return rv;
}

int Pthread_join(pthread_t thread, void **retval) {
    int rv;
    if ((rv = pthread_join(thread, retval)) < 0) {
        perror("pthread_create");
        exit(1);
    }
    return rv;
}

void* Malloc(size_t size) {
    void* rv;
    if ((rv = malloc(size)) == NULL) {
        perror("malloc");
        exit(1);
    }
    return rv;
}

void* Realloc(void *ptr, size_t size) {
    void* rv;
    if ((rv = realloc(ptr, size)) == NULL) {
        perror("realloc");
        exit(1);
    }
    return rv;
}

int Write(int fd, const void* buf, size_t count) {
    int rv;
    if ((rv = write(fd, buf, count)) < 0) {
        perror("write");
        exit(1);
    }
    return rv;
}

int Read(int fd, void* buf, size_t count) {
    int rv;
    if ((rv = read(fd, buf, count)) < 0) {
        perror("read");
        exit(1);
    }
    return rv;
}

int Chdir(const char* path) {
    int rv;
    if ((rv = chdir(path)) < 0) {
        perror("chdir");
        exit(1);
    }
    return rv;
}

DIR *Opendir(const char *name) {
    DIR *rv;
    if ((rv = opendir(name)) == NULL) {
        perror("opendir");
        exit(1);
    }
    return rv;
}

int Closedir(DIR *dirp) {
    int rv;
    if ((rv = closedir(dirp)) < 0) {
        perror("closedir");
        exit(1);
    }
    return rv;
}

int Stat(const char *path, struct stat *buf) {
    int rv;
    if ((rv = stat(path, buf)) < 0){
        perror("stat");
        exit(1);
    }
    return rv;
}

int Open(const char *pathname, int flags) {
    int rv;
    if ((rv = open(pathname, flags)) < 0){
        perror("open");
        exit(1);
    }
    return rv;
}

int Close(int fd) {
    int rv;
    if ((rv = close(fd)) < 0){
        perror("close");
        exit(1);
    }
    return rv;
}
