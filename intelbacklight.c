#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INTELBLPATH "/sys/class/backlight/intel_backlight"
#define VALFILE INTELBLPATH "/brightness"
#define MAXFILE INTELBLPATH "/max_brightness"

int readFileData(int *data, char *filename) {
    FILE *file;
    if(!(file = fopen(filename, "r"))) {
        fprintf(stderr, "error: failed to open the file: %s", filename);
        exit(1);
    }
    fscanf(file, "%d", data);
    fclose(file);
    return 0;
}

int wirteFileData(int data, char *filename) {
    FILE *file;
    if((file = fopen(filename, "w"))) {
        fprintf(file, "%d", data);
        fclose(file);
    } else {
        fprintf(stderr, "error: failed to write to the file %s\n", filename);
        exit(1);
    }
    return 0;
}

int processInput(int maxval, char** argv, int argc, int* data) {
    if (argc < 3) {
        fprintf(stderr, "error: you must provide a percentage\n");
        exit(1);
    }
    float inputval;
    if (sscanf(argv[2], "%f", &inputval) != 1) {
        fprintf(stderr, "error: please provide a percentage between 0.0 and 100, you gave: %s\n", argv[2]);
        exit(1);
    }
    if (inputval < 0 || inputval > 100) {
        fprintf(stderr, "error: please provide a percentage between 0.0 and 100, you gave: %F\n", inputval);
        exit(1);
    }
    *data = (((float) inputval / 100 ) * maxval);
    return 0;
}

int main(int argc, char** argv) {

    if (argc < 2 || !strcmp("--help", argv[1])) {
        printf("usage: intelbacklight [arguments]\n arguments:\n -get\n -set <percentage>\n -inc <percentage>\n -dec <percentage>\n --help\n --version\n");
        return 0;
    } else if (!strcmp("--version", argv[1])) {
        printf("intelbacklight 0.1 <harrison@rodgers.io>\n");
        return 0;
    }

    int current = 0, maxval = 0, input = 0;
    readFileData(&current, VALFILE);
    readFileData(&maxval, MAXFILE);

    if (!strcmp("-get", argv[1])) {
        printf("%F\n", ((float) current / maxval) * 100);
        return 0;
    }

    processInput(maxval, argv, argc, &input);
    if (!strcmp("-set", argv[1])) {
        current = input;
    } else if (!strcmp("-inc", argv[1])) {
        current += input;
    } else if (!strcmp("-dec", argv[1])) {
        current = input > current ? 0 : current-input;
    } else {
        fprintf(stderr, "error: unknown flag: %s\n", argv[1]);
        return 1;
    }

    wirteFileData(current, VALFILE);
    return 0;
}
