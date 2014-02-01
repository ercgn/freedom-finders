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
#define MAX_CAL_SIZE 1049000 
#define MAX_EVENTS 65536 
#define MAX_DAYS 34

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
    int freq; /* 1 if daily, 2 if week, 3 if month, 4 yearly */
    unsigned int byDay; /* UMTWRFS as bits; 1011010 is UTWF */
//    unsigned long dtstart;
    unsigned long dtend;
};

typedef struct rrule* rrule;

/* Function prototypes */
unsigned long event_stolu(char *str, bool startFlag, bool isStart);
void event_lutos(unsigned long encoding, char *str, bool *isStart);
event *parseICS(char *file, unsigned int *numEvents);
void printEventArray(event* events, int n);
event *getFreeTimes(long unsigned *times, unsigned int n, unsigned int *m);
void createICSFile(event *events, unsigned int n); /* WORKS */
rrule parseRRULE(char *rrStr);
void convertToArray(event_list e_list, unsigned int maxEvents,
                unsigned int *totalEvents, unsigned long *enc_list);
int compare(const void *a, const void *b);

/* Time manipulation functions */ 
int dayofweek(int d, int m, int y);
int grabLSB(unsigned long encoding);
unsigned long grabMonth(unsigned long encoding);
unsigned long grabDay(unsigned long encoding);
unsigned long grabYear(unsigned long encoding);
unsigned long grabHour(unsigned long encoding);
unsigned long grabMinute(unsigned long encoding);
/* This funciton can only add a max of 30 days */
unsigned long addDay(unsigned long encoding, int days);

/* Linked List Functions */
file_list fListInsert(file_list f_list, char *file);
event_list eListInsert(event_list e_list, event *events, unsigned int n);
void freeFileList(file_list f_list);
void freeEventList(event_list e_list);
void freeEvents(event *events, int n);






#endif /* __FFIND_H__ */