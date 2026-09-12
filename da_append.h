#ifndef _DA_APPEND_H
#define _DA_APPEND_H

#include <stdlib.h>

#define da_append(arr, value)                                              \
    do {                                                                   \
        if ((arr)->len >= (arr)->cap) {                                    \
            if ((arr)->cap == 0)                                           \
                (arr)->cap = 256;                                          \
            else                                                           \
                (arr)->cap *= 2;                                           \
            (arr)->items =                                                 \
                realloc((arr)->items, (arr)->cap * sizeof(*(arr)->items)); \
        }                                                                  \
        (arr)->items[(arr)->len++] = (value);                              \
    } while (0)


#endif