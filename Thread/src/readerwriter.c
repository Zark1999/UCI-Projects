#include "concurrency.h"

int readcnt;
sem_t mutex, w;

void snapshot_stats() {
    int i, sum = 0;
    int cfreq[HISTSIZE];
    statsnap s;
    s.n = 0;
    
    // part 2 put your locks here
	
	sem_wait(&mutex);
	readcnt ++;
	if (readcnt == 1){
		sem_wait(&w);
	}
	sem_post(&mutex);
	
    s.mode = -1;
    int maxfreq = -1;
    for (i = 0; i < HISTSIZE; i++) {
        s.n += histogram[i];
        cfreq[i] = s.n;
        sum += i * histogram[i];        
        if (maxfreq < histogram[i]) {
            s.mode = i;
            maxfreq = histogram[i];
        }
    }
	
	sem_wait(&mutex);
	readcnt --;
	if (readcnt == 0){
		sem_post(&w);
	}
	sem_post(&mutex);
    // part 2 put your locks here

    s.mean = calc_mean_median(sum, s.n, cfreq, &s.median);
    // send stats
    Write(statpipe[1], &s, sizeof(statsnap));
}

void snapshot_histogram() {
    int i;
    histsnap s;
    s.n = 0;
    
    // part 2 put your locks here
	sem_wait(&mutex);
	readcnt ++;
	if (readcnt == 1){
		sem_wait(&w);
	}
	sem_post(&mutex);
	
    for (i = 0; i < HISTSIZE; i++) {
        s.n += histogram[i];
        s.hist[i] = histogram[i];
    }
    
    // part 2 put your locks here
	sem_wait(&mutex);
	readcnt --;
	if (readcnt == 0){
		sem_post(&w);
	}
	sem_post(&mutex);
	
    // send n and histogram   
    Write(histpipe[1], &s, sizeof(histsnap));
}

void *readerwriter_stat_task(void *data) {
    while(1) {
        usleep(READSLEEP);
        snapshot_stats();
    }
    return NULL;
}

void *readerwriter_hist_task(void *data) {
    while(1) {
		
        usleep(READSLEEP);
        snapshot_histogram();
		
    }
    return NULL;
}

void start_readers(pthread_t *readers) {
    pthread_create(&readers[0], NULL, readerwriter_stat_task, NULL);
    pthread_create(&readers[1], NULL, readerwriter_hist_task, NULL);

}

void readerwriter(char *dirname) {
    // start reader threads
	sem_init(&mutex,0,1);
	sem_init(&w,0,1);
	readcnt = 0;
	
    pthread_t readers[2];
    start_readers(readers);
	
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
	
	int i;
	for (i = 0; i < count; i++) {
		Pthread_create(&tid[i],NULL,rw_thread,(void*)(&fds[i]));
	}
	
	for (i = 0; i < count; i++) {
		Pthread_join(tid[i],NULL);
	}
	
	pthread_cancel(readers[0]);
	pthread_cancel(readers[1]);
    // remember to cancel the reader threads when you are done processing all the files
}

void* rw_thread(void* arg){
	int fd = *((int*)arg);
	int rv;
	char c;
	while((rv = Read(fd, &c, 1)) != 0) {
		c -= '0';
		if (c < 0 || c >= HISTSIZE) {
			fprintf(stderr, "skipping %c\n", c);
			continue;
		}
		sem_wait(&w);
		histogram[(int)c]++;
		sem_post(&w);
	}
	Close(fd);
	
	return NULL;
}
