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

#endif /* __FFIND_H__ */