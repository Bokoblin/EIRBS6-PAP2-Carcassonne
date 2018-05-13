#PROJECT : EIRBS6-PAP2-ProjetCarcassonne

#######################################################
###				CONFIGURATION
#######################################################

SRC_DIR = src
TST_DIR = tst
INS_DIR = install
COV_DIR = coverage

CLI_DIR = $(SRC_DIR)/client
CLF_DIR = $(CLI_DIR)/common_clients_functions
COM_DIR = $(SRC_DIR)/common
ADT_DIR = $(COM_DIR)/ADT
SRV_DIR = $(SRC_DIR)/server

CC			= gcc
CFLAGS		= -Wall -Wextra -std=c99 -g -O0 -fPIC
CPPFLAGS	= -I ${SRC_DIR} -I ${TST_DIR} -I ${INS_DIR}
LFFLAGS		= -lm -ldl
SRVFLAGS	=

SERVER_SRC 	= $(wildcard $(COM_DIR)/*.c $(ADT_DIR)/*.c $(SRV_DIR)/*.c)
CLIENT_SRC 	= $(wildcard $(COM_DIR)/*.c $(ADT_DIR)/*.c $(CLF_DIR)/*.c)

SERVER_OBJ	= $(SERVER_SRC:%.c=%.o)
CLIENT_OBJ 	= $(CLIENT_SRC:%.c=%.o)

SERVER_EXEC	= server
TESTS_EXEC 	= test_board test_card test_client test_deck test_meeple test_queue test_set test_stack test_zone


#######################################################
###				MAKE DEFAULT COMMAND
#######################################################

.PHONY: all help prebuild build test vtest enable_coverage ctest install clean run vrun docs
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
		'\t' make ctest			'\n' \
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

build: prebuild $(SERVER_EXEC) $(CLIENT_OBJ)
	@echo building clients...
	@for client in `find $(CLI_DIR) -name "client_4410_*.c" | sed -e "s/\.c$$//" `; do \
		echo building $${client}.c; \
		$(CC) $(CFLAGS) -c $${client}.c -o $${client}.o; \
		$(CC) -shared -o $${client}.so $${client}.o $(CLIENT_OBJ); \
	done
	@echo Building complete.

$(SERVER_EXEC): $(SERVER_OBJ)
	@echo building server...
	$(CC) $(CPPFLAGS) $(SERVER_OBJ) -o $@ $(LFFLAGS) $(SRVFLAGS)


#######################################################
###				MAKE TEST
#######################################################

test: $(TESTS_EXEC)
ifneq ($(TESTS_EXEC),)
	@echo Starting tests...
	@for e in $(TESTS_EXEC); do \
		./$${e}; echo; \
	done
	@printf "\nTests complete.\n";
else
	@echo No test available
endif


#######################################################
###				MAKE TEST WITH VALGRIND
#######################################################

vtest: $(TESTS_EXEC)
ifneq ($(TESTS_EXEC),)
	@echo Starting tests...
	@for e in $(TESTS_EXEC); do \
		echo ======= $${e} =======; \
		filename=$$(echo $(TST_DIR)/$${e} | cut -d_ -f2); \
		printf "TESTED FILE:\t$$filename.c\n"; \
		valgrind --log-fd=1 ./$${e} \
		| grep "TESTS SUMMARY:\|ERROR SUMMARY:\|total heap usage:" \
		| $(VALGRIND_AWK) \
	done
	@printf "\nTests complete.\n";
else
	@echo No test available
endif

#######################################################
###				MAKE TEST WITH COVERAGE
#######################################################

enable_coverage:
	@find . -type f -name '*.o' -delete
	$(eval CFLAGS += --coverage)
	$(eval LFFLAGS += -lgcov)

ctest: enable_coverage $(TESTS_EXEC)
ifneq ($(TESTS_EXEC),)
	@mkdir -p $(COV_DIR)
	@echo Starting tests with coverage...
	@for e in $(TESTS_EXEC); do \
		filename=$$(echo $(TST_DIR)/$${e} | cut -d_ -f2); \
		echo ======= $${e} =======; \
		printf "TESTED FILE:\t$$filename.c\n"; \
		./$${e} | grep "TESTS SUMMARY:"; \
		printf "COVERAGE:\t"; \
		gcov $$(find -name $$filename.o) 2>/dev/null | grep "Lines" | cut -f 2 -d ':'; echo;\
		mv -f $$filename.c.gcov -t $(COV_DIR)/ 2>/dev/null; \
	done
	@find . -type f -name '*.o' -delete
	@printf "\nTests complete.\n";
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
	@mv $(SERVER_EXEC) $(INS_DIR)
	@mv $(CLI_DIR)/*.so $(INS_DIR)
	@rm -rf $(CLIENT_OBJ)
	@rm -rf $(SERVER_OBJ)
	@echo Installation complete.


#######################################################
###				MAKE CLEAN
#######################################################

clean:
	@echo Starting cleanup...
	@find . -type f -name '*.o' -delete
	@find . -type f -name '*.so' -delete
	@find . -type f -name '*.gcda' -delete
	@find . -type f -name '*.gcno' -delete
	@find . -maxdepth 1 -type f -name '*.gcov' -delete
	@rm -rf $(COV_DIR)
	@rm -rf $(TESTS_EXEC)
	@rm -rf $(TESTS_EXEC)
	@rm -rf $(INS_DIR)/*
	@echo Cleanup complete.


#######################################################
###				MAKE RUN
#######################################################

run:
	@echo Running program...
	@./$(INS_DIR)/$(SERVER_EXEC) ./$(INS_DIR)/*.so


#######################################################
###				MAKE RUN WITH VALGRING
#######################################################

vrun:
	@echo Running program...
	@valgrind ./$(INS_DIR)/$(SERVER_EXEC) ./$(INS_DIR)/*.so


#######################################################
###				MAKE DOCUMENTATION
#######################################################

docs:
	doxygen Doxyfile


#######################################################
###				TEST EXECUTABLES
#######################################################

test_board: $(TST_DIR)/test_board.o $(TST_DIR)/common_tests_utils.o $(COM_DIR)/utils.o $(SRV_DIR)/board.o \
			$(COM_DIR)/card.o $(COM_DIR)/meeple.o $(COM_DIR)/deck.o $(COM_DIR)/card_type.o $(COM_DIR)/interface.o \
			$(ADT_DIR)/set.o $(ADT_DIR)/stack.o $(ADT_DIR)/queue.o
	${CC} $(CPPFLAGS) $^ -o $@ $(LFFLAGS)

test_card: 	$(TST_DIR)/test_card.o $(TST_DIR)/common_tests_utils.o $(COM_DIR)/utils.o \
			$(COM_DIR)/card.o $(COM_DIR)/card_type.o $(COM_DIR)/interface.o $(ADT_DIR)/stack.o
	${CC} $(CPPFLAGS) $^ -o $@ $(LFFLAGS)

test_client: $(TST_DIR)/test_client.o $(TST_DIR)/common_tests_utils.o $(COM_DIR)/utils.o \
			$(COM_DIR)/card.o $(COM_DIR)/card_type.o $(COM_DIR)/interface.o $(CLF_DIR)/micro_board.o \
			$(CLF_DIR)/client.o $(ADT_DIR)/set.o
	${CC} $(CPPFLAGS) $^ -o $@ $(LFFLAGS)

test_deck: 	$(TST_DIR)/test_deck.o $(TST_DIR)/common_tests_utils.o $(COM_DIR)/utils.o \
			$(COM_DIR)/card.o $(COM_DIR)/card_type.o $(COM_DIR)/deck.o $(COM_DIR)/interface.o $(ADT_DIR)/stack.o
	${CC} $(CPPFLAGS) $^ -o $@ $(LFFLAGS)

test_meeple: $(TST_DIR)/test_meeple.o $(TST_DIR)/common_tests_utils.o $(COM_DIR)/utils.o \
			$(COM_DIR)/meeple.o $(COM_DIR)/card.o $(COM_DIR)/card_type.o $(COM_DIR)/interface.o $(ADT_DIR)/stack.o
	${CC} $(CPPFLAGS) $^ -o $@ $(LFFLAGS)

test_queue:	$(TST_DIR)/test_queue.o $(TST_DIR)/common_tests_utils.o $(COM_DIR)/utils.o $(ADT_DIR)/queue.o
	${CC} $(CPPFLAGS) $^ -o $@ $(LFFLAGS)

test_set:	$(TST_DIR)/test_set.o $(TST_DIR)/common_tests_utils.o $(COM_DIR)/utils.o $(ADT_DIR)/set.o
	${CC} $(CPPFLAGS) $^ -o $@ $(LFFLAGS)

test_stack:	$(TST_DIR)/test_stack.o $(TST_DIR)/common_tests_utils.o $(COM_DIR)/utils.o $(ADT_DIR)/stack.o
	${CC} $(CPPFLAGS) $^ -o $@ $(LFFLAGS)

test_zone: 	$(TST_DIR)/test_zone.o $(TST_DIR)/common_tests_utils.o $(COM_DIR)/utils.o $(COM_DIR)/interface.o \
			$(COM_DIR)/area_type.o $(COM_DIR)/card.o $(COM_DIR)/card_type.o $(COM_DIR)/meeple.o $(COM_DIR)/zone.o \
			$(ADT_DIR)/set.o
	${CC} $(CPPFLAGS) $^ -o $@ $(LFFLAGS)


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
	if (match( $$0, /TESTS.*/)) \
		printf "%s\n", $$0; \
	else \
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
			else if (match($$i, /total/)) \
				printf ""; \
			else if (match($$i, /heap/)) \
				printf "HEAP "; \
			else if (match($$i, /usage:/)) \
				printf "USAGE: \t"; \
			else if (match($$i, /frees\,/)) \
				{printf "frees", $$i; break;}\
			else if (match($$i, /contexts/)) \
				{printf "%s", $$i; break;}\
			else \
				printf "%s ", $$i; \
			} \
	}'; \
echo; echo;
