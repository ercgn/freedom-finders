/* ffind.h header file
 */

#ifndef __FFIND_H__
#define __FFIND_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csapp.h"

struct event {
    char *start;
    char *end;
    char *rrule;
};

typedef struct event* event;

#endif /* __FFIND_H__ */