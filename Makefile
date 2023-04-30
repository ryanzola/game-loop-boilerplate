CFLAGS = -Wall -Wfatal-errors -std=c99 `pkg-config --cflags SDL2_ttf`
LDFLAGS = -framework OpenGL `pkg-config --libs SDL2_ttf`

.PHONY: build run clean

build:
	gcc $(CFLAGS) src/*.c `sdl2-config --libs --cflags` $(LDFLAGS) -lm -lpthread -o game

run:
	./game

clean:
	rm game
