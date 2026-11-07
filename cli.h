#ifndef CSVSTAT_CLI_H
#define CSVSTAT_CLI_H
#include <stdio.h>
#include <string.h>

// Command Line Interface (CLI)
//представление команды:
// -1 misstake
// 1 schema
// 2 stats
// 3 report

void cli_print_help(void);
int cli_parse_args(int argc, char *argv[]);
#endif

