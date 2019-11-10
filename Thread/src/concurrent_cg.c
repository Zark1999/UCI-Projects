#include "concurrency.h"

sem_t mutex;

void concurrent_cg(char *dirname) {
    // your code here
	Chdir(dirname);

	DIR *dp = Opendir("./");
	
	struct dirent *ep;
	int numfds = 8;
	int *fds = Malloc(numfds*sizeof(int));
	int count = 0;
	while ((ep = readdir(dp)) != NULL) {
		struct stat sb;
		Stat(ep->d_name, &sb);
		if ((sb.st_mode & S_IFMT) != S_IFREG) {
			continue;
		}

		fds[count] = Open(ep->d_name, O_RDONLY);
		count++;
		
		if (count == numfds) {
			numfds = numfds * 2;
			fds = Realloc(fds, numfds*sizeof(int));
		}
	}
	Closedir(dp);
	
	pthread_t tid[count];
	sem_init(&mutex,0,1);
	
	int i;
	for (i = 0; i < count; i++) {
		Pthread_create(&tid[i],NULL,cg_thread,(void*)(&fds[i]));
	}
	
	for (i = 0; i < count; i++) {
		Pthread_join(tid[i],NULL);
	}
	
	free(fds);
}

void* cg_thread(void* arg){
	int fd = *((int*)arg);
	int his[HISTSIZE];
	int rv, i;
	char c;
	
	for (i = 0; i < HISTSIZE; i++){
		his[i] = 0;
	}
	
	while((rv = Read(fd, &c, 1)) != 0) {
		c -= '0';
		if (c < 0 || c >= HISTSIZE) {
			fprintf(stderr, "skipping %c\n", c);
			continue;
		}
		
		his[(int)c]++;
		
	}
	Close(fd);
	
	for (i = 0; i < HISTSIZE; i++){
		sem_wait(&mutex);
		histogram[i] = histogram[i] + his[i];
		sem_post(&mutex);
	}
	
	
	return NULL;
}
