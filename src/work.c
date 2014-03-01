#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#include "work.h"
#include "flist.h"
#include "objFuncs.h"

void *perform(void *data) {
  void *result = NULL;
  if (data != NULL) {
    Splitter *sp = (Splitter *)data;
    Object *(*f)(Object *) = sp->f;
    if (f != NULL) {
      Object **it = sp->it + sp->start, **end = it + sp->chunkSize;
      FList *fl = createFListWithSize(sp->chunkSize);
      while (it < end && it < sp->absEnd) {
        fl = fpush(fl, f(*it));
        ++it;
      }
      result = fl;
    }
  }

  return result;
}

FList *map(FList *fl, const uint32 nTh, Object *(*f)(Object *)) {
  FList *results = NULL;
  if (fl != NULL && nTh) {
    uint32 chunkSz = ceil((1.0 * getFSize(fl)) / nTh);
    printf("initChSZ: %d\n", chunkSz);
    chunkSz += (chunkSz == 0 || chunkSz * nTh <= getFSize(fl)) ? 1 : 0;
    printf("nTh: %d chunkSize: %d\n", nTh, chunkSz);
    int32 i = getFSize(fl)/chunkSz, j = 0, trav=0;
    pthread_t thL[i];
    printf("th: %p\n", thL);
    while (j < i) {
      Splitter s;
      s.it = fl->__mem, s.absEnd = fl->__mem + getFSize(fl);
      s.chunkSize = chunkSz;
      s.f = f;
      s.start = j;
      printf("start: %d\n", s.start);
      pthread_create(&thL[j], NULL, perform, (void *)&s);      
      trav += chunkSz;
      printf("j: %d\n", j);
      ++j;
    }

    int errCode;
    for (j=0; j < i; ++j) {
      FList *fl = NULL;
      if ((errCode = pthread_join(thL[j], (void **)&fl))) {
        printf("errCode: %d\n", errCode);
      }
      printFList(fl);
      putchar('\n');
    }
  }

  return results;
}

Object *f(Object *o) {
  return NULL;
}

int main() {
  FList *fl = NULL;
  uint32 i;
  for (i=0; i < 21; ++i) {
    fl = fpush(fl, intObject(i));
  }

  map(fl, 5, f);
  fl = destroyFList(fl);
  return 0;
}
