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


void print_ir(Commands *cmds) {
    for (int i = 0; i < cmds->len; i++) {
        Command cmd = cmds->items[i];
        switch (cmd.kind) {
        case CMDK_INC:
            printf("INC: %d\n", cmd.value);
            break;
        case CMDK_DEC:
            printf("DEC: %d\n", cmd.value);
            break;
        case CMDK_LEFT:
            printf("LEFT: %d\n", cmd.value);
            break;
        case CMDK_RIGHT:
            printf("RIGHT: %d\n", cmd.value);
            break;
        case CMDK_PRINT:
            printf("PRINT\n");
            break;
        case CMDK_READ:
            printf("READ\n");
            break;
        case CMDK_LOOPSTART:
            printf("LOOPSTART\n");
            break;
        case CMDK_LOOPEND:
            printf("LOOPEND\n");
            break;
        }
    }
}

#endif