CC=gcc
CFLAGS=-c -Wall -Wextra -std=c99

SRC_DIR=src
TST_DIR=tst
INS_DIR=install

SERVER_SOURCES=$(wildcard $(SRC_DIR)/server/*.c)
SERVER_OBJECTS=$(SERVER_SOURCES:.c=.o)
CLIENT_SOURCES=$(wildcard $(SRC_DIR)/client/*.c)
CLIENT_OBJECTS=$(CLIENT_SOURCES:.c=.o)

TEST_SOURCES=$(wildcard $(TST_DIR)/*.c $(SRC_DIR)/common/*.c)
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)

SERVER_EXECUTABLE=server
TEST_EXECUTABLE=tests


#######################################################
###				MAKE DEFAULT COMMAND
#######################################################

all: help


#######################################################
###				MAKE INSTRUCTIONS / HELP
#######################################################

.PHONY: help
help:
	@echo -e Available commands:'\n' \
		'\t' make help			'\n' \
		'\t' make build			'\n' \
		'\t' make test			'\n' \
		'\t' make install		'\n' \
		'\t' make clean			'\n' \
		'\t' make run


#######################################################
###				MAKE BUILD
#######################################################

.PHONY: prebuild
prebuild:
	@echo Starting building...

.PHONY: build
build: prebuild $(SERVER_EXECUTABLE)
	@echo building clients...
	@for client in `find src/client -name "*.c" | sed -e "s/\.c$$//" `; do \
		echo building $${client}.c; \
		$(CC) $(CFLAGS) -c -fPIC $${client}.c -o $${client}.o -lm; \
		$(CC) -shared -o $${client}.so $${client}.o -ldl; \
	done
	@echo Building complete.

utils.o: $(SRC_DIR)/common/utils.c $(SRC_DIR)/common/utils.h
	${CC} ${CFLAGS} $(SRC_DIR)/common/utils.c -c

server.o: $(SRC_DIR)/server/server.c
	${CC} ${CFLAGS} $(SRC_DIR)/server/server.c -c

client_container.o: $(SRC_DIR)/server/client_container.c $(SRC_DIR)/server/client_container.h
	${CC} ${CFLAGS} $(SRC_DIR)/server/client_container.c -c

stack.o: $(SRC_DIR)/common/stack.c $(SRC_DIR)/common/stack.h
	${CC} ${CFLAGS} $(SRC_DIR)/common/stack.c -c

board.o: $(SRC_DIR)/common/board.c $(SRC_DIR)/common/board.h
	${CC} ${CFLAGS} $(SRC_DIR)/common/board.c -c

card.o: $(SRC_DIR)/common/card.c $(SRC_DIR)/common/card.h
	${CC} ${CFLAGS} $(SRC_DIR)/common/card.c -c

$(SERVER_EXECUTABLE): server.o client_container.o utils.o
	@echo building server...
	$(CC) server.o utils.o client_container.o -o $@ -ldl


#######################################################
###				MAKE TEST
#######################################################

.PHONY: test
test: $(TEST_EXECUTABLE)
ifneq ($(TEST_OBJECTS),)
	@echo Starting tests...
	@for e in $(TEST_EXECUTABLE); do \
		echo; echo $${e}; ./$${e}; \
	done
	@echo Tests complete.
endif

$(TEST_EXECUTABLE): $(TEST_OBJECTS)
ifeq ($(TEST_OBJECTS),)
	@echo No test available
else
	@echo Building tests...
	$(CC) $(TEST_OBJECTS) -o $@ -lm -ldl
	@echo Building tests complete.
endif

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ -lm


#######################################################
###				MAKE INSTALL
#######################################################

.PHONY: install
install: build
	@echo
	@echo Starting installation...
	@echo Installation directory: "${PWD}/$(INS_DIR)"
	@mv server $(INS_DIR)
	@mv $(SRC_DIR)/client/*.so $(INS_DIR)
	@rm -rf $(CLIENT_OBJECTS)
	@rm -rf $(SERVER_OBJECTS)
	@echo Installation complete.


#######################################################
###				MAKE CLEAN
#######################################################

.PHONY: clean
clean:
	@echo Starting cleanup...
	@find . -type f -name '*.o' -delete
	@find . -type f -name '*.so' -delete
	@rm -rf $(SERVER_EXECUTABLE)
	@rm -rf $(TEST_EXECUTABLE)
	@rm -rf install/*
	@echo Cleanup complete.


#######################################################
###				MAKE RUN
#######################################################

.PHONY: run
run:
	@echo Running program...
	./install/server ./install/*.so
