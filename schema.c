#include "schema.h"

void schema_realisation(FILE *file) {
    int count_of_columns = 0;
    CsvField *header = csv_read_one_line(file, &count_of_columns);
    int count_of_lines = 0;
    ValueType *types_of_columns = malloc(sizeof(ValueType) * count_of_columns);
    for(int i = 0; i < count_of_columns; i++) {
        types_of_columns[i] = TYPE_NULL;
    }
    int current_columns = 0;
    CsvField *current_line = NULL; 
    ValueType current_type = TYPE_NULL;
    while (1) {
        current_line = csv_read_one_line(file, &current_columns);
        if (current_line == NULL) break;
        count_of_lines+=1;
        for (int i = 0; i < current_columns; i++) {
            current_type = type_identifier(current_line[i].value, current_line[i].quoted); // BUT
            types_of_columns[i] = column_type(types_of_columns[i], current_type);
        }
    }
    printf("rows %d\n", count_of_lines);
    printf("columns %d\n", count_of_columns);
    for(int i = 0; i < count_of_columns; i++) {
        printf("%d \"%s\" ", i, header[i].value);
        if (types_of_columns[i] == TYPE_NULL) printf("NULL\n");
        else if (types_of_columns[i] == TYPE_INT) printf("INT\n");
        else if (types_of_columns[i] == TYPE_DOUBLE) printf("DOUBLE\n");
        else if (types_of_columns[i] == TYPE_STRING) printf("STRING\n");
    }
}