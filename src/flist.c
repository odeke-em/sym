// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <stdio.h>
#include <stdlib.h>
#include "flist.h"
#include "objFuncs.h"

#define FL_INCREMENT_SIZE 10

inline FList *allocFList(void) {
  return (FList *)malloc(sizeof(FList));
}

inline uint32 getFSize(const FList *fl) {
  return fl == NULL ? 0 : fl->__idxPtr;
}

inline uint32 getFCapacity(const FList *fl) {
  return fl == NULL ? 0 : fl->__size;
}

FList *createFListWithSize(uint32 sz) {
  FList *fl = NULL;
  if (sz) {
    if ((fl = allocFList()) == NULL) {
      raiseError("No memory for flat list!");
    }
    fl->__idxPtr = 0;
    fl->__size = sz;
    fl->__mem = (Object **)malloc(sizeof(Object *) * fl->__size);
    if (fl->__mem == NULL) {
      free(fl);
      raiseError("No memory for fl array memory");
    }

    Object **fIt = fl->__mem, **fEnd = fIt + fl->__size;
    while (fIt < fEnd) {
      *fIt++ = NULL;
    }
  }

  return fl;
}

Object *fpop(FList *fl) {
  Object *popd = NULL;
  if (fl != NULL) {
    if (fl->__idxPtr) {
      popd = *(fl->__mem + --fl->__idxPtr);
      if (popd != NULL) decrementRefCount(popd);
      *(fl->__mem + fl->__idxPtr) = NULL;
    }
  }

  return popd;
}

FList *fpush(FList *fl, Object *o) {
  if (fl == NULL) {
    fl = createFListWithSize(FL_INCREMENT_SIZE);
  } else if (fl->__idxPtr >= fl->__size) { // ReSizing time
    fl->__size += FL_INCREMENT_SIZE;
    fl->__mem = (Object **)realloc(fl->__mem, sizeof(Object *) * fl->__size);
    if (fl->__mem == NULL) {
      fl = destroyFList(fl);
    }
  }

  if (o != NULL) incrementRefCount(o);
  *(fl->__mem + fl->__idxPtr++) = o;

  return fl;
}

FList *destroyFList(FList *fl) {
  if (fl != NULL) {
    if (fl->__mem != NULL) {
      Object **fIt = fl->__mem, **fEnd = fIt + fl->__size;
      while (fIt < fEnd) {
        printf("fIt: %p\n", fIt);
        if (*fIt != NULL) {
          if ((*fIt)->refCount < 2) { // Object was created and immediately appended
            *fIt = destroyObject(*fIt);
          }
        }
        ++fIt;
      }
      free(fl->__mem);
      fl->__mem = NULL;
    }
    free(fl);
    fl = NULL;
  }

  return fl;
}

int main() {
  FList *fl = NULL;
  uint64 i;
  for (i=0; i < 20; ++i) {
    fl = fpush(fl, intObject(i));
    printf("i: %ld %d %d\n", i, getFSize(fl), getFCapacity(fl));
  }

  while(getFSize(fl)) {
    Object *o = fpop(fl);
    printObject(o);
    putchar('\n');
    o = destroyObject(o);
  }

  fl = destroyFList(fl);
  
  return 0;
}
