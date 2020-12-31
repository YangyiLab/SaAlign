#include "utilities.h"

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char *neigh_strdup(const char *src) {
    char *dst = NULL;

    if (src != NULL) {
        size_t length = strlen(src);

        dst = malloc(length + 1);

        if (dst != NULL) {
            strcpy(dst, src);
        }
    }

    return dst;
}

size_t trim_trailing_space(char *s) {
    size_t length = strlen(s);
    
    while (length > 0 && isspace(s[length - 1])) {
        length--;
    }
    
    s[length] = '\0';
    
    return length;
}

size_t filename_copy(const char *path, char *dest, size_t size) {
    size_t length = strlen(path);
    size_t end = length;
    
    while (end > 0 && path[end - 1] != '.') {
        end--;
    }
    
    if (end == 0) {
        /* No extension found */
        end = length;
    } else {
        /* Remove the trailing dot */
        end--;
    }
    
    size_t start = end;
    
    while (start > 0 && path[start - 1] != '/') {
        start--;
    }
    
    size_t count = end - start;
    
    if (dest != NULL) {
        size_t n = ((size - 1) < count) ? (size - 1) : count;
        
        memcpy(dest, path + start, n);
        dest[n] = '\0';
    }
    
    return count;
}
