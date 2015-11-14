#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "article.h"
#include "stringhash.h"

int articleHashFn(const void *elemAddr, int numBuckets) {
    article *a = (article *)elemAddr;
    return StringHash(&a->url, numBuckets);
}

int articleCmpFn(const void *elemAddr1, const void *elemAddr2) {
    article *a1 = (article *)elemAddr1;
    article *a2 = (article *)elemAddr2;
    return strcasecmp(a1->url, a2->url);
}
void articleFreeFn(void *elemAddr) {
    article *a = (article *)elemAddr;
    free(a->url);
    free(a->title);
}

void initPrevSeenArticles(hashset *s) {
    HashSetNew(s, sizeof(article), 1009, articleHashFn, articleCmpFn, articleFreeFn);
}

bool isNewArticle(hashset *s, char *title, char *url) {
    article a;
    a.title = title;
    a.url = url;
    void *pos = HashSetLookup(s, &a);
    if (pos != NULL) {
        return false;
    }
    else {
        a.title = strdup(title);
        a.url = strdup(url);
        HashSetEnter(s, &a);
        return true;
    }
}
