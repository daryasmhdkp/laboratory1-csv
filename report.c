#include "report.h"

void report_realisation(FILE *file, FILE *out_file) {
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
            ValueType old_type = statistics[i].type;
            statistics[i].type = column_type(old_type, current_type);
            if (old_type == TYPE_INT && statistics[i].type == TYPE_DOUBLE) {
                statistics[i].sum_double = (double)statistics[i].sum_int;
                statistics[i].min_number_double = (double)statistics[i].min_number_int;
                statistics[i].max_number_double = (double)statistics[i].max_number_int;
            }
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
                //statistics[i].sum_int+=number;
                if (number > 0) statistics[i].positive_count_numbers+=1;
                if (number < 0) statistics[i].negative_count_numbers+=1;
                if (number % 2 == 0) statistics[i].even_count_numbers_int+=1;
                statistics[i].integers_whole_number_for_DOUBLE+=1;

                if (statistics[i].type == TYPE_DOUBLE) {
                    statistics[i].sum_double += (double)(number);
                    if (statistics[i].max_number_double < (double)number) {
                        statistics[i].max_number_double = (double)number;
                    }
                    if (statistics[i].min_number_double > (double)number) {
                        statistics[i].min_number_double = (double)number;
                    }
                   
                    
                }
                else {
                    statistics[i].sum_int+=number;
                    if (statistics[i].max_number_int < number) {
                        statistics[i].max_number_int = number;
                    }
                    if (statistics[i].min_number_int > number) {
                        statistics[i].min_number_int = number;
                    }
                }
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

    fprintf(out_file, "_________________________________________________________________________________________\n"); 
    fprintf(out_file, "| %5s | %13s | %6s | %5s | %7s | %7s | %10s | %4s | %4s |\n", 
                "Index", "Name", "Type", "Nulls", "Min", "Max", "Mean", "<0", ">0");
    fprintf(out_file, "_________________________________________________________________________________________\n"); 

    for (int i = 0; i < count_of_columns; i++) {
        char *type = "NULL";
        if (statistics[i].type == TYPE_INT) type = "INT";
        else if (statistics[i].type == TYPE_STRING) type = "STRING";
        else if (statistics[i].type == TYPE_DOUBLE) type = "DOUBLE";

        fprintf(out_file, "| %5d | %13s | %6s | %5d |", 
                        i, header[i].value, type, statistics[i].nulls_count);
    
        if (statistics[i].type == TYPE_INT && statistics[i].meaningful_numbers_count > 0) {
            double mean = statistics[i].sum_int / (double)statistics[i].meaningful_numbers_count;
            fprintf(out_file, " %7lld | %7lld | %10.2f | %4d | %4d |\n", 
                statistics[i].min_number_int, statistics[i].max_number_int, mean,
                statistics[i].negative_count_numbers, statistics[i].positive_count_numbers); 
        }
        else if (statistics[i].type == TYPE_STRING && statistics[i].meaningful_length_string > 0) {
            double strmean = (double)statistics[i].sum_length_string / statistics[i].meaningful_length_string;
            fprintf(out_file, " %7d | %7d | %10.2f | %4s | %4s |\n", 
                statistics[i].min_length_string, statistics[i].max_length_string, strmean, "-", "-"); 
        }
        else if (statistics[i].type == TYPE_NULL) {
            fprintf(out_file, " %7s | %7s | %10s | %4s | %4s |\n", "-", "-", "-", "-", "-");
        }
        else if (statistics[i].type == TYPE_DOUBLE && statistics[i].meaningful_numbers_count != 0) {
            double domean = statistics[i].sum_double / statistics[i].meaningful_numbers_count;
            fprintf(out_file, " %7.2f | %7.2f | %10.2f | %4d | %4d |\n", 
                statistics[i].min_number_double, statistics[i].max_number_double, domean, 
                statistics[i].negative_count_numbers, statistics[i].positive_count_numbers);
        } 
    }
    fprintf(out_file, "_________________________________________________________________________________________\n"); 
}
