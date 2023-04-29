CFLAGS = -Wall -Wfatal-errors -std=c99
LDFLAGS = -framework OpenGL

.PHONY: build run clean

build:
	gcc $(CFLAGS) src/*.c `sdl2-config --libs --cflags` $(LDFLAGS) -lm -lpthread -o game

run:
	./game

clean:
	rm game
