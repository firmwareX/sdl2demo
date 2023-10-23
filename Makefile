default:
	mkdir -p bin
	gcc -Wall -o bin/demo status.c sprite.c process_events.c update.c draw.c main.c collision_detection.c -Wall `sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf

test:
	./bin/demo

testfull:
	./bin/demo full