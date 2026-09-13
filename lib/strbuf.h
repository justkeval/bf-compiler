#ifndef _STRBUF_H
#define _STRBUF_H

#include <stdlib.h>

#include "da_append.h"

typedef struct {
    char *items;
    int len;
    int cap;
} StrBuf;

static inline void strbuf_append(StrBuf *buf, const char *cstr) {
    for (int i = 0; cstr[i] != '\0'; i++) {
        da_append(buf, cstr[i]);
    }
}

static inline void strbuf_append_int(StrBuf *buf, int num) {
    int digits[32];
    int len = 0;
    do {
        digits[len++] = num % 10;
        num /= 10;
    } while (num > 0);

    while (len > 0) {
        int digit = digits[--len];
        da_append(buf, '0' + digit);
    }
}

static inline char *strbuf_to_cstr(StrBuf *buf) {
    da_append(buf, '\0');
    return buf->items;
}

#endif