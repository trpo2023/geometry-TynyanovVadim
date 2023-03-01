#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_ARGUMENT 2

#define CIRCLE "circle"
#define TRIANGLE "triangle"
#define POLYGON "polygon"

#define CIRCLE_TOKENS_AMOUNT 2
#define TRIANGLE_TOKENS_AMOUNT 4
#define POLYGON_TOKENS_AMOUNT 4

#define POLYGON_TRIANGLE_BRACKETS 2
#define CIRCLE_BRACKETS 1

#define ERROR_FIGURE_NOT_FOUND "expected 'circle', 'triangle' or 'polygon'"
#define ERROR_OPEN_BRACKER_NOT_FOUND "expected '('"
#define ERROR_CLOSE_BRACKER_NOT_FOUND "expected ')'"
#define ERROR_WRONG_ARGUMENT "expected '<double>'"
#define ERROR_UNEXPECTED_TOKEN "unexpected token"
#define ERROR_INVALID_ARGUMENTS_TYPE "invalid arguments"
#define ERROR_TOO_MANY_ARGUMENTS "too many arguments"
#define ERROR_NEGATIVE_RADIUS "raduis can not be negative"
#define ERROR_EXCEPTED_TWO_BRACKETS "expected two brackets"
#define ERROR_EXCEPTED_ONE_BRACKETS "expected one brackets"
#define ERROR_FIRST_AND_LAST_NOT_EQUALS "first and last point are not equals"
#define ERROR_ALLOCATE_MEMORY "failed to allocate memory"
#define ERROR_FILE_NO_SPECIFIED "No file specified"
#define ERROR_FILE_NOT_FOUND "File not found"

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point position;
    double radius;
} Circle;

typedef struct {
    Point cords[4];
} Triangle;

typedef struct {
    Point* cords;
    int size;
} Polygon;


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
            message_error(line, 0, ERROR_OPEN_BRACKER_NOT_FOUND);
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

void count_polygon_tokens(char* line, Polygon* polygon)
{
    int tokens = 0;

    char* curent_symbol = NULL;
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '(') {
            curent_symbol = &(line[i]);
        }
    }
    while (*curent_symbol != ')') {
        if (*curent_symbol == ',') {
            tokens++;
        }
        curent_symbol++;
    }
    tokens++;
    polygon->size = tokens;
}

int get_circle(Circle* circle, char* line)
{
    char* symbol = NULL;
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '(') {
            symbol = &(line[i]);
        }
    }
    circle->position.x = strtod(++symbol, &symbol);
    circle->position.y = strtod(symbol, &symbol);
    symbol = strstr(symbol, ",");
    circle->radius = strtod(++symbol, &symbol);
    return 1;
}

int get_triangle(Triangle* triangle, char* line)
{
    int i = 0;
    char* symbol = NULL;
    char* next = NULL;
    for (int i = 1; line[i] != '\0'; i++) {
        if (line[i - 1] == '(') {
            symbol = &(line[i]);
        }
    }
    i = 0;
    while (*symbol != ')') {
        (triangle->cords[i]).x = strtod(symbol, &next);
        (triangle->cords[i]).y = strtod(next, &symbol);
        i++;
        while (*symbol == ' ' || *symbol == ',') {
            symbol++;
        }
    }
    if ((triangle->cords[0]).x != (triangle->cords[i - 1]).x) {
        message_error(line, (next - line), ERROR_FIRST_AND_LAST_NOT_EQUALS);
        return 0;
    } else if ((triangle->cords[0]).y != (triangle->cords[i - 1]).y) {
        message_error(line, (symbol - line), ERROR_FIRST_AND_LAST_NOT_EQUALS);
        return 0;
    }
    return 1;
}

int get_polygon(Polygon* polygon, char* line)
{
    count_polygon_tokens(line, polygon);
    Point* cords = malloc(polygon->size * sizeof(Point));
    if (!cords) {
        fprintf(stderr, "%s\n", ERROR_ALLOCATE_MEMORY);
        return 0;
    }
    polygon->cords = cords;
    int i = 0;
    char* symbol = NULL;
    char* next = NULL;
    for (int i = 1; line[i] != '\0'; i++) {
        if (line[i - 1] == '(') {
            symbol = &(line[i]);
        }
    }

    i = 0;
    while (*symbol != ')') {
        (polygon->cords[i]).x = strtod(symbol, &next);
        (polygon->cords[i]).y = strtod(next, &symbol);
        i++;
        while (*symbol == ' ' || *symbol == ',') {
            symbol++;
        }
    }

    if ((polygon->cords[0]).x != (polygon->cords[i - 1]).x) {
        message_error(line, (next - line), ERROR_FIRST_AND_LAST_NOT_EQUALS);
        return 0;
    } else if ((polygon->cords[0]).y != (polygon->cords[i - 1]).y) {
        message_error(line, (symbol - line), ERROR_FIRST_AND_LAST_NOT_EQUALS);
        return 0;
    }
    return 1;
}

void count_figures(FILE* file, int* circles, int* triangles, int* polygons)
{
    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    while ((read = getline(&line, &len, file)) != -1) {
        if (find_figure(line, CIRCLE)) {
            (*circles)++;
        } else if (find_figure(line, TRIANGLE)) {
            (*triangles)++;
        } else if (find_figure(line, POLYGON)) {
            (*polygons)++;
        }
    }
    fseek(file, 0, SEEK_SET);
}

int main(int argc, char** argv)
{
    if (argc < FILE_ARGUMENT) {
        fprintf(stderr, "%s\n", ERROR_FILE_NO_SPECIFIED);
        return -1;
    }

    FILE* file = fopen(argv[1], "r"); // Open file
    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    int circles_size = 0;
    int triangles_size = 0;
    int polygons_size = 0;

    int circles_amount = 0;
    int triangles_amount = 0;
    int polygons_amount = 0;

    count_figures(
        file,
        &circles_size,
        &triangles_size,
        &polygons_size
    );

    Circle* circles = malloc(sizeof(Circle) * circles_size);
    Triangle* triangles = malloc(sizeof(Triangle) * triangles_size);
    Polygon* polygons = malloc(sizeof(Polygon) * polygons_size);

    if (!circles && circles_size > 0) {
        fprintf(stderr, "%s\n", ERROR_ALLOCATE_MEMORY);
        return -1;
    }
    if (!triangles && triangles_size > 0) {
        fprintf(stderr, "%s\n", ERROR_ALLOCATE_MEMORY);
        return -1;
    }
    if (!polygons && polygons_size > 0) {
        fprintf(stderr, "%s\n", ERROR_ALLOCATE_MEMORY);
        return -1;
    }

    if (file == NULL) {
        fprintf(stderr, "%s\n", ERROR_FILE_NOT_FOUND);
        return -1;
    }

    while ((read = getline(&line, &len, file)) != -1) {
        if (read == 1) {
            continue;
        }
        if (!is_syntax_correct(line, read)) {
            return -1;
        }
        if (find_figure(line, CIRCLE)) {
            if (get_circle(&(circles[circles_amount]), line)) {
                circles_amount++;
            }
        } else if (find_figure(line, TRIANGLE)) {
            if (get_triangle(&(triangles[triangles_amount]), line)) {
                triangles_amount++;
            }
        } else if (find_figure(line, POLYGON)) {
            if (get_polygon(&(polygons[polygons_amount]), line)) {
                polygons_amount++;
            }
        }
        printf("%s\n", line);
    }

    free(line);
    free(circles);
    free(triangles);

    for (int i = 0; i < polygons_amount; i++) {
        free(polygons[i].cords);
    }
    free(polygons);

    return 0;
}
