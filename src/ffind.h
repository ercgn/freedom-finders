/* ffind.h header file
 */

#ifndef __FFIND_H__
#define __FFIND_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "csapp.h"

struct event {
    char *start;
    char *end;
    char *rrule;
};

struct rrule {
    int freq; /* 0 if daily, 1 if week, 2 if month, 3 yearly */
    int interval;
    unsigned int byDay; /* UMTWRFS as bits; 1011010 is UTWF */
};

typedef struct event* event;

/* function prototypes */
unsigned long event_stolu(char *str, bool isStart);
void event_lutos(unsigned long encoding, char *str, bool *isStart);
event* parseICS(char *file, unsigned int *numEvents);
void printEventArray(event* events, int n);
event *getFreeTimes(long unsigned *times, unsigned int n, unsigned int *m);
//void parseRRULE(char *rrule, )

#endif /* __FFIND_H__ */