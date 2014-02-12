// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _MAP_ON_THREADS_H
#define _MAP_ON_THREADS_H
    typedef struct {
        char **data;
        unsigned int size;
    } WordShell;

    typedef struct {
        char **wStart, **wEnd;
        unsigned int (*filter)(const char *);
    } WrapFilterShell;

    WordShell *createWordShell(const int bufferSize);
    WordShell *wordShellFromFile(const char *path, int (*customFilter)(const char));
    WordShell *destroyWrap(WordShell *wp);
    void printWrap(WordShell *wp);

    // Prototype for function that will be passed as
    // argument to thread to achieve filtration
    void *filter(void *data);
    WordShell *mapOnThreads(const char *path, unsigned int threadCount, unsigned int (*filterFunc)(const char *w));
#endif
