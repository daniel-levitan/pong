build:
	gcc -Wall -std=c99 ./src/*.c -o pong -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -L/opt/homebrew/lib -lSDL2 

run:
	./pong

clean:
	rm pong