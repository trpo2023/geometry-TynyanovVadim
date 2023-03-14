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
double circle_surface(Circle* circle);
double gauss_surface(Point* cords, size_t len);
double triangel_surface(const Triangle triangle);
double polygon_surface(const Polygon polygon);
double circle_perimetr(const Circle circle);
