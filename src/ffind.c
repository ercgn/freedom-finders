#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csapp.h"
#include "qsort.h"




void usage() {
}

int main(int argc, char **argv) {
    char *file_list[argc-1];
    int i;

    // Check for valid argument numbers
    if (argc == 1) {
        fprintf(stderr, "Usage: %s [<file1>, <file2>, ...]", argv[0]);
        exit(0);
    }

    // add file string to file array 
    for (i = 1; i < argc; i++) {
        if (strstr(argv[i], ".ics") == NULL) 
            fprintf(stderr, "Invalid format for file %s. Please use *.ics",
                    argv[i]);
        file_list[i] = argv[i];
    }

    parse
    return 0;
}