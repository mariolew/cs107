void MultiTableNew(multitable *mt, int keySizeInBytes, int valueSizeInBytes,
int numBuckets, MultiTableHashFunction hash,
MultiTableCompareFunction compare) {
    mt->keySize = keySizeInBytes;
    mt->valueSize = valueSizeInBytes;
    HashSetNew(&mt->mappings, valueSizeInBytes, numBuckets, hash, compare, NULL);
}


void MultiTableEnter(multitable *mt, const void *keyAddr, const void *valueAddr) {
    vector *value;
    char buffer[sizeof(vector) + mt->keySize];
    void * found = HashSetLookUp(&mt->mappings, keyAddr);
    if (found == NULL) {
        memcpy(buffer, keyAddr, mt->keySize);
        value = (vector *)(buffer + mt->keySize);    
        VectorNew(value, mt->valueSize, NULL, 0);
        VectorAppend(value, valueAddr);
        HashSetEnter(&mt->mappings, buffer);
    }
    else {
        value = (vector *)(found + mt->keySize);
        VectorAppend(value, valueAddr);
    }

}
/**
* Function: MultiTableMap
* -----------------------
* Applies the specified MultiTableMapFunction to each key/value pair
* stored inside the specified multitable. The auxData parameter
* is ultimately channeled in as the third parameter to every single
* invocation of the MultiTableMapFunction. Just to be clear, a
* multitable with seven keys, where each key is associated with
* three different values, would prompt MultiTableMap to invoke the
* specified MultiTableMapFunction twenty-one times.
*/

typedef struct {
    MultiTableMapFunction map;
    void * auxData;
    int keySize;
} maphelper;

static void HashSetMapHelper(void *elem, void *auxData) {
    maphelper * helper = (maphelper *)auxData;
    vector *vec = (vector *)((char *)elem + helper->keySize);
    for (int i = 0; i < VectorLength(vec); i++) {
        helper->map(elem, VectorNth(vec, i), helper->auxData);
    }
}
typedef void (*MultiTableMapFunction)(const void *keyAddr,
void *valueAddr, void *auxData);

void MultiTableMap(multitable *mt, MultiTableMapFunction map, void *auxData) {
    maphelper helper = {map, auxData, keySize};
    HashSetMap(&mt->mappings, HashSetMapHelper, &helper);
}
