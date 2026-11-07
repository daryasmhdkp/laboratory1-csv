#include <stdio.h>
#include <string.h>
#include "cli.h"
#include "common.h"
#include "schema.h"
#include "stats.h"
#include "report.h"

int main(int argc, char **argv) {
    int command = cli_parse_args(argc, argv);
    if (command == 0) return CSVSTAT_EXIT_OK;

    if (command == -1) {
        fprintf(stderr, "ERROR: Command Line Interface - invalid arguments\n");
        return CSVSTAT_EXIT_BAD_CLI;
    }

    FILE *f = NULL;
    FILE *out_file = NULL;

    if (command == 1 || command == 2 || command == 3) {
        f = fopen(argv[2], "r");
        if (f == NULL) {
            fprintf(stderr, "ERROR: Runtime - a file, input data, memory allocation, etc\n");
            return CSVSTAT_EXIT_ERROR;
        }
    }
    if (command == 1) {
        schema_realisation(f);
        fclose(f);
        return CSVSTAT_EXIT_OK;
    }
    else if (command == 2) { 
        stats_realisation(f);
        fclose(f);
        return CSVSTAT_EXIT_OK;
    }
    else if (command == 3) {
        out_file = fopen(argv[3], "w");
        if (out_file == NULL) {
            fprintf(stderr, "ERROR: Runtime - a file, input data, memory allocation, etc\n");
            return CSVSTAT_EXIT_ERROR;
        }
        report_realisation(f, out_file);
        fclose(f);
        fclose(out_file); 
        return CSVSTAT_EXIT_OK;
    }
    return CSVSTAT_EXIT_BAD_CLI;
}