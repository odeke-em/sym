#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../objFuncs.h"

int main() {
  Object *iObj = intObject(100);
  assert(iObj != NULL);
  assert(iObj->refCount == 0);
  incrementRefCount(iObj);
  assert(iObj->refCount == 1);

  // Testing out destroying
  uint32 refCountBeforeFree = iObj->refCount;
  destroyObject(iObj);
  assert(iObj->isFreed == True);

  // No refCount operations are allowed
  // on already freed objects
  assert(iObj->refCount == refCountBeforeFree);

  // Testing out second destroy
  destroyObject(iObj);
  assert(iObj->isFreed == True);

  decrementRefCount(iObj);
  // No refCount operations are allowed
  // on already freed objects
  assert(iObj->refCount == refCountBeforeFree);
  return 0;
}
