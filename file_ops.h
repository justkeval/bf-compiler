#ifndef _FILE_OPS_H
#define _FILE_OPS_H

#include <stdio.h>

#include "strbuf.h"
#include "da_append.h"

void read_file(const char* filepath, StrBuf* buf) {
    FILE* file = fopen(filepath, "r");
    char c;
    while ((c = getc(file)) != EOF) {
        da_append(buf, c);
    }
}

void write_file(const char* filepath, StrBuf* buf) {
    FILE* file = fopen(filepath, "w");
    for (int i = 0; i < buf->len; i++) {
        putc(buf->items[i], file);
    }
}

#endif