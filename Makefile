#PROJECT : EIRBS6-PAP2-ProjetCarcassonne

SRC_DIR = src
TST_DIR = tst
INS_DIR = install

CC 		= gcc
CFLAGS	= -Wall -Wextra -std=c99 -g -O0
CPPFLAGS= -I ${SRC_DIR} -I ${TST_DIR} -I ${INS_DIR}

SERVER_SOURCES 	= $(wildcard $(SRC_DIR)/server/*.c $(SRC_DIR)/common/*.c)
CLIENT_SOURCES 	= $(wildcard $(SRC_DIR)/client/*.c)
TESTS_SOURCES 	= $(wildcard $(TST_DIR)/*.c $(SRC_DIR)/common/*.c)

SERVER_OBJECTS	= $(SERVER_SOURCES:%.c=%.o)
CLIENT_OBJECTS 	= $(CLIENT_SOURCES:%.c=%.o)
TESTS_OBJECTS 	= $(TESTS_SOURCES:%.c=%.o)

SERVER_EXECUTABLE 	= server
TESTS_EXECUTABLE 	= test_board test_card test_deck test_set test_stack test_queue


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
		'\t' make memory		'\n' \
		'\t' make install		'\n' \
		'\t' make clean			'\n' \
		'\t' make run			'\n' \
		'\t' make docs


#######################################################
###				MAKE BUILD
#######################################################

.PHONY: prebuild
prebuild:
	@echo Starting building...

.PHONY: build
build: prebuild $(SERVER_EXECUTABLE)
	@echo building clients...
	@for client in `find src/client -name "client*.c" | sed -e "s/\.c$$//" `; do \
		echo building $${client}.c; \
		$(CC) $(CFLAGS) -c -fPIC $${client}.c -o $${client}.o -lm; \
		$(CC) -shared -o $${client}.so $${client}.o -ldl; \
	done
	@echo Building complete.

$(SERVER_EXECUTABLE): $(SERVER_OBJECTS)
	@echo building server...
	$(CC) $(CPPFLAGS) $(SERVER_OBJECTS) -o $@ -ldl


#######################################################
###				MAKE TEST
#######################################################

.PHONY: test
test: $(TESTS_EXECUTABLE)
ifneq ($(TESTS_EXECUTABLE),)
	@echo Starting tests...
	@for e in $(TESTS_EXECUTABLE); do \
		./$${e}; echo; \
	done
	@echo Tests complete.
else
	@echo No test available
endif

test_board: test_board.o board.o utils.o card.o stack.o
	${CC} $(CPPFLAGS) test_board.o board.o utils.o card.o stack.o -o $@ -lm -ldl

test_card: test_card.o card.o utils.o stack.o
	${CC} $(CPPFLAGS) test_card.o card.o utils.o stack.o -o $@ -lm -ldl

test_deck: test_deck.o deck.o utils.o stack.o
	${CC} $(CPPFLAGS) test_deck.o deck.o utils.o stack.o -o $@ -lm -ldl

test_set: test_set.o set.o utils.o
	${CC} $(CPPFLAGS) test_set.o set.o utils.o -o $@ -lm -ldl

test_stack: test_stack.o stack.o utils.o
	${CC} $(CPPFLAGS) test_stack.o stack.o utils.o -o $@ -lm -ldl

test_queue: test_queue.o queue.o utils.o
	${CC} $(CPPFLAGS) test_queue.o queue.o utils.o -o $@ -lm -ldl


#######################################################
###				MAKE TEST MEMORY CHECK
#######################################################

.PHONY: memory
memory: $(TESTS_EXECUTABLE)
ifneq ($(TESTS_EXECUTABLE),)
	@echo Starting tests...
	@for e in $(TESTS_EXECUTABLE); do \
		echo =====  $${e} =====; \
		valgrind --log-fd=1 ./$${e} \
		| grep "ERROR SUMMARY:\|total heap usage:" \
		| awk '{	\
				for(i=2;i<=NF;i++) { \
					if (match($$((i+1)), /allocs/) && $$i > $$((i+2))) \
                        printf "\x1B[31m%s \x1b[0m", $$i; \
					else if (match($$i, /[1-9]+$$/) && match($$((i+1)), /errors/)) \
                        printf "\x1B[31m%s \x1b[0m", $$i; \
					else if (match($$i, /[1-9]+$$/) && match($$((i+1)), /contexts/)) \
                        printf "\x1B[31m%s \x1b[0m", $$i; \
					else if (match($$i, /[0-9]+$$/)) \
                    	printf "\x1B[32m%s \x1b[0m", $$i; \
                    else if (match($$i, /ERROR/)) \
                    	printf "\n%s ", $$i; \
                    else \
                    	printf "%s ", $$i; \
				} \
				}'; \
		echo; echo; \
	done
	@echo Tests complete.
else
	@echo No test available
endif


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
	@rm -rf $(TESTS_EXECUTABLE)
	@rm -rf install/*
	@echo Cleanup complete.


#######################################################
###				MAKE RUN
#######################################################

.PHONY: run
run:
	@echo Running program...
	./install/server ./install/*.so


#######################################################
###				MAKE DOCUMENTATION
#######################################################

.PHONY: docs
docs:
	doxygen Doxyfile


#######################################################
###				OBJECTS FILES
#######################################################

board.o: $(SRC_DIR)/common/board.c $(SRC_DIR)/common/board.h
	${CC} ${CFLAGS} $(SRC_DIR)/common/board.c -c

card.o: $(SRC_DIR)/common/card.c $(SRC_DIR)/common/card.h
	${CC} ${CFLAGS} $(SRC_DIR)/common/card.c -c

deck.o: $(SRC_DIR)/common/deck.c $(SRC_DIR)/common/deck.h
	${CC} ${CFLAGS} $(SRC_DIR)/common/deck.c -c

player.o: $(SRC_DIR)/server/player.c $(SRC_DIR)/server/player.h
	${CC} ${CFLAGS} $(SRC_DIR)/server/player.c -c

moves.o: $(SRC_DIR)/client/moves.c $(SRC_DIR)/client/moves.h
	${CC} ${CFLAGS} $(SRC_DIR)/client/moves.c -c

server.o: $(SRC_DIR)/server/server.c $(SRC_DIR)/server/function_pointer.h
	${CC} ${CFLAGS} $(SRC_DIR)/server/server.c -c

stack.o: $(SRC_DIR)/common/stack.c $(SRC_DIR)/common/stack.h
	${CC} ${CFLAGS} $(SRC_DIR)/common/stack.c -c

set.o: $(SRC_DIR)/common/set.c $(SRC_DIR)/common/set.h
	${CC} ${CFLAGS} $(SRC_DIR)/common/set.c -c

queue.o: $(SRC_DIR)/common/queue.c $(SRC_DIR)/common/queue.h
	${CC} ${CFLAGS} $(SRC_DIR)/common/queue.c -c

utils.o: $(SRC_DIR)/common/utils.c $(SRC_DIR)/common/utils.h
	${CC} ${CFLAGS} $(SRC_DIR)/common/utils.c -c

test_board.o: $(TST_DIR)/test_board.c
	${CC} ${CFLAGS} $(TST_DIR)/test_board.c -c

test_card.o: $(TST_DIR)/test_card.c
	${CC} ${CFLAGS} $(TST_DIR)/test_card.c -c

test_deck.o: $(TST_DIR)/test_deck.c
	${CC} ${CFLAGS} $(TST_DIR)/test_deck.c -c

test_queue.o: $(TST_DIR)/test_queue.c
	${CC} ${CFLAGS} $(TST_DIR)/test_queue.c -c

test_set.o: $(TST_DIR)/test_set.c
	${CC} ${CFLAGS} $(TST_DIR)/test_set.c -c

test_stack.o: $(TST_DIR)/test_stack.c
	${CC} ${CFLAGS} $(TST_DIR)/test_stack.c -c
