#ifndef CSVSTAT_TYPES_H
#define CSVSTAT_TYPES_H
#include "csv.h"
#include <string.h>

typedef enum {
    TYPE_NULL = 0,
    TYPE_INT = 1,
    TYPE_DOUBLE = 2,
    TYPE_STRING = 3
} ValueType;

ValueType type_identifier(const char *value, int quoted);
ValueType column_type(ValueType current, ValueType new_type);

#endif