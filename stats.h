#ifndef CSVSTAT_STATS_H
#define CSVSTAT_STATS_H
#include <stdio.h>
#include "types.h"
#include <limits.h>
#include <float.h>
#include "csv.h"
#include <string.h>

typedef struct {
    int column_index;
    char *name;
    ValueType type;
    int nulls_count;
    long long sum_int;
    double sum_double;
    int meaningful_numbers_count;
    long long min_number_int;
    long long max_number_int;
    double arithmetic_mean;
    int positive_count_numbers;
    int negative_count_numbers;
    int even_count_numbers_int; // чётные 
    double min_number_double;
    double max_number_double;
    int integers_whole_number_for_DOUBLE;
    int min_length_string;
    int max_length_string;
    long long sum_length_string;
    long long meaningful_length_string;
    double arithmetic_mean_length_string;
    int empty_strings_count;
} ColumnStats;

void stats_realisation(FILE *file);

#endif