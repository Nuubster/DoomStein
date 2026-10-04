build:
	gcc ./src/main.c ./src/map.c ./src/render.c -I./include `pkg-config --cflags --libs sdl3` -o DoomStein -lm
run :
	./DoomStein
