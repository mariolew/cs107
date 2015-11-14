#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <assert.h>

#include "wordcount.h"
#include "stringhash.h"


static int wordCountHash(const void *elemAddr, int numBuckets) {
    char *word = ((wordSet *)elemAddr)->word;
    return StringHash(&word, numBuckets);
}

static int wordCountCmp(const void *elem1, const void *elem2) {
    char *w1 = ((wordSet *)elem1)->word;
    char *w2 = ((wordSet *)elem2)->word;
    return strcasecmp((const char *)w1, (const char *)w2);
}

static void wordCountFree(void *elem) {
    wordSet *ws = (wordSet *)elem;
    free(ws->word);
    VectorDispose(&ws->count);
}

static int articleCountCmp(const void *elem1, const void *elem2) {
    articleCount *ac1 = (articleCount *)elem1;
    articleCount *ac2 = (articleCount *)elem2;
    return articleCmpFn(&ac1->news, &ac2->news);
}

static int articleSortCmp(const void *elem1, const void *elem2) {
    articleCount *ac1 = (articleCount *)elem1;
    articleCount *ac2 = (articleCount *)elem2;
    return ac2->occur - ac1->occur;
}

static void articleCountFree(void *elem) {
    articleCount *ac = (articleCount *)elem;
    articleFreeFn(&ac->news);
}

void initWordCount(hashset *s) {
    HashSetNew(s, sizeof(wordSet), 10007, wordCountHash, wordCountCmp, wordCountFree);
}

void wordCountEnter(hashset *s, const char *word, const char *url, const char *title) {
    wordSet ws;
    ws.word = word;
    wordSet *pos = (wordSet *)HashSetLookup(s, &ws);

    if (pos == NULL) {
        ws.word = strdup(word);
        VectorNew(&ws.count, sizeof(articleCount), articleFreeFn, 25);
        HashSetEnter(s, &ws);
        pos = (wordSet *)HashSetLookup(s, &ws);
    }

    assert(pos != NULL);
    article a;
    a.title = title;
    a.url = url;
    articleCount ac;
    ac.news = a;
    ac.occur = 1;

    int aPos = VectorSearch(&pos->count, &ac, articleCountCmp, 0, false);
    if (aPos == -1) {
        ac.news.title = strdup(title);
        ac.news.url = strdup(url);
        ac.occur = 1;
        VectorAppend(&pos->count, &ac);
    }
    else {
        articleCount *existingAc = VectorNth(&pos->count, aPos);
        existingAc->occur++;
    }
}

static void wordCountMapFn(void *elemAddr, void *auxData) {
    wordSet *ws = (wordSet *)elemAddr;
    VectorSort(&ws->count, articleSortCmp);
}

void wordCountSort(hashset *s) {
    HashSetMap(s, wordCountMapFn, NULL);
}

vector *wordCountLookup(hashset *s, const char *word) {
    wordSet ws;
    ws.word = word;
    wordSet *pos = (wordSet *)HashSetLookup(s, &ws);
    if (pos) {
        return &pos->count;
    }
    else {
        return NULL;
    }
}