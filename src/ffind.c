#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "csapp.h"
#include "ffind.h"

/* Function Prototypes */
unsigned long event_stolu(char *str);
void event_lutos(unsigned long encoding, char *str);
/* End Function Prototypes */

int main(int argc, char **argv) {
    char *file_list[argc-1];
//    char tStamp[MAXLINE];
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
    char date[MAXLINE];
    char hhmmss[MAXLINE];
    char lu_str[MAXLINE];
    unsigned long encoding;

    sscanf(str, "%[^T]T%s", &date, &hhmmss);
    strcat(lu_str, date);
    strcat(lu_str, hhmmss);
    sscanf(lu_str, "%lu", &encoding);
    return encoding;
}

void event_lutos(unsigned long encoding, char *str) {
    char str_enc[MAXLINE];
    char buf[MAXLINE];
    char date[MAXLINE];
    char hhmm[MAXLINE];
    strcpy(str, "");

    sprintf(str_enc, "%lu", encoding);
    strncpy(date, str_enc, 8);
    strcpy(hhmm, str_enc+8);
    date[8] = '\0';

    strcat(buf, date);
    strcat(buf, "T");
    strcat(buf, hhmm);
    strcat(buf, "00");

    strcpy(str, buf);
}

void *parseICS(char *file) {
    char line[MAXLINE];
    char date[MAXLINE];

    int fd = open(file, 'r');
    rio_t rio;
    Rio_readinitb(&rio, fd);

    event *events = Calloc(MAXLINE, sizeof(event));
    bool seenEndFlag = true;
    int i = 0;

    while((size_t n = Rio_readlineb(&rio, line, MAXLINE)) != 0) {
        if (strncmp(line, "BEGIN:VEVENT", 12) == 0) {
            // Create new event and increase counter
            if (seenEndFlag) {
                event *e = Malloc(sizeof(event));
                events[i] = e;
                seenEndFlag = false;
                i++;
            }
            else {
                printf("COULD NOT READ FILE OH NO\n")
            }
        }}
        else if (strncmp(line, "DTSTART", 7) == 0) {
            // events[i-1] because we i++ after we created event
            sscanf(line, "%[^:]:%s", &date);
            events[i-1].start = date;
        }
        else if (strncmp(line, "DTEND", 5) == 0) {
            sscanf(line, "%[^:]:%s", &date);
            events[i-1].end = date;
        }
        else if (strncmp(line, "RRULE", 5) == 0) {

        }
        else if (strncmp(line, "END:VEVENT", 10) == 0) {
            seenEndFlag = true;
        }
    }
    close(fd);
}