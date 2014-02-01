#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "csapp.h"
#include "ffind.h"

/* Function Prototypes */
unsigned long event_stolu(char *str);
void event_lutos(unsigned long encoding, char *str);
event* parseICS(char *file, unsigned int *numEvents);
void printEventArray(event* events, int n);
/* End Function Prototypes */

void testfun(int *i) {
    *i = 1000;
}

int main(int argc, char **argv) {
    char *file_list[argc-1];
//    char tStamp[MAXLINE];
    int i;
    unsigned int numEvents;

    event* events;

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
        strcpy(file_list[i], argv[i]);
    }

    // char *test = "20140131T090000";
    // unsigned long test2 = event_stolu(test);
    // char test3[MAXLINE];
    // event_lutos(test2, test3);

    // printf("%s\n%lu\n%s\n", test, test2, test3);

    //TODO this is just a test; needs to be changed
    events = parseICS(file_list[0], &numEvents); 

    printEventArray(events, numEvents);

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


    sscanf(str, "%[^T]T%s", date, hhmmss);
    strcat(lu_str, date);
    hhmmss[4] = '\0';
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

event* parseICS(char *file, unsigned int *numEvents) {
    char line[MAXLINE];
    char date[MAXLINE];

    int fd = Open(file, 'r', DEF_MODE & ~DEF_UMASK);
    rio_t rio;
    Rio_readinitb(&rio, fd);
    size_t n;

    event *events = Calloc(sizeof(event), MAXLINE);
    bool seenEndFlag = true;
    int i = 0;

    while((n = Rio_readlineb(&rio, line, MAXLINE)) != 0) {
        if (strncmp(line, "BEGIN:VEVENT", 12) == 0) {
            // Create new event and increase counter
            if (seenEndFlag) {
                event e = Malloc(sizeof(event));
                e->start = Calloc(sizeof(char), MAXLINE);
                e->end = Calloc(sizeof(char), MAXLINE);
                e->rrule = Calloc(sizeof(char), MAXLINE);
                events[i] = e;
                seenEndFlag = false;
                i++;
            }
            else {
                printf("COULD NOT READ FILE OH NO\n");
            }
        }
        else if (strncmp(line, "DTSTART", 7) == 0) {
            // events[i-1] because we i++ after we created event
            sscanf(line, "%*[^:]:%s", date);

            strcpy(events[i-1]->start, date);
        }
        else if (strncmp(line, "DTEND", 5) == 0) {
            sscanf(line, "%*[^:]:%s", date);
            strcpy(events[i-1]->end, date);
        }
        else if (strncmp(line, "RRULE", 5) == 0) {
            sscanf(line, "%*[^:]:%s", date);
            strcpy(events[i-1]->rrule, date);
        }
        else if (strncmp(line, "END:VEVENT", 10) == 0) {
            seenEndFlag = true;
        }
    }
    close(fd);
    *numEvents = i;
    return events;
}

void printEventArray(event* events, int n) {
    printf("Printing array...\n");
    for (int i = 0; i < n; i++) {
        printf("    %d: start: %s\n", i, events[i]->start);
        printf("       end:   %s\n", events[i]->end);
        printf("       rrule: %s\n", events[i]->rrule);
    }
}