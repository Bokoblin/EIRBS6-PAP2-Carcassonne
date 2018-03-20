CC = gcc
CFLAGS=-c -Wall -Wextra -std=c99

SRC_DIR=src
TST_DIR=tst
INS_DIR=install

APP_SOURCES=$(wildcard $(SRC_DIR)/*.c)
APP_OBJECTS=$(APP_SOURCES:.c=.o)

#Temp until knowing what mains we'll have
APP_SOURCES_NO_MAIN=$(filter-out src/main.c, $(wildcard $(SRC_DIR)/*.c))
APP_OBJECTS_NO_MAIN=$(APP_SOURCES_NO_MAIN:.c=.o)

TEST_SOURCES=$(wildcard $(TST_DIR)/*.c)
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)

APP_EXECUTABLE=server
TEST_EXECUTABLE=tests


#######################################################
###				MAKE ALL
#######################################################

all: test
	
	
#######################################################
###				MAKE BUILD
#######################################################

.PHONY: build
build: $(APP_SOURCES) $(APP_EXECUTABLE)

$(APP_EXECUTABLE): $(APP_OBJECTS)
	$(CC) $(APP_OBJECTS) -o $@ -lm

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ -lm


#######################################################
###				MAKE TEST
#######################################################

.PHONY: test
test: $(APP_SOURCES_NO_MAIN) $(TEST_SOURCES) $(TEST_EXECUTABLE)
	@for e in $(TEST_EXECUTABLE); do \
	echo; echo $${e}; ./$${e}; \
	done

$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(APP_OBJECTS_NO_MAIN)
	$(CC) $(TEST_OBJECTS) $(APP_OBJECTS_NO_MAIN) -o $@ -lm

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ -lm


#######################################################
###				MAKE INSTALL
#######################################################

.PHONY: install
install:
	build
	mkdir -p install/server
	mkdir -p install/client
	mv server install/server
	mv *.so install/client
	rm -f $(APP_OBJECTS)


#######################################################
###				MAKE CLEAN
#######################################################

.PHONY: clean
clean:
	rm -f $(APP_OBJECTS) $(APP_EXECUTABLE)
	rm -f $(TEST_OBJECTS) $(TEST_EXECUTABLE)
	rm -rf install/*
	