#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffind.h"
#include "csapp.h"

#define MAXLINE 8192

/* Function Prototypes */
unsigned long event_stolu(char *str);
void event_lutos(unsigned long encoding, char *str);
/* End Function Prototypes */

int main(int argc, char **argv) {
    char *file_list[argc-1];
    char tStamp[MAXLINE];
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


    //event_list = parse(&file_list);
    return 0;
}


// - date to hash - yyyyMMddthhmmss ->sscanf -> yyyyMMddhhmm (double)
// - hash to date - yyyyMMddhhmm -> yyyyMMddthhmmss (idk how)

/* 
 * assumes yyyymmddThhmmss format, from ics standard 
 */
unsigned long event_stolu(char *str) {
    char date[MAXSIZE];
    char hhmmss[MAXSIZE];
    char lu_str[MAXSIZE];
    unsigned long encoding;

    sscanf(str, "%[^/T]T%s", &date, &hhmmss);
    strcat(lu_str, date);
    strcat(lu_str, hhmmss);
    sscanf(lu_str, "%lu", encoding);
    return encoding;
}

void event_lutos(unsigned long encoding, char *str) {
    char str_enc[MAXSIZE];
    char buf[MAXSIZE];
    char date[MAXSIZE];
    char hhmm[MAXSIZE];
    strcpy(str, "");

    sprintf(str_enc, "%lu", encoding);
    strncpy(date, str_enc, 8);
    strcpy(hhmm, str_enc+8);
    date[8] = '\0'

    strcat(buf, date);
    strcat(buf, "T");
    strcat(buf, hhmm);
    strcat(buf, "00");

    strcpy(str, buf);
}

void *parseICS(char *file) {
    char line[MAXLINE];

    fd = open(file, "r");
    rio_t rio;
    Rio_readinitb(&rio, fd);

    event *events = Calloc(MAXLINE, sizeof(event));
    bool seenEndFlag = true;
    int i = 0;

    while((n = Rio_readlineb(&rio, line, MAXLINE)) != 0) {
        if (strncmp(line, "BEGIN", 5) == 0) {
            
        }
        if (strncmp(line, "DTSTART", 7) == 0) {
            // set start
        }
        else if (strncmp(line, "DTEND", 5) == 0) {
            // set end
        }
        else if (strncmp(line, "RRULE", 5) == 0) {

        }
    }
}