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

CTEST(collision_suite, two_figures_true)
{
    Point p1 = {0, 0};
    Point p2 = {4, 0};
    Point p3 = {4, 4};
    Point p4 = {0, 4};
    Point pcords[] = {p1, p2, p3, p4, p1};

    Point t1 = {2, 2};
    Point t2 = {4, 2};
    Point t3 = {4, 4};
    Point tcords[] = {t1, t2, t3, t1};

    ASSERT_TRUE(is_collision_figures(pcords, 5, tcords, 4));
}

CTEST(collision_suite, two_figures_false)
{
    Point p1 = {0, 0};
    Point p2 = {4, 0};
    Point p3 = {4, 4};
    Point p4 = {0, 4};
    Point pcords[] = {p1, p2, p3, p4, p1};

    Point t1 = {5, 5};
    Point t2 = {6, 5};
    Point t3 = {6, 6};
    Point tcords[] = {t1, t2, t3, t1};

    ASSERT_FALSE(is_collision_figures(pcords, 5, tcords, 4));
}

CTEST(collision_suite, two_circles_true)
{
    Point p1 = {0, 0};
    Point p2 = {0, 5};

    Circle circle1 = {p1, 3};
    Circle circle2 = {p2, 2};

    ASSERT_TRUE(is_collision_circles(circle1, circle2));
}

CTEST(collision_suite, two_circles_false)
{
    Point p1 = {0, 0};
    Point p2 = {0, 5};

    Circle circle1 = {p1, 2};
    Circle circle2 = {p2, 2};

    ASSERT_FALSE(is_collision_circles(circle1, circle2));
}

CTEST(collision_suite, figure_with_circle_true)
{
    Point center = {0, 0};
    Circle circle = {center, 3};

    Point t1 = {1, 0};
    Point t2 = {6, 0};
    Point t3 = {6, 6};
    Point tcords[] = {t1, t2, t3, t1};


    ASSERT_TRUE(is_collision_figure_with_circle(circle, tcords, 4));
}

CTEST(collision_suite, figure_with_circle_false)
{
    Point center = {0, 0};
    Circle circle = {center, 3};

    Point t1 = {5, 5};
    Point t2 = {6, 5};
    Point t3 = {6, 6};
    Point tcords[] = {t1, t2, t3, t1};


    ASSERT_FALSE(is_collision_figure_with_circle(circle, tcords, 4));
}

CTEST(collision_suite, segments_true)
{
    Point p1 = {0, 0};
    Point p2 = {5, 0};
    Point p3 = {2.5, 3};
    Point p4 = {2, -1.5};

    Point segment1[] = {p1, p2};
    Point segment2[] = {p3, p4};
    Point segment3[] = {p1, p3};
    Point segment4[] = {p1, p4};

    ASSERT_TRUE(is_segment_collision(segment1, segment2));
    ASSERT_TRUE(is_segment_collision(segment1, segment3));
    ASSERT_TRUE(is_segment_collision(segment1, segment4));
    ASSERT_TRUE(is_segment_collision(segment1, segment1));
}

CTEST(collision_suite, segments_false)
{
    Point p1 = {0, 0};
    Point p2 = {5, 0};
    Point p3 = {2.5, 3};
    Point p4 = {2, -1.5};

    Point segment1[] = {p1, p3};
    Point segment2[] = {p2, p4};

    ASSERT_FALSE(is_segment_collision(segment1, segment2));
}
