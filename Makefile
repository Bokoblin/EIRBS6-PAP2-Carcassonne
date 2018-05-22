#PROJECT : EIRBS6-PAP2-ProjetCarcassonne -- Main Makefile

#######################################################
###				CONFIGURATION
#######################################################

SRC_DIR = src
TST_DIR = tst
INS_DIR = install

CLI_DIR = $(SRC_DIR)/client
CLF_DIR = $(CLI_DIR)/common_clients_functions
COM_DIR = $(SRC_DIR)/common
ADT_DIR = $(COM_DIR)/ADT
SRV_DIR = $(SRC_DIR)/server
GUI_DIR = $(SRC_DIR)/ui

CC			= gcc
CFLAGS		= -Wall -Wextra -std=c99 -g -O0 -fPIC
CPPFLAGS	= -I ${SRC_DIR} -I ${TST_DIR}
LFFLAGS		= -ldl
SRVFLAGS	=

COMMON_SRC	= $(wildcard $(COM_DIR)/*.c $(ADT_DIR)/*.c)
SERVER_SRC	= $(wildcard $(SRV_DIR)/*.c)
GUI_SRC		= $(wildcard $(GUI_DIR)/*.c)
CLIENT_SRC 	= $(wildcard $(CLF_DIR)/*.c)

COMMON_OBJ	= $(COMMON_SRC:%.c=%.o)
SERVER_OBJ	= $(SERVER_SRC:%.c=%.o)
GUI_OBJ		= $(GUI_SRC:%.c=%.o)
CLIENT_OBJ 	= $(CLIENT_SRC:%.c=%.o)

SERVER_EXEC	= server


#######################################################
###				COMMANDS CATEGORIES
#######################################################

# Accessible commands
.PHONY: all help build install run vrun grun test vtest ctest docs clean

# Helper commands
.PHONY: --prebuild --check_sdl --enable_sdl __build_with_sdl __build_without_sdl

#######################################################
###				DEFAULT MAKE COMMAND
#######################################################

all: build


#######################################################
###				MAKE INSTRUCTIONS / HELP
#######################################################

help:
	@echo -e \
	======================================================================	'\n'\
		'\t' ENSEIRB S6 : Project Carcassonne \(2018\) by Team 4410			'\n'\
	======================================================================	'\n'\
	'\n'Available commands:													'\n'\
		'\t' make help		'\t' Show the availables commands				'\n'\
		'\t' make build		'\t' Build the server and clients				'\n'\
		'\t' make install	'\t' Install the binairies						'\n'\
		'\t' make run		'\t' Run the server with the clients			'\n'\
		'\t' make vrun		'\t' Run with memory check 						'\n'\
		'\t' make grun		'\t' Run in GUI mode							'\n'\
		'\t' make test		'\t' Execute all the tests						'\n'\
		'\t' make vtest		'\t' Execute all the tests with memory check 	'\n'\
		'\t' make ctest		'\t' Execute all the tests with a coverage		'\n'\
		'\t' make docs		'\t' Generate the documentation					'\n'\
		'\t' make clean 	'\t' Clean all generated objects and binairies	'\n'


#######################################################
###				MAKE BUILD
#######################################################

--prebuild:
	@echo Starting building...
	@rm -f $(SRV_DIR)/server.o

--check_sdl:
	@echo Checking computer configuration to enable SDL2...
	@if hash "sdl2-config" 2>/dev/null; \
	then \
		echo "SDL2 is installed, enabling SDL2..."; \
		$(MAKE) --no-print-directory __build_with_sdl; \
	else \
		echo "SDL2 isn't installed, moving to console-only building..."; \
		$(MAKE) --no-print-directory __build_without_sdl; \
	fi

--enable_sdl:
	$(eval CFLAGS += `sdl2-config --cflags`)
	$(eval CPPFLAGS += -DUSE_SDL)
	$(eval SRVFLAGS += -lSDL2 -lSDL2_image -lSDL2_ttf `sdl2-config --libs`)
	$(eval SERVER_OBJ += $(GUI_OBJ))

build: --prebuild $(COMMON_OBJ) $(CLIENT_OBJ) --check_sdl

__build_with_sdl: --enable_sdl $(GUI_OBJ) --build_end
__build_without_sdl: --build_end

--build_end: $(SERVER_OBJ)
	@echo building server...
	$(CC) $(CPPFLAGS) $(COMMON_OBJ) $(SERVER_OBJ) -o $(SERVER_EXEC) $(LFFLAGS) $(SRVFLAGS)
	@echo building clients...
	@for client in `find $(CLI_DIR) -name "client_4410_*.c" | sed -e "s/\.c$$//" `; do \
		echo building $${client}.c; \
		$(CC) $(CFLAGS) -c $${client}.c -o $${client}.o; \
		$(CC) -shared -o $${client}.so $${client}.o $(COMMON_OBJ) $(CLIENT_OBJ); \
	done
	@echo Building complete.


#######################################################
###				MAKE INSTALL
#######################################################

install: build
	@echo
	@echo Starting installation...
	@mkdir -p install
	@echo Installation directory: "${PWD}/$(INS_DIR)"
	@mv $(SERVER_EXEC) $(INS_DIR)
	@mv $(CLI_DIR)/*.so $(INS_DIR)
	@rm -rf $(COMMON_OBJ)
	@rm -rf $(CLIENT_OBJ)
	@rm -rf $(SERVER_OBJ)
	@echo Installation complete.


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
	@echo Running program with valgrind...
	@valgrind ./$(INS_DIR)/$(SERVER_EXEC) ./$(INS_DIR)/*.so


#######################################################
###				MAKE RUN IN GRAPHIC MODE
#######################################################

grun:
	@echo Running program in graphic mode...
	@./$(INS_DIR)/$(SERVER_EXEC) -g ./$(INS_DIR)/*.so


#######################################################
###				MAKE TEST
#######################################################

test: 
	@echo Starting the tests
	@$(MAKE) --no-print-directory -C $(TST_DIR) test


#######################################################
###				MAKE TEST WITH VALGRIND
#######################################################

vtest: 
	@echo Starting the tests with memory check
	@$(MAKE) --no-print-directory -C $(TST_DIR) vtest


#######################################################
###				MAKE TEST WITH COVERAGE
#######################################################

ctest: 
	@echo Starting the tests with coverage
	@$(MAKE) --no-print-directory -C $(TST_DIR) ctest


#######################################################
###				MAKE DOCUMENTATION
#######################################################

docs:
	doxygen Doxyfile


#######################################################
###				MAKE CLEAN
#######################################################

clean:
	@echo Starting cleanup...
	@find . -type f -name '*.o' -delete
	@find . -type f -name '*.so' -delete
	@rm -rf $(INS_DIR)/*
	@$(MAKE) --no-print-directory -C $(TST_DIR) clean
	@echo Cleanup complete.


#######################################################
###				OBJECTS FILES
#######################################################

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

