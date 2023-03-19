#include <ctest.h>
#include <math.h>

#include "libgeometry/parser.h"
#include "libgeometry/lexer.h"

CTEST(surface_suite, surf_circle)
{
    Point p = {0, 0};
    Circle circle = {p, 5};
    double expected_surface = (M_PI * circle.radius * circle.radius);
    double real_surface = circle_surface(circle);
    ASSERT_DBL_NEAR(expected_surface, real_surface);
}

CTEST(surface_suite, surf_polygon)
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

CTEST(surface_suite, surf_triangle)
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
