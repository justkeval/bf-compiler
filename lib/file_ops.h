#ifndef _FILE_OPS_H
#define _FILE_OPS_H

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

#include "strbuf.h"
#include "da_append.h"

static inline bool read_file(const char* filepath, StrBuf* buf, char** err) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        *err = strerror(errno);
        return false;
    }
    char c;
    while ((c = getc(file)) != EOF) {
        da_append(buf, c);
    }
    return true;
}

static inline void write_file(const char* filepath, StrBuf* buf) {
    FILE* file = fopen(filepath, "w");
    for (int i = 0; i < buf->len; i++) {
        putc(buf->items[i], file);
    }
}

#endif