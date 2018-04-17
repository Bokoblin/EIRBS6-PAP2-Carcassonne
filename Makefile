#PROJECT : EIRBS6-PAP2-ProjetCarcassonne

#######################################################
###				CONFIGURATION
#######################################################

SRC_DIR = src
TST_DIR = tst
INS_DIR = install

CLI_DIR = $(SRC_DIR)/client
COM_DIR = $(SRC_DIR)/common
ADT_DIR = $(COM_DIR)/ADT
SRV_DIR = $(SRC_DIR)/server

CC 		= gcc
CFLAGS	= -Wall -Wextra -std=c99 -g -O0
CPPFLAGS= -I ${SRC_DIR} -I ${TST_DIR} -I ${INS_DIR}

SERVER_SOURCES 	= $(wildcard $(SRC_DIR)/server/*.c $(SRC_DIR)/common/*.c $(SRC_DIR)/common/ADT/*.c)
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

.PHONY: all help prebuild build test vtest install clean run vrun docs
all: help


#######################################################
###				MAKE INSTRUCTIONS / HELP
#######################################################

help:
	@echo -e Available commands:'\n' \
		'\t' make help			'\n' \
		'\t' make build			'\n' \
		'\t' make test			'\n' \
		'\t' make vtest			'\n' \
		'\t' make install		'\n' \
		'\t' make clean			'\n' \
		'\t' make run			'\n' \
		'\t' make vrun			'\n' \
		'\t' make docs			'\n' \
								'\n' \
		Commands details can be found in the README


#######################################################
###				MAKE BUILD
#######################################################

prebuild:
	@echo Starting building...

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


#######################################################
###				MAKE TEST WITH VALGRIND
#######################################################

vtest: $(TESTS_EXECUTABLE)
ifneq ($(TESTS_EXECUTABLE),)
	@echo Starting tests...
	@for e in $(TESTS_EXECUTABLE); do \
		echo =====  $${e} =====; \
		valgrind --log-fd=1 ./$${e} \
		| grep "ERROR SUMMARY:\|total heap usage:" \
		| $(VALGRIND_AWK) \
	done
	@echo Tests complete.
else
	@echo No test available
endif


#######################################################
###				MAKE INSTALL
#######################################################

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

run:
	@echo Running program...
	@./install/server ./install/*.so


#######################################################
###				MAKE RUN WITH VALGRING
#######################################################

vrun:
	@echo Running program...
	@valgrind ./install/server ./install/*.so


#######################################################
###				MAKE DOCUMENTATION
#######################################################

docs:
	doxygen Doxyfile


#######################################################
###				TEST EXECUTABLES
#######################################################

test_board: $(SRV_DIR)/board.o $(COM_DIR)/card.o $(COM_DIR)/card_type.o $(SRV_DIR)/function_pointers.o \
			$(SRV_DIR)/player.o $(ADT_DIR)/set.o $(ADT_DIR)/stack.o $(COM_DIR)/utils.o $(TST_DIR)/test_board.o
	${CC} $(CPPFLAGS) $^ -o $@ -lm -ldl

test_card: $(COM_DIR)/card.o $(COM_DIR)/card_type.o $(ADT_DIR)/stack.o $(COM_DIR)/utils.o $(TST_DIR)/test_card.o
	${CC} $(CPPFLAGS) $^ -o $@ -lm -ldl

test_deck: $(COM_DIR)/deck.o $(ADT_DIR)/stack.o $(COM_DIR)/utils.o $(TST_DIR)/test_deck.o
	${CC} $(CPPFLAGS) $^ -o $@ -lm -ldl

test_queue: $(ADT_DIR)/queue.o $(COM_DIR)/utils.o $(TST_DIR)/test_queue.o
	${CC} $(CPPFLAGS) $^ -o $@ -lm -ldl

test_set: $(ADT_DIR)/set.o $(COM_DIR)/utils.o $(TST_DIR)/test_set.o
	${CC} $(CPPFLAGS) $^ -o $@ -lm -ldl

test_stack: $(ADT_DIR)/stack.o $(COM_DIR)/utils.o $(TST_DIR)/test_stack.o
	${CC} $(CPPFLAGS) $^ -o $@ -lm -ldl


#######################################################
###				OBJECTS FILES
#######################################################

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@


#######################################################
###				EXTRAS
#######################################################

VALGRIND_AWK = \
awk '{	\
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
echo; echo;
