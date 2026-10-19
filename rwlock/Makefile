CC = gcc
CFLAGS = -O2 -Wall -pthread

all: my_ll lib_ll

my_ll: my_ll_rwl.c my_rwlock.c my_rwlock.h my_rand.c my_rand.h timer.h
	$(CC) $(CFLAGS) my_ll_rwl.c my_rwlock.c my_rand.c -o my_ll

lib_ll: pth_ll_rwl.c my_rand.c my_rand.h timer.h
	$(CC) $(CFLAGS) pth_ll_rwl.c my_rand.c -o lib_ll

test: my_ll_rwl.c my_rwlock.c my_rwlock.h my_rand.c my_rand.h timer.h
	$(CC) -g -O1 -fsanitize=thread -pthread my_ll_rwl.c my_rwlock.c my_rand.c -o test
	echo "200 20000 0.5 0.25" | ./test 8

bench: all
	bash bench_rwl.sh | tee results_rwl.txt

clean:
	rm -f my_ll lib_ll test results_rwl.txt

.PHONY: all test bench clean