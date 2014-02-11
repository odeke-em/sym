// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _FL_H
#define _FL_H
  #include "typedefs.h"
  typedef struct {
    uint32 __size;
    uint32 __idxPtr;
    Object **__mem;
  } FList;

  inline FList *allocFList(void);
  FList *createFListWithSize(uint32 sz);
  FList *destroyFList(FList *fl);

  FList *fpush(FList *fl, Object *o);
  Object *fpop(FList *fl);
  inline uint32 getFSize(const FList *fl);
  inline uint32 getFCapacity(const FList *fl);
#endif
