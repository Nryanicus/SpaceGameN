SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(OBJECTS)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) -MMD -MP $< -o $@
 
# To remove generated files
.PHONY: clean
clean:
	rm -f $(OBJECTS) vgcore.* *.d *.o

-include *.d