build:
	gcc ./src/main.c `pkg-config --cflags --libs sdl3` -o DoomStein -lm
run :
	./DoomStein
