#ifndef __wordcount_
#define __wordcount_

#include "article.h"
typedef struct {
    char *word;
    vector count;
} wordSet;

typedef struct {
    article news;
    int occur;
} articleCount;

//int wordCountHash(const void *elemAddr, int numBuckets);

//int wordCountCmp(const void *elem1, const void *elem2);

//void wordCountFree(void *elem);

//int articleCountCmp(const void *elem, const void *elem2);

//void articleCountFree(void *elem);

void initWordCount(hashset *s);

//void wordCountEnter(hashset *s, const char *word, const char *url, const char *title);
void wordCountEnter(hashset *s, const char *word, const char *url, const char *title);

vector *wordCountLookup(hashset *s, const char *word);

void wordCountSort(hashset *s);

#endif