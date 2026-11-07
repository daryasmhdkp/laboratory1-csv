#include "cli.h"

void cli_print_help(void) {
    puts("csvstat --help");
    puts("csvstat schema <csv-file>");
    puts("csvstat stats <csv-file>");
    puts("csvstat report <csv-file> <output-file>");
    puts("");
}

int cli_parse_args(int argc, char *argv[]) {
    if (argc < 2) {
        return -1;
    }
    if (strcmp(argv[1],"--help") == 0) {
        if (argc == 2) {
            cli_print_help();
            return 0;
        }
        else return -1;
    }
    if (strcmp(argv[1], "schema") == 0) {
        if (argc == 3) return 1;
        else return -1;
    }
    if (strcmp(argv[1], "stats") == 0) {
        if (argc == 3) return 2;
        else return -1;
    }
    if (strcmp(argv[1], "report") == 0) {
        if (argc == 4) return 3;
        else return -1;
    }
    return -1;
}