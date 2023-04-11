#include <math.h>
#include <string.h>

#include "calculation.h"
#include "lexer.h"
#include "parser.h"

double circle_surface(const Circle* circle)
{
    return M_PI * pow(circle->radius, 2); // M_PI = 3.14
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

double triangle_surface(const Triangle* triangle)
{
    return gauss_surface(triangle->cords, TRIANGLE_TOKENS_AMOUNT);
}

double polygon_surface(const Polygon* polygon)
{
    return gauss_surface(polygon->cords, polygon->size - 1);
}

double circle_perimetr(const Circle* circle)
{
    return 2 * M_PI * circle->radius; // M_PI = 3.14
}

double two_points_distance(const Point* point1, const Point* point2)
{
    return sqrt(pow(point1->x - point2->x, 2) + pow(point1->y - point2->y, 2));
}

double figure_perimetr(const Point* cords, size_t len)
{
    double perimetr = 0;
    for (size_t i = 0; i < len - 1; i++) {
        perimetr += two_points_distance(&(cords[i]), &(cords[i + 1]));
    }
    return perimetr;
}

double triangle_perimetr(const Triangle* triangle)
{
    return figure_perimetr(triangle->cords, TRIANGLE_TOKENS_AMOUNT);
}

double polygon_perimetr(const Polygon* polygon)
{
    return figure_perimetr(polygon->cords, polygon->size);
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

double vector_len(const Point* vector)
{
    return sqrt(vector->x * vector->x + vector->y * vector->y);
}

double find_cos(const Point* vector_1, const Point* vector_2)
{
    return (vector_1->x * vector_2->x + vector_1->y * vector_2->y)
            / (vector_len(vector_1) + vector_len(vector_2));
}

double find_height_of_triangle(const Point* a, const Point* b, const Point* c)
{
    // a, b - segment`s cords, c - point`s cords
    double ab = two_points_distance(a, b);
    double ac = two_points_distance(a, c);
    double bc = two_points_distance(b, c);
    double p = (ab + ac + bc) / 2.0;

    double height = 2.0 * sqrt(p * (p - ab) * (p - ac) * (p - bc)) / ab;
    return height;
}

double point_segment_distance(const Point* a, const Point* b, const Point* c)
{
    // a, b - segment`s cords, c - point`s cords
    Point vector_ba = {a->x - b->x, a->y - b->y};
    Point vector_ab = {b->x - a->x, b->y - a->y};
    Point vector_bc = {c->x - b->x, c->y - b->y};
    Point vector_ac = {c->x - a->x, c->y - a->y};

    double abc_cos = find_cos(&vector_ba, &vector_bc);
    double bac_cos = find_cos(&vector_ab, &vector_ac);

    if (bac_cos <= 0) {
        return two_points_distance(a, c);
    } else if (abc_cos <= 0) {
        return two_points_distance(b, c);
    } else {
        return find_height_of_triangle(a, b, c);
    }
}

int is_collision_figure_with_circle(
        const Circle* circle, const Point* cords, size_t size)
{
    for (size_t i = 1; i < size; i++) {
        double distance = point_segment_distance(
                &cords[i - 1], &cords[i], &(circle->position));

        if (distance <= circle->radius) {
            return 1;
        }
    }
    return 0;
}

int is_collision_circles(const Circle* circle1, const Circle* circle2)
{
    double distance
            = two_points_distance(&(circle1->position), &(circle2->position));
    return distance <= (circle1->radius + circle2->radius);
}

int is_collision_figures(
        const Point* cords1,
        const size_t size1,
        const Point* cords2,
        const size_t size2)
{
    for (size_t i = 0; i < size1 - 1; i++) {
        for (size_t j = 0; j < size2 - 1; j++) {
            Point segment1[2] = {cords1[i], cords1[i + 1]};
            Point segment2[2] = {cords2[j], cords2[j + 1]};
            if (is_segment_collision(segment1, segment2)) {
                return 1;
            }
        }
    }
    return 0;
}

void print_figure(const void* figure, const char* type)
{
    printf("%s(", type);

    if (strcmp(type, CIRCLE) == 0) {
        printf("%lf %lf, %lf",
               ((Circle*)figure)->position.x,
               ((Circle*)figure)->position.y,
               ((Circle*)figure)->radius);

    } else if (strcmp(type, TRIANGLE) == 0) {
        printf("(");
        for (int n = 0; n < TRIANGLE_TOKENS_AMOUNT; n++) {
            printf("%f %f",
                   ((Triangle*)figure)->cords[n].x,
                   ((Triangle*)figure)->cords[n].y);
            if (n < TRIANGLE_TOKENS_AMOUNT - 1) {
                printf(", ");
            }
        }
        printf(")");

    } else if (strcmp(type, POLYGON) == 0) {
        printf("(");
        for (int n = 0; n < ((Polygon*)figure)->size; n++) {
            printf("%f %f", ((Polygon*)figure)->cords[n].x, ((Polygon*)figure)->cords[n].y);
            if (n < ((Polygon*)figure)->size - 1) {
                printf(", ");
            }
        }
        printf(")");
    }
    printf(")\n");
}

void print_figure_parametrs(const void* figure, const char* type)
{
    double perimetr;
    double surface;
    if (strcmp(type, CIRCLE) == 0) {
        perimetr = circle_perimetr(figure);
        surface = circle_surface(figure);
    } else if (strcmp(type, TRIANGLE) == 0) {
        perimetr = triangle_perimetr(figure);
        surface = triangle_surface(figure);
    } else if (strcmp(type, POLYGON) == 0) {
        perimetr = polygon_perimetr(figure);
        surface = polygon_surface(figure);
    }

    printf("\tPerimetr = %lf\n", perimetr);
    printf("\tSurface = %lf\n", surface);
}

// void calculate_and_print_collision(
//         const void* all_figures, const void* sizes, const char* type)
// {
//     if (srtcmp(type, CIRCLE) == 0) {
//         for (int i = 0; i < sizes[0]; i++) { //size[0] - circles_amount
//             Circle circle = circles[i];
//             print_figure(&circle, CIRCLE);
//             print_figure_parametrs(&circle, CIRCLE);
//             printf("\tintersects:\n");

//             for (int j = 0; j < sizes[0]; j++) {
//                 if (j == i)
//                     continue;
//                 if (is_collision_circles(circle, circles[j])) {
//                     print_figure(&(circles[j]), CIRCLE);
//                 }
//                 for (int j = 0; j < sizes[1]; j++) {
//                     if (is_collision_figure_with_circle(
//                                 circle,
//                                 triangles[j].cords,
//                                 TRIANGLE_TOKENS_AMOUNT)) {
//                         print_figure(&(triangles[j]), TRIANGLE);
//                     }
//                 }
//                 for (int j = 0; j < sizes[2]; j++) {
//                     if (is_collision_figure_with_circle(
//                                 circle, polygons[j].cords, polygons[j].size))
//                                 {
//                         print_figure(&(polygons[j]), POLYGON);
//                     }
//                 }
//             }
//         }
//     }
// }