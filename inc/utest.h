#ifndef UTEST_H
#define UTEST_H

#include <stdio.h>

#define ASSERT(cond) test_assert(cond, __FILE__, __LINE__)
#define CHECK_NULL(ptr) \
    do { \
        if (ptr == NULL) { perror("Erreur"); exit(2); } \
    } while (0)

#define COLOR_FAIL    "\e[01;31m"
#define COLOR_SUCCESS "\e[01;32m"
#define COLOR_CLR     "\e[00m"

#define MAX_MODULE_NAME_LENGTH 20

typedef struct function function;
struct function {
    char *name;
    void (*ptr)(void);
};

void test_assert(int cond, char *file, int line);
void run_tests(function *test_list);
void print_many(char c, int nb);
void print_results(int nb_modules);

#endif // UTEST_H
