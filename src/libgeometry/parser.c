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

double circle_surface(const Circle circle)
{
    return M_PI * pow(circle.radius, 2); // M_PI = 3.14
}

double gauss_surface(const Point* cords, size_t len)
{
    double surface = 0;
    for (size_t i = 0; i < len; i++) {
        surface += cords[i].x * cords[(i + 1) % len].y;
    }

    for (size_t i = 0; i < len; i++) {
        surface -= cords[i].x * cords[(i - 1) % len].y;
    }
    return fabs(surface) / 2.0;
}

double triangel_surface(const Triangle triangle)
{
    return gauss_surface(triangle.cords, 3);
}

double polygon_surface(const Polygon polygon)
{
    return gauss_surface(polygon.cords, polygon.size - 1);
}

double circle_perimetr(const Circle circle)
{
    return 2 * M_PI * circle.radius; // M_PI = 3.14
}

double two_points_distance(const Point point1, const Point point2)
{
    return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
}

double figure_perimetr(const Point* cords, size_t len)
{
    double perimetr = 0;
    for (size_t i = 0; i < len - 1; i++) {
        perimetr += two_points_distance(cords[i], cords[i + 1]);
    }
    return perimetr;
}

double triangle_perimetr(const Triangle triangle)
{
    return figure_perimetr(triangle.cords, TRIANGLE_TOKENS_AMOUNT);
}

double polygon_perimetr(const Polygon polygon)
{
    return figure_perimetr(polygon.cords, polygon.size);
}

int is_segment_collision(const Point* segment_1, const Point* segment_2)

{
    double v1, v2, v3, v4;
    Point vector_12 = {
            segment_1[1].x - segment_1[0].x, (segment_1[1].y - segment_1[0].y)};
    Point vector_34 = {
            segment_2[1].x - segment_2[0].x, (segment_2[1].y - segment_2[0].y)};

    if ((segment_1[0].x == segment_2[0].x && segment_1[0].y == segment_2[0].y)
        || (segment_1[0].x == segment_2[1].x
            && segment_1[0].y == segment_2[1].y)
        || (segment_1[1].x == segment_2[0].x
            && segment_1[1].y == segment_2[0].y)
        || (segment_1[1].x == segment_2[1].x
            && segment_1[1].y == segment_2[1].y)) {
        return 1;
    }

    v1 = vector_12.x * (segment_2[0].y - segment_1[0].y) - // 12-13
            vector_12.y * (segment_2[0].x - segment_1[0].x);
    v2 = vector_12.x * (segment_2[1].y - segment_1[0].y) - // 12-14
            vector_12.y * (segment_2[1].x - segment_1[0].x);
    v3 = vector_34.x * (segment_1[0].y - segment_2[0].y) - // 34-31
            vector_34.y * (segment_1[0].x - segment_2[0].x);
    v4 = vector_34.x * (segment_1[1].y - segment_2[1].y) - // 34-32
            vector_34.y * (segment_1[1].x - segment_2[1].x);

    return ((v1 * v2 < 0) && (v3 * v4 < 0));
}

double vector_len(const Point vector)
{
    return sqrt(vector.x * vector.x + vector.y + vector.y);
}

double find_cos(const Point vector_1, const Point vector_2)
{
    return (vector_1.x * vector_2.x + vector_1.y * vector_2.y)
            / (vector_len(vector_1) + vector_len(vector_2));
}

double find_height_of_triangle(const Point a, const Point b, const Point c)
{
    // a, b - segment`s cords, c - point`s cords
    double ab = two_points_distance(a, b);
    double ac = two_points_distance(a, c);
    double bc = two_points_distance(b, c);
    double p = (ab + ac + bc) / 2.0;

    double height = 2.0 * sqrt(p * (p - ab) * (p - ac) * (p - bc)) / ab;
    return height;
}

double point_segment_distance(const Point a, const Point b, const Point c)
{
    // a, b - segment`s cords, c - point`s cords
    Point vector_ba = {a.x - b.x, a.y - b.y};
    Point vector_ab = {b.x - a.x, b.y - a.y};
    Point vector_bc = {c.x - b.x, c.y - b.y};
    Point vector_ac = {c.x - a.x, c.y - a.y};

    double abc_cos = find_cos(vector_ba, vector_bc);
    double bac_cos = find_cos(vector_ab, vector_ac);

    if (bac_cos <= 0) {
        return two_points_distance(a, c);
    } else if (abc_cos <= 0) {
        return two_points_distance(b, c);
    } else {
        return find_height_of_triangle(a, b, c);
    }
}

int is_collision_figure_with_circle(const Circle* circle, const Point* cords, size_t size)
{
    for (size_t i = 1; i < size; i++) {
        double distance = point_segment_distance(
                cords[i - 1],
                cords[i],
                circle->position);
        if (distance <= circle->radius) {
            return 1;
        }
    }
    return 0;
}

int is_collision_triangle_circle(const Circle* circle, const Triangle* triangle)
{
    return is_collision_figure_with_circle(
            circle, triangle->cords,
            TRIANGLE_TOKENS_AMOUNT
            );
}

int is_collision_polygon_circle(const Circle* circle, const Polygon* polygon)
{
    return is_collision_figure_with_circle(
            circle, polygon->cords,
            polygon->size
            );
}
