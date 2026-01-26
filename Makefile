build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -o pong

run:
	./pong

clean:
	rm pong
