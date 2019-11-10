#include "concurrency.h"
#include "stats.h"

float calc_mean_median(int sum, int n, int *cfreq, float *median) {
    int i, index = (n+1)/2;
    for (i = 0; i < HISTSIZE; i++) {
        if (index <= cfreq[i]) {
            break;
        }
    }    
    if ((n & 1) == 0) { // even number (mask out the least significant bit, if its 1 then its an odd number)
        if (cfreq[i] == index) {
            *median = (float)(i + i + 1)/(float)2;
        } else {
            *median = (float)(i + i)/(float)2;
        }
    } else { // odd number
        *median = i;
    }
    return (float)sum/(float)n;
}

int statdata(float *mean, float *median, int *mode) {
    int i, sum = 0, n = 0;
    int cfreq[HISTSIZE];
    *mode = -1;
    int maxfreq = -1;
    for (i = 0; i < HISTSIZE; i++) {
        n += histogram[i];
        cfreq[i] = n;
        sum += i * histogram[i];        
        if (maxfreq < histogram[i]) {
            *mode = i;
            maxfreq = histogram[i];
        }
    }
    *mean = calc_mean_median(sum, n, cfreq, median);
    return n;
}

int histdata(int *copy) {
    int i, n = 0;
    for (i = 0; i < HISTSIZE; i++) {
        n += histogram[i];
        copy[i] = histogram[i];
    }
    return n;
}

