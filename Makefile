# Makefile for orchestrator

CC = gcc
CFLAGS = -Wall -g -Wno-unused-variable
LDFLAGS = -lm -lpthread

# List of source files
SOURCES = orchestrator.c scheduling.c network.c

# Generate the object file names from source files
OBJECTS = $(SOURCES:.c=.o)

# Target executable
TARGET = orchestrator

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

ifeq ($(CC),gcc)
    orchestrator.o: CFLAGS += -Wno-unknown-pragmas
endif

# Clean rule
clean:
	rm -f $(OBJECTS) $(TARGET)
