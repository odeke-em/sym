#include <stdio.h>
#include "../hashMap.h"
#include "../objFuncs.h"

inline Bool isOdd(const Object *qObj) {
  if (qObj != NULL && (qObj->typeTag == LIntTag || qObj->typeTag == IntTag)) {
    return (*(uint64 *)(qObj->data)) & 1 ? True : False;
  } else {
    return False;
  }
}

inline Bool isEven(const Object *qObj) {
  return isOdd(qObj) ? False : True;
}

int main() {
  uint64 i;
  Chain *mainChain = NULL;

  for (i=0; i < 100; ++i) {
    mainChain = prepend(mainChain, intObject(i));
  }

  Chain *oddChain  = filter(mainChain, isOdd);
  Chain *evenChain = filter(mainChain, isEven);

  printChain(oddChain);
  printChain(evenChain);

  oddChain  = destroyChain(oddChain);
  evenChain = destroyChain(evenChain);
  mainChain = destroyChain(mainChain);

  return 0;
}
