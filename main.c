#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void message_error(char* string, int column, char* message)
{
    int len = strlen(string);
    printf("%s", string);
    if (string[len - 1] != '\n') {
        printf("\n");
    }
    printf("%*c\n", column, '^');
    printf("Error at column %d: %s\n", column, message);
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("No file specified\n");
        return -1;
    }

    FILE* file = fopen(argv[1], "r");
    char* line = NULL; // Open file
    size_t len = 0;

    if (file == NULL) {
        printf("File not found\n");
        return -1;
    }

    return 0;
}
