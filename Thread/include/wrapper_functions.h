#ifndef WRAPPERFUNC_H
#define WRAPPERFUNC_H

int Pipe(int pipefd[2]);

int Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

int Pthread_join(pthread_t thread, void **retval);

void* Malloc(size_t size);

void* Realloc(void *ptr, size_t size);

int Write(int fd, const void* buf, size_t count);

int Read(int fd, void* buf, size_t count);

int Chdir(const char* path);

DIR *Opendir(const char *name);

int Closedir(DIR *dirp); 

int Stat(const char *path, struct stat *buf);

int Open(const char *pathname, int flags);

int Close(int fd);

void* cg_thread(void* arg);
void* fg_thread(void* arg);
void* rw_thread(void* arg);

#endif
