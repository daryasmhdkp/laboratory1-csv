#ifndef CSVSTAT_CSV_H
#define CSVSTAT_CSV_H
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *value; // text, указатель на строку
    int type; 
    int quoted;
} CsvField;
CsvField* check_and_extend(CsvField *fields, int *capacity, int used);
CsvField* csv_read_one_line(FILE *file, int *field_count);

#endif
