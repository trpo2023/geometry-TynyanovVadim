#include "lexer.h"

void message_error(char* string, int column, char* message)
{
    fprintf(stderr,
            "%s%*c\nError at column %d: %s\n",
            string,
            column,
            '^',
            column,
            message);
}

int find_figure(char* line, char* figure)
{
    int i, j;
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

int check_brackets(char* line, int len)
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
            message_error(line, (strstr(line, "(") - line) + 1, ERROR_OPEN_BRACKER_NOT_FOUND);
            return 0;
        }
    }
    if (attachments > 0) {
        message_error(line, len, ERROR_CLOSE_BRACKER_NOT_FOUND);
        return 0;
    } else if (find_figure(line, TRIANGLE) || find_figure(line, POLYGON)) {
        if (brackets != POLYGON_TRIANGLE_BRACKETS) {
            message_error(line, len, ERROR_EXCEPTED_TWO_BRACKETS);
            return 0;
        }
        if (*(strstr(line, "(") + 1) != '(') {
            message_error(
                    line,
                    strstr(line, "(") - line + 2,
                    ERROR_INVALID_ARGUMENTS_TYPE);
            return 0;
        }
        if (*(strstr(line, ")") + 1) != ')') {
            message_error(
                    line,
                    strstr(line, ")") - line + 2,
                    ERROR_INVALID_ARGUMENTS_TYPE);
            return 0;
        }
    } else if (find_figure(line, CIRCLE)) {
        if (brackets != CIRCLE_BRACKETS) {
            message_error(line, len, ERROR_EXCEPTED_ONE_BRACKETS);
            return 0;
        }
    }
    return 1;
}

int check_unexpected_token(char* line, int len)
{
    char* start = strstr(line, ")");
    while (start < line + len) {
        start++;
        if (*start != '\n' && *start != '\0' && *start != ' '
            && *start != ')') {
            message_error(line, start - line + 1, ERROR_UNEXPECTED_TOKEN);
            return 0;
        }
    }
    return 1;
}

int is_argument_correct(char* line)
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
                        message_error(
                                line,
                                curent_symbol - line + 1,
                                ERROR_NEGATIVE_RADIUS);
                        return 0;
                    }
                }

                curent_symbol++;
                if (*curent_symbol == '.') {
                    dot_count++;
                }
                if (dot_count > 1) {
                    message_error(
                            line,
                            curent_symbol - line + 1,
                            ERROR_WRONG_ARGUMENT);
                    return 0;
                }
            }
            continue;
        } else if (*curent_symbol == ',' || *curent_symbol == ')') {
            if (argumet_count != 2 && *curent_symbol == ',') {
                message_error(
                        line,
                        curent_symbol - line + 1,
                        ERROR_INVALID_ARGUMENTS_TYPE);
                return 0;
            }

            if (*curent_symbol == ',') {
                argumet_count = 0;
            }

            tokens++;
            if (find_figure(line, CIRCLE) && (tokens > CIRCLE_TOKENS_AMOUNT)) {
                message_error(
                        line,
                        curent_symbol - line + 1,
                        ERROR_TOO_MANY_ARGUMENTS);
                return 0;
            }

            if (find_figure(line, TRIANGLE)
                && (tokens > TRIANGLE_TOKENS_AMOUNT)) {
                message_error(
                        line,
                        curent_symbol - line + 1,
                        ERROR_TOO_MANY_ARGUMENTS);
                return 0;
            }

            if (*curent_symbol == ')') {
                if (find_figure(line, CIRCLE)) {
                    if (argumet_count != 1 || (tokens < CIRCLE_TOKENS_AMOUNT)) {
                        message_error(
                                line,
                                curent_symbol - line + 1,
                                ERROR_INVALID_ARGUMENTS_TYPE);
                        return 0;
                    }
                } else if (find_figure(line, TRIANGLE)) {
                    if (argumet_count != 2
                        || (tokens < TRIANGLE_TOKENS_AMOUNT)) {
                        message_error(
                                line,
                                curent_symbol - line + 1,
                                ERROR_INVALID_ARGUMENTS_TYPE);
                        return 0;
                    }
                } else if (find_figure(line, POLYGON)) {
                    if (argumet_count != 2
                        || (tokens < POLYGON_TOKENS_AMOUNT)) {
                        message_error(
                                line,
                                curent_symbol - line + 1,
                                ERROR_INVALID_ARGUMENTS_TYPE);
                        return 0;
                    }
                }
                break;
            }
        } else if (*curent_symbol == ' ' || *curent_symbol == '(') {
            curent_symbol++;
            continue;
        } else {
            message_error(line, curent_symbol - line + 1, ERROR_WRONG_ARGUMENT);
            return 0;
        }
        curent_symbol++;
    }
    return 1;
}

int is_syntax_correct(char* line, int len)
{
    if (!check_brackets(line, len)) {
        return 0;
    }
    if (!(find_figure(line, CIRCLE) || find_figure(line, TRIANGLE)
          || find_figure(line, POLYGON))) {
        message_error(line, 0, ERROR_FIGURE_NOT_FOUND);
        return 0;
    }
    if (!check_unexpected_token(line, len)) {
        return 0;
    }
    if (!is_argument_correct(line)) {
        return 0;
    }
    return 1;
}
