test:
	./bin/demo

testfull:
	./bin/demo full

build:
	mkdir -p bin
	gcc -Wall -o bin/demo main.c status.c sprite.c process_events.c -Wall `sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf