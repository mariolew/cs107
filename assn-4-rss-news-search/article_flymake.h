#ifndef __article_
#define __article_

//#include <stdio.h>
#include "hashset.h"
typedef struct {
    char *title;
    char *url;
} article;

int articleHashFn(const void *elemAddr, int numBuckets);
int articleCmpFn(const void *elemAddr1, const void *elemAddr2);
void articleFreeFn(void *elemAddr);

void initPrevSeenArticles(hashset *s);
bool isNewArticle(hashset *s, char *title, char *url);


#endif