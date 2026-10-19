CC = gcc
CFLAGS = -O2 -Wall -Wextra -pthread

main: main.c my_rand.c my_rand.h timer.h
	$(CC) $(CFLAGS) main.c my_rand.c -o main

time: main
	bash check_time.sh | tee results.txt

clean:
	rm -f main mandelbrot.csv results.txt

.PHONY: time clean