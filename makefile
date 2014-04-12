#Makefile modified from http://mrbook.org/tutorials/make/
CC=cc 
CFLAGS=-c -g -Wall
LDFLAGS=
SOURCES=test_cic.c cic.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=test_cic
TEST_DATA=test_temp
TEST_RESULTS=test_results

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
	rm -rf $(TEST_DATA) $(TEST_RESULTS)

test: all plot_cic.py plot_freq_response.py 
	mkdir -p $(TEST_DATA) $(TEST_RESULTS)
	python ./plot_cic.py
	python ./plot_freq_response.py