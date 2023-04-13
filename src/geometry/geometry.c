#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libgeometry/calculation.h"
#include "libgeometry/lexer.h"
#include "libgeometry/parser.h"

#define FILE_ARGUMENT 2

int is_allocate_memory(void* pointer, int size)
{
    if (!pointer && size > 0) {
        return -1;
    }
    return 0;
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

    count_figures(file, &circles_size, &triangles_size, &polygons_size);

    Circle* circles = malloc(sizeof(Circle) * circles_size);
    Triangle* triangles = malloc(sizeof(Triangle) * triangles_size);
    Polygon* polygons = malloc(sizeof(Polygon) * polygons_size);

    if (is_allocate_memory(circles, circles_amount)
        || is_allocate_memory(triangles, triangles_amount)
        || is_allocate_memory(polygons, polygons_amount)) {
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
        for (size_t i = 0; i < len; i++) {
            line[i] = tolower(line[i]);
        }
        if (!is_syntax_correct(line, read)) {
            return -1;
        }
        if (find_figure(line, CIRCLE)) {
            if (get_circle(&(circles[circles_amount]), line)) {
                circles_amount++;
            } else {
                return -1;
            }
        } else if (find_figure(line, TRIANGLE)) {
            if (get_triangle(&(triangles[triangles_amount]), line)) {
                triangles_amount++;
            } else {
                return -1;
            }
        } else if (find_figure(line, POLYGON)) {
            if (get_polygon(&(polygons[polygons_amount]), line)) {
                polygons_amount++;
            } else {
                return -1;
            }
        } 
    }

    for (int i = 0; i < circles_amount; i++) {
        Circle circle = circles[i];
        print_figure(&circle, CIRCLE);
        print_figure_parametrs(&circle, CIRCLE);

        printf("\tintersects:\n");

        for (int j = 0; j < circles_amount; j++) {
            if (j == i)
                continue;
            if (is_collision_circles(&circle, &circles[j])) {
                printf("\t\t");
                print_figure(&circles[j], CIRCLE);
            }
        }
        for (int j = 0; j < triangles_amount; j++) {
            if (is_collision_figure_with_circle(
                        &circle, triangles[j].cords, TRIANGLE_TOKENS_AMOUNT)) {
                printf("\t\t");
                print_figure(&triangles[j], TRIANGLE);
            }
        }
        for (int j = 0; j < polygons_amount; j++) {
            if (is_collision_figure_with_circle(
                        &circle, polygons[j].cords, polygons[j].size)) {
                printf("\t\t");
                print_figure(&polygons[j], POLYGON);
            }
        }
    }

    for (int i = 0; i < polygons_amount; i++) {
        Polygon polygon = polygons[i];
        print_figure(&polygon, POLYGON);
        print_figure_parametrs(&polygon, POLYGON);

        printf("\tintersects:\n");
        for (int j = 0; j < circles_amount; j++) {
            if (is_collision_figure_with_circle(
                        &circles[j], polygon.cords, polygon.size)) {
                printf("\t\t");
                print_figure(&circles[j], CIRCLE);
            }
        }
        for (int j = 0; j < triangles_amount; j++) {
            if (is_collision_figures(
                        triangles[j].cords,
                        TRIANGLE_TOKENS_AMOUNT,
                        polygon.cords,
                        polygon.size)) {
                printf("\t\t");
                print_figure(&triangles[j], TRIANGLE);
            }
        }
        for (int j = 0; j < polygons_amount; j++) {
            if (j == i)
                continue;
            if (is_collision_figures(
                        polygons[j].cords,
                        polygons[j].size,
                        polygon.cords,
                        polygon.size)) {
                printf("\t\t");
                print_figure(&polygons[j], POLYGON);
            }
        }
    }

    for (int i = 0; i < triangles_amount; i++) {
        Triangle triangle = triangles[i];
        print_figure(&triangle, TRIANGLE);
        print_figure_parametrs(&triangle, TRIANGLE);

        printf("\tintersects:\n");
        for (int j = 0; j < circles_amount; j++) {
            if (is_collision_figure_with_circle(
                        &circles[j], triangle.cords, TRIANGLE_TOKENS_AMOUNT)) {
                printf("\t\t");
                print_figure(&circles[j], CIRCLE);
            }
        }
        for (int j = 0; j < triangles_amount; j++) {
            if (j == i)
                continue;
            if (is_collision_figures(
                        triangles[j].cords,
                        TRIANGLE_TOKENS_AMOUNT,
                        triangle.cords,
                        TRIANGLE_TOKENS_AMOUNT)) {
                printf("\t\t");
                print_figure(&triangles[j], TRIANGLE);
            }
        }
        for (int j = 0; j < polygons_amount; j++) {
            if (is_collision_figures(
                        polygons[j].cords,
                        polygons[j].size,
                        triangle.cords,
                        TRIANGLE_TOKENS_AMOUNT)) {
                printf("\t\t");
                print_figure(&polygons[j], POLYGON);
            }
        }
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
