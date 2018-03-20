BIN=...


CC=gcc
CPPFLAGS=-I ${SET_DIR} -I ${SEN_DIR} -I ${DYN_DIR} -I ${LNK_DIR}
CFLAGS=-Wall -Wextra -std=c99

build: ${BIN}
