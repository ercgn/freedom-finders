/* ffind.h header file
 */

#ifndef __FFIND_H__
#define __FFIND_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "csapp.h"

#define MAXCAL 15

struct file_list {
    char *file;
    struct file_list *next;
};

typedef struct file_list* file_list;

struct event {
    char *start;
    char *end;
    char *rrule;
};

typedef struct event* event;

struct event_list {
    event *events;
    unsigned int numEvents;
    struct event_list *next;
};

typedef struct event_list* event_list;

struct rrule {
    int freq; /* 0 if daily, 1 if week, 2 if month, 3 yearly */
    int interval;
    unsigned int byDay; /* UMTWRFS as bits; 1011010 is UTWF */
    unsigned long dtstart;
    unsigned long dtend;
};

typedef struct rrule* rrule;

/* function prototypes */
unsigned long event_stolu(char *str, bool isStart);
void event_lutos(unsigned long encoding, char *str, bool *isStart);
file_list fListInsert(file_list f_list, char *file);
event_list eListInsert(event_list e_list, event *events, unsigned int n);
event *parseICS(char *file, unsigned int *numEvents);
void printEventArray(event* events, int n);
void freeFileList(file_list f_list);
void freeEventList(event_list e_list);
void freeEvents(event *events, int n);
event *getFreeTimes(long unsigned *times, unsigned int n, unsigned int *m);
void createICSFile(event *events, unsigned int n); /* WORKS */

#endif /* __FFIND_H__ */