#Makefile modified from http://mrbook.org/tutorials/make/
CC=cc 
CFLAGS=-c -g -Wall
LDFLAGS=
SOURCES=test_cic.c cic.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=test_cic

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)