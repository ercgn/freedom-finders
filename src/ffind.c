#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "csapp.h"
#include "ffind.h"

int main(int argc, char **argv) {

    file_list f_list = NULL;
    event_list e_list = NULL;
    file_list f_cur;
    event_list e_cur;
//    char tStamp[MAXLINE];
    int i;
    unsigned int numEvents, totalEvents;
    char request[MAXLINE];
    event* e;
    unsigned int *enc_list = Calloc(sizeof(unsigned int), MAX_EVENTS);

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
    for (f_cur = f_list; f_cur != NULL; f_cur = f_cur->next) {
        printf("%d: %s\n", count, f_cur->file);

        numEvents = 0;
        e = parseICS(f_cur->file, &numEvents); 
        e_list = eListInsert(e_list, e, numEvents);
        count++;
    }

    count = 0;
    for (e_cur = e_list; e_cur != NULL; e_cur = e_cur->next) {
        printf("Event %d:", count);
        printEventArray(e_cur->events, e_cur->numEvents);
        count++;

    }

    strcpy(request, e_list->events[1]->rrule);
    rrule rRule = parseRRULE(request);
    printf("RRULE Parsing results:\nRequest: %s\nResults: %d, %07x, %lu\n", 
            request, rRule->freq, rRule->byDay, rRule->dtend);

    int day = dayofweek(1,2,2014);
    printf("Today is: %d\n", day);

    convertToArray(e_list, MAX_EVENTS, &totalEvents, enc_list);
    //TODO create an array from events to quicksort
    //TODO free events

    //TODO output the free time slots in the form of events

    //TODO create a ics file from array of events :<
    createICSFile(e_list->events, numEvents);
    //TODO free events
    freeEventList(e_list);
    freeFileList(f_list);
    Free(enc_list);
    return 0;
}

