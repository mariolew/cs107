/**
* Function: packetize
* -------------------
* Takes the specified block of data and builds a linked list
* of nodes where each node is packetSize + sizeof(void *) bytes. The
* first packetSize bytes of each node stores some portion of the original
* image, and the final sizeof(void *) bytes store the address of the next
* node in the packet list. The intent is that the sequence of bytes held
* by all of the nodes is the same sequence of bytes held by the original
* image.
*/
void *packetize(const void *image, int size, int packetSize)
{
    void *head;
    void ** curr;
    curr = &head;
    int numProcessed = 0;

    while (numProcessed < size) {
        void *newNode
        if (size - numProcessed <= packetSize) {
            newNode = malloc(size - numProcessed + sizeof(void *));
            *curr = newNode;
            memcpy(newNode, (char *)image + numProcessed, size - numProcessed);
            *((char *)newNode + size - numProcessed) = NULL;
        }
        else {
            newNode = malloc(packetSize);
            *curr = newNode;
            memcpy(newNode, (char *)image + numProcessed, packetSize + sizeof(void *));
            curr = (void **)(char *)(newNode) + packetSize;
            numProcessed += packetSize;
        }
    }

    return head;
}



//Problem 3
typedef
 int (*MultiSetHashFunction)(const void *elem, int numBuckets);
typedef
 int (*MultiSetCompareFunction)(const void *elem1, const void* elem2);
typedef
 void (*MultiSetMapFunction)(void *elem, int count, void *auxData);
typedef
 void (*MultiSetFreeFunction)(void *elem);
typedef struct {
hashset elements;
int elemSize;
MultiSetFreeFunction free;
} multiset;
void MultiSetNew(multiset *ms, int elemSize, int numBuckets,
MultiSetHashFunction hash, MultiSetCompareFunction compare,
MultiSetFreeFunction free);
void MultiSetDispose(multiset *ms);
void MultiSetEnter(multiset *ms, const void *key);
void MultiSetMap(multiset *ms, MultiSetMapFunction map, void *auxData);


/**
* Function: MultiSetNew
* ---------------------
* Initializes the raw space addressed by ms to be an empty otherwise
* capable of storing an arbitrarily large number of client elements of the
* specified size. The numBuckets, hash, compare, and free parameters
* are all supplied with the understanding that they’ll be passed right
* right through to HashSetNew. You should otherwise interact with the
* embedded hashset using only those functions which have the authority
* to access the hashset's fields.
*/
void MultiSetNew(multiset *ms, int elemSize, int numBuckets,
MultiSetHashFunction hash, MultiSetCompareFunction compare,
MultiSetFreeFunction free)
{
    ms->elemSize = elemSize;
    ms->free = free;
    HashSetNew(&ms->elements, elemSize + sizeof(int) , numBuckets, hash, compare, free);
}
/**
* Function: MultiSetDispose
* -------------------------
* Disposes of all previously stored client elements by calling
* HashSetDispose.
*/
void MultiSetDispose(multiset *ms)
{
    if (ms->free) {
        HashSetDispose(&ms->elements);
    }
    // free(ms->elements);
}

/**
* Function: MultiSetEnter
* -----------------------
* Ensures that the client element addressed by elem is included
* in the multiset. If the element isn’t present, then the element
* is inserted for the very first time and its multiplicity is registered
* as 1. If the element matches one already present, then the old one
* is disposed of (if the free function is non-NULL), the new element
* is replicated over the space of the one just disposed of, and
* the multiplicity is incremented by 1.
*/
void MultiSetEnter(multiset *ms, const void *elem)
{
    void *found = HashSetLookUp(&ms->elements, elem);
    if (found == NULL) {
        void *newNode = malloc(ms->elemSize + sizeof(int));
        memcpy(newNode, elem, ms->elemSize);
        *(int *)((char *)newNode + ms->elemSize) = 1;
        HashSetEnter(&ms->elements, newNode);
    }
    else {
        if (ms->free != NULL) {
            ms->free(found);
        }
        memcpy(found, elem, ms->elemSize);
        *(int *)((char *)found + ms->elemSize) += 1;
    }
}


typedef struct {
    MultiSetHashFunction mhash;
    void *auxData;
    int elemSize;
} mhelper;

static void HashSetMapHelper(void *elem, void *auxData) {
    mhelper *helper = auxData;
    int count = *(int *)((char *)elem + helper->elemSize);
    helper->mhash(elem, count, helper->auxData);
}
/**
* Function: MultiSetMap
* ---------------------
* Applies the specified MultiSetMapFunction to every single element/int
* pair maintained by the multiset, passing the supplied auxData argument as
* the third argument to every single application.
*/
void MultiSetMap(multiset *ms, MultiSetMapFunction map, void *auxData)
{
    mhelper helper = {map, auxData, ms->elemSize};
    HashSetMap(&ms->elements, HashSetMapHelper, helper);
}

typedef struct {
    const char *licensePlate;
    int numTickets;
} maxTicketsStruct;
void FindQueenOfParkingInfractions(multiset *ms, char licensePlateOfQueen[])
{
    maxTicketsStruct mts = {NULL, 0};
    MultiSetMap(ms, mapfn, &mts);
    strcpy(licensePlateOfQueen, mts.licensePlate);
}

void mapfn(void *elem, int count, void *auxData) {
    maxTicketsStruct *mts = (maxTicketsStruct *)auxData;
    if (mts->numTickets < count) {
        mts->numTickets = count;
        mts->licensePlate = (const char *elem);
    }
}