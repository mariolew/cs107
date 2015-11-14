#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include "stringhash.h"
static const signed long kHashMultiplier = -1664117991L;
int StringHash(const void *elemAddr, int numBuckets) {
    char *s = *(char **)elemAddr;
    int i;
    unsigned long hashcode = 0;
    for (i = 0; i < strlen(s); i++) {
        hashcode = hashcode * kHashMultiplier + tolower(s[i]);
    }

    return hashcode % numBuckets;
}

int StringCmp(const void *s1, const void *s2) {
    return strcasecmp(*(const char **)s1, *(const char **)s2);
}

void StringFree(void *s) {
    free(*(void **)s);
}

