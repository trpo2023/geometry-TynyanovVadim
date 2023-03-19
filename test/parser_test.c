#include <ctest.h>
#include <math.h>

#include "libgeometry/parser.h"
#include "libgeometry/lexer.h"

CTEST(surface_suite, circle)
{
    Point p = {0, 0};
    Circle circle = {p, 5};
    double expected_surface = (M_PI * circle.radius * circle.radius);
    double real_surface = circle_surface(circle);
    ASSERT_DBL_NEAR(expected_surface, real_surface);
}

CTEST(surface_suite, polygon)
{
    Point p1 = {0, 0};
    Point p2 = {4, 0};
    Point p3 = {4, 4};
    Point p4 = {0, 4};
    Point cords[] = {p1, p2, p3, p4, p1};
    Polygon polygon = {cords, 5};
    double expected_surface = 16;
    double real_surface = polygon_surface(polygon);
    ASSERT_DBL_NEAR(expected_surface, real_surface);
}

CTEST(surface_suite, triangle)
{
    Point p1 = {0, 0};
    Point p2 = {3, 0};
    Point p3 = {3, 4};
    Point cords[] = {p1, p2, p3, p1};
    Triangle triangle = {cords};
    double expected_surface = 6;
    double real_surface = triangle_surface(triangle);
    ASSERT_DBL_NEAR(expected_surface, real_surface);
}

CTEST(perimetr_suite, circle)
{
    Point p = {0, 0};
    Circle circle = {p, 5};
    double expected_perimetr = (2 * M_PI * circle.radius);
    double real_perimetr = circle_perimetr(circle);
    ASSERT_DBL_NEAR(expected_perimetr, real_perimetr);
}

CTEST(perimetr_suite, polygon)
{
    Point p1 = {0, 0};
    Point p2 = {4, 0};
    Point p3 = {4, 4};
    Point p4 = {0, 4};
    Point cords[] = {p1, p2, p3, p4, p1};
    Polygon polygon = {cords, 5};
    double expected_perimetr = 16;
    double real_perimetr = polygon_perimetr(polygon);
    ASSERT_DBL_NEAR(expected_perimetr, real_perimetr);
}

CTEST(perimetr_suite, triangle)
{
    Point p1 = {0, 0};
    Point p2 = {3, 0};
    Point p3 = {3, 4};
    Point cords[] = {p1, p2, p3, p1};
    Triangle triangle = {cords};
    double expected_perimetr = 12;
    double real_perimetr = triangle_perimetr(triangle);
    ASSERT_DBL_NEAR(expected_perimetr, real_perimetr);
}
