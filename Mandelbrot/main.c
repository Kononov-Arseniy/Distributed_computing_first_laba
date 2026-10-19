#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include "my_rand.h"
#include "timer.h"

#define ITERS 1000
#define X_MIN (-2.0)
#define X_MAX ( 0.5)
#define Y_MIN (-1.25)
#define Y_MAX ( 1.25)

struct point {
    double x;
    double y;
};

long npoints;
int nthreads;
struct point* points;

bool isMandelbrot(double x, double y){
    double a = 0.0, b = 0.0, new_a = 0.0, new_b = 0.0;
    for (int i = 0; i < ITERS; i++){
        if (a*a + b*b >= 4){
            return false;
        }
        new_a = a*a-b*b+x;
        new_b = 2*a*b+y;
        a = new_a;
        b = new_b;
    }
    return true;
}


void* Thread_work(void* rank) {
    long my_rank = (long) rank;
    long base = npoints / nthreads;
    long extra = npoints % nthreads;
    long my_cnt = base + (my_rank < extra ? 1 : 0);
    long my_first = my_rank * base + (my_rank < extra ? my_rank : extra);
    unsigned local_seed = 1 + my_rank;
    long local_founded = 0;
    while (local_founded < my_cnt){
        double x = X_MIN + (X_MAX - X_MIN) * my_drand(&local_seed);
        double y = Y_MIN + (Y_MAX - Y_MIN) * my_drand(&local_seed);
        if (isMandelbrot(x, y)) {
            points[my_first+local_founded].x = x;
            points[my_first+local_founded].y = y;
            local_founded++;
        }
    }
    return NULL;
}

int main(int argc, char* argv[]){
    nthreads = strtol(argv[1], NULL, 10);
    npoints = strtol(argv[2], NULL, 10);
    points = malloc(npoints * sizeof(struct point));
    pthread_t* thread_handles = malloc(nthreads * sizeof(pthread_t));
    double start, finish;
    GET_TIME(start);
    for (long i = 0; i < nthreads; i++){
        pthread_create(&thread_handles[i], NULL, Thread_work, (void*) i);
    }
    for (long i = 0; i < nthreads; i++){
        pthread_join(thread_handles[i], NULL);
    }
    GET_TIME(finish);
    printf("%.3f\n", finish - start);
    FILE* out = fopen("mandelbrot.csv", "w");
    fprintf(out, "x,y\n");
    for (long i = 0; i < npoints; i++){
        fprintf(out, "%.17g,%.17g\n", points[i].x, points[i].y);
    }
    fclose(out);
    free(points);
    free(thread_handles);
    return 0;
}