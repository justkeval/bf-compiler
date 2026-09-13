#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strbuf.h"

#include "ir.h"
#include "compiler_config.h"

void ir_to_c(Commands *cmds, StrBuf *buf, CompilerConfig config) {
    char* cell_type = "uint8_t";
    if (config.cell_size == 8) {
        cell_type = "uint8_t";
    } else if (config.cell_size == 16) {
        cell_type = "uint16_t";
    } else if (config.cell_size == 32) {
        cell_type = "uint32_t";
    }
    strbuf_append(buf, "#include <stdio.h>\n");
    strbuf_append(buf, "#include <stdint.h>\n");
    strbuf_append(buf, "#define MEM_SIZE ");
    strbuf_append_int(buf, config.memory_size);
    strbuf_append(buf, "\n\n");
    strbuf_append(buf, "int main() {\n");
    strbuf_append(buf, "  ");
    strbuf_append(buf, cell_type);
    strbuf_append(buf, " mem[MEM_SIZE] = {0};\n");
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
        case CMDK_RIGHT: {
            int delta = cmd.kind == CMDK_LEFT ? -cmd.value : cmd.value;
            delta %= config.memory_size;
            if (delta < 0) delta += config.memory_size;

            strbuf_append(buf, "p = (p + ");
            strbuf_append_int(buf, delta);
            strbuf_append(buf, ") % MEM_SIZE;\n");
        } break;
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
