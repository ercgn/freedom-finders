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
    char *STARTTIME = "2014020102390";

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

    //TODO this is just a test; needs to be changed
    events = parseICS(file_list[0], &numEvents); 

    //TODO create an array to quicksort

    //TODO output the free time slots
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

    if (strstr(str, "Z") != NULL) {
        sscanf(str, "%[^T]T%[^Z]%*s", date, hhmmss);
    } else {
        sscanf(str, "%[^T]T%s", date, hhmmss);
    }
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

    event *events = Calloc(sizeof(struct event), MAXLINE);
    bool seenEndFlag = true;
    int i = 0;

    while((n = Rio_readlineb(&rio, line, MAXLINE)) != 0) {
        strcpy(date, "");
        if (strncmp(line, "BEGIN:VEVENT", 12) == 0) {
            // Create new event and increase counter
            if (seenEndFlag) {
                event e = Malloc(sizeof(struct event));
                e->start = Calloc(sizeof(char), MAXLINE);
                e->end = Calloc(sizeof(char), MAXLINE);
                e->rrule = Calloc(sizeof(char), MAXLINE);
                events[i] = e;
                seenEndFlag = false;
            }
            else {
                printf("COULD NOT READ FILE OH NO\n");
            }
        }
        else if (strncmp(line, "DTSTART", 7) == 0) {
            // events[i-1] because we i++ after we created event
            if (!seenEndFlag) {
                sscanf(line, "%*[^:]:%s", date);
                strcpy(events[i]->start, date);
            }
        }
        else if (strncmp(line, "DTEND", 5) == 0) {
            if (!seenEndFlag) {
                sscanf(line, "%*[^:]:%s", date);
                strcpy(events[i]->end, date);    
            }
        }
        else if (strncmp(line, "RRULE", 5) == 0) {
            if (!seenEndFlag) {
                sscanf(line, "RRULE:%s", date);
                strcpy(events[i]->rrule, date);
            }
        }
        else if (strncmp(line, "END:VEVENT", 10) == 0) {
            seenEndFlag = true;
            i++;
        }
    }
    close(fd);
    *numEvents = i;
    return events;
}

void printEventArray(event *events, int n) {
//    printf("Printing array...\n");
    for (int i = 0; i < n; i++) {
        printf("    %d: start: %s\n", i, events[i]->start);
        printf("       end:   %s\n", events[i]->end);
        printf("       rrule: %s\n", events[i]->rrule);
    }
}

void freeEvents(event *events, int n) {
    for (int i = 0; i < n; i++) {
        Free(events[i]->start);
        Free(events[i]->end);
        Free(events[i]->rrule);
        Free(events[i]);
    }
    Free(events);
}

events *getFreeTimes(long unsigned *times) {
    unsigned int i = 0;

}