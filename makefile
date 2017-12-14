CC=gcc
CFLAGS=-Wall
EXEC=xorcipher

SRC_DIR=src
OBJ_DIR=obj

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: clean

all: $(EXEC)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_DIR):
	mkdir -p $@

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -r $(OBJ_DIR)
