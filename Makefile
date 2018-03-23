CC=gcc
CFLAGS=-c -Wall -Wextra -std=c99

SRC_DIR=src
TST_DIR=tst
INS_DIR=install

SERVER_SOURCES=$(wildcard $(SRC_DIR)/server/*.c)
SERVER_OBJECTS=$(SERVER_SOURCES:.c=.o)
CLIENT_SOURCES=$(wildcard $(SRC_DIR)/client/*.c)
CLIENT_OBJECTS=$(CLIENT_SOURCES:.c=.o)

#Temp until knowing what mains we'll have
APP_SOURCES_NO_MAIN=$(filter-out src/main.c, $(wildcard $(SRC_DIR)/*.c))
APP_OBJECTS_NO_MAIN=$(APP_SOURCES_NO_MAIN:.c=.o)

TEST_SOURCES=$(wildcard $(TST_DIR)/*.c)
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)

SERVER_EXECUTABLE=server
TEST_EXECUTABLE=tests


#######################################################
###				MAKE ALL
#######################################################

all: test
	
	
#######################################################
###				MAKE BUILD
#######################################################

.PHONY: build
build: $(SERVER_EXECUTABLE)
	@for client in `find src/client -name "*.c"`; do \
		echo building $${client} library; \
		$(CC) $(CFLAGS) -c -fPIC $${client} -o $${client}.o -lm; \
		$(CC) -shared -o $${client}.so $${client}.o -ldl; \
	done

$(SERVER_EXECUTABLE): $(SERVER_OBJECTS)
	$(CC) $(CFLAGS) $(SERVER_SOURCES) -o $@ -lm; \

$(SERVER_OBJECTS): $(SERVER_SOURCES)
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
	make build
	mkdir -p install/server
	mkdir -p install/client
	mv server install/server
	mv $(SRC_DIR)/client/*.so install/client
	rm -f $(SOURCE_OBJECTS)
	rm -f $(SERVER_OBJECTS)


#######################################################
###				MAKE CLEAN
#######################################################

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf *.so
	rm -rf $(SOURCE_OBJECTS)
	rm -rf $(SERVER_OBJECTS)
	rm -rf $(SERVER_EXECUTABLE)
	rm -rf install/*
	