#include "stats.h"

void stats_realisation(FILE *file) {
    int count_of_columns = 0;
    int count_of_lines = 0;
    CsvField *header = csv_read_one_line(file, &count_of_columns);
    ColumnStats *statistics = malloc(sizeof(ColumnStats) * count_of_columns);
    for(int i = 0; i < count_of_columns; i++) {
        statistics[i].column_index = i;
        statistics[i].name = header[i].value;
        statistics[i].type = TYPE_NULL;
        statistics[i].nulls_count = 0;
        statistics[i].sum_int = 0;
        statistics[i].sum_double = 0.0;
        statistics[i].meaningful_numbers_count = 0;
        statistics[i].min_number_int = LLONG_MAX;
        statistics[i].max_number_int = LLONG_MIN;
        statistics[i].arithmetic_mean = 0.0;
        statistics[i].positive_count_numbers = 0;
        statistics[i].negative_count_numbers = 0;
        statistics[i].even_count_numbers_int = 0;
        statistics[i].min_number_double = DBL_MAX;
        statistics[i].max_number_double = -DBL_MAX;
        statistics[i].integers_whole_number_for_DOUBLE = 0;
        statistics[i].min_length_string = INT_MAX;
        statistics[i].max_length_string = INT_MIN;
        statistics[i].sum_length_string = 0;
        statistics[i].arithmetic_mean_length_string = 0.0;
        statistics[i].empty_strings_count = 0;
        statistics[i].meaningful_length_string = 0;
    }
    int current_columns = 0;
    CsvField *current_line = NULL;
    ValueType current_type = TYPE_NULL;
    while (1) {
        current_line = csv_read_one_line(file, &current_columns);
        if (current_line == NULL) break;
        count_of_lines+=1;
        for(int i = 0; i < current_columns; i++) {
            current_type = type_identifier(current_line[i].value, current_line[i].quoted);
            statistics[i].type = column_type(statistics[i].type, current_type);
            if (current_type == TYPE_NULL) statistics[i].nulls_count+=1;
            else if (current_type == TYPE_STRING) {
                statistics[i].meaningful_numbers_count+=1;
                int length_of_current_line = strlen(current_line[i].value);
                statistics[i].sum_length_string+=length_of_current_line;
                if (length_of_current_line == 0) statistics[i].empty_strings_count+=1;
                if (statistics[i].min_length_string > length_of_current_line) statistics[i].min_length_string = length_of_current_line;
                if (statistics[i].max_length_string < length_of_current_line) statistics[i].max_length_string = length_of_current_line;
                if (length_of_current_line > 0) statistics[i].meaningful_length_string+=1;
            }
            else if (current_type == TYPE_INT) {
                statistics[i].meaningful_numbers_count+=1;
                long long number = atoll(current_line[i].value); // array to long long
                statistics[i].sum_int+=number;
                if (number > 0) statistics[i].positive_count_numbers+=1;
                if (number < 0) statistics[i].negative_count_numbers+=1;
                if (number % 2 == 0) statistics[i].even_count_numbers_int+=1;
                if (statistics[i].max_number_int < number) statistics[i].max_number_int = number;
                if (statistics[i].min_number_int > number) statistics[i].min_number_int = number;
            }
            else if (current_type == TYPE_DOUBLE) {
                statistics[i].meaningful_numbers_count+=1;
                double donumber = atof(current_line[i].value); // array to float
                long long whole_part_of_donumber = (long long)donumber; 
                if (donumber == (double)whole_part_of_donumber) statistics[i].integers_whole_number_for_DOUBLE+=1;
                statistics[i].sum_double+= donumber;
                if (donumber > 0) statistics[i].positive_count_numbers+=1;
                if (donumber < 0) statistics[i].negative_count_numbers+=1;
                if (statistics[i].max_number_double < donumber) statistics[i].max_number_double = donumber;
                if (statistics[i].min_number_double > donumber) statistics[i].min_number_double = donumber;
            }
        }
    }

    printf("rows %d\n", count_of_lines);
    printf("columns %d\n", count_of_columns);
    printf("\n");
    for (int i = 0; i < count_of_columns; i++) {

        if (statistics[i].type == TYPE_INT) {
            printf("column %d\n", i);
            printf("name \"%s\"\n", header[i].value);
            printf("type INT\n");
            printf("nulls %d\n", statistics[i].nulls_count);
            double mean = 0;
            if (statistics[i].meaningful_numbers_count > 0) {
                printf("min %lld\n", statistics[i].min_number_int);
                printf("max %lld\n", statistics[i].max_number_int);
                mean = statistics[i].sum_int / (double)statistics[i].meaningful_numbers_count;
            }
            else {
                mean = 0.0;
                printf("min 0\n");
                printf("max 0\n");
            }
            printf("mean %f\n", mean);
            printf("positive %d\n", statistics[i].positive_count_numbers);
            printf("negative %d\n", statistics[i].negative_count_numbers);
            printf("even %d\n", statistics[i].even_count_numbers_int);
            printf("\n");
        }
        else if (statistics[i].type == TYPE_NULL) {
            printf("column %d\n", i);
            printf("name \"%s\"\n", header[i].value);
            printf("type NULL\n");
            printf("rows %d\n", count_of_lines);
            printf("\n");
        }
        else if (statistics[i].type == TYPE_STRING) {
            printf("column %d\n", i);
            printf("name \"%s\"\n", header[i].value);
            printf("type STRING\n");
            printf("nulls %d\n", statistics[i].nulls_count);
            if (statistics[i].min_length_string == 0) statistics[i].min_length_string = 0.0;
            if (statistics[i].max_length_string == 0) statistics[i].max_length_string = 0.0;
            printf("min_length %d\n", statistics[i].min_length_string);
            printf("max_length %d\n", statistics[i].max_length_string);
            double strmean = 0;
            if (statistics[i].meaningful_length_string != 0) {
                strmean = (double)statistics[i].sum_length_string / statistics[i].meaningful_length_string;
            }
            else strmean = 0.0;
            printf("mean_length %f\n", strmean);
            printf("empty %d\n", statistics[i].empty_strings_count);
            printf("\n");
        }
        else if (statistics[i].type == TYPE_DOUBLE) {
            printf("column %d\n", i);
            printf("name \"%s\"\n", header[i].value);
            printf("type DOUBLE\n");
            printf("nulls %d\n", statistics[i].nulls_count);
            if (statistics[i].min_number_double == 0.0) statistics[i].min_number_double  = 0.0;
            if (statistics[i].max_number_double  == 0.0) statistics[i].max_number_double  = 0.0;
            printf("min %f\n", statistics[i].min_number_double);
            printf("max %f\n", statistics[i].max_number_double);
            double domean = 0;
            if (statistics[i].meaningful_numbers_count != 0) {
                domean = statistics[i].sum_double / statistics[i].meaningful_numbers_count;
            }
            else domean = 0;
            printf("mean %f\n", domean);
            printf("positive %d\n", statistics[i].positive_count_numbers);
            printf("negative %d\n", statistics[i].negative_count_numbers);
            printf("integer %d\n", statistics[i].integers_whole_number_for_DOUBLE);
            printf("\n");
        }
    }
}