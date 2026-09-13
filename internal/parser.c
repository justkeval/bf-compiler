#include <stdbool.h>

#include "lib/strbuf.h"
#include "lib/da_append.h"

#include "errors.h"
#include "ir.h"

bool source_to_ir(StrBuf* source, Commands *cmds, Errors* errors) {
    int braceStack = 0;
    for (int i = 0; i < source->len; i++) {
        int cmdc = source->items[i];
        switch (cmdc) {
        case '+':
        case '-':
        case '<':
        case '>': {
            int count = 1;
            while (source->items[i + 1] == cmdc) {
                count++;
                i++;
            }
            CommandKind kind;
            switch (cmdc) {
            case '+':
                kind = CMDK_INC;
                break;
            case '-':
                kind = CMDK_DEC;
                break;
            case '<':
                kind = CMDK_LEFT;
                break;
            case '>':
                kind = CMDK_RIGHT;
                break;
            }
            Command cmd = {kind, count};
            da_append(cmds, cmd);
        } break;
        case '.':
        case ',':
        case '[':
        case ']': {
            CommandKind kind;
            switch (cmdc) {
            case '.':
                kind = CMDK_PRINT;
                break;
            case ',':
                kind = CMDK_READ;
                break;
            case '[':
                kind = CMDK_LOOPSTART;
                braceStack++;
                break;
            case ']':
                kind = CMDK_LOOPEND;
                braceStack--;
                break;
            }
            Command cmd = {kind, 0};
            da_append(cmds, cmd);
        } break;
        }
        if (braceStack < 0) {
            Error err = {"No equivalent opening brace found", i};
            da_append(errors, err);
            return false;
        }
    }
    if (braceStack != 0) {
        Error err = {"Too many opening braces", source->len};
        da_append(errors, err);
        return false;
    }
    return true;
}
