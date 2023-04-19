#include <ctest.h>
#include <string.h>

#include "libgeometry/lexer.h"

CTEST(lexer_siute, find_figure_true)
{
    char* circle = "circle";
    char* triangle = "TRiangLE";
    char* polygon = "POLYGON";

    ASSERT_TRUE(find_figure(circle, CIRCLE));
    ASSERT_TRUE(find_figure(triangle, TRIANGLE));
    ASSERT_TRUE(find_figure(polygon, POLYGON));
}

CTEST(lexer_siute, find_figure_false)
{
    char* circle = "circlllle";
    char* triangle = "circle";
    char* polygon = "triangle";

    ASSERT_FALSE(find_figure(circle, CIRCLE));
    ASSERT_FALSE(find_figure(triangle, TRIANGLE));
    ASSERT_FALSE(find_figure(polygon, POLYGON));
}

CTEST(lexer_siute, check_brackets_true)
{
    char* ln1 = "circle(0 0, 5)";
    char* ln2 = "triangle((1 1, 5 5, 0 0, 1 1))";
    char* ln3 = "polygon((1 1, 5 5, 0 0, 2 2, 1 1))";

    char* error1;
    char* error2;
    char* error3;

    check_brackets(ln1, strlen(ln1), &error1);
    check_brackets(ln2, strlen(ln2), &error2);
    check_brackets(ln3, strlen(ln3), &error3);

    ASSERT_NULL(error1);
    ASSERT_NULL(error2);
    ASSERT_NULL(error3);
}

CTEST(lexer_siute, check_brackets_false)
{
    char* ln1 = "circle(0 0, 5))";
    char* ln2 = "triangle(1 1, 5 5, 0 0, 1 1)";
    char* ln3 = "polygon(1 1, 5 5, 0 0, 2 2, 1 1))(";

    char* error1;
    char* error2;
    char* error3;

    check_brackets(ln1, strlen(ln1), &error1);
    check_brackets(ln2, strlen(ln2), &error2);
    check_brackets(ln3, strlen(ln3), &error3);

    ASSERT_NOT_NULL(error1);
    ASSERT_NOT_NULL(error2);
    ASSERT_NOT_NULL(error3);
}

CTEST(lexer_siute, unexpected_token_true)
{
    char* ln1 = "circle(0 0, 5)";
    char* ln2 = "triangle((1 1, 5 5, 0 0, 1 1))";
    char* ln3 = "polygon((1 1, 5 5, 0 0, 2 2, 1 1))";

    char* error1;
    char* error2;
    char* error3;

    check_unexpected_token(ln1, strlen(ln1), &error1);
    check_unexpected_token(ln2, strlen(ln2), &error2);
    check_unexpected_token(ln3, strlen(ln3), &error3);

    ASSERT_NULL(error1);
    ASSERT_NULL(error2);
    ASSERT_NULL(error3);
}

CTEST(lexer_siute, unexpected_token_false)
{
    char* ln1 = "circle(0 0, 5) 11";
    char* ln2 = "triangle((1 1, 5 5, 0 0, 1 1)) ()()";
    char* ln3 = "polygon((1 1, 5 5, 0 0, 2 2, 1 1)) ,;";

    char* error1;
    char* error2;
    char* error3;

    check_unexpected_token(ln1, strlen(ln1), &error1);
    check_unexpected_token(ln2, strlen(ln2), &error2);
    check_unexpected_token(ln3, strlen(ln3), &error3);

    ASSERT_NOT_NULL(error1);
    ASSERT_NOT_NULL(error2);
    ASSERT_NOT_NULL(error3);
}

CTEST(lexer_siute, is_argument_correct_true)
{
    char* ln1 = "circle(0 0, 5)";
    char* ln2 = "triangle((1 1, 5 5, 0 0, 1 1))";
    char* ln3 = "polygon((1 1, 5 5, 0 0, 2 2, 1 1))";

    char* error1;
    char* error2;
    char* error3;

    is_argument_correct(ln1, &error1);
    is_argument_correct(ln2, &error2);
    is_argument_correct(ln3, &error3);

    ASSERT_NULL(error1);
    ASSERT_NULL(error2);
    ASSERT_NULL(error3);
}

CTEST(lexer_siute, is_argument_correct_false)
{
    char* ln1 = "circle(0 d, 5)";
    char* ln2 = "triangle((1 1, 5 5, 0 0, 1 1, 0 0))";
    char* ln3 = "polygon((1 1, 5 5, 1 1))";

    char* error1;
    char* error2;
    char* error3;

    is_argument_correct(ln1, &error1);
    is_argument_correct(ln2, &error2);
    is_argument_correct(ln3, &error3);

    ASSERT_NOT_NULL(error1);
    ASSERT_NOT_NULL(error2);
    ASSERT_NOT_NULL(error3);
}
