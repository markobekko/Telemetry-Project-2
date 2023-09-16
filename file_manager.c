#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

const char FILE_PATH[] = "/home/marko/Desktop/Eagle/project_2/candump.log";

int get_rows_of_file(){
    FILE *file = fopen(FILE_PATH, "r");

    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    int lineCount = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lineCount++;
        }
    }

    if (ch != '\n' && lineCount > 0) {
        lineCount++;
    }

    fclose(file);

    return lineCount;
}

int get_start_stop_occurences() {
    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: File not found or cannot be opened.\n");
        return -1;
    }

    char line[1024];
    int num = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "// Start") || strstr(line, "// Stop")) {
            num++;
        }
    }

    fclose(file);

    return num;
}
