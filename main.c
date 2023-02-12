#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define File_argument 2
#define Circle "circle"
#define Triangle "triangle"
#define Polygon "polygon"

#define Error_figure_not_found "expected 'circle', 'triangle' or 'polygon'"
#define Error_open_bracket_not_found "expected '('"
#define Error_close_bracket_not_found "expected ')'"
#define Error_wrong_type_argument "expected '<double>'"
#define Error_unexpected_token "unexpected token"

void message_error(char* string, int column, char* message)
{
    int len = strlen(string);
    printf("%s", string);
    if (string[len - 1] != '\n') {
        printf("\n");
    }
    printf("%*c\n", column, '^');
    printf("Error at column %d: %s\n", column, message);
}

int check_bracket(char* line, int len)
{
    int attachments = 0;
    int flag_is_bracket = 0;
    for (int i = 0; i < len; i++) {
        if (line[i] == '(') {
            attachments++;
            flag_is_bracket = 1;
        } else if (line[i] == ')') {
            attachments--;
            flag_is_bracket = 1;
        }
        if (attachments < 0) {
            message_error(line, 0, Error_open_bracket_not_found);
            return 0;
        }
    }
    if (attachments < 0 || flag_is_bracket == 0) {
        message_error(line, 0, Error_open_bracket_not_found);
        return 0;
    } else if (attachments > 0) {
        message_error(line, len, Error_close_bracket_not_found);
        return 0;
    }
    return 1;
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
        } else if (line[i] != figure[j]) {
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

int check_unexpected_token(char* line, int len)
{
    char* start = strstr(line, ")");
    while (start < line + len) {
        start++;
        if (*start != '\n' && *start != '\0' && *start != ' '
            && *start != ')') {
            message_error(line, start - line + 1, Error_unexpected_token);
            return 0;
        }
    }
    return 1;
}

int is_syntax_correct(char* line, int len)
{
    if (!check_bracket(line, len)) {
        return 0;
    }
    if (!(find_figure(line, Circle) || find_figure(line, Triangle)
          || find_figure(line, Polygon))) {
        message_error(line, 0, Error_figure_not_found);
        return 0;
    }
    if (!check_unexpected_token(line, len)) {
        return 0;
    }
    return 1;
}

int main(int argc, char** argv)
{
    if (argc < File_argument) {
        printf("No file specified\n");
        return -1;
    }

    FILE* file = fopen(argv[1], "r");
    char* line = NULL; // Open file
    size_t len = 0;
    ssize_t read = 0;

    if (file == NULL) {
        printf("File not found\n");
        return -1;
    }

    while ((read = getline(&line, &len, file)) != -1) {
        for (int i = 0; i < len; i++) {
            line[i] = tolower(line[i]);
        }
        if (!is_syntax_correct(line, read)) {
            return -1;
        }
    }
    return 0;
}
