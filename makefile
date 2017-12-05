CC=gcc
CFLAGS=-Wall
EXEC=xorcipher

SRC_DIR=src
OBJ_DIR=obj

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(EXEC)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)


$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ_DIR)/*.o
