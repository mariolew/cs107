#ifndef __stringhash_
#define __stringhash_

int StringHash(const void *elemAddr, int numBuckets);

int StringCmp(const void *s1, const void *s2);

void StringFree(void *s);
#endif