CC = gcc
CFLAGS = -g -O2
LDLIBS = -lSDL2 -lSDL2_ttf -lm 
layerlife: main.c $(wildcard *.h)
	$(CC) $(CFLAGS) main.c $(LDLIBS) -o $@
