#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../inc/utest.h"

static char **modules = NULL;  // Nom des modules
static int *nb_tests = NULL;  // Nombre de tests passés par les modules
static int *nb_passed_tests = NULL;  // Nombre de tests réussis par les modules
static bool function_failed = false;  // Fonction en cours de test a échoué ?
static int i_module_in_test = 0;  // Indice du module en cours de test

/**
 *  Fonction : test_assert
 *  -----------------------
 *  Vérifie si la condition cond est vraie.
 *
 *  Paramètres :
 *    - cond : condition à tester
 *    - file : nom du fichier contenant l'assertion
 *    - line : ligne du fichier à laquelle se trouve l'assertion
 */
void test_assert(int cond, char *file, int line) {
    nb_tests[i_module_in_test] += 1;

    if (cond) {
        nb_passed_tests[i_module_in_test] += 1;
    } else {
        function_failed = true;
        printf("\n");
        fprintf(stderr, "    %s:%d : "COLOR_FAIL"échec assertion"COLOR_CLR,
                file, line);
    }
}

/**
 *  Fonction : print_many
 *  ----------------------
 *  Affiche sur stdout nb fois le caractère c.
 *
 *  Paramètres :
 *    - c  : caractère à afficher
 *    - nb : nombre de répétitions désiré
 */
void print_many(char c, int nb) {
    for (int i = 0; i < nb; ++i) {
        printf("%c", c);
    }
}

/**
 *  Fonction : print_results
 *  -------------------------
 *  Affiche le résultats des tests pour chaque module.
 *
 *  Paramètre :
 *    - nb_modules : nombre de modules
 */
void print_results(int nb_modules) {
    double score = 0.0;
    char color[20] = {0};
    int max_length = 0;
    int cur_length = 0;

    // On récupère la longueur maximale du nom des modules
    for (int i = 0; i < nb_modules; ++i) {
        cur_length = strlen(modules[i]);
        if (cur_length > max_length) max_length = cur_length;
    }

    printf("\n-- Résultats --\n");
    print_many('-', max_length + 18);
    printf("\n");

    // Pour chaque module
    for (int i = 0; i < nb_modules; ++i) {
        printf("| Module %s", modules[i]);
        print_many(' ', max_length - strlen(modules[i]));
        printf(" :");

        if (nb_tests[i] != 0) {
            score = (double) nb_passed_tests[i] / nb_tests[i] * 100;
        }

        if (score != 100.0) strcpy(color, COLOR_FAIL);
        else strcpy(color, COLOR_SUCCESS);

        printf(" %s%3d%%"COLOR_CLR" |\n", color, (int) score);

        if (i == nb_modules - 1) {
            print_many('-', max_length + 18);
        } else {
            printf("|");
            print_many('-', max_length + 16);
            printf("|");
        }
        printf("\n");
    }
}

/**
 *  Fonction : run_tests
 *  ---------------------
 *  Lance le test des fonctions de functions_list.
 *
 *  Paramètre :
 *    - functions_list : tableau contenant les fonctions à tester
 */
void run_tests(function *functions_list) {
    int nb_modules = 0;

    // Parcours des tests pour déterminer le nombre de modules
    for (int i = 0; functions_list[i].name; ++i) {
        if (!functions_list[i].ptr) ++nb_modules;
    }

    // Allocation du tableau qui stocke le nom des modules
    modules = malloc(nb_modules * sizeof(char *));
    CHECK_NULL(modules);
    for (int i = 0; i < nb_modules; ++i) {
        modules[i] = malloc(MAX_MODULE_NAME_LENGTH * sizeof(char));
        CHECK_NULL(modules[i]);
    }
    // Allocation du tableau qui stocke le nombre de tests
    nb_tests = calloc(nb_modules, sizeof(int));
    CHECK_NULL(nb_tests);
    // Allocation du tableau qui stocke le nombre de tests réussis
    nb_passed_tests = calloc(nb_modules, sizeof(int));
    CHECK_NULL(nb_passed_tests);

    // Parcours des fonctions de test dans functions_list
    i_module_in_test = -1;
    for (int i = 0; functions_list[i].name; ++i) {
        if (!functions_list[i].ptr) {
            ++i_module_in_test;
            if (i != 0) printf("\n");
            printf("-- Test du module %s --\n", functions_list[i].name);
            strcpy(modules[i_module_in_test], functions_list[i].name);
        } else {
            function_failed = false;
            printf("%s() ", functions_list[i].name);
            fflush(stdout);

            functions_list[i].ptr();

            if (function_failed) {
                printf("\n==> "COLOR_FAIL"KO"COLOR_CLR"\n");
            } else {
                printf("==> "COLOR_SUCCESS"OK"COLOR_CLR"\n");
            }
        }
    }

    print_results(nb_modules);

    // Libération de la mémoire
    for (int i = 0; i < nb_modules; ++i) {
        free(modules[i]);
        modules[i] = NULL;
    }
    free(modules);
    modules = NULL;
    free(nb_tests);
    nb_tests = NULL;
    free(nb_passed_tests);
    nb_passed_tests = NULL;
}
