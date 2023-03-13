#pragma once

#define CIRCLE "circle"
#define TRIANGLE "triangle"
#define POLYGON "polygon"

#define CIRCLE_TOKENS_AMOUNT 2
#define TRIANGLE_TOKENS_AMOUNT 4
#define POLYGON_TOKENS_AMOUNT 4

#define POLYGON_TRIANGLE_BRACKETS 2
#define CIRCLE_BRACKETS 1

#define ERROR_FIGURE_NOT_FOUND "expected 'circle', 'triangle' or 'polygon'"
#define ERROR_OPEN_BRACKER_NOT_FOUND "expected '('"
#define ERROR_CLOSE_BRACKER_NOT_FOUND "expected ')'"
#define ERROR_WRONG_ARGUMENT "expected '<double>'"
#define ERROR_UNEXPECTED_TOKEN "unexpected token"
#define ERROR_INVALID_ARGUMENTS_TYPE "invalid arguments"
#define ERROR_TOO_MANY_ARGUMENTS "too many arguments"
#define ERROR_NEGATIVE_RADIUS "raduis can not be negative"
#define ERROR_EXCEPTED_TWO_BRACKETS "expected two brackets"
#define ERROR_EXCEPTED_ONE_BRACKETS "expected one brackets"
#define ERROR_FIRST_AND_LAST_NOT_EQUALS "first and last point are not equals"

void message_error(char* string, int column, char* message);
int find_figure(char* line, char* figure);
int check_brackets(char* line, int len);
int check_unexpected_token(char* line, int len);
int is_argument_correct(char* line);
int is_syntax_correct(char* line, int len);