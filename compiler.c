#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strbuf.h"

#include "ir.h"

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

void ir_to_c(Commands *cmds, StrBuf *buf) {
    strbuf_append(buf, "#include <stdio.h>\n");
    strbuf_append(buf, "#define MEM_SIZE 2048\n");
    strbuf_append(buf, "int main() {\n");
    strbuf_append(buf, "  int mem[MEM_SIZE] = {0};\n");
    strbuf_append(buf, "  int p = 0;\n");
    int indent = 1;
    for (int i = 0; i < cmds->len; i++) {
        Command cmd = cmds->items[i];
        for (int i = 0; i < (cmd.kind == CMDK_LOOPEND ? indent - 1 : indent); i++) {
            strbuf_append(buf, "  ");
        }
        switch (cmd.kind) {
        case CMDK_INC:
            strbuf_append(buf, "mem[p] += ");
            strbuf_append_int(buf, cmd.value);
            strbuf_append(buf, ";\n");
            break;
        case CMDK_DEC:
            strbuf_append(buf, "mem[p] -= ");
            strbuf_append_int(buf, cmd.value);
            strbuf_append(buf, ";\n");
            break;
        case CMDK_LEFT:
            strbuf_append(buf, "p = (p - ");
            strbuf_append_int(buf, cmd.value);
            strbuf_append(buf, ") % MEM_SIZE;\n");
            break;
        case CMDK_RIGHT:
            strbuf_append(buf, "p = (p + ");
            strbuf_append_int(buf, cmd.value);
            strbuf_append(buf, ") % MEM_SIZE;\n");
            break;
        case CMDK_PRINT:
            strbuf_append(buf, "putchar(mem[p]);\n");
            break;
        case CMDK_READ:
            strbuf_append(buf, "mem[p] = getchar();\n");
            break;
        case CMDK_LOOPSTART:
            strbuf_append(buf, "if (mem[p]) do {\n");
            indent++;
            break;
        case CMDK_LOOPEND:
            strbuf_append(buf, "} while (mem[p]);\n");
            indent--;
            break;
        }
    }
    strbuf_append(buf, "}\n");
}
