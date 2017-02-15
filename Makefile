CC		=g++
CFLAGS	=-Wall -g
EXEC	=ptrace
FILES	=./PixelTracer.cpp ./Board.cpp
LINK	=-lSDL
SDL		=`pkg-config --cflags --libs sdl`

all:
	$(CC) $(CFLAGS) -o $(EXEC) $(FILES) -I./ $(SDL) $(LINK)
