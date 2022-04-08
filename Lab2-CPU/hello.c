#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int nThreads;
void* Hello(void* thread);


int main() {
    long thread;   // 64 bits
    nThreads = 4;
    
    pthread_t* handles;
    handles = (pthread_t*)malloc( nThreads*sizeof(pthread_t) );

    for (thread=0;thread<nThreads;thread++){
        pthread_create(&handles[thread],NULL,Hello,(void*)thread);
    }
    printf("Lanzamos a todos :D\n");
    for (thread=0;thread<nThreads;thread++){
        pthread_join(handles[thread],NULL);
    }

    free(handles);
    return 0;
}

void* Hello(void* thread) {
    long threadLocal = (long) thread;
    printf("Hola hilo %ld, como estas?\n",threadLocal);
    return NULL;
}
