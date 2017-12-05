#include <stdio.h>
#include "../inc/utils.h"
#include "../inc/xor.h"
#include "../inc/c1.h"
#include "../inc/c2.h"
#include "../inc/c3.h"

int main(int argc, char *argv[]) {
    FILE *f_in, *f_out;
    byte *str_in, *str_out;
    uint32 str_length;

    f_in = fopen(argv[1], "r");
    f_out = fopen(argv[2], "w");

    str_length = file_to_str(f_in, &str_in);

    str_out = xor(str_in, str_length, (byte *) argv[3]);

    str_to_file(str_out, str_length, f_out);

    fclose(f_in);
    fclose(f_out);

    return 0;
}
