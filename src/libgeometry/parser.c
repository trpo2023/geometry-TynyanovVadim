#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"

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
    Point* cords = malloc(sizeof(Point) * TRIANGLE_TOKENS_AMOUNT);
    triangle->cords = cords;
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
