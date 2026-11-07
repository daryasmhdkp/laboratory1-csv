#include "types.h"

ValueType type_identifier(const char *value, int quoted) {
    int len = strlen(value);
    int count_before_dot = 0;
    int dot = 0;
    int count_after_dot = 0;
    int start_index = 0;
    if (value == NULL || value[0] == '\0') {
        if (quoted == 1) {
            return TYPE_STRING;
        }
        return TYPE_NULL;
    }
    if (value[0] == '+' || value[0] == '-') {
        start_index = 1;
        if (value[start_index] == '\0') {
            return TYPE_STRING;
        }
    }
    for (int i = start_index; i < len; i++) {
        if (value[i] <= '9' && value[i] >= '0') {
            if (dot == 0) {
                count_before_dot +=1;
            }
            else if (dot == 1) {
                count_after_dot+=1;
            }
        }
        else if (value[i] == '.') {
            if (dot == 1) {
                return TYPE_STRING;
            }
            else dot = 1;
        }
        else return TYPE_STRING;
    }
    if (dot == 1) {
        if (count_after_dot == 0 || count_before_dot == 0) return TYPE_STRING;
        else return TYPE_DOUBLE;
    }
    if (count_before_dot == 0) return TYPE_STRING;
    else if (dot == 0) return TYPE_INT;

    return TYPE_STRING;
}


ValueType column_type(ValueType current, ValueType new_type) {
    if (current == TYPE_STRING) return TYPE_STRING;
    else if (current == TYPE_DOUBLE) {
        if (new_type == TYPE_STRING) return TYPE_STRING;
        else return TYPE_DOUBLE;
    }
    else if (current == TYPE_INT) {
        if (new_type == TYPE_DOUBLE) return TYPE_DOUBLE;
        else if (new_type == TYPE_STRING) return TYPE_STRING;
        else return TYPE_INT;
    }
    else if (current == TYPE_NULL && new_type != TYPE_NULL) return new_type;
    else if (current == TYPE_NULL && new_type == TYPE_NULL) return TYPE_NULL;
    return TYPE_STRING;
}