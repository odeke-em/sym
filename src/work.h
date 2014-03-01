#ifndef _WORK_H
#define  _WORK_H
  #include "objFuncs.h"
  #include "flist.h"

  typedef struct {
    uint32 chunkSize, start;
    Object **it, **absEnd;
    Object *(*f)(Object *);
  } Splitter;

  void *perform(void *data);
  FList *map(FList *fl, const uint32 nTh, Object *(*f)(Object *));
#endif
