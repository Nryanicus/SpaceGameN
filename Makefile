CC = g++
CC_FLAGS = -Og -std=c++11
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
 
# File names
EXEC = main
DEBUG = debug
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
 
# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LIBS)

# debug target
$(DEBUG): CC_FLAGS = -g -O0 -std=c++11
$(DEBUG): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(DEBUG) $(LIBS)
 
# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) -MMD $< -o $@
 
# To remove generated files
clean:
	rm -f $(EXEC) $(DEBUG) $(OBJECTS) vgcore.*

-include *.d
