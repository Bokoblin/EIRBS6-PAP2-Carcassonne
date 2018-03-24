CC=gcc
CFLAGS=-c -Wall -Wextra -std=c99

SRC_DIR=src
TST_DIR=tst
INS_DIR=install

SERVER_SOURCES=$(wildcard $(SRC_DIR)/server/*.c)
SERVER_OBJECTS=$(SERVER_SOURCES:.c=.o)
CLIENT_SOURCES=$(wildcard $(SRC_DIR)/client/*.c)
CLIENT_OBJECTS=$(CLIENT_SOURCES:.c=.o)

TEST_SOURCES=$(wildcard $(TST_DIR)/*.c)
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)

SERVER_EXECUTABLE=server
TEST_EXECUTABLE=tests


#######################################################
###				MAKE INSTRUCTIONS
#######################################################

all:
	@echo Available commands:
	@echo -e '\t' make build
	@echo -e '\t' make test
	@echo -e '\t' make install
	@echo -e '\t' make clean


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

$(SERVER_OBJECTS): $(SERVER_SOURCES)
	$(CC) -c $(CFLAGS) $< -o $@

$(SERVER_EXECUTABLE): $(SERVER_OBJECTS)
	@echo building server...
	$(CC) $(SERVER_OBJECTS) -o $@ -ldl



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
	$(CC) $(TEST_OBJECTS) -o $@ -lm
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
	@mkdir -p $(INS_DIR)/server
	@mkdir -p $(INS_DIR)/client
	@mv server $(INS_DIR)/server
	@mv $(SRC_DIR)/client/*.so $(INS_DIR)/client
	@rm -rf $(CLIENT_OBJECTS)
	@rm -rf $(SERVER_OBJECTS)
	@echo Installation complete.


#######################################################
###				MAKE CLEAN
#######################################################

.PHONY: clean
clean:
	@echo Starting cleanup...
	@rm -rf *.o
	@rm -rf *.so
	@rm -rf $(SOURCE_OBJECTS)
	@rm -rf $(SERVER_OBJECTS)
	@rm -rf $(SERVER_EXECUTABLE)
	@rm -rf install/*
	@echo Cleanup complete.
