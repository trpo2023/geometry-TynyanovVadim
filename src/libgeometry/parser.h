#include <stdio.h>

#pragma once

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
    Point* cords;
} Triangle;

typedef struct {
    Point* cords;
    int size;
} Polygon;

void count_polygon_tokens(char* line, Polygon* polygon);
int get_circle(Circle* circle, char* line);
int get_triangle(Triangle* triangle, char* line);
int get_polygon(Polygon* polygon, char* line);
void count_figures(FILE* file, int* circles, int* triangles, int* polygons);
double circle_surface(const Circle circle);
double gauss_surface(const Point* cords, size_t len);
double triangle_surface(const Triangle triangle);
double polygon_surface(const Polygon polygon);
double circle_perimetr(const Circle circle);
double two_points_distance(const Point point1, const Point point2);
double figure_perimetr(const Point* cords, size_t len);
double triangle_perimetr(const Triangle triangle);
double polygon_perimetr(const Polygon polygon);
int is_segment_collision(const Point *segment_1, const Point *segment_2);
double vector_len(const Point vector);
double find_cos(const Point vector_1, const Point vector_2);
double find_height_of_triangle(const Point a, const Point b, const Point c);
double point_segment_distance(const Point a, const Point b, const Point c);
int is_collision_figure_with_circle(const Circle circle, const Point* cords, size_t size);
int is_collision_circles(const Circle circle1, const Circle circle2);
int is_collision_figures(
    const Point* cords1, const size_t size1,
    const Point* cords2, const size_t size2);