void convertToArray(event_list e_list, unsigned int maxEvents,
        unsigned int *totalEvents, unsigned int *enc_list) {
    event_list cur;
    int i, freq; 
    int counter = 0;
    unsigned long start, end, dtend;
    rrule rRule;
    unsigned int byDay;

    for (cur = e_list; cur != NULL; cur = cur->next) {
        for (i = 0; i < cur->numEvents; i++) {
            if (counter > MAX_EVENTS) {
                fprintf(stderr, 
                    "Too many events!\nMax number of allowed events is %d\n", 
                    MAX_EVENTS/2);
            }
            start = event_stolu(cur->events[i]->start,true,true);
            end = event_stolu(cur->events[i]->end,true,false);
            enc_list[counter] = start;
            counter++;
            enc_list[counter] = end;
            counter++;
            if (strcmp(cur->events[i]->rrule, "") != 0) {
                rRule = parseRRULE(cur->events[i]->rrule);
                freq = rRule->freq;
                byDay = rRule->byDay;
                dtend = rRule->dtend;
                switch (freq) {
                    case 0: {

                        break;
                    } case 1: {

                        break;
                    } case 2: {

                        break;
                    } case 3: {

                        break;
                    } default: {
                        printf("This field has yet to be implemented\n");
                        printf("Using single recurrence instead...\n");
                    }
                }

                counter++;
            }
        }
    }

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
event_list eListInsert(event_list e_list, event *events, unsigned int n) {
    event_list new_node = Malloc(sizeof(struct event_list));
    new_node->events = events; 
    new_node->numEvents = n;
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

int grabLSB(unsigned long encoding) {
    return encoding & 1;
}

unsigned long grabMonth(unsigned long encoding) {
    char buf[MAXLINE];
    char buf2[MAXLINE];
    unsigned long month;
    sprintf(buf, "%lu", encoding);
    strncpy(buf2, buf+4, 2);
    buf2[2] = '\0';
    sscanf(buf2, "%lu", month);
    return month;
}

unsigned long grabDay(unsigned long encoding) {
    char buf[MAXLINE];
    char buf2[MAXLINE];
    unsigned long day;
    sprintf(buf, "%lu", encoding);
    strncpy(buf2, buf+6, 2);
    buf2[2] = '\0';
    sscanf(buf2, "%lu", day);
    return day;
}

unsigned long grabYear(unsigned long encoding) {
    char buf[MAXLINE];
    char buf2[MAXLINE];
    unsigned long year;
    sprintf(buf, "%lu", encoding);
    strncpy(buf2, buf, 4);
    buf2[4] = '\0';
    sscanf(buf2, "%lu", year);
    return year;
    
}

unsigned long grabHour(unsigned long encoding) {
    char buf[MAXLINE];
    char buf2[MAXLINE];
    unsigned long hour;
    sprintf(buf, "%lu", encoding);
    strncpy(buf2, buf+8, 2);
    buf2[2] = '\0';
    sscanf(buf2, "%lu", hour);
    return hour;
}

unsigned long grabMinute(unsigned long encoding) {
    char buf[MAXLINE];
    char buf2[MAXLINE];
    unsigned long min;
    sprintf(buf, "%lu", encoding);
    strncpy(buf2, buf+10, 2);
    buf2[2] = '\0';
    sscanf(buf2, "%lu", min);
    return min;
}

unsigned long addDay(unsigned long encoding, int days) {
    unsigned long y, m, d;
    y = grabYear(encoding);
    m = grabMonth(encoding);
    d = grabDay(encoding);


}

unsigned long event_stolu(char *str, bool startFlag, bool isStart) {
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
    if (startFlag) {
        if (isStart) {
            strcat(lu_str, "0");
        } else {
            strcat(lu_str, "1");
        }
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
        freeEvents(cur->events, cur->numEvents);
    }
}

void freeEvents(event *events, int n) {
    int i;
    for (i = 0; i < n; i++) {
        Free(events[i]->start);
        Free(events[i]->end);
        Free(events[i]->rrule);
        Free(events[i]);
    }
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

// returns 0 for Sun, 1 for Mon, 2 for Tues etc.
int dayofweek(int d, int m, int y)
{
    static int t[] = {0,3,2,5,0,3,5,1,4,6,2,4};
    y -= m < 3;
    return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

rrule parseRRULE(char *rrStr) {
    char buf[MAXLINE];
    char attr[MAXLINE];
    char field[MAXLINE];
    strcpy(buf, rrStr);
    int flag = 1;
    rrule rRule = Calloc(sizeof(struct rrule), 1);
    while (flag) {
        strcpy(attr, "");
        if (strstr(buf, ";") != NULL) {
            sscanf(buf, "%[^;];%s", attr, buf);
        } else {
            flag = 0;
            sscanf(buf, "%s", attr);
        }

        if (strstr(attr, "FREQ=") != NULL) {
            sscanf(attr, "FREQ=%s", field);
            if (strstr(field, "DAILY") != NULL) {
                rRule->freq = 1;
            } else if (strstr(field, "WEEKLY") != NULL) {
                rRule->freq = 2;
            } else if (strstr(field, "MONTHLY") != NULL) {
                rRule->freq = 3;
            } else if (strstr(field, "YEARLY") != NULL) {
                rRule->freq = 4;
            } else {
                //placeholder for now
                rRule->freq = 5;
            }
        } else if (strstr(attr, "UNTIL=") != NULL) {
            sscanf(attr, "UNTIL=%s", field);
            rRule->dtend = event_stolu(field, false, false);


        } else if (strstr(attr, "BYDAY=") != NULL) {
            sscanf(attr, "BYDAY=%s", field);
            rRule->byDay = 0x0000000;
            if (strstr(field, "MO") != NULL) {
                rRule->byDay += 0x0100000;
            }
            if (strstr(field, "TU") != NULL) {
                rRule->byDay += 0x0010000;
            }
            if (strstr(field, "WE") != NULL) {
                rRule->byDay += 0x0001000;
            }
            if (strstr(field, "TH") != NULL) {
                rRule->byDay += 0x0000100;
            }
            if (strstr(field, "FR") != NULL) {
                rRule->byDay += 0x0000010;
            }
            if (strstr(field, "SA") != NULL) {
                rRule->byDay += 0x0000001;
            }
            if (strstr(field, "SU") != NULL) {
                rRule->byDay += 0x1000000;
            }
        } else if (strstr(attr, "INTERVAL=") != NULL) {
            //TODO: Implement
        } else if (strstr(attr, "COUNT=") != NULL) {
            //TODO: Implement
        }
    }
    return rRule;
}



void createICSFile(event *events, unsigned int n) {
    int fd = Open("free_times.ics", 
                   O_WRONLY | O_CREAT | O_TRUNC, 
                   DEF_MODE & ~DEF_UMASK);
    rio_t rio;
    Rio_readinitb(&rio, fd);
    char buf[MAX_CAL_SIZE];

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