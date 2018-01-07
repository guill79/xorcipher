# -- Commandes --
# make       : compilation du programme principal (xorcipher)
# make test  : compilation des tests unitaires (test)
# make clean : suppression du dossier obj et des exécutables

CC=gcc
CFLAGS=-Wall -DNDEBUG -Ofast
EXEC=xorcipher
EXEC_TESTS=test

SRC_DIR=src
OBJ_DIR=obj

# Récupération des fichiers sources
SRCS=$(wildcard $(SRC_DIR)/*.c)
# Récupération des fichiers .o
OBJS=$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# Récupération des fichiers .o de test (c-à-d avec unit_tests.o au lieu de main.o)
TESTS=$(OBJS:$(OBJ_DIR)/main.o=$(OBJ_DIR)/unit_tests.o)

.PHONY: all test clean

all: $(EXEC)

# Création des fichiers .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -o $@ -c $< $(CFLAGS)

# Création du dossier obj s'il n'existe pas
$(OBJ_DIR):
	mkdir -p $@

# Création de l'exécutable principal
$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Création du fichier .o de tests unitaires
$(OBJ_DIR)/unit_tests.o: tests/modules/unit_tests.c
	$(CC) -o $@ -c $<

# Création de l'exécutable de tests unitaires
test: $(TESTS)
	$(CC) -o $(EXEC_TESTS) $^

# Nettoyage des fichiers .o et exécutables
clean:
	rm -rf $(OBJ_DIR) $(EXEC) $(EXEC_TESTS)
