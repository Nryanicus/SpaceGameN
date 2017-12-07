# export all variables, for the Makefiles we'll call in sub directories
export
CC = g++
CC_FLAGS = -Og -std=c++11 -Wall -I$(CURDIR)
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# File names
EXEC = main
DEBUG = debug
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
SUBDIRS = utilities rendering gamelogic

define GET_SUBDIR_OBJECTS
SUBDIR_OBJECTS = $(foreach dir, $(SUBDIRS), $(wildcard $(dir)/*.o))
endef

all: $(EXEC)

# fake targets
.PHONY: clean $(SUBCLEAN) subdirs $(SUBDIRS)

# Main target
$(EXEC): subdirs $(OBJECTS)
	$(MAKE) -f makemain

# subdirectories
subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

# subdir dependencies
rendering: utilities
gamelogic: utilities

# debug target
$(DEBUG): CC_FLAGS = -g -O0 -std=c++11 -Wall -I$(CURDIR)
$(DEBUG): $(EXEC) 

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) -MMD -MP $< -o $@

# To remove generated files
SUBCLEAN = $(addsuffix .clean,$(SUBDIRS))

clean: $(SUBCLEAN)
	rm -f $(EXEC) $(DEBUG) $(OBJECTS) vgcore.*

$(SUBCLEAN): %.clean:
		$(MAKE) -C $* clean

# cpp file dependencies, so we only recompile what has been changed
-include *.d