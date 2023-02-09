#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("No file specified\n");
        return 0;
    }

    FILE* file = fopen(argv[1], "r");

    if (file == NULL) {
        printf("File not found\n");
        return -1;
    }
    return 0;
}
