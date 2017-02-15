CC		=g++
CFLAGS	=-Wall -g
EXEC	=ptrace
FILES	=./PixelTracer.cpp ./Board.cpp
LINK	=-lSDL
SDL		=`pkg-config --cflags --libs sdl`

all:
	$(CC) $(CFLAGS) $(FILES) $(SDL) -o $(EXEC) -I./ $(LINK)

clean:
	rm $(EXEC)
