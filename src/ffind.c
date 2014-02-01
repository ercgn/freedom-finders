#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "csapp.h"
#include "ffind.h"


/* Function Prototypes */
unsigned long event_stolu(char *str, bool isStart);
void event_lutos(unsigned long encoding, char *str, bool *isStart);
event* parseICS(char *file, unsigned int *numEvents);
void printEventArray(event* events, int n);
event *getFreeTimes(long unsigned *times);
//void parseRRULE(char *rrule, )
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


// - date to hash - yyyyMMddthhmmss ->sscanf -> yyyyMMddhhmmb (double)
// - hash to date - yyyyMMddhhmmb -> yyyyMMddthhmmss (idk how)

/* 
 * assumes yyyymmddThhmmss format, from ics standard 
 * true = start
 * false = end
 */
unsigned long event_stolu(char *str, bool isStart) {
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
    if (isStart) {
        strcat(lu_str, "0");
    } else {
        strcat(lu_str, "1");
    }
    sscanf(lu_str, "%lu", &encoding);
    return encoding;
}

void event_lutos(unsigned long encoding, char *str, bool *isStart) {
    char str_enc[MAXLINE];
    char buf[MAXLINE];
    char date[MAXLINE];
    char hhmm[MAXLINE];
    char start[1];
    strcpy(str, "");

    sprintf(str_enc, "%lu", encoding);
    strncpy(date, str_enc, 8);
    strncpy(hhmm, str_enc+8, 4);
    strcpy(start, str_enc+12);
    date[8] = '\0';
    hhmm[12] = '\0';

    strcat(buf, date);
    strcat(buf, "T");
    strcat(buf, hhmm);
    strcat(buf, "00");

    if (atoi(start) == 0) {
        *isStart = true;
    } else {
        *isStart = false;
    }

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
    printf("Printing array...\n");
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

event *getFreeTimes(long unsigned *times) {
    unsigned int i = 0;

}