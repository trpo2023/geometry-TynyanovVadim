#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libgeometry/lexer.h"
#include "libgeometry/parser.h"

#define FILE_ARGUMENT 2

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
        for (int i = 0; i < len; i++) {
            line[i] = tolower(line[i]);
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
