#ifndef _ERRORS_H
#define _ERRORS_H

typedef struct {
    char* message;
    int pos;
} Error;

typedef struct {
    Error* items;
    int len;
    int cap;
} Errors;

#endif