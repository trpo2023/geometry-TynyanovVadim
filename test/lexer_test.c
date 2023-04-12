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

    ASSERT_TRUE(check_brackets(ln1, strlen(ln1)));
    ASSERT_TRUE(check_brackets(ln2, strlen(ln2)));
    ASSERT_TRUE(check_brackets(ln3, strlen(ln3)));
}

CTEST(lexer_siute, check_brackets_false)
{
    char* ln1 = "circle(0 0, 5))";
    char* ln2 = "triangle(1 1, 5 5, 0 0, 1 1)";
    char* ln3 = "polygon(1 1, 5 5, 0 0, 2 2, 1 1))(";

    ASSERT_FALSE(check_brackets(ln1, strlen(ln1)));
    ASSERT_FALSE(check_brackets(ln2, strlen(ln2)));
    ASSERT_FALSE(check_brackets(ln3, strlen(ln3)));
}

CTEST(lexer_siute, unexpected_token_true)
{
    char* ln1 = "circle(0 0, 5)";
    char* ln2 = "triangle((1 1, 5 5, 0 0, 1 1))";
    char* ln3 = "polygon((1 1, 5 5, 0 0, 2 2, 1 1))";

    ASSERT_TRUE(check_unexpected_token(ln1, strlen(ln1)));
    ASSERT_TRUE(check_unexpected_token(ln2, strlen(ln2)));
    ASSERT_TRUE(check_unexpected_token(ln3, strlen(ln3)));
}

CTEST(lexer_siute, unexpected_token_false)
{
    char* ln1 = "circle(0 0, 5) 11";
    char* ln2 = "triangle((1 1, 5 5, 0 0, 1 1)) ()()";
    char* ln3 = "polygon((1 1, 5 5, 0 0, 2 2, 1 1)) ,;";

    ASSERT_FALSE(check_unexpected_token(ln1, strlen(ln1)));
    ASSERT_FALSE(check_unexpected_token(ln2, strlen(ln2)));
    ASSERT_FALSE(check_unexpected_token(ln3, strlen(ln3)));
}

CTEST(lexer_siute, is_argument_correct_true)
{
    char* ln1 = "circle(0 0, 5)";
    char* ln2 = "triangle((1 1, 5 5, 0 0, 1 1))";
    char* ln3 = "polygon((1 1, 5 5, 0 0, 2 2, 1 1))";

    ASSERT_TRUE(is_argument_correct(ln1));
    ASSERT_TRUE(is_argument_correct(ln2));
    ASSERT_TRUE(is_argument_correct(ln3));
}

CTEST(lexer_siute, is_argument_correct_false)
{
    char* ln1 = "circle(0 d, 5)";
    char* ln2 = "triangle((1 1, 5 5, 0 0, 1 1, 0 0))";
    char* ln3 = "polygon((1 1, 5 5, 1 1))";

    ASSERT_FALSE(is_argument_correct(ln1));
    ASSERT_FALSE(is_argument_correct(ln2));
    ASSERT_FALSE(is_argument_correct(ln3));
}

CTEST(lexer_siute, is_syntax_correct_true)
{
    char* ln1 = "circle(0 0, 5)";
    char* ln2 = "triangle((1 1, 5 5, 0 0, 1 1))";
    char* ln3 = "polygon((1 1, 5 5, 0 0, 2 2, 1 1))";

    ASSERT_TRUE(is_syntax_correct(ln1, strlen(ln1)));
    ASSERT_TRUE(is_syntax_correct(ln2, strlen(ln2)));
    ASSERT_TRUE(is_syntax_correct(ln3, strlen(ln3)));
}

CTEST(lexer_siute, is_syntax_correct_false)
{
    char* ln1 = "circle(0 d, 5)";
    char* ln2 = "triangle((1 1, 5 5, 0 0, 1 1, 0 0))";
    char* ln3 = "polygon((1 1, 5 5, 0 0, 1 d))";

    ASSERT_FALSE(is_syntax_correct(ln1, strlen(ln1)));
    ASSERT_FALSE(is_syntax_correct(ln2, strlen(ln2)));
    ASSERT_FALSE(is_syntax_correct(ln3, strlen(ln3)));
}