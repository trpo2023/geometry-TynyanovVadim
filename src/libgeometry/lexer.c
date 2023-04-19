#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"

void message_error(char* string, int column, char* message)
{
    fprintf(stderr,
            "%s\n%*c\nError at column %d: %s\n",
            string,
            column,
            '^',
            column,
            message);
}

int find_figure(char* line, char* figure)
{
    size_t i, j;
    i = j = 0;
    while ((line[i] != '(') && (line[i] != '\n') && (line[i] != '\0')) {
        if (j == 0 && isspace(line[i])) {
            i++;
            continue;
        } else if (j > strlen(figure)) {
            return 0;
        } else if (tolower(line[i]) != figure[j]) {
            return 0;
        }
        i++;
        j++;
    }
    if (j < strlen(figure)) {
        return 0;
    }
    return 1;
}

int check_brackets(char* line, int len, char** error)
{
    int attachments = 0;
    int brackets = 0;
    for (int i = 0; i < len; i++) {
        if (line[i] == '(') {
            attachments++;
        } else if (line[i] == ')') {
            attachments--;
            brackets++;
        }
        if (attachments < 0) {
            *error = ERROR_OPEN_BRACKER_NOT_FOUND;
            return (strstr(line, "(") - line) + 1;
        }
    }
    if (attachments > 0) {
        *error = ERROR_CLOSE_BRACKER_NOT_FOUND;
        return len;
    } else if (find_figure(line, TRIANGLE) || find_figure(line, POLYGON)) {
        if (brackets != POLYGON_TRIANGLE_BRACKETS) {
            *error = ERROR_EXCEPTED_TWO_BRACKETS;
            return len;
        }
        if (*(strstr(line, "(") + 1) != '(') {
            *error = ERROR_INVALID_ARGUMENTS_TYPE;
            return strstr(line, "(") - line + 2;
        }
        if (*(strstr(line, ")") + 1) != ')') {
            *error = ERROR_INVALID_ARGUMENTS_TYPE;
            return strstr(line, ")") - line + 2;
        }
    } else if (find_figure(line, CIRCLE)) {
        if (brackets != CIRCLE_BRACKETS) {
            *error = ERROR_EXCEPTED_ONE_BRACKETS;
            return len;
        }
    }
    *error = NULL;
    return 1;
}

int check_unexpected_token(char* line, int len, char** error)
{
    char* start = strstr(line, ")");
    while (start < line + len) {
        start++;
        if (*start != '\n' && *start != '\0' && *start != ' '
            && *start != ')') {
            *error = ERROR_UNEXPECTED_TOKEN;
            return start - line + 1;
        }
    }
    *error = NULL;
    return 1;
}

int is_argument_correct(char* line, char** error)
{
    int argumet_count = 0;
    int dot_count = 0;
    int tokens = 0;

    char* curent_symbol = NULL;
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '(') {
            curent_symbol = &(line[i]);
        }
    }

    while (*curent_symbol != '\0' && *curent_symbol != '\n') {
        if (isdigit(*curent_symbol)
            || (*curent_symbol == '-' && isdigit(*(curent_symbol + 1)))) {
            argumet_count++;
            while (isdigit(*curent_symbol) || *curent_symbol == '.'
                   || (*curent_symbol == '-'
                       && isdigit(*(curent_symbol + 1)))) {
                if (*curent_symbol == '-') {
                    if (find_figure(line, CIRCLE) && tokens == 1) {
                        *error = ERROR_NEGATIVE_RADIUS;
                        return curent_symbol - line + 1;
                    }
                }

                curent_symbol++;
                if (*curent_symbol == '.') {
                    dot_count++;
                }
                if (dot_count > 1) {
                    *error = ERROR_WRONG_ARGUMENT;
                    return curent_symbol - line + 1;
                }
            }
            continue;
        } else if (*curent_symbol == ',' || *curent_symbol == ')') {
            dot_count = 0;
            if (argumet_count != 2 && *curent_symbol == ',') {
                *error = ERROR_INVALID_ARGUMENTS_TYPE;
                return curent_symbol - line + 1;
            }

            if (*curent_symbol == ',') {
                argumet_count = 0;
            }

            tokens++;
            if (find_figure(line, CIRCLE) && (tokens > CIRCLE_TOKENS_AMOUNT)) {
                *error = ERROR_TOO_MANY_ARGUMENTS;
                return curent_symbol - line + 1;
            }

            if (find_figure(line, TRIANGLE)
                && (tokens > TRIANGLE_TOKENS_AMOUNT)) {
                *error = ERROR_TOO_MANY_ARGUMENTS;
                return curent_symbol - line + 1;
            }

            if (*curent_symbol == ')') {
                if (find_figure(line, CIRCLE)) {
                    if (argumet_count != 1 || (tokens < CIRCLE_TOKENS_AMOUNT)) {
                        *error = ERROR_INVALID_ARGUMENTS_TYPE;
                        return curent_symbol - line + 1;
                    }
                } else if (find_figure(line, TRIANGLE)) {
                    if (argumet_count != 2
                        || (tokens < TRIANGLE_TOKENS_AMOUNT)) {
                        *error = ERROR_INVALID_ARGUMENTS_TYPE;
                        return curent_symbol - line + 1;
                    }
                } else if (find_figure(line, POLYGON)) {
                    if (argumet_count != 2
                        || (tokens < POLYGON_TOKENS_AMOUNT)) {
                        *error = ERROR_INVALID_ARGUMENTS_TYPE;
                        return curent_symbol - line + 1;
                    }
                }
                break;
            }
        } else if (*curent_symbol == ' ' || *curent_symbol == '(') {
            dot_count = 0;
            curent_symbol++;
            continue;
        } else {
            *error = ERROR_WRONG_ARGUMENT;
            return curent_symbol - line + 1;
        }
        curent_symbol++;
    }
    *error = NULL;
    return 1;
}

int is_syntax_correct(char* line, int len)
{
    if (!(find_figure(line, CIRCLE) || find_figure(line, TRIANGLE)
          || find_figure(line, POLYGON))) {
        message_error(line, 0, ERROR_FIGURE_NOT_FOUND);
        return 0;
    }

    int column;
    char* error = NULL;

    column = check_brackets(line, len, &error);
    if (error) {
        message_error(line, column, error);
        return 0;
    }
    column = check_unexpected_token(line, len, &error);
    if (error) {
        message_error(line, column, error);
        return 0;
    }

    column = is_argument_correct(line, &error);
    if (error) {
        message_error(line, column, error);
        return 0;
    }
    return 1;
}
