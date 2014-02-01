#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csapp.h"


int main(int argc, char **argv) {
    char *file_list[argc-1];
    int i;


    // Check for valid argument numbers
    if (argc == 1) {
        fprintf(stderr, "Usage: %s [<file1>, <file2>, ...]\n", argv[0]);
        exit(0);
    }

    // add file string to file array 
    for (i = 1; i < argc; i++) {
        if (strstr(argv[i], ".ics") == NULL) 
            fprintf(stderr, "Invalid format for file %s. Please use *.ics\n",
                    argv[i]);
        file_list[i] = argv[i];
    }

    unsigned long test = 2147483647111;
    unsigned long test2;
    char *test3 = "There are 2147483647111 cats";

    sscanf(test3, "There are %lu cats", &test2);

    if (test2 == test) {
        printf("Hallelujah! %lu = %lu!\n", test, test2);
    }


    //event_list = parse(&file_list);
    return 0;
}