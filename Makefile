default:
	mkdir -p bin
	gcc -Wall -o bin/demo ./lib/status.c ./lib/sprite.c ./lib/process_events.c ./lib/update.c ./lib/draw.c ./lib/collision_detection.c main.c -Wall `sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf

test:
	./bin/demo

testfull:
	./bin/demo full