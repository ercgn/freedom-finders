#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "csapp.h"
#include "ffind.h"

int main(int argc, char **argv) {

    file_list f_list = NULL;
    event_list e_list = NULL;
//    char tStamp[MAXLINE];
    int i;
    unsigned int numEvents;
    file_list cur;

    // Check for valid argument numbers
    if (argc == 1) {
        fprintf(stderr, "Usage: %s [<file1>, <file2>, ...]\n", argv[0]);
        exit(0);
    } else if (argc > 15) {
        fprintf(stderr, "Maximum number of calenders allowed is %d\n",
                MAXCAL);
    }

    // add file string to file array 
    for (i = 1; i < argc; i++) {
        if (strstr(argv[i], ".ics") == NULL) {
            fprintf(stderr, "Invalid format for file %s. Please use *.ics\n",
                    argv[i]);
        }
        f_list = fListInsert(f_list, argv[i]);
    }

    //TODO this is just a test; needs to be changed
    int count = 0;
    for (cur = f_list; cur != NULL; cur = cur->next) {
        printf("%d: %s\n", count, cur->file);
        count++;
    }
    e_list = eListInsert(e_list, parseICS(f_list->file, &numEvents));
    printEventArray(e_list->events, numEvents);
    createICSFile(e_list->events, numEvents);

    //TODO create an array from events to quicksort
    //TODO free events

    //TODO output the free time slots in the form of events

    //TODO create a ics file from array of events :<
    //TODO free events


    for (i = 0; i < MAXCAL; i++) {
        //insert free code here.        
    } 
    freeEventList(e_list);
    freeFileList(f_list);
    return 0;
}

// insertion is in front
file_list fListInsert(file_list f_list, char *file) {
    file_list new_node = Malloc(sizeof(struct file_list));
    new_node->file = Calloc(sizeof(char), MAXLINE);
    strcpy(new_node->file, file);
    new_node->next = f_list;
    return new_node;
}


// insertion is in front
event_list eListInsert(event_list e_list, event *events) {
    event_list new_node = Malloc(sizeof(struct event_list));
    new_node->events = events; 
    new_node->next = e_list;
    return new_node;
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

    if (isStart != NULL) {
        if (atoi(start) == 0) {
            *isStart = true;
        } else {
            *isStart = false;
        }
    }
    strcpy(str, buf);
}

event* parseICS(char *file, unsigned int *numEvents) {
    char line[MAXLINE];
    char date[MAXLINE];

    int fd = Open(file, O_RDONLY, DEF_MODE & ~DEF_UMASK);
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

void freeFileList(file_list f_list) {
    file_list cur;
    for (cur = f_list; cur != NULL; cur = cur->next) {
        Free(cur);
    }
}

void freeEventList(event_list e_list) {
    event_list cur;
    for (cur = e_list; cur != NULL; cur = cur->next) {
        Free(cur);
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


long unsigned STARTTIME = 2014020102390;
event *getFreeTimes(long unsigned *times, unsigned int n, unsigned int *m) {
    unsigned int count = 0;
    unsigned int event_pointer = 0;
    event *events = Calloc(sizeof(struct event), MAXLINE);

    for (int i = 0; i < n; i++) {
        if (count == 0) { //TODO: fix edge case when start time is actually in middle of event
            // The previous time interval was a free time
            event e = Malloc(sizeof(struct event));
            e->start = Calloc(sizeof(char), MAXLINE);
            e->end = Calloc(sizeof(char), MAXLINE);
            e->rrule = Calloc(sizeof(char), MAXLINE);

            if (i == 0) {
                event_lutos(STARTTIME, e->start, NULL);
            } else {
                event_lutos(times[i-1], e->start, NULL);
            }
            event_lutos(times[i], e->end, NULL);
            events[event_pointer] = e;
            event_pointer++;

            // Handle the time
            if (times[i] % 2 == 0) count++;
            else {
                printf("SOMETHING HORRIBLE HAS GONE WRONG, WE STOPPED TIME WHEN NOTHING WAS SCHEDULED\n");
                exit(0);
            }
        } else {
            if (times[i] % 2 == 0) count++;
            else count--;
        }
    }
    *m = event_pointer;
    return events;
}

void createICSFile(event *events, unsigned int n) {
    int fd = Open("free_times.ics", 
                   O_WRONLY | O_CREAT | O_TRUNC, 
                   DEF_MODE & ~DEF_UMASK);
    rio_t rio;
    Rio_readinitb(&rio, fd);
    char buf[MAXLINE];

    // Write header
    strcpy(buf, "BEGIN:VCALENDAR\nVERSION:2.0\n");
    for (int i = 0; i < n; i++) {
        sprintf(buf, "%sBEGIN:VEVENT\n", buf);
        sprintf(buf, "%sDTSTART:%s\n", buf, events[i]->start);
        sprintf(buf, "%sDTEND:%s\n", buf, events[i]->end);
        sprintf(buf, "%sSUMMARY:Free Time!\n", buf);
        sprintf(buf, "%sEND:VEVENT\n", buf);
    }
    //Write footer
    sprintf(buf, "%sEND:VCALENDAR\n", buf);

    Rio_writen(fd, buf, strlen(buf));
    close(fd);
}