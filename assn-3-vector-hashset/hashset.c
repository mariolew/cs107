#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void HashSetNew(hashset *h, int elemSize, int numBuckets,
		HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn)
{
    assert(elemSize >= 0 && numBuckets >= 0);
    assert(hashfn != NULL && comparefn != NULL);
    h->elemSize = elemSize;
    h->numBuckets = numBuckets;
    h->hashfn = hashfn;
    h->comparefn = comparefn;
    h->freefn = freefn;
    h->buckets = malloc(numBuckets * sizeof(vector));

    for (int i = 0; i < numBuckets; i++) {
        VectorNew(&h->buckets[i], elemSize, freefn, 0);
    }
}

void HashSetDispose(hashset *h)
{
    for (int i = 0; i < h->numBuckets; i++) {
        VectorDispose(&h->buckets[i]);
    }
}

int HashSetCount(const hashset *h)
{
    int count = 0;
    for (int i = 0; i < h->numBuckets; i++) {
        count += VectorLength(&h->buckets[i]);
    }
    return count;
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData)
{
    assert(mapfn != NULL);
    for (int i = 0; i < h->numBuckets; i++) {
        VectorMap(&h->buckets[i], mapfn, auxData);
    }
}

void HashSetEnter(hashset *h, const void *elemAddr)
{
    assert(elemAddr != NULL);
    int hashcode = h->hashfn(elemAddr, h->numBuckets);
    assert(hashcode >=0 && hashcode < h->numBuckets);
    void *position = HashSetLookup(h, elemAddr);
    if (position == NULL) {
        VectorAppend(&h->buckets[hashcode], elemAddr);
    }
    else {
        memcpy(position, elemAddr, h->elemSize);
    }
}

static const int kNotFound = -1;
void *HashSetLookup(const hashset *h, const void *elemAddr)
{
    assert(elemAddr != NULL);
    int hashcode = h->hashfn(elemAddr, h->numBuckets);
    assert(hashcode >= 0 && hashcode < h->numBuckets);
    int position = VectorSearch(&h->buckets[hashcode], elemAddr, h->comparefn, 0, false);
    if (position != kNotFound) {
        return VectorNth(&h->buckets[hashcode], position);
    }
    return NULL;
}
