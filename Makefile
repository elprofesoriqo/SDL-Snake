CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lSDL2 -lpthread -ldl -lrt
SOURCES=main.cpp game.cpp snake.cpp food.cpp renderer.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=snake

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)