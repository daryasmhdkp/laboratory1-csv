#include "csv.h"


CsvField* check_and_extend(CsvField *fields, int *capacity, int used) {
    if (used >= *capacity) {
        *capacity = (*capacity) * 2;
        CsvField *new_fields = realloc(fields, (*capacity) * sizeof(CsvField));
        if (new_fields == NULL) {
            return NULL;
        }
    return new_fields;
    }
    return fields;
}
// getchar() == fgetc(FILE *file)

CsvField* csv_read_one_line(FILE *file, int *field_count) {
    int c = fgetc(file);
    if (c == EOF) {
        *field_count = 0;
        return NULL;
    }
    *field_count = 0;
    int fields_capacity = 2;
    CsvField *fields = malloc(fields_capacity * sizeof(CsvField));
    int current_word_length = 0;
    int quoted = 0;
    int word_capacity = 8;
    char *word = malloc(word_capacity * sizeof(char));
    int are_we_in_quotes = 0;
    int is_it_the_first_char_of_fields = 1;
    while (c != EOF) {
        if (c == '"') { // встретила кавычку
            if (is_it_the_first_char_of_fields == 1) { // это НАЧАЛО поля
                are_we_in_quotes = 1; // тогда мы в кавычках
                quoted = 1;
                is_it_the_first_char_of_fields = 0; // уже не начало поля
                //кавычку не добавляем тк поле заключено в кавычки это ок
            }
            else if (are_we_in_quotes == 1) { // мы встретили кавычку уже будучи в кавычках до этого
                int next_c = fgetc(file); // чекаем следующий элемент
                if (next_c == '"') { //следующий снова кавычка->дано ""приве"",отдаём "приве"=экранирование
                    if (word_capacity <= current_word_length) {
                        word_capacity*=2;
                        word = realloc(word, word_capacity * sizeof(char));
                    }
                    word[current_word_length] = '"'; // записываем в поле кавычку
                    current_word_length+=1;
                }
                else if (next_c != '"') { // мы в кавычках но следующий элемент не кавычка
                    // это не экранирование а просто конец поля в кавычках
                    are_we_in_quotes = 0; // кавычки закончились кавычку не добавляем
                    ungetc(next_c, file); // возвращаем символ с в поток
                    int look = fgetc(file);
                    while (look == ' ' || look == '\t') {
                        look = fgetc(file);
                    }
                    ungetc(look, file);
                }
            }
        }
        else if (c == ',') { // встретила запятую
            if (are_we_in_quotes == 1) { //встретила запятую будучи в кавычках -> запятая = часть поля
                if (word_capacity <= current_word_length) {
                        word_capacity*=2;
                        word = realloc(word, word_capacity * sizeof(char));
                }
                word[current_word_length] = ','; //записываем в поле запятую
                current_word_length+=1;
            }
            else if (are_we_in_quotes == 0) { //встретила запятую находясь не в кавычках - конец поля
                word[current_word_length] = '\0';
                fields = check_and_extend(fields, &fields_capacity, *field_count);
                if (fields == NULL) return NULL;
                if (quoted == 0) {
                    while (current_word_length > 0 && (word[0] == ' ' || word[0] == '\t')) {
                        for(int i = 0; i < current_word_length; i++) {
                            word[i] = word[i+1];
                        }
                        current_word_length-=1;
                    }
                    while (current_word_length > 0 && 
                        (word[current_word_length-1] == ' ' || word[current_word_length-1] == '\t')) {
                        current_word_length-=1;
                    }
                }
                word[current_word_length] = '\0';

                fields[*field_count].value = word;
                fields[*field_count].type = 0; 
                fields[*field_count].quoted = quoted;
                (*field_count)+=1;
                current_word_length = 0;
                word_capacity = 8;
                word = malloc(word_capacity * sizeof(char));
                is_it_the_first_char_of_fields = 1;
                quoted = 0;
            }
        }
        else if (c == '\n') { // встретили конец  строки
            if (are_we_in_quotes == 1) { // мы в кавычках, сохраняем слово после цикла
                word[current_word_length] = '\0';
                break;
            }
            else if (are_we_in_quotes == 0) { // встретили конец строки и мы не в кавычках
                word[current_word_length] = '\0'; // сохраняем слово после цикла
                break;
            }
        }
        else { // встретили ЛЮБОЙ другой символ
            if (is_it_the_first_char_of_fields == 1 && (c == ' ' || c == '\t')) {
                //is_it_the_first_char_of_fields = 0;
            }
            else { 
                if (word_capacity <= current_word_length) {
                    word_capacity*=2;
                    word = realloc(word, word_capacity * sizeof(char));
                }
                word[current_word_length] = c; // записываем в поле запятую
                current_word_length+=1; // сохраняем эти символы
                is_it_the_first_char_of_fields = 0;
            }
        }
        c = fgetc(file); // читаем элементы
    } // end of while

    if (current_word_length > 0 || quoted == 1 || *field_count == 0) {
        fields = check_and_extend(fields, &fields_capacity, *field_count);
        if (fields == NULL) return NULL;
        while (current_word_length > 0 && (word[0] == ' ' || word[0] == '\t')) {
            for(int i = 0; i < current_word_length; i++) {
                word[i] = word[i+1];
            }
            current_word_length-=1;
        }
        while (current_word_length > 0 && (word[current_word_length-1] == ' ' || 
            word[current_word_length-1] == '\t')) {
                current_word_length-=1;
            }
            word[current_word_length] = '\0';
        fields[*field_count].quoted = quoted;
        fields[*field_count].type = 0; 
        fields[*field_count].value = word;
        (*field_count)++;
    }
    else {
        free(word);
    }
    return fields;
}
