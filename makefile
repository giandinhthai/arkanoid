CC=g++
CFLAGS=-c -Wall -I./SFML/include
LDFLAGS=-L./SFML/lib -Wl,-rpath,'$$ORIGIN/SFML/lib' -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SOURCES=arkanoid.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=arkanoid

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
run: $(EXECUTABLE)
	./$(EXECUTABLE)
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

