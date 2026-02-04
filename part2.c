#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

// 5 readers
#define NUM_READERS 5
// 2 writers 
#define NUM_WRITERS 2

#define READS_PER_READER 10
#define WRITES_PER_WRITER 2

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_reader = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_writer = PTHREAD_COND_INITIALIZER;

int active_readers = 0;
int waiting_readers = 0;
int active_writers = 0;
int waiting_writers = 0;

void reader_acquire() {
    pthread_mutex_lock(&mutex);
    waiting_readers++;
    
    // Wait if there are active writers OR if there are waiting writers and no active readers
    while (active_writers > 0 || waiting_writers > 0) {
        pthread_cond_wait(&cond_reader, &mutex);
    }
    
    waiting_readers--;
    active_readers++;
    pthread_mutex_unlock(&mutex);
}

void reader_release() {
    pthread_mutex_lock(&mutex);
    assert(active_readers > 0);
    active_readers--;
    
    // If this is the last reader, signal a waiting writer
    if (active_readers == 0) {
        pthread_cond_signal(&cond_writer);
    }
    
    pthread_mutex_unlock(&mutex);
}

void writer_acquire() {
    pthread_mutex_lock(&mutex);
    waiting_writers++;
    
    // Wait if there are active readers or active writers
    while (active_readers > 0 || active_writers > 0) {
        pthread_cond_wait(&cond_writer, &mutex);
    }
    
    waiting_writers--;
    active_writers = 1;
    pthread_mutex_unlock(&mutex);
}

void writer_release() {
    pthread_mutex_lock(&mutex);
    assert(active_writers == 1);
    active_writers = 0;
    
    // Signal ALL waiting writers first, then readers
    if (waiting_writers > 0) {
        pthread_cond_signal(&cond_writer);
    } else {
        pthread_cond_broadcast(&cond_reader);
    }
    
    pthread_mutex_unlock(&mutex);
}

void *reader(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < READS_PER_READER; i++) {
        reader_acquire();
        assert(active_writers == 0);
        sleep(rand() % 3); 
        printf("Reader %d did task %d.\n", id, i + 1);
        reader_release();
        usleep(10000);
    }
    return NULL;
}

void *writer(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < WRITES_PER_WRITER; i++) {
        writer_acquire();
        assert(active_readers == 0 && active_writers == 1);
        sleep(rand() % 3); 
        printf("Writer %d did task %d.\n", id, i + 1);
        writer_release();
        usleep(10000); 
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];
    
    srand(time(NULL));
    
    // Create writer threads first to give them a chance
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }
    
    // Short delay before creating readers
    usleep(10000);
    
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }
    
    // Join threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }
    
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    
    return 0;
}