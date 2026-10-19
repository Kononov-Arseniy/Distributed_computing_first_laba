#include <stdio.h>
#include "my_rwlock.h"


void rwlock_init(struct Rwlock* rw){
    pthread_mutex_init(&rw->mutex, NULL);
    pthread_cond_init(&rw->readers_cond, NULL);
    pthread_cond_init(&rw->writers_cond, NULL);
    rw->writers_wait = 0;
    rw->readers_wait = 0;
    rw->readers_now = 0;
    rw->writer_now_is_writing = 0;
}

void rwlock_delete(struct Rwlock* rw){
    pthread_mutex_destroy(&rw->mutex);
    pthread_cond_destroy(&rw->readers_cond);
    pthread_cond_destroy(&rw->writers_cond);
}

void rwlock_rdlock(struct Rwlock* rw){
    pthread_mutex_lock(&rw->mutex);
    rw->readers_wait++;
    while (rw->writer_now_is_writing || rw->writers_wait != 0){
        pthread_cond_wait(&rw->readers_cond, &rw->mutex);
    }
    rw->readers_wait--;
    rw->readers_now++;
    pthread_mutex_unlock(&rw->mutex);
}

void rwlock_wrlock(struct Rwlock* rw){
    pthread_mutex_lock(&rw->mutex);
    rw->writers_wait++;
    while (rw->writer_now_is_writing || rw->readers_now != 0)
    {
        pthread_cond_wait(&rw->writers_cond, &rw->mutex);
    }
    rw->writer_now_is_writing = 1;
    rw->writers_wait--;

    pthread_mutex_unlock(&rw->mutex);
}

void rwlock_unlock(struct Rwlock* rw){
    pthread_mutex_lock(&rw->mutex);
    if (rw->writer_now_is_writing){
        rw->writer_now_is_writing = 0;
        if (rw->writers_wait > 0){
            pthread_cond_signal(&rw->writers_cond);
        } else if (rw->readers_wait > 0) {
            pthread_cond_broadcast(&rw->readers_cond);
        }
    } else {
        rw->readers_now--;
        if (rw->readers_now == 0 && rw->writers_wait > 0){
            pthread_cond_signal(&rw->writers_cond);
        }
    }
    pthread_mutex_unlock(&rw->mutex);
}