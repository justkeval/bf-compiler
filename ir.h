#ifndef _IR_H
#define _IR_H

typedef enum {
    CMDK_INC,
    CMDK_DEC,
    CMDK_LEFT,
    CMDK_RIGHT,
    CMDK_PRINT,
    CMDK_READ,
    CMDK_LOOPSTART,
    CMDK_LOOPEND
} CommandKind;

typedef struct {
    CommandKind kind;
    int value;
} Command;

typedef struct {
    Command *items;
    int len;
    int cap;
} Commands;

#endif