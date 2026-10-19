#include <pthread.h>

struct Rwlock{
    pthread_mutex_t mutex;
    pthread_cond_t readers_cond;
    pthread_cond_t writers_cond;
    int writers_wait;
    int readers_now;
    int readers_wait;
    int writer_now_is_writing;
};

void rwlock_init(struct Rwlock* rw);

void rwlock_delete(struct Rwlock* rw);

void rwlock_rdlock(struct Rwlock* rw);

void rwlock_wrlock(struct Rwlock* rw);

void rwlock_unlock(struct Rwlock* rw);