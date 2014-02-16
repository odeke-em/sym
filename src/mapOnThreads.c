// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "mapOnThreads.h"

WordShell *createWordShell(const int bufferSize) {
    WordShell *wp = (WordShell *)malloc(sizeof(WordShell));
    wp->data = (char **)malloc(sizeof(char *) * bufferSize);
    wp->size = bufferSize;
    return wp;
}

void *filter(void *data) {
    void *result = NULL;
    if (data != NULL) {
        WrapFilterShell *wpShell = (WrapFilterShell *)data;
        // printf("wpFilterShell: %p\n", wpShell);
        if (wpShell->wStart != NULL && wpShell->wEnd != NULL && wpShell->filter != NULL) {
            char **wIt = wpShell->wStart, **wEnd = wpShell->wEnd;
            printf("wStart: %p wEnd: %p diff: %ld\n", wIt, wEnd, wEnd - wIt);
            int resIndex = 0, resBufSize = wEnd - wIt;
            WordShell *resWrap = createWordShell(resBufSize);
            while (wIt < wEnd) {
                if (wpShell->filter(*wIt)) {
                    if (resIndex >= resBufSize) {
                        resBufSize += 15;
                        resWrap->data = (char **)realloc(resWrap->data, sizeof(char *) * resBufSize);
                    }
                    *(resWrap->data + resIndex++) = strdup(*wIt);
                }
                ++wIt;
            }

            if (resIndex) {
                resWrap->size = resIndex;
                resWrap->data = (char **)realloc(resWrap->data, sizeof(char *) * resIndex);
                result = resWrap;
            } else {
                free(resWrap);
            }
        }
    }

    return result;
}

WordShell *wordShellFromFile(const char *path, int (*customFilter)(const char)) {
    WordShell *wrap = NULL;
    FILE *ifp = fopen(path, "r");
    if (ifp != NULL) {
        wrap = (WordShell *)malloc(sizeof(WordShell));
        int bufSize = 100, j=0;
        wrap = createWordShell(bufSize);
        while (! feof(ifp)) {
            if (j >= bufSize) {
                bufSize += 10;
                wrap->data = (char **)realloc(wrap->data, sizeof(char *) * bufSize);
            }

            int cBufSize = 10, i=0;
            char *cIn = (char *)malloc(sizeof(char) * cBufSize), c;
            while (! feof(ifp) && customFilter(c = getc(ifp)) && c != EOF) {
                if (i >= cBufSize) {
                    cBufSize += 10;
                    cIn = (char *)realloc(cIn, sizeof(char) * cBufSize);
                }
                *(cIn + i++) = c;
            }
            if (i) {
                *(cIn + i++) = '\0';
                cIn = (char *)realloc(cIn, sizeof(char) * i);
                *(wrap->data + j++) = cIn;
            } else {
                free(cIn);
            }
        }

        if (j) {
            wrap->data = (char **)realloc(wrap->data, sizeof(char *) * j);
            wrap->size = j;
        } else {
            free(wrap->data);
            free(wrap);
            wrap = NULL;
        }
    }

    return wrap;
}

void printWrap(WordShell *wp) {
    if (wp != NULL && wp->data != NULL) {
        char **wIt = wp->data, **wEnd = wIt + wp->size;
        while (wIt < wEnd) {
            printf("%s\n", *wIt++);
        }
    }
}

WordShell *destroyWrap(WordShell *wp) {
    if (wp != NULL) {
        if (wp->data != NULL) {
            char **wIt = wp->data, **wEnd = wIt + wp->size;
            while (wIt < wEnd) {
                if (*wIt != NULL) free(*wIt);
                ++wIt;
            }
            free(wp->data);
        }
        free(wp);
    }

    return wp;
}

WordShell *mapOnThreads(const char *path, unsigned int threadCount, unsigned int (*filterFunc)(const char *w)) {
    WordShell *mainWrap = wordShellFromFile(path, isalpha);
    if (! threadCount) 
        threadCount = 1;

    int i=0, sliceSize = 1 + (mainWrap->size / threadCount);
    int isRunning = 1, startIdx=0, endIdx;
    printf("ThreadCount: %d sliceSize: %d\n", threadCount, sliceSize);
    pthread_t thList[threadCount];

    WrapFilterShell wpFShellList[threadCount];
    while (isRunning) {
        endIdx = startIdx + sliceSize;
        if (endIdx > mainWrap->size) {
            endIdx = mainWrap->size;
            isRunning = 0;
        }

        WrapFilterShell *wpFShell = &(wpFShellList[i]);

        wpFShell->wStart = mainWrap->data + startIdx;
        wpFShell->wEnd = mainWrap->data + endIdx;
        wpFShell->filter = filterFunc;
        // printf("wStart: %p wEnd: %p\n", wpFShell->wStart, wpFShell->wEnd);
        pthread_create(&(thList[i]), NULL, filter, (void *)wpFShell);

        startIdx = endIdx;
        ++i;
    }

    WordShell *wrapResult = createWordShell(mainWrap->size); // In the worst case all segments are matched
    int wrapIndex = 0;
    for (i=0; i < threadCount; ++i) {
        WordShell *wResult = NULL;
        if (pthread_join(thList[i], (void **)&wResult)) {
            printf("\033[91mFailed to join thread: %d\033[00m\n", i);
        } else {
            if (wResult != NULL && wResult->data != NULL) {
                printf("Result size: %d\n", wResult->size);
                char **it = wResult->data, **end = it + wResult->size;
                while (it < end) {
                    if (*it != NULL) {
                        *(wrapResult->data + wrapIndex++) = *it;
                    }
                    ++it;
                }
            }
        }
    }

    printf("Combined Results length: %d\n", wrapIndex);
    if (wrapIndex) {
        wrapResult->size = wrapIndex;
        wrapResult->data = (char **)realloc(wrapResult->data, sizeof(char *) * wrapIndex);
    } else {
        wrapResult = destroyWrap(wrapResult);
        wrapResult = NULL;
    }

    return wrapResult;
}

inline unsigned int longerThan8(const char *s) {
    return s != NULL && strlen(s) > 8 ? 1 : 0;
}

int main(int argc, char *argv[]) {
    char *srcPath = __FILE__;
    unsigned int threadCount = 7, maxLen=10;
    printf("argc: %d\n", argc);
    if (argc > 1) {
        srcPath = argv[1];
        if (argc > 2) {
            if (sscanf(argv[2], "%d", &threadCount) != 1) {
                fprintf(stderr, "\033[31mCouldn't parse a number from argv!\n\033[00m");
            }
        }
    }

    fprintf(stderr, "\033[92mGetting words longer than 8 from \033[00m%s\n ", srcPath);
    WordShell *thMapped = mapOnThreads(srcPath, threadCount, longerThan8);
    printWrap(thMapped);
    thMapped = destroyWrap(thMapped);
    return 0;
}
